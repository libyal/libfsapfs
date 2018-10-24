dnl Checks for required headers and functions
dnl
dnl Version: 20180907

dnl Function to detect if libfsapfs dependencies are available
AC_DEFUN([AX_LIBFSAPFS_CHECK_LOCAL],
  [dnl Check for internationalization functions in libfsapfs/libfsapfs_i18n.c
  AC_CHECK_FUNCS([bindtextdomain])

  dnl Check if library should be build with verbose output
  AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT

  dnl Check if library should be build with debug output
  AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT

  dnl Check if DLL support is needed
  AS_IF(
    [test "x$enable_shared" = xyes],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])
    ])
  ])
])

dnl Function to detect if fsapfstools dependencies are available
AC_DEFUN([AX_FSAPFSTOOLS_CHECK_LOCAL],
  [AC_CHECK_HEADERS([signal.h sys/signal.h unistd.h])

  AC_CHECK_FUNCS([close getopt setvbuf])

  AS_IF(
   [test "x$ac_cv_func_close" != xyes],
   [AC_MSG_FAILURE(
     [Missing function: close],
     [1])
  ])

  dnl Headers included in fsapfstools/fsapfsmount.c
  AC_CHECK_HEADERS([errno.h])

  dnl Functions included in fsapfstools/fsapfsmount.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_FUNCS([getegid geteuid])
  ])

  dnl Check if tools should be build as static executables
  AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES

  dnl Check if DLL support is needed
  AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw*],
      [AC_SUBST(
        [LIBFSAPFS_DLL_IMPORT],
        ["-DLIBFSAPFS_DLL_IMPORT"])
    ])
  ])
])

