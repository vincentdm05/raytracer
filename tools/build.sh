#!/usr/bin/env bash

usage()
{
	echo "usage: $0 project_name [-h|--help] [-r|--root root_dir] [-b|--bin-dir bin_dir]"
	echo "  project_name is the name of a .cpp file with entry point and its location. If a Makefile"
	echo "  is located next to project_name, it will be used to build the project."
	echo "output: the name of the successfully built executable, or an error message in stderr."
	echo "options:"
	echo "  -h, --help                 Prints this message."
	echo "  -r, --root                 Specifies where included files are rooted."
	echo "  -b, --bin-dir bin_dir      Specifies where built binaries will be located. By default"
	echo "    they are written to a new folder called 'bin' under the project's location."
}

absolute_path()
{
	( cd "$1" && pwd -P || echo "$1" )
}

# Strip of path and extension
barename()
{
	name="$1"
	name="$(basename "$name")"
	name="${name%%.*}"
	echo "$name"
}

needHelp=0
rootDir=
projectName=
binDir=

# Parse arguments
while [[ $# -gt 0 ]]; do
	arg="$1"
	case "$arg" in
		-h|--help)
		needHelp=1
		shift
		;;
		-r|--root)
		rootDir="$2"
		shift
		shift
		;;
		-b|--bin-dir)
		binDir="$2"
		shift
		shift
		;;
		*)
		if [[ -z "$projectName" ]]; then
			projectName="$arg"
		else
			echo "Unrecognized input \"$arg\" will be ignored." 1>&2
		fi
		shift
		;;
	esac
done

if [[ -z "$projectName" ]]; then
	exitCode=0
	if [[ $needHelp -eq 0 ]]; then
		echo "Error: required input 'project_name' not provided." 1>&2
		exitCode=1
	fi
	usage
	exit $exitCode
fi

toolDir="$(absolute_path "$(dirname "$0")" )"

# Default root directory to parent of tools folder
if [[ -z "${rootDir}" ]]; then
	rootDir="$toolDir"/..
fi

if [[ ! -d "${rootDir}" ]]; then
	echo "Root directory ${rootDir} doesn't exist" 1>&2
	exit 1
fi

# Extract path to project
projectDir="$(absolute_path "$(dirname "$projectName")" )"

# Strip projectName of path and extension
projectName="$(barename "$projectName")"

# Default binary directory to within project directory
if [[ -z "$binDir" ]]; then
	binDir="$projectDir"/bin
fi

# Find binary directory
if [ ! -d "$binDir" ]; then
	mkdir "$binDir" ||
	exit 1
fi
binDir="$(absolute_path "$binDir")"

outputFile="${binDir}/${projectName}"

# Find source file
projectSrc="${projectDir}/${projectName}.cpp"
if [ ! -f "$projectSrc" ]; then
	echo "${projectName}.cpp does not exist in $projectDir" 1>&2
	exit 1
fi

if [ -f "${projectDir}/Makefile" ]; then
	( cd "${projectDir}" && make >/dev/null )
else
	g++ -O3 -Wall -std=c++11 -I "${rootDir}" -o "$outputFile" "$projectSrc"
fi

if [[ $? -eq 0 ]]; then
	echo "$outputFile"
else
	echo "Could not build ${outputFile}" 1>&2
	exit 1
fi
