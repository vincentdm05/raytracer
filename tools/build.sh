#!/usr/bin/env bash

usage()
{
	echo "usage: $0 project_name [-h] [-b|--build-dir build_dir]"
	echo "  project_name is the name of a .cpp file with entry point and its location."
	echo "output: the name of the successfully built executable, or an error message in stderr."
	echo "options:"
	echo "  -h                         Prints this message."
	echo "  -b, --build-dir build_dir  Specifies where built binaries will be located. By default"
	echo "    they are written to a new folder called 'bin' under the project's location."
}

absolute_path()
{
	( cd "$1" ; pwd )
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
projectName=
buildDir=

# Parse arguments
while [[ $# -gt 0 ]]; do
	arg="$1"
	case "$arg" in
		-h|--help)
		needHelp=1
		shift
		;;
		-b|--build-dir)
		buildDir="$2"
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
	usage
	if [[ $needHelp -eq 0 ]]; then
		echo "Error: required input 'project_name' not provided." 1>&2
		exit 1
	fi
	exit 0
fi

# Extract path to project
projectDir="$(absolute_path "$(dirname "$projectName")" )"

# Default build directory to within project directory
if [[ -z "$buildDir" ]]; then
	buildDir="$projectDir"/bin
fi

# Strip projectName of path and extension
projectName="$(barename "$projectName")"

# Find source file
projectSrc="${projectDir}/${projectName}.cpp"
if [ ! -f "$projectSrc" ]; then
	echo "${projectName}.cpp does not exist in $projectDir" 1>&2
	exit 1
fi

# Find build directory
if [ ! -d "$buildDir" ]; then
	mkdir "$buildDir" ||
	exit 1
fi
buildDir="$(absolute_path "$buildDir")"

outputFile="${buildDir}/${projectName}"

g++ -O3 -Wall -std=c++11 -o "$outputFile" "$projectSrc"
if [[ $? -eq 0 ]]; then
	echo "$outputFile"
else
	echo "Could not build file ${outputFile}" 1>&2
	exit 1
fi
