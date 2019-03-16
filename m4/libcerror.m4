dnl Checks for libcerror required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libcerror is available
dnl ac_libcerror_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCERROR_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcerror" = xno],
    [ac_cv_libcerror=no],
    [ac_cv_libcerror=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libcerror" != x && test "x$ac_cv_with_libcerror" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libcerror"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libcerror}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libcerror}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libcerror],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcerror],
          [libcerror >= 20120425],
          [ac_cv_libcerror=yes],
          [ac_cv_libcerror=check])
        ])
      AS_IF(
        [test "x$ac_cv_libcerror" = xyes],
        [ac_cv_libcerror_CPPFLAGS="$pkg_cv_libcerror_CFLAGS"
        ac_cv_libcerror_LIBADD="$pkg_cv_libcerror_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libcerror" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libcerror.h])

      AS_IF(
        [test "x$ac_cv_header_libcerror_h" = xno],
        [ac_cv_libcerror=no],
        [dnl Check for the individual functions
        ac_cv_libcerror=yes

        AC_CHECK_LIB(
          cerror,
          libcerror_get_version,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])

        dnl Error functions
        AC_CHECK_LIB(
          cerror,
          libcerror_error_free,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])
        AC_CHECK_LIB(
          cerror,
          libcerror_error_set,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])
        AC_CHECK_LIB(
          cerror,
          libcerror_error_matches,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])
        AC_CHECK_LIB(
          cerror,
          libcerror_error_fprint,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])
        AC_CHECK_LIB(
          cerror,
          libcerror_error_sprint,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])
        AC_CHECK_LIB(
          cerror,
          libcerror_error_backtrace_fprint,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])
        AC_CHECK_LIB(
          cerror,
          libcerror_error_backtrace_sprint,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])

        dnl System error functions
        AC_CHECK_LIB(
          cerror,
          libcerror_system_set_error,
          [ac_cv_libcerror_dummy=yes],
          [ac_cv_libcerror=no])

        ac_cv_libcerror_LIBADD="-lcerror"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libcerror" != x && test "x$ac_cv_with_libcerror" != xauto-detect && test "x$ac_cv_libcerror" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libcerror in directory: $ac_cv_with_libcerror],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libcerror" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCERROR],
      [1],
      [Define to 1 if you have the `cerror' library (-lcerror).])
    ])

  AS_IF(
    [test "x$ac_cv_libcerror" = xyes],
    [AC_SUBST(
      [HAVE_LIBCERROR],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCERROR],
      [0])
    ])
  ])

dnl Function to detect if libcerror dependencies are available
AC_DEFUN([AX_LIBCERROR_CHECK_LOCAL],
  [dnl Headers included in libcerror/libcerror_error.c
  AC_CHECK_HEADERS([stdarg.h varargs.h])

  AS_IF(
    [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
    [AC_MSG_FAILURE(
      [Missing headers: stdarg.h and varargs.h],
      [1])
    ])

  dnl Wide character string functions used in libcerror/libcerror_error.c
  AS_IF(
    [test "x$ac_cv_enable_wide_character_type" != xno],
    [AC_CHECK_FUNCS([wcstombs])

    AS_IF(
      [test "x$ac_cv_func_wcstombs" != xyes],
      [AC_MSG_FAILURE(
        [Missing function: wcstombs],
        [1])
      ])
    ])

  dnl Check for error string functions used in libcerror/libcerror_system.c
  AC_FUNC_STRERROR_R()

  AS_IF(
    [test "x$ac_cv_have_decl_strerror_r" != xyes],
    [AC_CHECK_FUNCS([strerror])

    AS_IF(
      [test "x$ac_cv_func_strerror" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: strerror_r and strerror],
        [1])
      ])
    ])

  ac_cv_libcerror_CPPFLAGS="-I../libcerror";
  ac_cv_libcerror_LIBADD="../libcerror/libcerror.la";

  ac_cv_libcerror=local
  ])

dnl Function to detect how to enable libcerror
AC_DEFUN([AX_LIBCERROR_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libcerror],
    [libcerror],
    [search for libcerror in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBCERROR_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libcerror" != xyes],
    [AX_LIBCERROR_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBCERROR],
      [1],
      [Define to 1 if the local version of libcerror is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBCERROR],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCERROR],
    [test "x$ac_cv_libcerror" = xlocal])
  AS_IF(
    [test "x$ac_cv_libcerror_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCERROR_CPPFLAGS],
      [$ac_cv_libcerror_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcerror_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCERROR_LIBADD],
      [$ac_cv_libcerror_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcerror" = xyes],
    [AC_SUBST(
      [ax_libcerror_pc_libs_private],
      [-lcerror])
    ])

  AS_IF(
    [test "x$ac_cv_libcerror" = xyes],
    [AC_SUBST(
      [ax_libcerror_spec_requires],
      [libcerror])
    AC_SUBST(
      [ax_libcerror_spec_build_requires],
      [libcerror-devel])
    ])
  ])

