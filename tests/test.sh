#!/usr/bin/env bash

# Perform tests from tests/ directory
cd "$(dirname "$0")"

do_test()
{
	test="$1"
	test="$(basename "$test")"
	test="${test%%.*}"

	if [ ! -f "${test}".cpp ]; then
		return 1
	fi

	if [ ! -d ../bin/ ]; then
		mkdir ../bin ||
		return 1
	fi

	g++ -O3 -Wall -std=c++11 -I ../ -o ../bin/"${test}"-test "${test}".cpp &&
	echo "Testing ${test}" &&
	../bin/"${test}"-test
}

list=()
if [ $# -eq 0 ]; then
	list=./*
else
	list="$@"
fi

for item in $list
do
	do_test "$item"
done
