#!/bin/sh
# Script to generate test_inputs.at
#
# Version: 20260615

ignore_list_add() {
    if ! ignore_list_contains "$1"; then
        IGNORE_LIST="${IGNORE_LIST}|$1"
    fi
}

ignore_list_contains() {
    case "${IGNORE_LIST}|" in
        *"|${1}|"*) return 0 ;;
        *)          return 1 ;;
    esac
}

read_ignore_list() {
    IGNORE_LIST=""

    FILE="$1/ignore"
    if test -f "${FILE}"; then
        while IFS= read -r LINE; do
        if test -n "${LINE}"; then
            ignore_list_add "${LINE}"
        fi
        done < "${FILE}"
    fi
}

options_add() {
    if test -n "${OPTIONS}"; then
        OPTIONS="${OPTIONS} $1"
    else
        OPTIONS="$1"
    fi
}

read_options() {
    OPTIONS=""

    FILE="$1/$2.$3"
    if test -f "${FILE}"; then
        # The original format of the options file contains all options on the first line.
        OPTIONS=`head -n 1 "${FILE}" | sed 's/[\r\n]*$//'`

        # The newer format of the options file has a distinct header.
        if test "${OPTIONS}" = "# libyal test data options"; then
            OPTIONS=""
            while IFS= read -r LINE; do
                case "${LINE}" in
                    offset=*)            OPTION="-o${LINE#offset=}" ;;
                    password=*)          OPTION="-p${LINE#password=}" ;;
                    recovery_password=*) OPTION="-r${LINE#recovery_password=}" ;;
                    startup_key=*)       OPTION="-s${LINE#startup_key=}" ;;
                    virtual_address=*)   OPTION="-v${LINE#virtual_address=}" ;;
                    *)                   OPTION= ;;
                esac
            if test -n "${OPTION}"; then
                options_add "${OPTION}"
            fi
            done < "${FILE}"
        fi
    fi
}

test_files_push() {
    if test -n "${TEST_FILES}"; then
        TEST_FILES="${TEST_FILES}|$1"
    else
        TEST_FILES="$1"
    fi
}

test_files_pop() {
    if test -z "${TEST_FILES}" || test "${TEST_FILES}" = "|"; then
        TEST_FILE=""
    else
        TEST_FILE="${TEST_FILES%%|*}"

        case "${TEST_FILES}" in
            *\|*) TEST_FILES="${TEST_FILES#*|}" ;;
            *)    TEST_FILES="" ;;
        esac
    fi
}

glob_test_files() {
    TEST_FILES=""

    for LINE in "$1"/${INPUT_GLOB}; do
        if test -e "${LINE}"; then
            test_files_push "${LINE}"
        fi
    done
}

read_test_files() {
    TEST_FILES=""

    FILE="$1/files"
    if test -f "${FILE}"; then
        while IFS= read -r LINE; do
        if test -n "${LINE}"; then
            test_files_push "$2/${LINE}"
        fi
        done < "${FILE}"
    fi
}

test_inputs_push() {
    if test -n "${TEST_INPUTS}"; then
        TEST_INPUTS="${TEST_INPUTS}|$1"
    else
        TEST_INPUTS="$1"
    fi
}

test_inputs_pop() {
    if test -z "${TEST_INPUTS}" || test "${TEST_INPUTS}" = "|"; then
        TEST_INPUT=""
    else
        TEST_INPUT="${TEST_INPUTS%%|*}"

        case "${TEST_INPUTS}" in
            *\|*) TEST_INPUTS="${TEST_INPUTS#*|}" ;;
            *)    TEST_INPUTS="" ;;
        esac
    fi
}

option_sets_pop() {
    if test -z "${OPTION_SETS}" || test "${OPTION_SETS}" = "|"; then
        OPTION_SET=""
    else
        OPTION_SET="${OPTION_SETS%%|*}"

        case "${OPTION_SETS}" in
            *\|*) OPTION_SETS="${OPTION_SETS#*|}" ;;
            *)    OPTION_SETS="" ;;
        esac
    fi
}

read_project_ini() {
    INPUT_GLOB="*"
    OPTION_SETS=""

    FILE="../$1.ini"
    if test -f "${FILE}"; then
        CURRENT_SECTION=""

        while IFS= read -r LINE || test -n "${LINE}"; do
            LINE="${LINE%"${LINE##*[!$'\r']}"}"

            case "${LINE}" in
                \[*\])
                    CURRENT_SECTION="${LINE#\[}"
                    CURRENT_SECTION="${CURRENT_SECTION%\]}"
                    ;;

                *)
                    if test "${CURRENT_SECTION}" = "tests"; then
                        case "${LINE}" in
                            input_glob:*)
                                INPUT_GLOB=`printf '%s' "${LINE#*:}" | tr -d '" '`
                                ;;

                            option_sets:*)
                                OPTION_SETS=`printf '%s' "${LINE#*:}" | tr -d '[]" ' | tr ',' '|'`
                                ;;
                        esac
                    fi
                    ;;
            esac
        done < "${FILE}"
    fi
}

