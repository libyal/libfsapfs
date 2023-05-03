#!/usr/bin/env bash
# Tests man pages.
#
# Version: 20230410

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

run_test()
{
	local INPUT_FILE=$1;
	local RESULT=0

	TEST_NAME=`basename ${INPUT_FILE}`;
	echo -n "Testing man with input: ${TEST_NAME}";

	LC_ALL=en_US.UTF-8 MANROFFSEQ='' MANWIDTH=80 man --warnings -E UTF-8 -l -Tutf8 -Z ${INPUT_FILE} > /dev/null 2> ${TMPDIR}/${TEST_NAME}.warnings;
	RESULT=$?;

	# For now line break warnings are ignored.
	if test -f ${TMPDIR}/${TEST_NAME}.warnings;
	then
		sed "/can't break line/ d" -i ${TMPDIR}/${TEST_NAME}.warnings;
	fi
	if test -s ${TMPDIR}/${TEST_NAME}.warnings;
	then
		RESULT=${EXIT_FAILURE};
	fi
	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	if test -s ${TMPDIR}/${TEST_NAME}.warnings;
	then
		cat ${TMPDIR}/${TEST_NAME}.warnings;
	fi
	return ${RESULT};
}

if test "${OSTYPE}" = "msys";
then
	exit ${EXIT_IGNORE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

assert_availability_binary man;

RESULT=${EXIT_IGNORE};

TMPDIR="tmp$$";

rm -rf ${TMPDIR};
mkdir ${TMPDIR};

MANUALS_PATH="../manuals";

if ! test -d ${MANUALS_PATH};
then
	MANUALS_PATH="manuals";
fi

if ! test -d ${MANUALS_PATH};
then
	echo "Manuals directory not found.";

	exit ${EXIT_IGNORE};
fi

for INPUT_FILE in ${MANUALS_PATH}/*.[13];
do
	run_test "${INPUT_FILE}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

rm -rf ${TMPDIR};

exit ${RESULT};

