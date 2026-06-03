dnl Functions for testing
dnl
dnl Version: 20260601

dnl Function to detect whether the file system is case-insensitive
AC_DEFUN([AX_TEST_CHECK_FILE_SYSTEM_IS_CASE_INSENSITIVE],
  [AC_MSG_CHECKING([whether the file system is case-insensitive])

  as_tmp_case_file="conftest.case.tst"
  touch "$as_tmp_case_file" 2>/dev/null

  AS_IF(
    [test -f CONFTEST.CASE.TST],
    [ac_cv_file_system_case_insentive=yes],
    [ac_cv_file_system_case_insentive=no])

  rm -f "$as_tmp_case_file" CONFTEST.CASE.TST 2>/dev/null

  AC_MSG_RESULT(
    [$ac_cv_file_system_case_insentive])

  AS_IF(
    [test "x$ac_cv_file_system_case_insentive" = xyes],
    [AC_DEFINE(
      [HAVE_CASE_INSENSITIVE_FILE_SYSTEM],
      [1],
      [Define to 1 whether file system is case-insensitive.])
    ])
  ])

dnl Function to check whether libasan is functional
AC_DEFUN([AX_TESTS_CHECK_LIBASAN],
  [AC_MSG_CHECKING([whether libasan is functional])

  BACKUP_CFLAGS="$CFLAGS"
  BACKUP_LDFLAGS="$LDFLAGS"

  ASAN_CFLAGS="-fno-omit-frame-pointer -fsanitize=address -g -O0"
  ASAN_LDFLAGS="-fsanitize=address"

  CFLAGS="$CFLAGS $ASAN_CFLAGS"
  LDFLAGS="$LDFLAGS $ASAN_LDFLAGS"

  AC_LANG_PUSH(C)

  AC_RUN_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <stdlib.h>]],
      [[char *array = (char *) malloc(10 * sizeof(char));
if (array == NULL) { return 0; }
array[10] = 42;
free(array);]] )],
      [ac_cv_have_asan=no],
      [ac_cv_have_asan=yes],
      [ac_cv_have_asan=undetermined])

  AC_LANG_POP(C)

  CFLAGS="$BACKUP_CFLAGS"
  LDFLAGS="$BACKUP_LDFLAGS"

  AC_MSG_RESULT(
    [$ac_cv_have_asan])
  ])

dnl Function to check whether libubsan is functional
AC_DEFUN([AX_TESTS_CHECK_LIBUBSAN],
  [AC_MSG_CHECKING([whether libubsan is functional])

  BACKUP_CFLAGS="$CFLAGS"
  BACKUP_LDFLAGS="$LDFLAGS"

  UBSAN_CFLAGS="-fsanitize=undefined"
  UBSAN_LDFLAGS="-fsanitize=undefined"

  CFLAGS="$CFLAGS $UBSAN_CFLAGS"
  LDFLAGS="$LDFLAGS $UBSAN_LDFLAGS"

  AC_LANG_PUSH(C)

  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <stdio.h>]]
      [[int shift = 1;
int negative = -5;
int result = (negative << shift);]] )],
      [ac_cv_have_ubsan=no],
      [ac_cv_have_ubsan=yes],
      [ac_cv_have_ubsan=undetermined])

  AC_LANG_POP(C)

  CFLAGS="$BACKUP_CFLAGS"
  LDFLAGS="$BACKUP_LDFLAGS"

  AC_MSG_RESULT(
    [$ac_cv_have_asan])
  ])

dnl Function to detect whether asan support should be enabled
AC_DEFUN([AX_TESTS_CHECK_ENABLE_ASAN],
  [AX_COMMON_ARG_ENABLE(
    [asan],
    [asan],
    [build with asan],
    [no])

  AS_IF(
    [test "x$ac_cv_enable_asan" != xno],
    [AX_TESTS_CHECK_LIBASAN

    AS_IF(
      [test "x$ac_cv_have_asan" = xno],
      [AC_MSG_FAILURE(
        [Unable to build with functional libasan],
        [1])
      ])

    CFLAGS="$CFLAGS $ASAN_CFLAGS"
    LDFLAGS="$LDFLAGS $ASAN_LDFLAGS"
    ])
  ])

dnl Function to detect whether ubsan support should be enabled
AC_DEFUN([AX_TESTS_CHECK_ENABLE_UBSAN],
  [AX_COMMON_ARG_ENABLE(
    [ubsan],
    [ubsan],
    [build with ubsan],
    [no])

  AS_IF(
    [test "x$ac_cv_enable_ubsan" != xno],
    [AX_TESTS_CHECK_LIBUBSAN

    AS_IF(
      [test "x$ac_cv_have_ubsan" = xno],
      [AC_MSG_FAILURE(
        [Unable to build with functional libubsan],
        [1])
      ])

    CFLAGS="$CFLAGS $UBSAN_CFLAGS -fno-sanitize-recover=undefined"
    LDFLAGS="$LDFLAGS $UBSAN_LDFLAGS"
    ])
  ])

dnl Function to detect whether code coverage support should be enabled
AC_DEFUN([AX_TESTS_CHECK_ENABLE_CODE_COVERAGE],
  [AX_COMMON_ARG_ENABLE(
    [code-coverage],
    [code_coverage],
    [build for code coverage)],
    [no])

  AS_IF(
    [test "x$ac_cv_enable_code_coverage" != xno],
    [AX_COMMON_CHECK_COMPILER_FLAG(-fno-builtin-memcpy)

    AS_IF(
      [test "x$ac_cv_with_fno_builtin_memcpy" != xno],
      [AC_DEFINE(
        [HAVE_NO_BUILTIN_MEMCPY],
        [1],
        [Define to 1 if the compiler supports -fno-builtin-memcpy.])

      CFLAGS="$CFLAGS --coverage -fno-builtin-memcpy -O0"],
      [CFLAGS="$CFLAGS --coverage -O0"])

    CPPFLAGS="$CPPFLAGS -DOPTIMIZATION_DISABLED"
    LDFLAGS="$LDFLAGS --coverage"

    enable_shared=no])
  ])

dnl Function to detect if tests dependencies are available
AC_DEFUN([AX_TESTS_CHECK_LOCAL],
  [AC_CHECK_HEADERS([dlfcn.h])

  AC_CHECK_FUNCS([fmemopen getopt mkstemp setenv tzset unlink])

  AC_CHECK_LIB(
    dl,
    dlsym)

  AS_IF(
    [test "x$lt_cv_prog_gnu_ld" = xyes && test "x$ac_cv_lib_dl_dlsym" = xyes],
    [AC_DEFINE(
      [HAVE_GNU_DL_DLSYM],
      [1],
      [Define to 1 if dlsym function is available in GNU dl.])
    ])

  AX_TEST_CHECK_FILE_SYSTEM_IS_CASE_INSENSITIVE

  AC_SUBST(
    [TESTS_USE_WINAPI],
    [$ac_cv_enable_winapi])
  ])

dnl Function to detect if OSS-Fuzz build environment is available
AC_DEFUN([AX_TESTS_CHECK_OSSFUZZ],
  [AM_CONDITIONAL(
    HAVE_LIB_FUZZING_ENGINE,
    [test "x${LIB_FUZZING_ENGINE}" != x])
  AC_SUBST(
    [LIB_FUZZING_ENGINE],
    ["${LIB_FUZZING_ENGINE}"])
  ])

