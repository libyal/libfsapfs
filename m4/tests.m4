dnl Functions for testing
dnl
dnl Version: 20180905

dnl Function to detect if tests dependencies are available
AC_DEFUN([AX_TESTS_CHECK_LOCAL],
  [AC_CHECK_HEADERS([dlfcn.h])

  AC_CHECK_FUNCS([fmemopen getopt mkstemp unlink])

  AC_CHECK_LIB(
    dl,
    dlsym)

  AS_IF(
    [test "x$lt_cv_prog_gnu_ld" = xyes && test "x$ac_cv_lib_dl_dlsym" = xyes],
    [AC_DEFINE(
      [HAVE_GNU_DL_DLSYM],
      [1],
      [Define to 1 if dlsym funtion is available in GNU dl.])
  ])
])

