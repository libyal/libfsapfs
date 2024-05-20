dnl Checks for libuna or required headers and functions
dnl
dnl Version: 20240513

dnl Function to detect if libuna is available
dnl ac_libuna_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBUNA_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libuna" = xno],
    [ac_cv_libuna=no],
    [ac_cv_libuna=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libuna which returns "yes" and --with-libuna= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libuna" != x && test "x$ac_cv_with_libuna" != xauto-detect && test "x$ac_cv_with_libuna" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libuna])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libuna],
          [libuna >= 20230702],
          [ac_cv_libuna=yes],
          [ac_cv_libuna=check])
        ])
      AS_IF(
        [test "x$ac_cv_libuna" = xyes],
        [ac_cv_libuna_CPPFLAGS="$pkg_cv_libuna_CFLAGS"
        ac_cv_libuna_LIBADD="$pkg_cv_libuna_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libuna" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libuna.h])

      AS_IF(
        [test "x$ac_cv_header_libuna_h" = xno],
        [ac_cv_libuna=no],
        [ac_cv_libuna=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libuna],
          [una],
          [[libuna_get_version],
           [libuna_base16_stream_size_to_byte_stream],
           [libuna_base16_stream_copy_to_byte_stream],
           [libuna_base16_stream_size_from_byte_stream],
           [libuna_base16_stream_copy_from_byte_stream],
           [libuna_base16_stream_with_index_copy_from_byte_stream],
           [libuna_base32_quintuplet_copy_from_base32_stream],
           [libuna_base32_quintuplet_copy_to_base32_stream],
           [libuna_base32_quintuplet_copy_from_byte_stream],
           [libuna_base32_quintuplet_copy_to_byte_stream],
           [libuna_base32_stream_size_to_byte_stream],
           [libuna_base32_stream_copy_to_byte_stream],
           [libuna_base32_stream_size_from_byte_stream],
           [libuna_base32_stream_copy_from_byte_stream],
           [libuna_base32_stream_with_index_copy_from_byte_stream],
           [libuna_base64_triplet_copy_from_base64_stream],
           [libuna_base64_triplet_copy_to_base64_stream],
           [libuna_base64_triplet_copy_from_byte_stream],
           [libuna_base64_triplet_copy_to_byte_stream],
           [libuna_base64_stream_size_to_byte_stream],
           [libuna_base64_stream_copy_to_byte_stream],
           [libuna_base64_stream_size_from_byte_stream],
           [libuna_base64_stream_copy_from_byte_stream],
           [libuna_base64_stream_with_index_copy_from_byte_stream],
           [libuna_byte_stream_size_from_utf8],
           [libuna_byte_stream_copy_from_utf8],
           [libuna_byte_stream_size_from_utf16],
           [libuna_byte_stream_copy_from_utf16],
           [libuna_byte_stream_size_from_utf32],
           [libuna_byte_stream_copy_from_utf32],
           [libuna_unicode_character_size_to_byte_stream],
           [libuna_unicode_character_copy_from_byte_stream],
           [libuna_unicode_character_copy_to_byte_stream],
           [libuna_unicode_character_size_to_ucs2],
           [libuna_unicode_character_copy_from_ucs2],
           [libuna_unicode_character_copy_to_ucs2],
           [libuna_unicode_character_size_to_ucs4],
           [libuna_unicode_character_copy_from_ucs4],
           [libuna_unicode_character_copy_to_ucs4],
           [libuna_unicode_character_copy_from_utf7_stream],
           [libuna_unicode_character_copy_to_utf7_stream],
           [libuna_unicode_character_size_to_utf8],
           [libuna_unicode_character_copy_from_utf8],
           [libuna_unicode_character_copy_to_utf8],
           [libuna_unicode_character_size_to_utf8_rfc2279],
           [libuna_unicode_character_copy_from_utf8_rfc2279],
           [libuna_unicode_character_copy_to_utf8_rfc2279],
           [libuna_unicode_character_size_to_utf16],
           [libuna_unicode_character_copy_from_utf16],
           [libuna_unicode_character_copy_to_utf16],
           [libuna_unicode_character_copy_from_utf16_stream],
           [libuna_unicode_character_copy_to_utf16_stream],
           [libuna_unicode_character_copy_from_utf32],
           [libuna_unicode_character_copy_to_utf32],
           [libuna_unicode_character_copy_from_utf32_stream],
           [libuna_unicode_character_copy_to_utf32_stream],
           [libuna_utf8_stream_size_from_utf8],
           [libuna_utf8_stream_copy_from_utf8],
           [libuna_utf8_stream_size_from_utf16],
           [libuna_utf8_stream_copy_from_utf16],
           [libuna_utf8_stream_size_from_utf32],
           [libuna_utf8_stream_copy_from_utf32],
           [libuna_utf16_stream_size_from_utf8],
           [libuna_utf16_stream_copy_from_utf8],
           [libuna_utf16_stream_size_from_utf16],
           [libuna_utf16_stream_copy_from_utf16],
           [libuna_utf16_stream_size_from_utf32],
           [libuna_utf16_stream_copy_from_utf32],
           [libuna_utf32_stream_size_from_utf8],
           [libuna_utf32_stream_copy_from_utf8],
           [libuna_utf32_stream_size_from_utf16],
           [libuna_utf32_stream_copy_from_utf16],
           [libuna_utf32_stream_size_from_utf32],
           [libuna_utf32_stream_copy_from_utf32],
           [libuna_utf8_string_size_from_byte_stream],
           [libuna_utf8_string_copy_from_byte_stream],
           [libuna_utf8_string_with_index_copy_from_byte_stream],
           [libuna_utf8_string_compare_with_byte_stream],
           [libuna_utf8_string_size_from_utf7_stream],
           [libuna_utf8_string_copy_from_utf7_stream],
           [libuna_utf8_string_with_index_copy_from_utf7_stream],
           [libuna_utf8_string_compare_with_utf7_stream],
           [libuna_utf8_string_size_from_utf8_stream],
           [libuna_utf8_string_copy_from_utf8_stream],
           [libuna_utf8_string_with_index_copy_from_utf8_stream],
           [libuna_utf8_string_compare_with_utf8_stream],
           [libuna_utf8_string_size_from_utf16],
           [libuna_utf8_string_copy_from_utf16],
           [libuna_utf8_string_with_index_copy_from_utf16],
           [libuna_utf8_string_compare_with_utf16],
           [libuna_utf8_string_size_from_utf16_stream],
           [libuna_utf8_string_copy_from_utf16_stream],
           [libuna_utf8_string_with_index_copy_from_utf16_stream],
           [libuna_utf8_string_compare_with_utf16_stream],
           [libuna_utf8_string_size_from_utf32],
           [libuna_utf8_string_copy_from_utf32],
           [libuna_utf8_string_with_index_copy_from_utf32],
           [libuna_utf8_string_compare_with_utf32],
           [libuna_utf8_string_size_from_utf32_stream],
           [libuna_utf8_string_copy_from_utf32_stream],
           [libuna_utf8_string_with_index_copy_from_utf32_stream],
           [libuna_utf8_string_compare_with_utf32_stream],
           [libuna_utf8_string_size_from_scsu_stream],
           [libuna_utf8_string_copy_from_scsu_stream],
           [libuna_utf8_string_with_index_copy_from_scsu_stream],
           [libuna_utf16_string_size_from_byte_stream],
           [libuna_utf16_string_copy_from_byte_stream],
           [libuna_utf16_string_with_index_copy_from_byte_stream],
           [libuna_utf16_string_compare_with_byte_stream],
           [libuna_utf16_string_size_from_utf7_stream],
           [libuna_utf16_string_copy_from_utf7_stream],
           [libuna_utf16_string_with_index_copy_from_utf7_stream],
           [libuna_utf16_string_compare_with_utf7_stream],
           [libuna_utf16_string_size_from_utf8],
           [libuna_utf16_string_copy_from_utf8],
           [libuna_utf16_string_with_index_copy_from_utf8],
           [libuna_utf16_string_size_from_utf8_stream],
           [libuna_utf16_string_copy_from_utf8_stream],
           [libuna_utf16_string_with_index_copy_from_utf8_stream],
           [libuna_utf16_string_compare_with_utf8_stream],
           [libuna_utf16_string_size_from_utf16_stream],
           [libuna_utf16_string_copy_from_utf16_stream],
           [libuna_utf16_string_with_index_copy_from_utf16_stream],
           [libuna_utf16_string_compare_with_utf16_stream],
           [libuna_utf16_string_size_from_utf32],
           [libuna_utf16_string_copy_from_utf32],
           [libuna_utf16_string_with_index_copy_from_utf32],
           [libuna_utf16_string_compare_with_utf32],
           [libuna_utf16_string_size_from_utf32_stream],
           [libuna_utf16_string_copy_from_utf32_stream],
           [libuna_utf16_string_with_index_copy_from_utf32_stream],
           [libuna_utf16_string_compare_with_utf32_stream],
           [libuna_utf16_string_size_from_scsu_stream],
           [libuna_utf16_string_copy_from_scsu_stream],
           [libuna_utf16_string_with_index_copy_from_scsu_stream],
           [libuna_utf32_string_size_from_byte_stream],
           [libuna_utf32_string_copy_from_byte_stream],
           [libuna_utf32_string_with_index_copy_from_byte_stream],
           [libuna_utf32_string_compare_with_byte_stream],
           [libuna_utf32_string_size_from_utf7_stream],
           [libuna_utf32_string_copy_from_utf7_stream],
           [libuna_utf32_string_with_index_copy_from_utf7_stream],
           [libuna_utf32_string_compare_with_utf7_stream],
           [libuna_utf32_string_size_from_utf8_stream],
           [libuna_utf32_string_size_from_utf8],
           [libuna_utf32_string_copy_from_utf8],
           [libuna_utf32_string_copy_from_utf8_stream],
           [libuna_utf32_string_with_index_copy_from_utf8_stream],
           [libuna_utf32_string_compare_with_utf8_stream],
           [libuna_utf32_string_size_from_utf16],
           [libuna_utf32_string_copy_from_utf16],
           [libuna_utf32_string_with_index_copy_from_utf16],
           [libuna_utf32_string_size_from_utf16_stream],
           [libuna_utf32_string_copy_from_utf16_stream],
           [libuna_utf32_string_with_index_copy_from_utf16_stream],
           [libuna_utf32_string_compare_with_utf16_stream],
           [libuna_utf32_string_size_from_utf32_stream],
           [libuna_utf32_string_copy_from_utf32_stream],
           [libuna_utf32_string_with_index_copy_from_utf32_stream],
           [libuna_utf32_string_compare_with_utf32_stream],
           [libuna_utf32_string_size_from_scsu_stream],
           [libuna_utf32_string_copy_from_scsu_stream],
           [libuna_utf32_string_with_index_copy_from_scsu_stream]])

        AX_CHECK_LIB_DEFINITIONS(
          [libuna],
          [[LIBUNA_COMPARE_LESS],
           [LIBUNA_COMPARE_EQUAL],
           [LIBUNA_COMPARE_GREATER],
           [LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE]])

        ac_cv_libuna_LIBADD="-luna"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libuna])
    ])

  AS_IF(
    [test "x$ac_cv_libuna" = xyes],
    [AC_DEFINE(
      [HAVE_LIBUNA],
      [1],
      [Define to 1 if you have the `una' library (-luna).])
    ])

  AS_IF(
    [test "x$ac_cv_libuna" = xyes],
    [AC_SUBST(
      [HAVE_LIBUNA],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBUNA],
      [0])
    ])
  ])

dnl Function to detect if libuna dependencies are available
AC_DEFUN([AX_LIBUNA_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libuna_CPPFLAGS="-I../libuna -I\$(top_srcdir)/libuna";
  ac_cv_libuna_LIBADD="../libuna/libuna.la";

  ac_cv_libuna=local
  ])

dnl Function to detect how to enable libuna
AC_DEFUN([AX_LIBUNA_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libuna],
    [libuna],
    [search for libuna in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBUNA_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libuna" != xyes],
    [AX_LIBUNA_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBUNA],
      [1],
      [Define to 1 if the local version of libuna is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBUNA],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBUNA],
    [test "x$ac_cv_libuna" = xlocal])
  AS_IF(
    [test "x$ac_cv_libuna_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBUNA_CPPFLAGS],
      [$ac_cv_libuna_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libuna_LIBADD" != "x"],
    [AC_SUBST(
      [LIBUNA_LIBADD],
      [$ac_cv_libuna_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libuna" = xyes],
    [AC_SUBST(
      [ax_libuna_pc_libs_private],
      [-luna])
    ])

  AS_IF(
    [test "x$ac_cv_libuna" = xyes],
    [AC_SUBST(
      [ax_libuna_spec_requires],
      [libuna])
    AC_SUBST(
      [ax_libuna_spec_build_requires],
      [libuna-devel])
    ])
  ])

