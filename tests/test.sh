#!/usr/bin/env bash

absolute_path()
{
	( cd "$1" && pwd -P || echo "$1" )
}

barename()
{
	name="$(basename "$1")"
	name="${name%%.*}"
	echo "$name"
}

scriptName="$( basename "$0" )"
testsDir="$( absolute_path "$(dirname "$0")" )"
failure=0

do_test()
{
	testName="$1"
	testPath="$2"

	execName="$( "${testsDir}"/../tools/build.sh "${testPath}" -r "${testsDir}/../src/" -b "${testsDir}/../bin/tests/" )"

	if [[ $? -ne 0 ]]; then
		echo 1>&2
		echo "[Test>${testName}] Failed to build '${testPath}'" 1>&2
		failure=1
		return 1
	fi

	"${execName}" |& awk '{print "[Test>'"${testName}"'] " $0}'

	status="${PIPESTATUS[0]}"
	if [[ "${status}" -ne 0 ]]; then
		echo "[${scriptName}] Errors running '${testPath}'" 1>&2
		failure=1
	fi
	return "${status}"
}

list=()
if [ $# -eq 0 ]; then
	# List all the items in the tests/ folder except for the current script
	shopt -s extglob
	list="${testsDir}/*.!(sh)"
else
	list="$@"
fi

for item in $list
do
	testName="$( barename "$item" )"
	testPath="$( absolute_path "$(dirname "$item")" )"/"$( basename "$item" )"

	do_test "${testName}" "${testPath}" &&
	echo "[Test>${testName}] Successfully ran '${testPath}'"
done

if [[ $failure -ne 0 ]]; then
	echo "[${scriptName}] Some tests failed." 1>&2
	# Remove the stack dumps on Windows
	rm "${testsDir}"/*.stackdump >&/dev/null
else
	echo "[${scriptName}] All tests passed!"
fi
