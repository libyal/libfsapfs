#!/usr/bin/env bash
# Script that runs tests.
#
# Version: 20260617

initialize_configure_options()
{
	CONFIGURE_HELP=`./configure --help`

	echo "${CONFIGURE_HELP}" | grep -- '--enable-wide-character-type' > /dev/null
	HAVE_ENABLE_WIDE_CHARACTER_TYPE=$?

	echo "${CONFIGURE_HELP}" | grep -- '--enable-multi-threading-support' > /dev/null
	HAVE_ENABLE_MULTI_THREADING_SUPPORT=$?

	echo "${CONFIGURE_HELP}" | grep -- '--enable-verbose-output' > /dev/null
	HAVE_ENABLE_VERBOSE_OUTPUT=$?

	echo "${CONFIGURE_HELP}" | grep -- '--enable-debug-output' > /dev/null
	HAVE_ENABLE_DEBUG_OUTPUT=$?

	echo "${CONFIGURE_HELP}" | grep -- '--with-bzip2' > /dev/null
	HAVE_WITH_BZIP2=$?

	echo "${CONFIGURE_HELP}" | grep -- '--with-libfuse' > /dev/null
	HAVE_WITH_LIBFUSE=$?

	echo "${CONFIGURE_HELP}" | grep -- '--with-lzma' > /dev/null
	HAVE_WITH_LZMA=$?

	echo "${CONFIGURE_HELP}" | grep -- '--with-pthread' > /dev/null
	HAVE_WITH_PTHREAD=$?

	echo "${CONFIGURE_HELP}" | grep -- '--with-openssl' > /dev/null
	HAVE_WITH_OPENSSL=$?

	echo "${CONFIGURE_HELP}" | grep -- '--with-zlib' > /dev/null
	HAVE_WITH_ZLIB=$?

	echo "${CONFIGURE_HELP}" | grep -- '--enable-python' > /dev/null
	HAVE_ENABLE_PYTHON=$?

	echo "${CONFIGURE_HELP}" | grep -- '--enable-static-executables' > /dev/null
	HAVE_ENABLE_STATIC_EXECUTABLES=$?
}

run_configure_make()
{
	local CONFIGURE_OPTIONS=$@

	./configure ${CONFIGURE_OPTIONS[@]} | sed '1,/^configure:$/ d'
	make clean > /dev/null
	make "-j${NUMBER_OF_JOBS}" > /dev/null
}

run_configure_make_check()
{
	run_configure_make $@

	./tests/runtests.sh ${MAKE_CHECK_OPTIONS} | sed -E '/Making check in / d'
}

run_configure_make_check_with_asan()
{
	local LDCONFIG=`which ldconfig 2> /dev/null`

	if test -z ${LDCONFIG} || test ! -x ${LDCONFIG}
	then
		return
	fi
	local LIBASAN=`ldconfig -p | grep libasan | sed 's/^.* => //' | sort | tail -n 1`

	if test -z ${LIBASAN} || test ! -f ${LIBASAN}
	then
		return
	fi
	CONFIGURE_OPTIONS="--enable-asan"

	if test ${HAVE_ENABLE_WIDE_CHARACTER_TYPE} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-wide-character-type"
	fi
	if test ${HAVE_WITH_BZIP2} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-bzip2=no"
	fi
	if test ${HAVE_WITH_LZMA} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-lzma=no"
	fi
	if test ${HAVE_WITH_OPENSSL} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-openssl=no"
	fi
	if test ${HAVE_WITH_ZLIB} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-zlib=no"
	fi
	if test ${HAVE_ENABLE_PYTHON} -eq 0 && test -n "${PYTHON_CONFIG}"
	then
		# Issue with running the python bindings with asan disabled for now.
		# CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-python"
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS}"
	fi
	run_configure_make ${CONFIGURE_OPTIONS}

	./tests/runtests.sh ${MAKE_CHECK_OPTIONS} | sed -E '/Making check in / d'
}

run_configure_make_check_with_code_coverage()
{
	CONFIGURE_OPTIONS="--disable-shared --enable-code-coverage"

	if test ${HAVE_ENABLE_WIDE_CHARACTER_TYPE} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-wide-character-type"
	fi
	if test ${HAVE_WITH_BZIP2} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-bzip2=no"
	fi
	if test ${HAVE_WITH_LZMA} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-lzma=no"
	fi
	if test ${HAVE_WITH_OPENSSL} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-openssl=no"
	fi
	if test ${HAVE_WITH_ZLIB} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-zlib=no"
	fi
	run_configure_make_check CPPFLAGS="-DHAVE_MEMORY_TEST"
}

