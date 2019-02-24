#!/bin/bash
# Tests Python module functions and types.
#
# Version: 20190223

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_FUNCTIONS="support";
TEST_FUNCTIONS_WITH_INPUT="";
OPTION_SETS="offset password";

TEST_TOOL_DIRECTORY=".";
INPUT_GLOB="*";

test_python_function()
{
	local TEST_FUNCTION=$1;

	local TEST_DESCRIPTION="Testing Python-bindings functions: ${TEST_FUNCTION}";
	local TEST_SCRIPT="${TEST_TOOL_DIRECTORY}/pyfsapfs_test_${TEST_FUNCTION}.py";

	run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_SCRIPT}";
	local RESULT=$?;

	return ${RESULT};
}

test_python_function_with_input()
{
	local TEST_FUNCTION=$1;

	local TEST_DESCRIPTION="Testing Python-bindings functions: ${TEST_FUNCTION}";
	local TEST_SCRIPT="${TEST_TOOL_DIRECTORY}/pyfsapfs_test_${TEST_FUNCTION}.py";

	if ! test -d "input";
	then
		echo "Test input directory not found.";

		return ${EXIT_IGNORE};
	fi
	local RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		echo "No files or directories found in the test input directory";

		return ${EXIT_IGNORE};
	fi

	local TEST_PROFILE_DIRECTORY=$(get_test_profile_directory "input" "pyfsapfs");

	local IGNORE_LIST=$(read_ignore_list "${TEST_PROFILE_DIRECTORY}");

	RESULT=${EXIT_SUCCESS};

	for TEST_SET_INPUT_DIRECTORY in input/*;
	do
		if ! test -d "${TEST_SET_INPUT_DIRECTORY}";
		then
			continue;
		fi
		if check_for_directory_in_ignore_list "${TEST_SET_INPUT_DIRECTORY}" "${IGNORE_LIST}";
		then
			continue;
		fi

		local TEST_SET_DIRECTORY=$(get_test_set_directory "${TEST_PROFILE_DIRECTORY}" "${TEST_SET_INPUT_DIRECTORY}");

		local OLDIFS=${IFS};

		# IFS="\n"; is not supported by all platforms.
		IFS="
";

		if test -f "${TEST_SET_DIRECTORY}/files";
		then
			for INPUT_FILE in `cat ${TEST_SET_DIRECTORY}/files | sed "s?^?${TEST_SET_INPUT_DIRECTORY}/?"`;
			do
				run_test_on_input_file_with_options "${TEST_SET_DIRECTORY}" "${TEST_DESCRIPTION}" "default" "${OPTION_SETS}" "${TEST_SCRIPT}" "${INPUT_FILE}";
				RESULT=$?;

				if test ${RESULT} -ne ${EXIT_SUCCESS};
				then
					break;
				fi
			done
		else
			for INPUT_FILE in `ls -1d ${TEST_SET_INPUT_DIRECTORY}/${INPUT_GLOB}`;
			do
				run_test_on_input_file_with_options "${TEST_SET_DIRECTORY}" "${TEST_DESCRIPTION}" "default" "${OPTION_SETS}" "${TEST_SCRIPT}" "${INPUT_FILE}";
				RESULT=$?;

				if test ${RESULT} -ne ${EXIT_SUCCESS};
				then
					break;
				fi
			done
		fi
		IFS=${OLDIFS};

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			break;
		fi
	done

	return ${RESULT};
}

if ! test -z ${SKIP_PYTHON_TESTS};
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

RESULT=${EXIT_IGNORE};

for TEST_FUNCTION in ${TEST_FUNCTIONS};
do
	test_python_function "${TEST_FUNCTION}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

if test ${RESULT} -ne ${EXIT_SUCCESS} && test ${RESULT} -ne ${EXIT_IGNORE};
then
	exit ${RESULT};
fi

for TEST_FUNCTION in ${TEST_FUNCTIONS_WITH_INPUT};
do
	if test -d "input";
	then
		test_python_function_with_input "${TEST_FUNCTION}";
		RESULT=$?;
	else
		test_python_function "${TEST_FUNCTION}";
		RESULT=$?;
	fi

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};

