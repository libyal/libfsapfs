dnl Checks for libfdatetime required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libfdatetime is available
dnl ac_libfdatetime_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFDATETIME_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfdatetime" = xno],
    [ac_cv_libfdatetime=no],
    [ac_cv_libfdatetime=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libfdatetime" != x && test "x$ac_cv_with_libfdatetime" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libfdatetime"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfdatetime}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfdatetime}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfdatetime],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfdatetime],
          [libfdatetime >= 20180910],
          [ac_cv_libfdatetime=yes],
          [ac_cv_libfdatetime=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfdatetime" = xyes],
        [ac_cv_libfdatetime_CPPFLAGS="$pkg_cv_libfdatetime_CFLAGS"
        ac_cv_libfdatetime_LIBADD="$pkg_cv_libfdatetime_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfdatetime" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfdatetime.h])

      AS_IF(
        [test "x$ac_cv_header_libfdatetime_h" = xno],
        [ac_cv_libfdatetime=no],
        [dnl Check for the individual functions
        ac_cv_libfdatetime=yes

        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_get_version,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])

        dnl FAT date time functions
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_initialize,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_free,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_from_byte_stream,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_from_32bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_to_32bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_to_utf8_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_to_utf8_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_to_utf16_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_to_utf16_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_to_utf32_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_fat_date_time_copy_to_utf32_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])

        dnl FILETIME functions
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_initialize,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_free,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_from_byte_stream,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_from_64bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_to_64bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_to_utf8_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_to_utf8_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_to_utf16_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_to_utf16_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_to_utf32_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_copy_to_utf32_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_filetime_add,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])

        dnl Floatingtime functions
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_initialize,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_free,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_from_byte_stream,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_from_64bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_to_64bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_to_utf8_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_to_utf8_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_to_utf16_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_to_utf16_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_to_utf32_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_floatingtime_copy_to_utf32_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])

        dnl HFS time functions
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_initialize,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_free,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_from_byte_stream,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_from_32bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_to_32bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_to_utf8_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_to_utf8_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_to_utf16_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_to_utf16_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_to_utf32_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_hfs_time_copy_to_utf32_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])

        dnl NSF timedate functions
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_initialize,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_free,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_from_byte_stream,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_from_64bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_to_64bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_to_utf8_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_to_utf8_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_to_utf16_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_to_utf16_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_to_utf32_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_nsf_timedate_copy_to_utf32_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])

        dnl POSIX time functions
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_initialize,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_free,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_from_byte_stream,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_from_32bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_to_32bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_from_64bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_to_64bit,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_to_utf8_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_to_utf8_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_to_utf16_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_to_utf16_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_to_utf32_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_posix_time_copy_to_utf32_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])

        dnl Systemtime functions
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_initialize,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_free,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_copy_from_byte_stream,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_copy_to_utf8_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_copy_to_utf8_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_copy_to_utf16_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_copy_to_utf16_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_copy_to_utf32_string,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])
        AC_CHECK_LIB(
          fdatetime,
          libfdatetime_systemetime_copy_to_utf32_string_with_index,
          [ac_cv_libfdatetime_dummy=yes],
          [ac_cv_libfdatetime=no])

        ac_cv_libfdatetime_LIBADD="-lfdatetime"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libfdatetime" != x && test "x$ac_cv_with_libfdatetime" != xauto-detect && test "x$ac_cv_libfdatetime" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libfdatetime in directory: $ac_cv_with_libfdatetime],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfdatetime" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFDATETIME],
      [1],
      [Define to 1 if you have the `fdatetime' library (-lfdatetime).])
    ])

  AS_IF(
    [test "x$ac_cv_libfdatetime" = xyes],
    [AC_SUBST(
      [HAVE_LIBFDATETIME],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFDATETIME],
      [0])
    ])
  ])

dnl Function to detect if libfdatetime dependencies are available
AC_DEFUN([AX_LIBFDATETIME_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfdatetime_CPPFLAGS="-I../libfdatetime";
  ac_cv_libfdatetime_LIBADD="../libfdatetime/libfdatetime.la";

  ac_cv_libfdatetime=local
  ])

dnl Function to detect how to enable libfdatetime
AC_DEFUN([AX_LIBFDATETIME_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfdatetime],
    [libfdatetime],
    [search for libfdatetime in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  AX_LIBFDATETIME_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfdatetime" != xyes],
    [AX_LIBFDATETIME_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFDATETIME],
      [1],
      [Define to 1 if the local version of libfdatetime is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFDATETIME],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFDATETIME],
    [test "x$ac_cv_libfdatetime" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfdatetime_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFDATETIME_CPPFLAGS],
      [$ac_cv_libfdatetime_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfdatetime_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFDATETIME_LIBADD],
      [$ac_cv_libfdatetime_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfdatetime" = xyes],
    [AC_SUBST(
      [ax_libfdatetime_pc_libs_private],
      [-lfdatetime])
    ])

  AS_IF(
    [test "x$ac_cv_libfdatetime" = xyes],
    [AC_SUBST(
      [ax_libfdatetime_spec_requires],
      [libfdatetime])
    AC_SUBST(
      [ax_libfdatetime_spec_build_requires],
      [libfdatetime-devel])
    ])
  ])

