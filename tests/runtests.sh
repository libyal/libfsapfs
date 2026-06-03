#!/bin/sh
# Script to run tests
#
# Version: 20260602

if test -f ${PWD}/libfsapfs/.libs/libfsapfs.1.dylib && test -f ./pyfsapfs/.libs/pyfsapfs.so
then
	install_name_tool -change /usr/local/lib/libfsapfs.1.dylib ${PWD}/libfsapfs/.libs/libfsapfs.1.dylib ./pyfsapfs/.libs/pyfsapfs.so
fi

make check $@
RESULT=$?

if test ${RESULT} -ne 0 && test -f tests/test-suite.log
then
	cat tests/test-suite.log
fi
exit ${RESULT}

