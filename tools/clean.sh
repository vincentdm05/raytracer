#!/usr/bin/env bash

rootDir=$(cd "$(dirname $0)/../" ; pwd)

if [[ ! -d "${rootDir}" || "$(basename ${rootDir})" != "raytracer" ]]; then
	echo "Incorrect root directory '${rootDir}'." 1>&2
	exit 1
fi

rm -r "${rootDir}"/bin
rm -r "${rootDir}"/build
