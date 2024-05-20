dnl Checks for libcaes required headers and functions
dnl
dnl Version: 20240520

dnl Function to detect if libcaes is available
dnl ac_libcaes_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCAES_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcaes" = xno],
    [ac_cv_libcaes=no],
    [ac_cv_libcaes=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libcaes which returns "yes" and --with-libcaes= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libcaes" != x && test "x$ac_cv_with_libcaes" != xauto-detect && test "x$ac_cv_with_libcaes" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libcaes])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcaes],
          [libcaes >= 20220529],
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
        [ac_cv_libcaes=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libcaes],
          [caes],
          [[libcaes_get_version],
           [libcaes_context_initialize],
           [libcaes_context_free],
           [libcaes_context_set_key],
           [libcaes_tweaked_context_initialize],
           [libcaes_tweaked_context_free],
           [libcaes_tweaked_context_set_keys],
           [libcaes_crypt_cbc],
           [libcaes_crypt_ccm],
           [libcaes_crypt_ecb],
           [libcaes_crypt_xts]])

        ac_cv_libcaes_LIBADD="-lcaes"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libcaes])
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

  ac_cv_libcaes_CPPFLAGS="-I../libcaes -I\$(top_srcdir)/libcaes";
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

