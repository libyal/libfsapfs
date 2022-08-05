dnl Checks for libfmos required headers and functions
dnl
dnl Version: 20220804

dnl Function to detect if libfmos is available
dnl ac_libfmos_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFMOS_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfmos" = xno],
    [ac_cv_libfmos=no],
    [ac_cv_libfmos=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libfmos" != x && test "x$ac_cv_with_libfmos" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libfmos"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfmos}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfmos}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfmos],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfmos],
          [libfmos >= 20191217],
          [ac_cv_libfmos=yes],
          [ac_cv_libfmos=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfmos" = xyes],
        [ac_cv_libfmos_CPPFLAGS="$pkg_cv_libfmos_CFLAGS"
        ac_cv_libfmos_LIBADD="$pkg_cv_libfmos_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfmos" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfmos.h])

      AS_IF(
        [test "x$ac_cv_header_libfmos_h" = xno],
        [ac_cv_libfmos=no],
        [dnl Check for the individual functions
        ac_cv_libfmos=yes

        AC_CHECK_LIB(
          fmos,
          libfmos_get_version,
          [ac_cv_libfmos_dummy=yes],
          [ac_cv_libfmos=no])

        dnl ADC functions
        AC_CHECK_LIB(
          fmos,
          libfmos_adc_decompress,
          [ac_cv_libfmos_dummy=yes],
          [ac_cv_libfmos=no])

        dnl LZFSE functions
        AC_CHECK_LIB(
          fmos,
          libfmos_lzfse_decompress,
          [ac_cv_libfmos_dummy=yes],
          [ac_cv_libfmos=no])

        dnl LZVN functions
        AC_CHECK_LIB(
          fmos,
          libfmos_lzvn_decompress,
          [ac_cv_libfmos_dummy=yes],
          [ac_cv_libfmos=no])

        ac_cv_libfmos_LIBADD="-lfmos"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libfmos" != x && test "x$ac_cv_with_libfmos" != xauto-detect && test "x$ac_cv_libfmos" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libfmos in directory: $ac_cv_with_libfmos],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfmos" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFMOS],
      [1],
      [Define to 1 if you have the `fmos' library (-lfmos).])
    ])

  AS_IF(
    [test "x$ac_cv_libfmos" = xyes],
    [AC_SUBST(
      [HAVE_LIBFMOS],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFMOS],
      [0])
    ])
  ])

dnl Function to detect if libfmos dependencies are available
AC_DEFUN([AX_LIBFMOS_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfmos_CPPFLAGS="-I../libfmos";
  ac_cv_libfmos_LIBADD="../libfmos/libfmos.la";

  ac_cv_libfmos=local
  ])

dnl Function to detect how to enable libfmos
AC_DEFUN([AX_LIBFMOS_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfmos],
    [libfmos],
    [search for libfmos in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFMOS_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfmos" != xyes],
    [AX_LIBFMOS_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFMOS],
      [1],
      [Define to 1 if the local version of libfmos is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFMOS],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFMOS],
    [test "x$ac_cv_libfmos" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfmos_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFMOS_CPPFLAGS],
      [$ac_cv_libfmos_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfmos_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFMOS_LIBADD],
      [$ac_cv_libfmos_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfmos" = xyes],
    [AC_SUBST(
      [ax_libfmos_pc_libs_private],
      [-lfmos])
    ])

  AS_IF(
    [test "x$ac_cv_libfmos" = xyes],
    [AC_SUBST(
      [ax_libfmos_spec_requires],
      [libfmos])
    AC_SUBST(
      [ax_libfmos_spec_build_requires],
      [libfmos-devel])
    ])
  ])

