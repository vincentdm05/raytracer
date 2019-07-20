#!/usr/bin/env bash

example="$1"
example="$(basename "$example")"
example="${example%%.*}"

if [ ! -f "${example}".cpp ]; then
	echo "${example}.cpp does not exist in $(pwd)" 1>&2
	exit 1
fi

cd ..
if [ ! -d bin/ ]; then
	mkdir bin ||
	exit 1
fi

show()
{
	environment="$(uname -s)"
	case "$environment" in
		Darwin*) open "$1";;
		MINGW*) nomacs "$1";;
		*) echo "Couldn't show $1, please open manually."
	esac
}

g++ -O3 -Wall -std=c++11 -o bin/"${example}" examples/"${example}".cpp &&
time { ./bin/"${example}" > examples/"${example}".ppm ; } &&
show examples/"${example}".ppm
