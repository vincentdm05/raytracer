#!/usr/bin/env bash

usage()
{
	echo "usage: $0 example_name"
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

if [[ $# -ne 1 ]]; then
	echo "Error: Missing argument." 1>&2
	usage
	exit 1
fi
example="$1"

scriptDir="$(dirname "$0")"
scriptDir="$(absolute_path "$scriptDir")"

# Build
execName="$("${scriptDir}"/build.sh "$example" -b "$scriptDir/../bin")"
if [[ $? -ne 0 ]]; then
	exit 1
fi

example="$(barename "$example")"

baseFilename="${scriptDir}/../examples/${example}"

# Run
time { "$execName" "$baseFilename" ; } &&
show "$baseFilename".ppm
