dnl Checks for libfguid required headers and functions
dnl
dnl Version: 20240413

dnl Function to detect if libfguid is available
dnl ac_libfguid_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFGUID_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfguid" = xno],
    [ac_cv_libfguid=no],
    [ac_cv_libfguid=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libfguid which returns "yes" and --with-libfguid= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libfguid" != x && test "x$ac_cv_with_libfguid" != xauto-detect && test "x$ac_cv_with_libfguid" != xyes],
      [AS_IF(
        [test -d "$ac_cv_with_libfguid"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfguid}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfguid}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfguid],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfguid],
          [libfguid >= 20120426],
          [ac_cv_libfguid=yes],
          [ac_cv_libfguid=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfguid" = xyes],
        [ac_cv_libfguid_CPPFLAGS="$pkg_cv_libfguid_CFLAGS"
        ac_cv_libfguid_LIBADD="$pkg_cv_libfguid_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfguid" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfguid.h])

      AS_IF(
        [test "x$ac_cv_header_libfguid_h" = xno],
        [ac_cv_libfguid=no],
        [dnl Check for the individual functions
        ac_cv_libfguid=yes

        AC_CHECK_LIB(
          fguid,
          libfguid_get_version,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])

        dnl Identifier functions
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_initialize,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_free,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_copy_from_byte_stream,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_get_string_size,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_copy_to_utf8_string,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_copy_to_utf8_string_with_index,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_copy_to_utf16_string,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_copy_to_utf16_string_with_index,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_copy_to_utf32_string,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])
        AC_CHECK_LIB(
          fguid,
          libfguid_identifier_copy_to_utf32_string_with_index,
          [ac_cv_libfguid_dummy=yes],
          [ac_cv_libfguid=no])

        ac_cv_libfguid_LIBADD="-lfguid"])
      ])

    AS_IF(
      [test "x$ac_cv_libfguid" != xyes && test "x$ac_cv_with_libfguid" != x && test "x$ac_cv_with_libfguid" != xauto-detect && test "x$ac_cv_with_libfguid" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libfguid in directory: $ac_cv_with_libfguid],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfguid" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFGUID],
      [1],
      [Define to 1 if you have the `fguid' library (-lfguid).])
    ])

  AS_IF(
    [test "x$ac_cv_libfguid" = xyes],
    [AC_SUBST(
      [HAVE_LIBFGUID],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFGUID],
      [0])
    ])
  ])

dnl Function to detect if libfguid dependencies are available
AC_DEFUN([AX_LIBFGUID_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfguid_CPPFLAGS="-I../libfguid -I\$(top_srcdir)/libfguid";
  ac_cv_libfguid_LIBADD="../libfguid/libfguid.la";

  ac_cv_libfguid=local
  ])


dnl Function to detect how to enable libfguid
AC_DEFUN([AX_LIBFGUID_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfguid],
    [libfguid],
    [search for libfguid in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFGUID_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfguid" != xyes],
    [AX_LIBFGUID_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFGUID],
      [1],
      [Define to 1 if the local version of libfguid is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFGUID],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFGUID],
    [test "x$ac_cv_libfguid" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfguid_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFGUID_CPPFLAGS],
      [$ac_cv_libfguid_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfguid_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFGUID_LIBADD],
      [$ac_cv_libfguid_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfguid" = xyes],
    [AC_SUBST(
      [ax_libfguid_pc_libs_private],
      [-lfguid])
    ])

  AS_IF(
    [test "x$ac_cv_libfguid" = xyes],
    [AC_SUBST(
      [ax_libfguid_spec_requires],
      [libfguid])
    AC_SUBST(
      [ax_libfguid_spec_build_requires],
      [libfguid-devel])
    ])
  ])

