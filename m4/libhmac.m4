dnl Checks for libhmac required headers and functions
dnl
dnl Version: 20200104

dnl Function to detect if libhmac is available
dnl ac_libhmac_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBHMAC_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libhmac" = xno],
    [ac_cv_libhmac=no],
    [ac_cv_libhmac=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libhmac" != x && test "x$ac_cv_with_libhmac" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libhmac"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libhmac}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libhmac}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libhmac],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libhmac],
          [libhmac >= 20200104],
          [ac_cv_libhmac=yes],
          [ac_cv_libhmac=check])
        ])
      AS_IF(
        [test "x$ac_cv_libhmac" = xyes],
        [ac_cv_libhmac_CPPFLAGS="$pkg_cv_libhmac_CFLAGS"
        ac_cv_libhmac_LIBADD="$pkg_cv_libhmac_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libhmac" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libhmac.h])

      AS_IF(
        [test "x$ac_cv_header_libhmac_h" = xno],
        [ac_cv_libhmac=no],
        [dnl Check for the individual functions
        ac_cv_libhmac=yes

        AC_CHECK_LIB(
          hmac,
          libhmac_get_version,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])

        dnl MD5 functions
        AC_CHECK_LIB(
          hmac,
          libhmac_md5_initialize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_md5_update,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_md5_finalize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_md5_free,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])

        dnl SHA1 functions
        AC_CHECK_LIB(
          hmac,
          libhmac_sha1_initialize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha1_update,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha1_finalize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha1_free,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])

        dnl SHA224 functions
        AC_CHECK_LIB(
          hmac,
          libhmac_sha224_initialize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha224_update,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha224_finalize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha224_free,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])

        dnl SHA256 functions
        AC_CHECK_LIB(
          hmac,
          libhmac_sha256_initialize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha256_update,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha256_finalize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha256_free,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])

        dnl SHA512 functions
        AC_CHECK_LIB(
          hmac,
          libhmac_sha512_initialize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha512_update,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha512_finalize,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])
        AC_CHECK_LIB(
          hmac,
          libhmac_sha512_free,
          [ac_cv_libhmac_dummy=yes],
          [ac_cv_libhmac=no])

        ac_cv_libhmac_LIBADD="-lhmac"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libhmac" != x && test "x$ac_cv_with_libhmac" != xauto-detect && test "x$ac_cv_libhmac" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libhmac in directory: $ac_cv_with_libhmac],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libhmac" = xyes],
    [AC_DEFINE(
      [HAVE_LIBHMAC],
      [1],
      [Define to 1 if you have the `hmac' library (-lhmac).])
    ])

  AS_IF(
    [test "x$ac_cv_libhmac" = xyes],
    [ac_cv_libhmac_md5=libhmac
    ac_cv_libhmac_sha1=libhmac
    ac_cv_libhmac_sha224=libhmac
    ac_cv_libhmac_sha256=libhmac
    ac_cv_libhmac_sha512=libhmac],
    [ac_cv_libhmac_md5=no
    ac_cv_libhmac_sha1=no
    ac_cv_libhmac_sha224=no
    ac_cv_libhmac_sha256=no
    ac_cv_libhmac_sha512=no])

  AS_IF(
    [test "x$ac_cv_libhmac" = xyes],
    [AC_SUBST(
      [HAVE_LIBHMAC],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBHMAC],
      [0])
    ])
  ])

dnl Function to detect if libhmac dependencies are available
AC_DEFUN([AX_LIBHMAC_CHECK_LOCAL],
  [dnl Check for libcrypto (openssl) support
  AX_LIBCRYPTO_CHECK_ENABLE

  AS_IF(
    [test "x$ac_cv_libcrypto" != xno],
    [AX_LIBCRYPTO_CHECK_MD5
    AX_LIBCRYPTO_CHECK_SHA1
    AX_LIBCRYPTO_CHECK_SHA224
    AX_LIBCRYPTO_CHECK_SHA256
    AX_LIBCRYPTO_CHECK_SHA512])

  dnl Fallback to local versions if necessary
  AS_IF(
    [test "x$ac_cv_libcrypto" = xno || test "x$ac_cv_libcrypto_md5" = xno],
    [ac_cv_libhmac_md5=local],
    [ac_cv_libhmac_md5=$ac_cv_libcrypto_md5])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno || test "x$ac_cv_libcrypto_sha1" = xno],
    [ac_cv_libhmac_sha1=local],
    [ac_cv_libhmac_sha1=$ac_cv_libcrypto_sha1])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno || test "x$ac_cv_libcrypto_sha224" = xno],
    [ac_cv_libhmac_sha224=local],
    [ac_cv_libhmac_sha224=$ac_cv_libcrypto_sha224])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno || test "x$ac_cv_libcrypto_sha256" = xno],
    [ac_cv_libhmac_sha256=local],
    [ac_cv_libhmac_sha256=$ac_cv_libcrypto_sha256])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno || test "x$ac_cv_libcrypto_sha512" = xno],
    [ac_cv_libhmac_sha512=local],
    [ac_cv_libhmac_sha512=$ac_cv_libcrypto_sha512])

  ac_cv_libhmac_CPPFLAGS="-I../libhmac";
  ac_cv_libhmac_LIBADD="../libhmac/libhmac.la";

  ac_cv_libhmac=local
  ])

dnl Function to detect how to enable libhmac
AC_DEFUN([AX_LIBHMAC_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libhmac],
    [libhmac],
    [search for libhmac in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBHMAC_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libhmac" != xyes],
    [AX_LIBHMAC_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBHMAC],
      [1],
      [Define to 1 if the local version of libhmac is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBHMAC],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBHMAC],
    [test "x$ac_cv_libhmac" = xlocal])
  AS_IF(
    [test "x$ac_cv_libhmac_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBHMAC_CPPFLAGS],
      [$ac_cv_libhmac_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libhmac_LIBADD" != "x"],
    [AC_SUBST(
      [LIBHMAC_LIBADD],
      [$ac_cv_libhmac_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libhmac" = xyes],
    [AC_SUBST(
      [ax_libhmac_pc_libs_private],
      [-lhmac])
    ])

  AS_IF(
    [test "x$ac_cv_libhmac" = xyes],
    [AC_SUBST(
      [ax_libhmac_spec_requires],
      [libhmac])
    AC_SUBST(
      [ax_libhmac_spec_build_requires],
      [libhmac-devel])
    ])
  ])

