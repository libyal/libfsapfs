dnl Checks for zlib required headers and functions
dnl
dnl Version: 20201230

dnl Function to detect if zlib is available
AC_DEFUN([AX_ZLIB_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_zlib" = xno],
    [ac_cv_zlib=no],
    [ac_cv_zlib=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_zlib" != x && test "x$ac_cv_with_zlib" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_zlib"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_zlib}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_zlib}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_zlib],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [zlib],
          [zlib >= 1.2.5],
          [ac_cv_zlib=zlib],
          [ac_cv_zlib=check])
        ])
      AS_IF(
        [test "x$ac_cv_zlib" = xzlib],
        [ac_cv_zlib_CPPFLAGS="$pkg_cv_zlib_CFLAGS"
        ac_cv_zlib_LIBADD="$pkg_cv_zlib_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_zlib" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([zlib.h])

      AS_IF(
        [test "x$ac_cv_header_zlib_h" = xno],
        [ac_cv_zlib=no],
        [dnl Check for the individual functions
        ac_cv_zlib=zlib

        AC_CHECK_LIB(
          z,
          zlibVersion,
          [],
          [ac_cv_zlib=no])

        AS_IF(
          [test "x$ac_cv_lib_z_zlibVersion" = xno],
          [AC_MSG_FAILURE(
            [Missing function: zlibVersion in library: zlib.],
            [1])
          ])

        ac_cv_zlib_LIBADD="-lz";
        ])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_zlib" = xzlib],
    [AC_DEFINE(
      [HAVE_ZLIB],
      [1],
      [Define to 1 if you have the 'zlib' library (-lz).])
    ])

  AS_IF(
    [test "x$ac_cv_zlib" != xno],
    [AC_SUBST(
      [HAVE_ZLIB],
      [1]) ],
    [AC_SUBST(
      [HAVE_ZLIB],
      [0])
    ])
  ])

dnl Function to detect if the adler32 function is available
AC_DEFUN([AX_ZLIB_CHECK_ADLER32],
  [AX_COMMON_ARG_WITH(
    [adler32],
    [adler32],
    [specify which alder32 implementation to use, options: 'auto-detect', 'zlib' or 'local'],
    [auto-detect],
    [auto-detect])

  AS_IF(
    [test "x$ac_cv_zlib" != xzlib],
    [ac_cv_adler32=local],
    [AC_CHECK_LIB(
      z,
      adler32,
      [ac_zlib_dummy=yes])

    AS_IF(
      [test "x$ac_cv_lib_z_adler32" = xyes],
      [AS_IF(
        [test "x$ac_cv_with_adler32" != xzlib && test "x$ac_cv_with_adler32" != xauto-detect],
        [ac_cv_adler32=local],
        [AC_DEFINE(
          [HAVE_ZLIB_ADLER32],
          [1],
          [Define to 1 if adler32 funtion is available in zlib.])
        ac_cv_adler32=zlib])],
      [AS_IF(
        [test "x$ac_cv_with_adler32" = xzlib],
        [AC_MSG_FAILURE(
          [Missing function: adler32 in library: zlib.],
          [1])
        ])
      ac_cv_adler32=local])
    ])
  ])

dnl Function to detect if the compress2 function is available
AC_DEFUN([AX_ZLIB_CHECK_COMPRESS2],
  [AS_IF(
    [test "x$ac_cv_zlib" = xzlib],
    [AC_CHECK_LIB(
      z,
      compress2,
      [ac_zlib_dummy=yes])

    AS_IF(
      [test "x$ac_cv_lib_z_compress2" = xyes],
      [AC_DEFINE(
        [HAVE_ZLIB_COMPRESS2],
        [1],
        [Define to 1 if compress2 funtion is available in zlib.])
      ])
    ])
  ])

dnl Function to detect if the compressBound function is available
AC_DEFUN([AX_ZLIB_CHECK_COMPRESSBOUND],
  [AS_IF(
    [test "x$ac_cv_zlib" = xzlib],
    [AC_CHECK_LIB(
      z,
      compressBound,
      [ac_zlib_dummy=yes])

    AS_IF(
      [test "x$ac_cv_lib_z_compressBound" = xyes],
      [AC_DEFINE(
        [HAVE_ZLIB_COMPRESSBOUND],
        [1],
        [Define to 1 if compressBound funtion is available in zlib.])
      ])
    ])
  ])

