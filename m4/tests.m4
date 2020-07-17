dnl Functions for testing
dnl
dnl Version: 20200712

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

