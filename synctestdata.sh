#!/bin/sh
# Script that synchronizes the local test data
#
# Version: 20251208

TEST_SET="public";
TEST_INPUT_DIRECTORY="tests/input";
TEST_FILES="apfs.raw";

mkdir -p "${TEST_INPUT_DIRECTORY}/${TEST_SET}";

for TEST_FILE in ${TEST_FILES};
do
	URL="https://github.com/log2timeline/dfvfs/raw/refs/heads/main/test_data/${TEST_FILE}";

	curl -L -o "${TEST_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL};
done

