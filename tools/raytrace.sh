#!/usr/bin/env bash

usage()
{
	echo "usage: $0 example_name [-o|--output-dir output_dir]"
	echo "  example_name is the name of a .cpp file with entry point and its location. The location"
	echo "  can be either relative or absolute, but it must exist. The example name does not need to"
	echo "  contain the extension, but must correspond to an existing '.cpp' file."
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

show()
{
	environment="$(uname -s)"
	case "$environment" in
		Darwin*) open "$1";;
		MINGW*|MSYS*) nomacs "$1" &;;
		*) echo "Couldn't show $1, please open manually."
	esac
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
fi

scriptDir="$(dirname "$0")"
scriptDir="$(absolute_path "$scriptDir")"

exampleDir="$(dirname "$example")"
exampleDir="$(absolute_path "$exampleDir")"
exampleName="$(barename "$example")"
example="${exampleDir}/${exampleName}"

# Build
execName="$("${scriptDir}"/build.sh "$example" -b "$scriptDir/../bin")"
if [[ $? -ne 0 ]]; then
	exit 1
fi

outputName="$example"
if [[ ! -z "$outputDir" ]]; then
	outputName="${outputDir}/${exampleName}"
fi

# Run
time { "$execName" "$outputName" ; } &&
show "$outputName".ppm
