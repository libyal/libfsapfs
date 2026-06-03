#!/usr/bin/env bash
# Script that runs the tests
#
# Version: 20260531

EXIT_SUCCESS=0
EXIT_FAILURE=1

run_configure_make()
{
	local CONFIGURE_OPTIONS=$@

	./configure ${CONFIGURE_OPTIONS[@]}
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		echo "Running: './configure' failed"

		return ${RESULT}
	fi

	make clean > /dev/null
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		echo "Running: 'make clean' failed"

		return ${RESULT}
	fi

	make > /dev/null
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		echo "Running: 'make' failed"

		return ${RESULT}
	fi
	return ${EXIT_SUCCESS}
}

run_configure_make_check()
{
	run_configure_make $@
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		return ${RESULT}
	fi

	make check CHECK_WITH_STDERR=1
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		echo "Running: 'make check' failed"

		if test -f tests/test-suite.log
		then
			cat tests/test-suite.log
		fi

		return ${RESULT}
	fi
	return ${EXIT_SUCCESS}
}

run_configure_make_check_with_asan()
{
	local LDCONFIG=`which ldconfig 2> /dev/null`

	if test -z ${LDCONFIG} || test ! -x ${LDCONFIG}
	then
		return ${EXIT_SUCCESS}
	fi
	local LIBASAN=`ldconfig -p | grep libasan | sed 's/^.* => //' | sort | tail -n 1`

	if test -z ${LIBASAN} || test ! -f ${LIBASAN}
	then
		return ${EXIT_SUCCESS}
	fi

	run_configure_make --enable-asan
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		return ${RESULT}
	fi

	make check CHECK_WITH_STDERR=1
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		echo "Running: 'make check' failed"

		if test -f tests/test-suite.log
		then
			cat tests/test-suite.log
		fi

		return ${RESULT}
	fi
	return ${RESULT}
}

run_configure_make_check_with_ubsan()
{
	local LDCONFIG=`which ldconfig 2> /dev/null`

	if test -z ${LDCONFIG} || test ! -x ${LDCONFIG}
	then
		return ${EXIT_SUCCESS}
	fi
	local LIBASAN=`ldconfig -p | grep libubsan | sed 's/^.* => //' | sort | tail -n 1`

	if test -z ${LIBASAN} || test ! -f ${LIBASAN}
	then
		return ${EXIT_SUCCESS}
	fi

	run_configure_make --enable-ubsan
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		return ${RESULT}
	fi

	make check CHECK_WITH_STDERR=1
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		echo "Running: 'make check' failed"

		if test -f tests/test-suite.log
		then
			cat tests/test-suite.log
		fi

		return ${RESULT}
	fi
	return ${RESULT}
}

run_configure_make_check_with_coverage()
{
	export CPPFLAGS="-DHAVE_MEMORY_TEST"

	run_configure_make_check --enable-code-coverage
	RESULT=$?

	export CPPFLAGS=""

	return ${RESULT}
}

run_configure_make_check_python()
{
	run_configure_make $@
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		return ${RESULT}
	fi

	make check CHECK_WITH_STDERR=1 SKIP_LIBRARY_TESTS=1 SKIP_TOOLS_TESTS=1
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		echo "Running: 'make check' failed"

		if test -f tests/test-suite.log
		then
			cat tests/test-suite.log
		fi

		return ${RESULT}
	fi
	return ${EXIT_SUCCESS}
}

PROJECT_NAME=`basename $PWD`

CONFIGURE_HELP=`./configure --help`

echo "${CONFIGURE_HELP}" | grep -- '--enable-wide-character-type' > /dev/null

HAVE_ENABLE_WIDE_CHARACTER_TYPE=$?

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

PYTHON_CONFIG=""

if test -x /usr/bin/whereis
then
	PYTHON_CONFIG=`/usr/bin/whereis python-config | sed 's/^.*:[ ]*//' 2> /dev/null`
fi

# Test "./configure && make && make check" without options.

run_configure_make_check
RESULT=$?

if test ${RESULT} -ne ${EXIT_SUCCESS}
then
	exit ${EXIT_FAILURE}
fi

if test ${HAVE_ENABLE_VERBOSE_OUTPUT} -eq 0 && test ${HAVE_ENABLE_DEBUG_OUTPUT} -eq 0
then
	# Test "./configure && make && make check" with verbose and debug output.

	run_configure_make_check "--enable-verbose-output --enable-debug-output"
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		exit ${EXIT_FAILURE}
	fi
fi

if test ${HAVE_WITH_PTHREAD} -eq 0 && test ${PROJECT_NAME} != "libcthreads"
then
	# Test "./configure && make && make check" without multi-threading support.

	run_configure_make_check "--with-pthread=no"
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		exit ${EXIT_FAILURE}
	fi
fi

if test ${HAVE_WITH_ZLIB} -eq 0
then
	# Test "./configure && make && make check" with fallback zlib implementation.

	run_configure_make_check "--with-zlib=no"
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		exit ${EXIT_FAILURE}
	fi
fi

if test ${HAVE_WITH_OPENSSL} -eq 0
then
	# Test "./configure && make && make check" with fallback crypto implementation.

	run_configure_make_check "--with-openssl=no"
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		exit ${EXIT_FAILURE}
	fi

	# Test "./configure && make && make check" with OpenSSL non-EVP implementation.

	run_configure_make_check "--disable-openssl-evp-cipher --disable-openssl-evp-md"
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		exit ${EXIT_FAILURE}
	fi

	# Test "./configure && make && make check" with OpenSSL EVP implementation.

	run_configure_make_check "--enable-openssl-evp-cipher --enable-openssl-evp-md"
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		exit ${EXIT_FAILURE}
	fi
fi

if test ${HAVE_ENABLE_PYTHON} -eq 0 && test -n "${PYTHON_CONFIG}"
then
	run_configure_make_check_python "--enable-python"
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		exit ${EXIT_FAILURE}
	fi
fi

if test ${HAVE_ENABLE_STATIC_EXECUTABLES} -eq 0
then
	CONFIGURE_OPTIONS="--disable-multi-threading-support --enable-static-executables"

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
	RESULT=$?

	if test ${RESULT} -ne ${EXIT_SUCCESS}
	then
		exit ${EXIT_FAILURE}
	fi
fi

# Run tests with asan.
CONFIGURE_OPTIONS=""

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

run_configure_make_check_with_asan ${CONFIGURE_OPTIONS}
RESULT=$?

if test ${RESULT} -ne ${EXIT_SUCCESS}
then
	exit ${EXIT_FAILURE}
fi

# Run tests with ubsan.
CONFIGURE_OPTIONS=""

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

run_configure_make_check_with_ubsan ${CONFIGURE_OPTIONS}
RESULT=$?

if test ${RESULT} -ne ${EXIT_SUCCESS}
then
	exit ${EXIT_FAILURE}
fi

# Run tests with coverage.
CONFIGURE_OPTIONS="--disable-shared"

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

run_configure_make_check_with_coverage ${CONFIGURE_OPTIONS}
RESULT=$?

if test ${RESULT} -ne ${EXIT_SUCCESS}
then
	exit ${EXIT_FAILURE}
fi

exit ${EXIT_SUCCESS}

