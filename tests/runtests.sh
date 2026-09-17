#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libfsapfs/.libs/libfsapfs.1.dylib" ] && [ -f ./pyfsapfs/.libs/pyfsapfs.so ]
then
    install_name_tool -change /usr/local/lib/libfsapfs.1.dylib "${PWD}/libfsapfs/.libs/libfsapfs.1.dylib" ./pyfsapfs/.libs/pyfsapfs.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

