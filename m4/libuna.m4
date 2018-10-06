dnl Checks for libuna or required headers and functions
dnl
dnl Version: 20181006

dnl Function to detect if libuna is available as library
dnl ac_libuna_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBUNA_CHECK_LIB],
  [dnl Check if parameters were provided
  AS_IF(
    [test "x$ac_cv_with_libuna" != x && test "x$ac_cv_with_libuna" != xno && test "x$ac_cv_with_libuna" != xauto-detect],
    [AS_IF(
      [test -d "$ac_cv_with_libuna"],
      [CFLAGS="$CFLAGS -I${ac_cv_with_libuna}/include"
      LDFLAGS="$LDFLAGS -L${ac_cv_with_libuna}/lib"],
      [AC_MSG_WARN([no such directory: $ac_cv_with_libuna])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_with_libuna" = xno],
    [ac_cv_libuna=no],
    [dnl Check for a pkg-config file
    AS_IF(
      [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
      [PKG_CHECK_MODULES(
        [libuna],
        [libuna >= 20181006],
        [ac_cv_libuna=yes],
        [ac_cv_libuna=no])
      ])

    AS_IF(
      [test "x$ac_cv_libuna" = xyes],
      [ac_cv_libuna_CPPFLAGS="$pkg_cv_libuna_CFLAGS"
      ac_cv_libuna_LIBADD="$pkg_cv_libuna_LIBS"],
      [dnl Check for headers
      AC_CHECK_HEADERS([libuna.h])

      AS_IF(
        [test "x$ac_cv_header_libuna_h" = xno],
        [ac_cv_libuna=no],
        [dnl Check for the individual functions
        ac_cv_libuna=yes

        AC_CHECK_LIB(
          una,
          libuna_get_version,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl Base16 stream functions
        AC_CHECK_LIB(
          una,
          libuna_base16_stream_size_to_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base16_stream_copy_to_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base16_stream_size_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base16_stream_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base16_stream_with_index_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl Base32 stream functions
        AC_CHECK_LIB(
          una,
          libuna_base32_quintuplet_copy_from_base32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base32_quintuplet_copy_to_base32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base32_quintuplet_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base32_quintuplet_copy_to_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        AC_CHECK_LIB(
          una,
          libuna_base32_stream_size_to_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base32_stream_copy_to_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base32_stream_size_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base32_stream_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base32_stream_with_index_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl Base64 stream functions
        AC_CHECK_LIB(
          una,
          libuna_base64_triplet_copy_from_base64_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base64_triplet_copy_to_base64_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base64_triplet_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base64_triplet_copy_to_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        AC_CHECK_LIB(
          una,
          libuna_base64_stream_size_to_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base64_stream_copy_to_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base64_stream_size_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base64_stream_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_base64_stream_with_index_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl Byte stream functions
        AC_CHECK_LIB(
          una,
          libuna_byte_stream_size_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_byte_stream_copy_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_byte_stream_size_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_byte_stream_copy_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_byte_stream_size_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_byte_stream_copy_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl UTF-8 stream functions
        AC_CHECK_LIB(
          una,
          libuna_utf8_stream_size_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_stream_copy_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_stream_size_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_stream_copy_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_stream_size_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_stream_copy_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl UTF-16 stream functions
        AC_CHECK_LIB(
          una,
          libuna_utf16_stream_size_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_stream_copy_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_stream_size_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_stream_copy_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_stream_size_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_stream_copy_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl UTF-32 stream functions
        AC_CHECK_LIB(
          una,
          libuna_utf32_stream_size_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_stream_copy_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_stream_size_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_stream_copy_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_stream_size_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_stream_copy_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl UTF-8 string functions
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_size_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_with_index_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_size_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_copy_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_with_index_copy_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_size_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_copy_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_with_index_copy_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_size_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_copy_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_with_index_copy_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_size_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_copy_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_with_index_copy_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_size_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_copy_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_with_index_copy_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_size_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_copy_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf8_string_with_index_copy_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl UTF-16 string functions
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_size_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_with_index_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_size_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_copy_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_with_index_copy_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_size_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_copy_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_with_index_copy_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_size_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_copy_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_with_index_copy_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_size_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_copy_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_with_index_copy_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_size_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_copy_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_with_index_copy_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_size_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_copy_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf16_string_with_index_copy_from_utf32,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        dnl UTF-32 string functions
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_size_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_with_index_copy_from_byte_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_size_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_copy_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_with_index_copy_from_utf7_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_size_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_copy_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_with_index_copy_from_utf8_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_size_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_copy_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_with_index_copy_from_utf16_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_size_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_copy_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_with_index_copy_from_utf32_stream,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_size_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_copy_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          libuna_utf32_string_with_index_copy_from_utf8,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_size_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_copy_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])
        AC_CHECK_LIB(
          una,
          libuna_utf32_string_with_index_copy_from_utf16,
          [ac_cv_libuna_dummy=yes],
          [ac_cv_libuna=no])

        ac_cv_libuna_LIBADD="-luna"
        ])
      ])
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

  ac_cv_libuna_CPPFLAGS="-I../libuna";
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

