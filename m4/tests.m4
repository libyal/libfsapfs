dnl Functions for testing
dnl
dnl Version: 20260612

dnl Function to check if pthread_rwlock_unlock can be hooked for testing
AC_DEFUN([AX_TESTS_CHECK_CAN_HOOK_PTHREAD_RWLOCK_UNLOCK],
  [AC_REQUIRE([AC_PROG_CC])

  AC_MSG_CHECKING([whether pthread_rwlock_unlock can be hooked for testing])

  BACKUP_LIBS="$LIBS"
  LIBS="-ldl -lpthread $LIBS"
  AC_LANG_PUSH(C)

  AC_RUN_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU]],
      [[static int (*hooked_pthread_rwlock_unlock)(pthread_rwlock_t *) = NULL;
hooked_pthread_rwlock_unlock = dlsym( RTLD_NEXT, "pthread_rwlock_unlock" );
if( hooked_pthread_rwlock_unlock == NULL ) { return 1; }
pthread_rwlock_t rwlock;
if( pthread_rwlock_init( &rwlock, NULL ) != 0 ) { return 2; }
pthread_rwlock_wrlock( &rwlock );
if( hooked_pthread_rwlock_unlock( &rwlock ) != 0 ) { return 3; }
if( pthread_rwlock_destroy( &rwlock ) != 0 ) { return 4; }]] )],
    [ac_cv_have_pthread_rwlock_unlock_hook=yes],
    [ac_cv_have_pthread_rwlock_unlock_hook=no],
    [ac_cv_have_pthread_rwlock_unlock_hook=undetermined])

  AC_LANG_POP(C)
  LIBS="$BACKUP_LIBS"

  AC_MSG_RESULT(
    [$ac_cv_have_pthread_rwlock_unlock_hook])

  AS_IF(
    [test "x$ac_cv_have_pthread_rwlock_unlock_hook" = xyes],
    [AC_DEFINE(
      [HAVE_PTHREAD_RWLOCK_UNLOCK_HOOK],
      [1],
      [Define to 1 whether pthread_rwlock_unlock can be hooked for testing.])
    ])
  ])

dnl Function to detect whether the file system is case-insensitive
AC_DEFUN([AX_TESTS_CHECK_FILE_SYSTEM_IS_CASE_INSENSITIVE],
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

dnl Function to check for a manpage linting command
AC_DEFUN([AX_TESTS_CHECK_HAVE_MANPAGE_LINTER],
  [AC_PATH_PROG([MAN], [man])
  AC_PATH_PROG([MANDOC], [mandoc])

  AC_CACHE_CHECK(
    [for manpage linter],
    [ac_cv_have_manpage_linter],
    [ac_cv_have_manpage_linter=no

    printf '.Dd June 10, 2026\n.Dt TITLE 1\n.Os\n.Sh NAME\n.Nm command\n.Nd example\n' > conftest.1

    AS_IF(
      [test "x$MAN" != x],
      [LC_ALL="en_US.UTF-8" MANROFFSEQ="" MANWIDTH="80" "$MAN" --warnings -E UTF-8 -l -Tutf8 -Z conftest.1 >/dev/null 2>&1
      AS_IF(
        [test $? -eq 0],
	[ac_cv_have_manpage_linter="man-db"])
      ])

    AS_IF(
      [test "x$ac_cv_have_manpage_linter" = xno && test "x$MANDOC" != x],
      [LC_ALL="en_US.UTF-8" "$MANDOC" -O width=80 -T lint -W all ./conftest.1 >/dev/null 2>&1

      AS_IF(
        [test $? -eq 0],
	[ac_cv_have_manpage_linter="mandoc"])
      ])

    rm -f conftest.1
    ])
  ])

dnl Function to check whether libasan is functional
AC_DEFUN([AX_TESTS_CHECK_LIBASAN],
  [AC_MSG_CHECKING([whether libasan is functional])

  ASAN_CFLAGS="-fno-omit-frame-pointer -fsanitize=address -g -O0"
  ASAN_LDFLAGS="-fsanitize=address"

  BACKUP_CFLAGS="$CFLAGS"
  BACKUP_LDFLAGS="$LDFLAGS"
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
    [ac_cv_have_ubsan=yes])

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

  AX_TESTS_CHECK_CAN_HOOK_PTHREAD_RWLOCK_UNLOCK
  AX_TESTS_CHECK_FILE_SYSTEM_IS_CASE_INSENSITIVE

  dnl Check for programs used in test_macros.at
  AC_PATH_PROG([CYGPATH], [cygpath])
  AC_PATH_PROG([WINEPATH], [winepath])

  dnl Check for programs used in test_manpages.at
  AX_TESTS_CHECK_HAVE_MANPAGE_LINTER

  dnl Check for programs used in test_tools.at
  AC_PROG_SED

  dnl Note cannot use GZIP given it has a special purpose within gzip
  AC_PATH_PROG(GZIP_COMMAND, [gzip])
  AC_PATH_PROGS([MD5SUM], [md5sum md5])
  AC_PATH_PROG(ZCAT, [zcat])

  AC_SUBST(
    [TESTS_MANPAGE_LINTER],
    [$ac_cv_have_manpage_linter])

  ac_tests_target_string="$target"

  AS_IF(
    [test "x$ac_tests_target_string" = x],
    [ac_tests_target_string="$host"])

  AS_CASE(
    [$ac_tests_target_string],
    [*mingw*],[ac_cv_tests_use_mingw=yes],
    [*],[ac_cv_tests_use_mingw=no])

  AC_SUBST(
    [TESTS_USE_MINGW],
    [$ac_cv_tests_use_mingw])
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