run_configure_make_check_with_ubsan()
{
	local LDCONFIG=`which ldconfig 2> /dev/null`

	if test -z ${LDCONFIG} || test ! -x ${LDCONFIG}
	then
		return
	fi
	local LIBASAN=`ldconfig -p | grep libubsan | sed 's/^.* => //' | sort | tail -n 1`

	if test -z ${LIBASAN} || test ! -f ${LIBASAN}
	then
		return
	fi
	CONFIGURE_OPTIONS="--enable-ubsan"

	if test ${HAVE_ENABLE_WIDE_CHARACTER_TYPE} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-wide-character-type"
	fi
	if test ${HAVE_WITH_BZIP2} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-bzip2=no"
	fi
	if test ${HAVE_WITH_LZMA} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-lzma=no"
	fi
	if test ${HAVE_WITH_OPENSSL} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-openssl=no"
	fi
	if test ${HAVE_WITH_ZLIB} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-zlib=no"
	fi
	if test ${HAVE_ENABLE_PYTHON} -eq 0 && test -n "${PYTHON_CONFIG}"
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-python"
	fi
	run_configure_make ${CONFIGURE_OPTIONS}

	./tests/runtests.sh ${MAKE_CHECK_OPTIONS} | sed -E '/Making check in / d'
}

run_configure_make_check_python()
{
	run_configure_make $@

	./tests/runtests.sh ${MAKE_CHECK_OPTIONS} SKIP_LIBRARY_TESTS=1 SKIP_TOOLS_TESTS=1 | sed -E '/Making check in / d'
}

run_configure_make_check_static_executables()
{
	CONFIGURE_OPTIONS="--enable-static-executables"

	if test ${HAVE_ENABLE_MULTI_THREADING_SUPPORT} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --disable-multi-threading-support"
	fi
	if test ${HAVE_WITH_BZIP2} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-bzip2=no"
	fi
	if test ${HAVE_WITH_LIBFUSE} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-libfuse=no"
	fi
	if test ${HAVE_WITH_LZMA} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-lzma=no"
	fi
	if test ${HAVE_WITH_OPENSSL} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-openssl=no"
	fi
	if test ${HAVE_WITH_ZLIB} -eq 0
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-zlib=no"
	fi
	run_configure_make_check ${CONFIGURE_OPTIONS}
}

PROJECT_NAME=`basename $PWD`

initialize_configure_options

PYTHON_CONFIG=""

if test -x /usr/bin/whereis
then
	PYTHON_CONFIG=`/usr/bin/whereis python-config | sed 's/^.*:[ ]*//' 2> /dev/null`
fi

set -e
set -o pipefail

NUMBER_OF_JOBS=4
MAKE_CHECK_OPTIONS="-j${NUMBER_OF_JOBS} --no-print-directory --quiet TESTSUITEFLAGS=--color=always V=0 VERBOSE=1"

echo -e "\033[1mTesting without options.\033[0m"
run_configure_make_check

if test ${HAVE_ENABLE_VERBOSE_OUTPUT} -eq 0 && test ${HAVE_ENABLE_DEBUG_OUTPUT} -eq 0
then
	echo -e "\033[1mTesting with verbose and debug output.\033[0m"
	run_configure_make_check "--enable-verbose-output --enable-debug-output"
fi
if test ${HAVE_WITH_PTHREAD} -eq 0 && test ${PROJECT_NAME} != "libcthreads"
then
	echo -e "\033[1mTesting with without multi-threading support.\033[0m"
	run_configure_make_check "--with-pthread=no"
fi
if test ${HAVE_WITH_ZLIB} -eq 0
then
	echo -e "\033[1mTesting without zlib support.\033[0m"
	run_configure_make_check "--with-zlib=no"
fi
if test ${HAVE_WITH_OPENSSL} -eq 0
then
	echo -e "\033[1mTesting without OpenSSL support.\033[0m"
	run_configure_make_check "--with-openssl=no"

	echo -e "\033[1mTesting without OpenSSL EVP support.\033[0m"
	run_configure_make_check "--disable-openssl-evp-cipher --disable-openssl-evp-md"

	echo -e "\033[1mTesting with OpenSSL EVP support.\033[0m"
	run_configure_make_check "--enable-openssl-evp-cipher --enable-openssl-evp-md"
fi
if test ${HAVE_ENABLE_PYTHON} -eq 0 && test -n "${PYTHON_CONFIG}"
then
	echo -e "\033[1mTesting with Python support.\033[0m"
	run_configure_make_check_python "--enable-python"
fi
if test ${HAVE_ENABLE_STATIC_EXECUTABLES} -eq 0
then
	echo -e "\033[1mTesting with static executables.\033[0m"
	run_configure_make_check_static_executables
fi

echo -e "\033[1mTesting with asan.\033[0m"
run_configure_make_check_with_asan

echo -e "\033[1mTesting with ubsan.\033[0m"
run_configure_make_check_with_ubsan

echo -e "\033[1mTesting with code coverage.\033[0m"
run_configure_make_check_with_code_coverage
