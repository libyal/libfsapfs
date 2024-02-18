#!/usr/bin/env bash
# Tests Python module functions and types.
#
# Version: 20240120

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_NO_TESTS_RAN=5;
EXIT_IGNORE=77;

TEST_FUNCTIONS="support";
TEST_FUNCTIONS_WITH_INPUT="container volume";
OPTION_SETS=("offset" "password");

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

		if test -f "${TEST_SET_DIRECTORY}/files";
		then
			IFS="" read -a INPUT_FILES <<< $(cat ${TEST_SET_DIRECTORY}/files | sed "s?^?${TEST_SET_INPUT_DIRECTORY}/?");
		else
			IFS="" read -a INPUT_FILES <<< $(ls -1d ${TEST_SET_INPUT_DIRECTORY}/${INPUT_GLOB});
		fi
		for INPUT_FILE in "${INPUT_FILES[@]}";
		do
			local TESTED_WITH_OPTIONS=0;

			for OPTION_SET in ${OPTION_SETS[@]};
			do
				local TEST_DATA_OPTION_FILE=$(get_test_data_option_file "${TEST_SET_DIRECTORY}" "${INPUT_FILE}" "${OPTION_SET}");

				if test -f ${TEST_DATA_OPTION_FILE};
				then
					TESTED_WITH_OPTIONS=1;

					IFS=" " read -a OPTIONS <<< $(read_test_data_option_file "${TEST_SET_DIRECTORY}" "${INPUT_FILE}" "${OPTION_SET}");

					run_test_on_input_file "${TEST_SET_DIRECTORY}" "${TEST_DESCRIPTION}" "default" "${OPTION_SET}" "${TEST_SCRIPT}" "${INPUT_FILE}" "${OPTIONS[@]}";
					RESULT=$?;

					if test ${RESULT} -ne ${EXIT_SUCCESS};
					then
						break;
					fi
				fi
			done

			if test ${TESTED_WITH_OPTIONS} -eq 0;
			then
				run_test_on_input_file "${TEST_SET_DIRECTORY}" "${TEST_DESCRIPTION}" "default" "" "${TEST_SCRIPT}" "${INPUT_FILE}";
				RESULT=$?;
			fi

			if test ${RESULT} -ne ${EXIT_SUCCESS};
			then
				break;
			fi
		done

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			break;
		fi
	done

	return ${RESULT};
}

if test -n "${SKIP_PYTHON_TESTS}";
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

	if test ${RESULT} -eq ${EXIT_NO_TESTS_RAN};
	then
		RESULT=${EXIT_IGNORE};
	fi
	if test ${RESULT} -ne ${EXIT_SUCCESS} && test ${RESULT} -ne ${EXIT_IGNORE};
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
	if test ${RESULT} -eq ${EXIT_NO_TESTS_RAN};
	then
		RESULT=${EXIT_IGNORE};
	fi
	if test ${RESULT} -ne ${EXIT_SUCCESS} && test ${RESULT} -ne ${EXIT_IGNORE};
	then
		break;
	fi
done

exit ${RESULT};

