#!/usr/bin/env bash

usage()
{
	echo "usage: $0 example_name [-h|--help] [-o|--output-dir output_dir] [-s|--show] [-t|--time]"
	echo "  'example_name' is the name of a .cpp file with entry point and its location. The location"
	echo "  can be either relative or absolute, but it must exist. The example name does not need to"
	echo "  contain the extension, but must correspond to an existing '.cpp' file."
	echo "options:"
	echo "  -h|--help                   Prints this message."
	echo "  -o|--output-dir output_dir  Specify output location for result image."
	echo "  -s|--show                   Tell whether to show resulting image at the end."
	echo "  -t|--time                   Time the execution."
}

absolute_path()
{
	( cd "$1" && pwd -P || echo "$1" )
}

# Strip off path and extension
barename()
{
	name="$1"
	name="$(basename "$name")"
	name="${name%%.*}"
	echo "$name"
}

showResult=0
show()
{
	if [[ $showResult -eq 1 ]]; then
		environment="$(uname -s)"
		case "$environment" in
			Darwin*) open "$1";;
			MINGW*|MSYS*) nomacs "$1" &;;
			*) echo "Couldn't show $1, please open manually."
		esac
	fi
}

timeExec=0
execute()
{
	if [[ $timeExec -eq 1 ]]; then
		time { "$1" "$2" ; }
	else
		"$1" "$2"
	fi
}

needHelp=0
example=
outputDir=

while [[ $# -gt 0 ]]; do
	arg="$1"
	case "$arg" in
		-h|--help)
		needHelp=1
		shift
		;;
		-o|--output-dir)
		outputDir="$2"
		shift
		shift
		;;
		-s|--show)
		showResult=1
		shift
		;;
		-t|--time)
		timeExec=1
		shift
		;;
		*)
		if [[ -z "$example" ]]; then
			example="$arg"
		else
			echo "Unrecognized input \"$arg\" will be ignored." 1>&2
		fi
		shift
		;;
	esac
done

if [[ -z "$example" ]]; then
	exitCode=0
	if [[ $needHelp -eq 0 ]]; then
		echo "Error: required input 'example_name' not provided." 1>&2
		exitCode=1
	fi
	usage
	exit $exitCode
elif [[ $needHelp -eq 1 ]]; then
	usage
fi

scriptDir="$(dirname "$0")"
scriptDir="$(absolute_path "$scriptDir")"

exampleDir="$(dirname "$example")"
exampleDir="$(absolute_path "$exampleDir")"
if [[ ! -d "$exampleDir" ]]; then
	echo "Couldn't find ${exampleDir}" 1>&2
	exit 1
fi

binDir=
if [ -f "${projectDir}/Makefile" ]; then
	binDir="${scriptDir}"/../../bin
else
	binDir="${scriptDir}"/../bin
fi

exampleName="$(barename "$example")"
example="${exampleDir}/${exampleName}"

# Build
execName="$("${scriptDir}"/build.sh "$example" -b "${binDir}")"
if [[ $? -ne 0 ]]; then
	exit 1
fi

if [[ ! -f "${execName}" ]]; then
	echo "Could not find binary '${execName}'" 1>&2
	exit 1
fi

outputName="$example"
if [[ ! -z "$outputDir" ]]; then
	outputName="${outputDir}/${exampleName}"
fi

# Run
execute "${execName}" "${outputName}" &&
show "$outputName".ppm
