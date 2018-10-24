dnl Functions for libfuse
dnl
dnl Version: 20180403

dnl Function to detect if libfuse is available
dnl ac_libfuse_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFUSE_CHECK_LIB],
  [dnl Check if parameters were provided
  AS_IF(
    [test "x$ac_cv_with_libfuse" != x && test "x$ac_cv_with_libfuse" != xno && test "x$ac_cv_with_libfuse" != xauto-detect],
    [AS_IF(
      [test -d "$ac_cv_with_libfuse"],
      [CFLAGS="$CFLAGS -I${ac_cv_with_libfuse}/include"
      LDFLAGS="$LDFLAGS -L${ac_cv_with_libfuse}/lib"],
      [AC_MSG_WARN([no such directory: $ac_cv_with_libfuse])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_with_libfuse" = xno],
    [ac_cv_libfuse=no],
    [dnl Check for a pkg-config file
    AS_IF(
      [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
      [PKG_CHECK_MODULES(
        [fuse],
        [fuse >= 2.6],
        [ac_cv_libfuse=libfuse],
        [ac_cv_libfuse=no])
      ])

    AS_IF(
      [test "x$ac_cv_libfuse" = xlibfuse],
      [ac_cv_libfuse_CPPFLAGS="$pkg_cv_fuse_CFLAGS"
      ac_cv_libfuse_LIBADD="$pkg_cv_fuse_LIBS"],
      [dnl Check for headers
      AC_CHECK_HEADERS(
        [fuse.h],
        [ac_cv_header_fuse_h=yes],
        [ac_cv_header_fuse_h=no])

      dnl libfuse sometimes requires -D_FILE_OFFSET_BITS=64 to be set
      AS_IF(
        [test "x$ac_cv_header_fuse_h" = xno],
        [AS_UNSET([ac_cv_header_fuse_h])
        CPPFLAGS="$CPPFLAGS -D_FILE_OFFSET_BITS=64"
        AC_CHECK_HEADERS([fuse.h])
      ])

      AS_IF(
        [test "x$ac_cv_header_fuse_h" = xno],
        [ac_cv_libfuse=no],
        [dnl Check for the individual functions
        ac_cv_libfuse=libfuse

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

        ac_cv_libfuse_LIBADD="-lfuse";
        ])
      ])

    dnl Check for libosxfuse
    AS_IF(
      [test "x$ac_cv_with_libfuse" != xno && test "x$ac_cv_header_fuse_h" = xno],
      [CPPFLAGS="$CPPFLAGS -DFUSE_USE_VERSION=26"
      AC_CHECK_HEADERS([osxfuse/fuse.h])

      dnl libosxfuse sometimes requires -D_FILE_OFFSET_BITS=64 to be set
      AS_IF(
        [test "x$ac_cv_header_osxfuse_fuse_h" = xno],
        [AS_UNSET([ac_cv_header_osxfuse_fuse_h])
        CPPFLAGS="$CPPFLAGS -D_FILE_OFFSET_BITS=64"
        AC_CHECK_HEADERS([osxfuse/fuse.h])
      ])

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

        ac_cv_libfuse_LIBADD="-losxfuse";
        ])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfuse" = xlibfuse],
    [AC_DEFINE(
      [HAVE_LIBFUSE],
      [1],
      [Define to 1 if you have the 'fuse' library (-lfuse).])
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
  ])

