#!/bin/bash
# Script that runs the tests
#
# Version: 20190103

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

run_configure_make()
{
	local CONFIGURE_OPTIONS=$@;

	./configure ${CONFIGURE_OPTIONS[@]};
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: './configure' failed";

		return ${RESULT};
	fi

	make clean > /dev/null;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make clean' failed";

		return ${RESULT};
	fi

	make > /dev/null;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make' failed";

		return ${RESULT};
	fi
	return ${EXIT_SUCCESS};
}

run_configure_make_check()
{
	run_configure_make $@;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		return ${RESULT};
	fi

	make check CHECK_WITH_STDERR=1;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make check' failed";

		if test -f tests/test-suite.log;
		then
			cat tests/test-suite.log;
		fi

		return ${RESULT};
	fi
	return ${EXIT_SUCCESS};
}

run_configure_make_check_with_asan()
{
	local LDCONFIG=`which ldconfig 2> /dev/null`;

	if test -z ${LDCONFIG} || test ! -x ${LDCONFIG};
	then
		return ${EXIT_SUCCESS};
	fi
	local LIBASAN=`ldconfig -p | grep libasan | sed 's/^.* => //' | sort | tail -n 1`;

	if test -z ${LIBASAN} || test ! -f ${LIBASAN};
	then
		return ${EXIT_SUCCESS};
	fi
	# Using libasan is platform dependent.
	if test ${LIBASAN} != "/lib64/libasan.so.4" && test ${LIBASAN} != "/lib64/libasan.so.5";
	then
		return ${EXIT_SUCCESS};
	fi

	export CPPFLAGS="-DHAVE_ASAN";
	export CFLAGS="-fno-omit-frame-pointer -fsanitize=address -g";
	export LDFLAGS="-fsanitize=address -g";

	if test -z ${CC} || test ${CC} != "clang";
	then
		LDFLAGS="${LDFLAGS} -lasan";
	fi

	run_configure_make $@;
	RESULT=$?;

	export CPPFLAGS=;
	export CFLAGS=;
	export LDFLAGS=;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		return ${RESULT};
	fi

	make check CHECK_WITH_ASAN=1 CHECK_WITH_STDERR=1;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make check' failed";

		if test -f tests/test-suite.log;
		then
			cat tests/test-suite.log;
		fi

		return ${RESULT};
	fi
	return ${RESULT};
}

run_configure_make_check_with_coverage()
{
	# Disable optimization so we can hook malloc and realloc.
	export CPPFLAGS="-DOPTIMIZATION_DISABLED";
	export CFLAGS="--coverage -O0";
	export LDFLAGS="--coverage";

	# Disable creating a shared library so we can hook memset.
	run_configure_make_check $@;
	RESULT=$?;

	export CPPFLAGS=;
	export CFLAGS=;
	export LDFLAGS=;

	return ${RESULT};
}

run_configure_make_check_python()
{
	run_configure_make $@;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		return ${RESULT};
	fi

	make check CHECK_WITH_STDERR=1 SKIP_LIBRARY_TESTS=1 SKIP_TOOLS_TESTS=1;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make check' failed";

		if test -f tests/test-suite.log;
		then
			cat tests/test-suite.log;
		fi

		return ${RESULT};
	fi
	return ${EXIT_SUCCESS};
}

run_setup_py_tests()
{
	# Skip this test when running Cygwin on AppVeyor.
	if test -n "${APPVEYOR}" && test ${TARGET} = "cygwin";
	then
		echo "Running: 'setup.py build' skipped";

		return ${EXIT_SUCCESS};
	fi
	PYTHON=$1;

	${PYTHON} setup.py build;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'setup.py build' failed";

		return ${RESULT};
	fi
	return ${EXIT_SUCCESS};
}

CONFIGURE_HELP=`./configure --help`;

echo "${CONFIGURE_HELP}" | grep -- '--enable-wide-character-type' > /dev/null;

HAVE_ENABLE_WIDE_CHARACTER_TYPE=$?;

echo "${CONFIGURE_HELP}" | grep -- '--enable-verbose-output' > /dev/null;

HAVE_ENABLE_VERBOSE_OUTPUT=$?;

echo "${CONFIGURE_HELP}" | grep -- '--enable-debug-output' > /dev/null;

HAVE_ENABLE_DEBUG_OUTPUT=$?;

echo "${CONFIGURE_HELP}" | grep -- '--with-pthread' > /dev/null;

HAVE_WITH_PTHREAD=$?;

echo "${CONFIGURE_HELP}" | grep -- '--with-zlib' > /dev/null;

HAVE_WITH_ZLIB=$?;

echo "${CONFIGURE_HELP}" | grep -- '--with-openssl' > /dev/null;

HAVE_WITH_OPENSSL=$?;

echo "${CONFIGURE_HELP}" | grep -- '--enable-python' > /dev/null;

HAVE_ENABLE_PYTHON=$?;

echo "${CONFIGURE_HELP}" | grep -- '--enable-static-executables' > /dev/null;

HAVE_ENABLE_STATIC_EXECUTABLES=$?;

PYTHON_CONFIG="";