PACKAGE_NAME="$1"
TEST_PROFILE="$2"
AT_FILE="test_inputs_${TEST_PROFILE}.at"

IGNORE_LIST=""
INPUT="input"
INPUT_GLOB="*"
OPTION_SETS=""
OPTIONS=""
TEST_FILES=""
TEST_INPUTS=""

read_project_ini "${PACKAGE_NAME}"

BACKUP_OPTION_SETS="${OPTION_SETS}"

if test -d "${INPUT}"; then
    read_ignore_list "${INPUT}/.${TEST_PROFILE}"

    for TEST_SET in "${INPUT}"/*; do
        TEST_SET=`basename "${TEST_SET}"`

        if test ! -d "${INPUT}/${TEST_SET}"; then
            echo "Skipping '${TEST_SET}' not a directory"
            continue
        fi
        if ignore_list_contains "${TEST_SET}"; then
            echo "Skipping '${TEST_SET}' defined in ignore list"
            continue
        fi
        if test -f "${INPUT}/.${TEST_PROFILE}/${TEST_SET}/files"; then
            GLOB_FILES=0
        else
            GLOB_FILES=1
        fi
        if test ${GLOB_FILES} -eq 0; then
            read_test_files "${INPUT}/.${TEST_PROFILE}/${TEST_SET}" "${INPUT}/${TEST_SET}"
        else
            glob_test_files "${INPUT}/${TEST_SET}"
        fi
        if test -z "${TEST_FILES}"; then
            echo "Skipping '${TEST_SET}' no test files"
            continue
        fi
        while test_files_pop; test -n "${TEST_FILE}"; do
            TEST_FILENAME=`basename "${TEST_FILE}"`

            if test ${GLOB_FILES} -eq 0 && test ! -f "${TEST_FILE}"; then
                echo "Skipping missing file '${TEST_FILENAME}' defined in '${TEST_SET}/files'"
                continue
            fi
            TEST_FILE="${TEST_FILE#*input/}"

            # Escape [ and ] as @<:@ and @:>@
            TEST_FILE=`echo "${TEST_FILE}" | sed 's/\[/@<:@/g;s/\]/@:>@/g'`

            OPTIONS_PER_PROFILE=""
            OPTIONS_FILE="${INPUT}/.${TEST_PROFILE}/options"
            if test -f "${OPTIONS_FILE}"; then
                OPTIONS_PER_PROFILE=`head -n 1 "${OPTIONS_FILE}" | sed 's/[\r\n]*$//'`
            fi
            TEST_WITH_OPTIONS=0

            OPTION_SETS="${BACKUP_OPTION_SETS}"
            while option_sets_pop; test -n "${OPTION_SET}"; do
                read_options "${INPUT}/.${TEST_PROFILE}/${TEST_SET}" "${TEST_FILENAME}" "${OPTION_SET}"

                if test -n "${OPTIONS}"; then
                    if test -n "${OPTIONS_PER_PROFILE}"; then
                        OPTIONS="${OPTIONS_PER_PROFILE} ${OPTIONS}"
                    fi
                    TEST_INPUT="[${OPTION_SET}], [${OPTIONS}], [${TEST_FILE}]"
                    test_inputs_push "${TEST_INPUT}"

                    TEST_WITH_OPTIONS=1
                fi
            done

            if test ${TEST_WITH_OPTIONS} -eq 0; then
                TEST_INPUT="[], [${OPTIONS_PER_PROFILE}], [${TEST_FILE}]"
                test_inputs_push "${TEST_INPUT}"
            fi
        done
    done
fi

cat <<EOT >"${AT_FILE}"
dnl Automatically generated by setup_test_input.sh
dnl A test input consist of: [[[option set], [options], [input file]]]

EOT

COUNTER=1
ENTRIES=""

while test_inputs_pop; test -n "${TEST_INPUT}"; do
    echo "m4_define([_test_inputs_${TEST_PROFILE}_${COUNTER}], [[${TEST_INPUT}]])" >> "${AT_FILE}"

    ENTRY="_test_inputs_${TEST_PROFILE}_${COUNTER}"
    if test -n "${ENTRIES}"; then
        ENTRIES="${ENTRIES} ${ENTRY}"
    else
        ENTRIES="${ENTRY}"
    fi
    COUNTER=`expr "${COUNTER}" + 1`
done

echo "" >> "${AT_FILE}"
echo -n "m4_define([test_inputs_${TEST_PROFILE}], [[${ENTRIES}]])" >> "${AT_FILE}"