dnl Function to detect if the inflate functions are available
AC_DEFUN([AX_ZLIB_CHECK_INFLATE],
  [AS_IF(
    [test "x$ac_cv_zlib" != xzlib],
    [ac_cv_inflate=local],
    [AC_CHECK_LIB(
      z,
      inflate,
      [ac_cv_inflate=zlib],
      [ac_cv_inflate=local])

    AC_CHECK_LIB(
      z,
      inflateInit2,
      [ac_zlib_dummy=yes])

    dnl Some versions of zlib provide inflateInit2_ instead of inflateInit2
    AS_IF(
      [test "x$ac_cv_lib_z_inflateinit2" = xno],
      [AC_CHECK_LIB(
        z,
        inflateInit2_,
        [ac_zlib_dummy=yes],
        [ac_cv_inflate=local])
      ])

    AC_CHECK_LIB(
      z,
      inflateEnd,
      [ac_zlib_dummy=yes],
      [ac_cv_inflate=local])
    AC_CHECK_LIB(
      z,
      inflateGetDictionary,
      [ac_zlib_dummy=yes],
      [ac_cv_inflate=local])
    AC_CHECK_LIB(
      z,
      inflatePrime,
      [ac_zlib_dummy=yes],
      [ac_cv_inflate=local])
    AC_CHECK_LIB(
      z,
      inflateSetDictionary,
      [ac_zlib_dummy=yes],
      [ac_cv_inflate=local])

    AS_IF(
      [test "x$ac_cv_inflate" = xzlib],
      [AC_DEFINE(
        [HAVE_ZLIB_INFLATE],
        [1],
        [Define to 1 if you have the `inflateInit', `inflate', `inflateEnd' functions.])
      ])

    AS_IF(
      [test "x$ac_cv_lib_z_inflateinit2" != xno],
      [AC_DEFINE(
        [HAVE_ZLIB_INFLATE_INIT2],
        [1],
        [Define to 1 if you have the `inflateInit2' function.])
      ])
    ])
  ])

dnl Function to detect if the uncompress function is available
AC_DEFUN([AX_ZLIB_CHECK_UNCOMPRESS],
  [AS_IF(
    [test "x$ac_cv_zlib" != xzlib],
    [ac_cv_uncompress=local],
    [AC_CHECK_LIB(
      z,
      uncompress,
      [ac_cv_uncompress=zlib],
      [ac_cv_uncompress=local])

    AS_IF(
      [test "x$ac_cv_uncompress" = xzlib],
      [AC_DEFINE(
        [HAVE_ZLIB_UNCOMPRESS],
        [1],
        [Define to 1 if you have the `uncompress' function.])
      ])
    ])
  ])

dnl Function to detect if the uncompress2 function is available
AC_DEFUN([AX_ZLIB_CHECK_UNCOMPRESS2],
  [AS_IF(
    [test "x$ac_cv_zlib" != xzlib],
    [ac_cv_uncompress2=local],
    [AC_CHECK_LIB(
      z,
      uncompress2,
      [ac_cv_uncompress2=zlib],
      [ac_cv_uncompress2=local])

    AS_IF(
      [test "x$ac_cv_uncompress2" = xzlib],
      [AC_DEFINE(
        [HAVE_ZLIB_UNCOMPRESS2],
        [1],
        [Define to 1 if you have the `uncompress2' function.])
      ])
    ])
  ])

dnl Function to detect how to enable zlib
AC_DEFUN([AX_ZLIB_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [zlib],
    [zlib],
    [search for zlib in includedir and libdir or in the specified DIR, or no if not to use zlib],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_ZLIB_CHECK_LIB

  AS_IF(
    [test "x$ac_cv_zlib_CPPFLAGS" != "x"],
    [AC_SUBST(
      [ZLIB_CPPFLAGS],
      [$ac_cv_zlib_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_zlib_LIBADD" != "x"],
    [AC_SUBST(
      [ZLIB_LIBADD],
      [$ac_cv_zlib_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_zlib" = xzlib],
    [AC_SUBST(
      [ax_zlib_pc_libs_private],
      [-lz])
    ])

  AS_IF(
    [test "x$ac_cv_zlib" = xzlib],
    [AC_SUBST(
      [ax_zlib_spec_requires],
      [zlib])
    AC_SUBST(
      [ax_zlib_spec_build_requires],
      [zlib-devel])
    AC_SUBST(
      [ax_zlib_static_spec_requires],
      [zlib-static])
    AC_SUBST(
      [ax_zlib_static_spec_build_requires],
      [zlib-static])
    ])
  ])