if test -x /usr/bin/whereis;
then
	PYTHON_CONFIG=`/usr/bin/whereis python-config | sed 's/^.*:[ ]*//' 2> /dev/null`;
fi

# Test "./configure && make && make check" without options.

run_configure_make_check;
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

if test ${HAVE_ENABLE_VERBOSE_OUTPUT} -eq 0 && test ${HAVE_ENABLE_DEBUG_OUTPUT} -eq 0;
then
	# Test "./configure && make && make check" with verbose and debug output.

	run_configure_make_check "--enable-verbose-output --enable-debug-output";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi
fi

if test ${HAVE_WITH_PTHREAD} -eq 0;
then
	# Test "./configure && make && make check" without multi-threading support.

	run_configure_make_check "--with-pthread=no";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi
fi

if test ${HAVE_WITH_ZLIB} -eq 0;
then
	# Test "./configure && make && make check" with fallback zlib implementation.

	run_configure_make_check "--with-zlib=no";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi
fi

if test ${HAVE_WITH_OPENSSL} -eq 0;
then
	# Test "./configure && make && make check" with fallback crypto implementation.

	run_configure_make_check "--with-openssl=no";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi

	# Test "./configure && make && make check" with OpenSSL non-EVP implementation.

	run_configure_make_check "--enable-openssl-evp-cipher=no --enable-openssl-evp-md=no";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi

	# Test "./configure && make && make check" with OpenSSL EVP implementation.

	run_configure_make_check "--enable-openssl-evp-cipher=yes --enable-openssl-evp-md=yes";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi
fi

if test ${HAVE_ENABLE_PYTHON} -eq 0 && test -n "${PYTHON_CONFIG}";
then
	# Test with Python 2.
	PYTHON2=`which python2 2> /dev/null`;

        # Note that "test -x" on Mac OS X will succeed if the argument is not set.
	if test -n "${PYTHON2}" && test -x ${PYTHON2};
	then
		export PYTHON_VERSION=2;

		run_configure_make_check_python "--enable-python";
		RESULT=$?;

		export PYTHON_VERSION=;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			exit ${EXIT_FAILURE};
		fi
		export PYTHON_VERSION=2;

		run_configure_make_check_python "--enable-python2";
		RESULT=$?;

		export PYTHON_VERSION=;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			exit ${EXIT_FAILURE};
		fi

		if test -f "setup.py" && ! run_setup_py_tests ${PYTHON2};
		then
			exit ${EXIT_FAILURE};
		fi
	fi

	# Test with Python 3.
	PYTHON3=`which python3 2> /dev/null`;

        # Note that "test -x" on Mac OS X will succeed if the argument is not set.
	if test -n "${PYTHON3}" && test -x ${PYTHON3};
	then
		export PYTHON_VERSION=3;

		run_configure_make_check_python "--enable-python";
		RESULT=$?;

		export PYTHON_VERSION=;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			exit ${EXIT_FAILURE};
		fi
		export PYTHON_VERSION=3;

		run_configure_make_check_python "--enable-python3";
		RESULT=$?;

		export PYTHON_VERSION=;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			exit ${EXIT_FAILURE};
		fi

		if test -f "setup.py" && ! run_setup_py_tests ${PYTHON3};
		then
			exit ${EXIT_FAILURE};
		fi
	fi

	# Test with the default Python version.
	if test -z ${PYTHON2} && test -z ${PYTHON3};
	then
		run_configure_make_check_python "--enable-python";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			exit ${EXIT_FAILURE};
		fi

		PYTHON=`which python 2> /dev/null`;

		if test -f "setup.py" && ! run_setup_py_tests ${PYTHON};
		then
			exit ${EXIT_FAILURE};
		fi
	fi
fi

if test ${HAVE_ENABLE_STATIC_EXECUTABLES} -eq 0;
then
	run_configure_make_check "--enable-static-executables";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi
fi

# Run tests with asan.
CONFIGURE_OPTIONS="";

if test ${HAVE_ENABLE_WIDE_CHARACTER_TYPE} -eq 0;
then
	CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-wide-character-type";
fi
if test ${HAVE_WITH_ZLIB} -eq 0;
then
	CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-zlib=no";
fi
if test ${HAVE_WITH_OPENSSL} -eq 0;
then
	CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-openssl=no";
fi
if test ${HAVE_ENABLE_PYTHON} -eq 0 && test -n "${PYTHON_CONFIG}";
then
	# Issue with running the python bindings with asan disabled for now.
	# CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-python";
	CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS}";
fi

run_configure_make_check_with_asan ${CONFIGURE_OPTIONS};
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

# Run tests with coverage.
CONFIGURE_OPTIONS="--enable-shared=no";

if test ${HAVE_ENABLE_WIDE_CHARACTER_TYPE} -eq 0;
then
	CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-wide-character-type";
fi
if test ${HAVE_WITH_ZLIB} -eq 0;
then
	CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-zlib=no";
fi
if test ${HAVE_WITH_OPENSSL} -eq 0;
then
	CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --with-openssl=no";
fi

run_configure_make_check_with_coverage ${CONFIGURE_OPTIONS};
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

