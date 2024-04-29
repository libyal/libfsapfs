dnl Checks for libfuse required headers and functions
dnl
dnl Version: 20240413

dnl Function to detect if libfuse is available
dnl ac_libfuse_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFUSE_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfuse" = xno],
    [ac_cv_libfuse=no],
    [dnl Check if the directory provided as parameter exists
    dnl For both --with-libfuse which returns "yes" and --with-libfuse= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libfuse" != x && test "x$ac_cv_with_libfuse" != xauto-detect && test "x$ac_cv_with_libfuse" != xyes],
      [AS_IF(
        [test -d "$ac_cv_with_libfuse"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfuse}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfuse}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfuse],
          [1])
        ])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [fuse3],
          [fuse3 >= 3.0],
          [ac_cv_libfuse=libfuse3],
          [ac_cv_libfuse=no])

        AS_IF(
          [test "x$ac_cv_libfuse" = xno],
          [PKG_CHECK_MODULES(
            [fuse],
            [fuse >= 2.6],
            [ac_cv_libfuse=libfuse],
            [ac_cv_libfuse=no])
          ])

        AS_IF(
          [test "x$ac_cv_libfuse" = xlibfuse3],
          [ac_cv_libfuse_CPPFLAGS="$pkg_cv_fuse3_CFLAGS -D_FILE_OFFSET_BITS=64"
          ac_cv_libfuse_LIBADD="$pkg_cv_fuse3_LIBS"])

        AS_IF(
          [test "x$ac_cv_libfuse" = xlibfuse],
          [ac_cv_libfuse_CPPFLAGS="$pkg_cv_fuse_CFLAGS -D_FILE_OFFSET_BITS=64"
          ac_cv_libfuse_LIBADD="$pkg_cv_fuse_LIBS"])
        ])
      ])

    backup_CPPFLAGS="$CPPFLAGS"

    dnl Check for libfuse and libfuse3
    AS_IF(
      [test "x$ac_cv_libfuse" != xlibfuse && test "x$ac_cv_libfuse" != xlibfuse3],
      [dnl Check for headers

      CPPFLAGS="$backup_CPPFLAGS -DFUSE_USE_VERSION=30"
      AC_CHECK_HEADERS([fuse.h])

      AS_IF(
        [test "x$ac_cv_header_fuse_h" = xyes],
        [ac_cv_libfuse=libfuse3],
        [CPPFLAGS="$backup_CPPFLAGS -DFUSE_USE_VERSION=26"
        AC_CHECK_HEADERS([fuse.h])

        AS_IF(
          [test "x$ac_cv_header_fuse_h" = xyes],
          [ac_cv_libfuse=libfuse])
        ])

      AS_IF(
        [test "x$ac_cv_header_fuse_h" = xno],
        [ac_cv_libfuse=no],
        [dnl Check for the individual functions
        AC_CHECK_LIB(
          fuse,
          fuse_invalidate,
          [ac_cv_libfuse=libfuse],
          [ac_cv_libfuse=libfuse3])

        AC_CHECK_LIB(
          fuse,
          fuse_daemonize,
          [ac_cv_libfuse_dummy=yes],
          [ac_cv_libfuse=no])
        AC_CHECK_LIB(
          fuse,
          fuse_destroy,
          [ac_cv_libfuse_dummy=yes],
          [ac_cv_libfuse=no])
        AC_CHECK_LIB(
          fuse,
          fuse_mount,
          [ac_cv_libfuse_dummy=yes],
          [ac_cv_libfuse=no])
        AC_CHECK_LIB(
          fuse,
          fuse_new,
          [ac_cv_libfuse_dummy=yes],
          [ac_cv_libfuse=no])

        dnl libfuse and libfuse3 require -D_FILE_OFFSET_BITS=64 to be set
        ac_cv_libfuse_CPPFLAGS="-D_FILE_OFFSET_BITS=64"

        AS_IF(
          [test "x$ac_cv_libfuse" = xlibfuse3],
          [ac_cv_libfuse_LIBADD="-lfuse3"],
          [ac_cv_libfuse_LIBADD="-lfuse"])
        ])
      ])

    dnl Check for libosxfuse
    AS_IF(
      [test "x$ac_cv_with_libfuse" != xno && test "x$ac_cv_header_fuse_h" = xno],
      [CPPFLAGS="$backup_CPPFLAGS -DFUSE_USE_VERSION=26"
      AC_CHECK_HEADERS([osxfuse/fuse.h])

      AS_IF(
        [test "x$ac_cv_header_osxfuse_fuse_h" = xno],
        [ac_cv_libfuse=no],
        [dnl Check for the individual functions
        ac_cv_libfuse=libosxfuse

        AC_CHECK_LIB(
          osxfuse,
          fuse_daemonize,
          [ac_cv_libfuse_dummy=yes],
          [ac_cv_libfuse=no])
        AC_CHECK_LIB(
          osxfuse,
          fuse_destroy,
          [ac_cv_libfuse_dummy=yes],
          [ac_cv_libfuse=no])
        AC_CHECK_LIB(
          osxfuse,
          fuse_mount,
          [ac_cv_libfuse_dummy=yes],
          [ac_cv_libfuse=no])
        AC_CHECK_LIB(
          osxfuse,
          fuse_new,
          [ac_cv_libfuse_dummy=yes],
          [ac_cv_libfuse=no])

        dnl libosxfuse requires -D_FILE_OFFSET_BITS=64 to be set
        ac_cv_libfuse_CPPFLAGS="-D_FILE_OFFSET_BITS=64"

        ac_cv_libfuse_LIBADD="-losxfuse";
        ])
      ])

    AS_IF(
      [test "x$ac_cv_libfuse" != xyes && test "x$ac_cv_with_libfuse" != x && test "x$ac_cv_with_libfuse" != xauto-detect && test "x$ac_cv_with_libfuse" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libfuse in directory: $ac_cv_with_libfuse],
        [1])
      ])

    CPPFLAGS="$backup_CPPFLAGS"
    ])

  AS_IF(
    [test "x$ac_cv_libfuse" = xlibfuse],
    [AC_DEFINE(
      [HAVE_LIBFUSE],
      [1],
      [Define to 1 if you have the 'fuse' library (-lfuse).])
    ])
  AS_IF(
    [test "x$ac_cv_libfuse" = xlibfuse3],
    [AC_DEFINE(
      [HAVE_LIBFUSE3],
      [1],
      [Define to 1 if you have the 'fuse3' library (-lfuse3).])
    ])
  AS_IF(
    [test "x$ac_cv_libfuse" = xlibosxfuse],
    [AC_DEFINE(
      [HAVE_LIBOSXFUSE],
      [1],
      [Define to 1 if you have the 'osxfuse' library (-losxfuse).])
    ])

  AS_IF(
    [test "x$ac_cv_libfuse" != xno],
    [AC_SUBST(
      [HAVE_LIBFUSE],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFUSE],
      [0])
    ])
  ])

