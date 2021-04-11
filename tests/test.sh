#!/usr/bin/env bash

# Perform tests from tests/ directory
cd "$(dirname "$0")"

do_test()
{
	test="$1"

	execName="$(../tools/build.sh "${test}" -r "../src/" -b "../bin/test/")"

	if [[ $? -ne 0 ]]; then
		echo "Failed to build test '${test}'" 1>&2
		return 1
	fi

	echo "Testing '${test}'"
	"${execName}"
}

list=()
if [ $# -eq 0 ]; then
	# List all the items in the tests/ folder except for the current script
	shopt -s extglob
	list="*.!(sh)"
else
	list="$@"
fi

result=true
for item in $list
do
	do_test "$item" || result=false
done

if [ $result = true ]; then
	echo "All tests passed!"
else
	echo "Some tests failed." 1>&2
	# Remove the stack dumps on Windows
	rm ./*.stackdump >&/dev/null
fi
