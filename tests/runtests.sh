#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libfsapfs/.libs/libfsapfs.1.dylib && test -f ./pyfsapfs/.libs/pyfsapfs.so
then
	install_name_tool -change /usr/local/lib/libfsapfs.1.dylib ${PWD}/libfsapfs/.libs/libfsapfs.1.dylib ./pyfsapfs/.libs/pyfsapfs.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