dnl Function to detect how to enable libfuse
AC_DEFUN([AX_LIBFUSE_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfuse],
    [libfuse],
    [search for libfuse in includedir and libdir or in the specified DIR, or no if not to use libfuse],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFUSE_CHECK_LIB

  AS_IF(
    [test "x$ac_cv_libfuse_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFUSE_CPPFLAGS],
      [$ac_cv_libfuse_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfuse_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFUSE_LIBADD],
      [$ac_cv_libfuse_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfuse" = xlibfuse],
    [AC_SUBST(
      [ax_libfuse_pc_libs_private],
      [-lfuse])
    ])
  AS_IF(
    [test "x$ac_cv_libfuse" = xlibfuse3],
    [AC_SUBST(
      [ax_libfuse_pc_libs_private],
      [-lfuse3])
    ])
  AS_IF(
    [test "x$ac_cv_libfuse" = xlibosxfuse],
    [AC_SUBST(
      [ax_libfuse_pc_libs_private],
      [-losxfuse])
    ])

  AS_IF(
    [test "x$ac_cv_libfuse" = xlibfuse],
    [AC_SUBST(
      [ax_libfuse_spec_requires],
      [fuse-libs])
    AC_SUBST(
      [ax_libfuse_spec_build_requires],
      [fuse-devel])
    ])
  AS_IF(
    [test "x$ac_cv_libfuse" = xlibfuse3],
    [AC_SUBST(
      [ax_libfuse_spec_requires],
      [fuse3-libs])
    AC_SUBST(
      [ax_libfuse_spec_build_requires],
      [fuse3-devel])
    ])
  ])

