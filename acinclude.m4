dnl Checks for required headers and functions
dnl
dnl Version: 20260603

dnl Function to detect if libfsapfs dependencies are available
AC_DEFUN([AX_LIBFSAPFS_CHECK_LOCAL],
  [dnl Check for internationalization functions in libfsapfs/libfsapfs_i18n.c
  AC_CHECK_FUNCS([bindtextdomain])
])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBFSAPFS_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBFSAPFS_DLL_EXPORT],
        ["-DLIBFSAPFS_DLL_EXPORT"])

      AC_SUBST(
        [LIBFSAPFS_DLL_IMPORT],
        ["-DLIBFSAPFS_DLL_IMPORT"])
      ])
    ])
  ])

