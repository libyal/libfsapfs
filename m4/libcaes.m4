dnl Checks for libcaes required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libcaes is available
dnl ac_libcaes_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCAES_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcaes" = xno],
    [ac_cv_libcaes=no],
    [ac_cv_libcaes=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libcaes" != x && test "x$ac_cv_with_libcaes" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libcaes"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libcaes}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libcaes}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libcaes],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcaes],
          [libcaes >= 20161025],
          [ac_cv_libcaes=yes],
          [ac_cv_libcaes=check])
        ])
      AS_IF(
        [test "x$ac_cv_libcaes" = xyes],
        [ac_cv_libcaes_CPPFLAGS="$pkg_cv_libcaes_CFLAGS"
        ac_cv_libcaes_LIBADD="$pkg_cv_libcaes_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libcaes" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libcaes.h])

      AS_IF(
        [test "x$ac_cv_header_libcaes_h" = xno],
        [ac_cv_libcaes=no],
        [dnl Check for the individual functions
        ac_cv_libcaes=yes

        AC_CHECK_LIB(
          caes,
          libcaes_get_version,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])

        dnl Context functions
        AC_CHECK_LIB(
          caes,
          libcaes_context_initialize,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])
        AC_CHECK_LIB(
          caes,
          libcaes_context_free,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])
        AC_CHECK_LIB(
          caes,
          libcaes_context_set_key,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])

        dnl Tweaked context functions
        AC_CHECK_LIB(
          caes,
          libcaes_tweaked_context_initialize,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])
        AC_CHECK_LIB(
          caes,
          libcaes_tweaked_context_free,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])
        AC_CHECK_LIB(
          caes,
          libcaes_tweaked_context_set_keys,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])

        dnl Crypt functions
        AC_CHECK_LIB(
          caes,
          libcaes_crypt_cbc,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])
        AC_CHECK_LIB(
          caes,
          libcaes_crypt_ccm,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])
        AC_CHECK_LIB(
          caes,
          libcaes_crypt_ecb,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])
        AC_CHECK_LIB(
          caes,
          libcaes_crypt_xts,
          [ac_cv_libcaes_dummy=yes],
          [ac_cv_libcaes=no])

        ac_cv_libcaes_LIBADD="-lcaes"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libcaes" != x && test "x$ac_cv_with_libcaes" != xauto-detect && test "x$ac_cv_libcaes" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libcaes in directory: $ac_cv_with_libcaes],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libcaes" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCAES],
      [1],
      [Define to 1 if you have the `caes' library (-lcaes).])
    ])

  AS_IF(
    [test "x$ac_cv_libcaes" = xyes],
    [ac_cv_libcaes_aes=libcaes],
    [ac_cv_libcaes_aes=no])

  AS_IF(
    [test "x$ac_cv_libcaes" = xyes],
    [AC_SUBST(
      [HAVE_LIBCAES],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCAES],
      [0])
    ])
  ])

dnl Function to detect if libcaes dependencies are available
AC_DEFUN([AX_LIBCAES_CHECK_LOCAL],
  [dnl Check for libcrypto (openssl) support
  AX_LIBCRYPTO_CHECK_ENABLE

  AS_IF(
    [test "x$ac_cv_libcrypto" != xno],
    [AX_LIBCRYPTO_CHECK_AES
    AX_LIBCRYPTO_CHECK_AES_XTS])

  dnl Fallback to local versions if necessary
  AS_IF(
    [test "x$ac_cv_libcrypto" = xno || test "x$ac_cv_libcrypto_aes_cbc" = xno],
    [ac_cv_libcaes_aes_cbc=local],
    [ac_cv_libcaes_aes_cbc=$ac_cv_libcrypto_aes_cbc])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno || test "x$ac_cv_libcrypto_aes_ecb" = xno],
    [ac_cv_libcaes_aes_ecb=local],
    [ac_cv_libcaes_aes_ecb=$ac_cv_libcrypto_aes_ecb])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno || test "x$ac_cv_libcrypto_aes_xts" = xno],
    [ac_cv_libcaes_aes_xts=local],
    [ac_cv_libcaes_aes_xts=$ac_cv_libcrypto_aes_xts])

  ac_cv_libcaes_CPPFLAGS="-I../libcaes";
  ac_cv_libcaes_LIBADD="../libcaes/libcaes.la";

  ac_cv_libcaes=local
  ])

dnl Function to detect how to enable libcaes
AC_DEFUN([AX_LIBCAES_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libcaes],
    [libcaes],
    [search for libcaes in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBCAES_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libcaes" != xyes],
    [AX_LIBCAES_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBCAES],
      [1],
      [Define to 1 if the local version of libcaes is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBCAES],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCAES],
    [test "x$ac_cv_libcaes" = xlocal])
  AS_IF(
    [test "x$ac_cv_libcaes_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCAES_CPPFLAGS],
      [$ac_cv_libcaes_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcaes_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCAES_LIBADD],
      [$ac_cv_libcaes_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcaes" = xyes],
    [AC_SUBST(
      [ax_libcaes_pc_libs_private],
      [-lcaes])
    ])

  AS_IF(
    [test "x$ac_cv_libcaes" = xyes],
    [AC_SUBST(
      [ax_libcaes_spec_requires],
      [libcaes])
    AC_SUBST(
      [ax_libcaes_spec_build_requires],
      [libcaes-devel])
    ])
  ])

