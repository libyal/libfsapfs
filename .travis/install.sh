#!/bin/sh
# Script to run install step on Travis-CI
#
# Version: 20190802

# Exit on error.
set -e;

if test ${TARGET} != "docker";
then
	if test ${TRAVIS_OS_NAME} = "osx";
	then
		export SED="/usr/local/bin/gsed";
	fi

	./synclibs.sh --use-head;
	./autogen.sh;

	if test ${TARGET} = "linux-gcc-shared" || test ${TARGET} = "linux-gcc-shared-wide-character-type";
	then
		./configure > /dev/null;
		make > /dev/null;

		./syncsharedlibs.sh --use-head;
	fi

	if test -x "synctestdata.sh";
	then
		./synctestdata.sh;
	fi
fi

