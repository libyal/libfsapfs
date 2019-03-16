dnl Checks for libclocale required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libclocale is available
dnl ac_libclocale_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCLOCALE_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libclocale" = xno],
    [ac_cv_libclocale=no],
    [ac_cv_libclocale=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libclocale" != x && test "x$ac_cv_with_libclocale" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libclocale"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libclocale}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libclocale}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libclocale],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libclocale],
          [libclocale >= 20120425],
          [ac_cv_libclocale=yes],
          [ac_cv_libclocale=check])
        ])
      AS_IF(
        [test "x$ac_cv_libclocale" = xyes && test "x$ac_cv_enable_wide_character_type" != xno],
        [AC_CACHE_CHECK(
          [whether libclocale/features.h defines LIBCLOCALE_HAVE_WIDE_CHARACTER_TYPE as 1],
          [ac_cv_header_libclocale_features_h_have_wide_character_type],
          [AC_LANG_PUSH(C)
          AC_COMPILE_IFELSE(
            [AC_LANG_PROGRAM(
              [[#include <libclocale/features.h>]],
              [[#if !defined( LIBCLOCALE_HAVE_WIDE_CHARACTER_TYPE ) || ( LIBCLOCALE_HAVE_WIDE_CHARACTER_TYPE != 1 )
#error LIBCLOCALE_HAVE_WIDE_CHARACTER_TYPE not defined
#endif]] )],
            [ac_cv_header_libclocale_features_h_have_wide_character_type=yes],
            [ac_cv_header_libclocale_features_h_have_wide_character_type=no])
          AC_LANG_POP(C)],
          [ac_cv_header_libclocale_features_h_have_wide_character_type=no])

        AS_IF(
          [test "x$ac_cv_header_libclocale_features_h_have_wide_character_type" = xno],
          [ac_cv_libclocale=no])
        ])
      AS_IF(
        [test "x$ac_cv_libclocale" = xyes],
        [ac_cv_libclocale_CPPFLAGS="$pkg_cv_libclocale_CFLAGS"
        ac_cv_libclocale_LIBADD="$pkg_cv_libclocale_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libclocale" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libclocale.h])

      AS_IF(
        [test "x$ac_cv_header_libclocale_h" = xno],
        [ac_cv_libclocale=no],
        [dnl Check for the individual functions
        ac_cv_libclocale=yes

        AC_CHECK_LIB(
          clocale,
          libclocale_get_version,
          [ac_cv_libclocale_dummy=yes],
          [ac_cv_libclocale=no])

        dnl Codepage functions
        AC_CHECK_LIB(
          clocale,
          libclocale_codepage,
          [ac_cv_libclocale_dummy=yes],
          [ac_cv_libclocale=no])
        AC_CHECK_LIB(
          clocale,
          libclocale_codepage_get,
          [ac_cv_libclocale_dummy=yes],
          [ac_cv_libclocale=no])
        AC_CHECK_LIB(
          clocale,
          libclocale_codepage_set,
          [ac_cv_libclocale_dummy=yes],
          [ac_cv_libclocale=no])
        AC_CHECK_LIB(
          clocale,
          libclocale_codepage_copy_from_string,
          [ac_cv_libclocale_dummy=yes],
          [ac_cv_libclocale=no])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AC_CHECK_LIB(
            clocale,
            libclocale_codepage_copy_from_string_wide,
            [ac_cv_libclocale_dummy=yes],
            [ac_cv_libclocale=no])
          ])

        dnl Locale functions
        AC_CHECK_LIB(
          clocale,
          libclocale_locale_get_codepage,
          [ac_cv_libclocale_dummy=yes],
          [ac_cv_libclocale=no])
        AC_CHECK_LIB(
          clocale,
          libclocale_locale_get_decimal_point,
          [ac_cv_libclocale_dummy=yes],
          [ac_cv_libclocale=no])

        dnl Support functions
        AC_CHECK_LIB(
          clocale,
          libclocale_initialize,
          [ac_cv_libclocale_dummy=yes],
          [ac_cv_libclocale=no])

        ac_cv_libclocale_LIBADD="-lclocale"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libclocale" != x && test "x$ac_cv_with_libclocale" != xauto-detect && test "x$ac_cv_libclocale" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libclocale in directory: $ac_cv_with_libclocale],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libclocale" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCLOCALE],
      [1],
      [Define to 1 if you have the `clocale' library (-lclocale).])
    ])

  AS_IF(
    [test "x$ac_cv_libclocale" = xyes],
    [AC_SUBST(
      [HAVE_LIBCLOCALE],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCLOCALE],
      [0])
    ])
  ])

dnl Function to detect whether nl_langinfo supports CODESET
AC_DEFUN([AX_LIBCLOCALE_CHECK_FUNC_LANGINFO_CODESET],
  [AC_CHECK_FUNCS([nl_langinfo])

  AS_IF(
    [test "x$ac_cv_func_nl_langinfo" = xyes],
    [AC_CACHE_CHECK(
      [for nl_langinfo CODESET support],
      [ac_cv_cv_langinfo_codeset],
      [AC_LANG_PUSH(C)
      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [[#include <langinfo.h>]],
          [[char* charset = nl_langinfo( CODESET );]] )],
        [ac_cv_cv_langinfo_codeset=yes],
        [ac_cv_cv_langinfo_codeset=no])
      AC_LANG_POP(C) ]) ],
    [ac_cv_cv_langinfo_codeset=no])

  AS_IF(
    [test "x$ac_cv_cv_langinfo_codeset" = xyes],
    [AC_DEFINE(
      [HAVE_LANGINFO_CODESET],
      [1],
      [Define if nl_langinfo has CODESET support.])
    ])
  ])

dnl Function to detect if libclocale dependencies are available
AC_DEFUN([AX_LIBCLOCALE_CHECK_LOCAL],
  [dnl Headers included in libclocale/libclocale_locale.c
  AC_CHECK_HEADERS([langinfo.h locale.h])

  dnl Check for environment functions in libclocale/libclocale_locale.c
  AC_CHECK_FUNCS([getenv])

  AS_IF(
    [test "x$ac_cv_func_getenv" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: getenv],
      [1])
    ])

  dnl Check for localization functions in libclocale/libclocale_locale.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_FUNCS([localeconv])

    AS_IF(
      [test "x$ac_cv_func_localeconv" != xyes],
      [AC_MSG_FAILURE(
        [Missing function: localeconv],
        [1])
      ])
    ])

  AC_CHECK_FUNCS([setlocale])

  AS_IF(
    [test "x$ac_cv_func_setlocale" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: setlocale],
      [1])
    ])

  AX_LIBCLOCALE_CHECK_FUNC_LANGINFO_CODESET

  ac_cv_libclocale_CPPFLAGS="-I../libclocale";
  ac_cv_libclocale_LIBADD="../libclocale/libclocale.la";

  ac_cv_libclocale=local
  ])

dnl Function to detect how to enable libclocale
AC_DEFUN([AX_LIBCLOCALE_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libclocale],
    [libclocale],
    [search for libclocale in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBCLOCALE_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libclocale" != xyes],
    [AX_LIBCLOCALE_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBCLOCALE],
      [1],
      [Define to 1 if the local version of libclocale is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBCLOCALE],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCLOCALE],
    [test "x$ac_cv_libclocale" = xlocal])
  AS_IF(
    [test "x$ac_cv_libclocale_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCLOCALE_CPPFLAGS],
      [$ac_cv_libclocale_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libclocale_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCLOCALE_LIBADD],
      [$ac_cv_libclocale_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libclocale" = xyes],
    [AC_SUBST(
      [ax_libclocale_pc_libs_private],
      [-lclocale])
    ])

  AS_IF(
    [test "x$ac_cv_libclocale" = xyes],
    [AC_SUBST(
      [ax_libclocale_spec_requires],
      [libclocale])
    AC_SUBST(
      [ax_libclocale_spec_build_requires],
      [libclocale-devel])
    ])
  ])

