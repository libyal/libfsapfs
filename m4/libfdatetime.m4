dnl Checks for libfdatetime required headers and functions
dnl
dnl Version: 20240518

dnl Function to detect if libfdatetime is available
dnl ac_libfdatetime_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFDATETIME_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfdatetime" = xno],
    [ac_cv_libfdatetime=no],
    [ac_cv_libfdatetime=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libfdatetime which returns "yes" and --with-libfdatetime= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libfdatetime" != x && test "x$ac_cv_with_libfdatetime" != xauto-detect && test "x$ac_cv_with_libfdatetime" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libfdatetime])],
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
        [ac_cv_libfdatetime=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libfdatetime],
          [fdatetime],
          [[libfdatetime_get_version],
           [libfdatetime_fat_date_time_initialize],
           [libfdatetime_fat_date_time_free],
           [libfdatetime_fat_date_time_copy_from_byte_stream],
           [libfdatetime_fat_date_time_copy_from_32bit],
           [libfdatetime_fat_date_time_copy_to_32bit],
           [libfdatetime_fat_date_time_copy_to_utf8_string],
           [libfdatetime_fat_date_time_copy_to_utf8_string_with_index],
           [libfdatetime_fat_date_time_copy_to_utf16_string],
           [libfdatetime_fat_date_time_copy_to_utf16_string_with_index],
           [libfdatetime_fat_date_time_copy_to_utf32_string],
           [libfdatetime_fat_date_time_copy_to_utf32_string_with_index],
           [libfdatetime_filetime_initialize],
           [libfdatetime_filetime_free],
           [libfdatetime_filetime_copy_from_byte_stream],
           [libfdatetime_filetime_copy_from_64bit],
           [libfdatetime_filetime_copy_to_64bit],
           [libfdatetime_filetime_copy_to_utf8_string],
           [libfdatetime_filetime_copy_to_utf8_string_with_index],
           [libfdatetime_filetime_copy_to_utf16_string],
           [libfdatetime_filetime_copy_to_utf16_string_with_index],
           [libfdatetime_filetime_copy_to_utf32_string],
           [libfdatetime_filetime_copy_to_utf32_string_with_index],
           [libfdatetime_filetime_add],
           [libfdatetime_floatingtime_initialize],
           [libfdatetime_floatingtime_free],
           [libfdatetime_floatingtime_copy_from_byte_stream],
           [libfdatetime_floatingtime_copy_from_64bit],
           [libfdatetime_floatingtime_copy_to_64bit],
           [libfdatetime_floatingtime_copy_to_utf8_string],
           [libfdatetime_floatingtime_copy_to_utf8_string_with_index],
           [libfdatetime_floatingtime_copy_to_utf16_string],
           [libfdatetime_floatingtime_copy_to_utf16_string_with_index],
           [libfdatetime_floatingtime_copy_to_utf32_string],
           [libfdatetime_floatingtime_copy_to_utf32_string_with_index],
           [libfdatetime_hfs_time_initialize],
           [libfdatetime_hfs_time_free],
           [libfdatetime_hfs_time_copy_from_byte_stream],
           [libfdatetime_hfs_time_copy_from_32bit],
           [libfdatetime_hfs_time_copy_to_32bit],
           [libfdatetime_hfs_time_copy_to_utf8_string],
           [libfdatetime_hfs_time_copy_to_utf8_string_with_index],
           [libfdatetime_hfs_time_copy_to_utf16_string],
           [libfdatetime_hfs_time_copy_to_utf16_string_with_index],
           [libfdatetime_hfs_time_copy_to_utf32_string],
           [libfdatetime_hfs_time_copy_to_utf32_string_with_index],
           [libfdatetime_nsf_timedate_initialize],
           [libfdatetime_nsf_timedate_free],
           [libfdatetime_nsf_timedate_copy_from_byte_stream],
           [libfdatetime_nsf_timedate_copy_from_64bit],
           [libfdatetime_nsf_timedate_copy_to_64bit],
           [libfdatetime_nsf_timedate_copy_to_utf8_string],
           [libfdatetime_nsf_timedate_copy_to_utf8_string_with_index],
           [libfdatetime_nsf_timedate_copy_to_utf16_string],
           [libfdatetime_nsf_timedate_copy_to_utf16_string_with_index],
           [libfdatetime_nsf_timedate_copy_to_utf32_string],
           [libfdatetime_nsf_timedate_copy_to_utf32_string_with_index],
           [libfdatetime_posix_time_initialize],
           [libfdatetime_posix_time_free],
           [libfdatetime_posix_time_copy_from_byte_stream],
           [libfdatetime_posix_time_copy_from_32bit],
           [libfdatetime_posix_time_copy_to_32bit],
           [libfdatetime_posix_time_copy_from_64bit],
           [libfdatetime_posix_time_copy_to_64bit],
           [libfdatetime_posix_time_copy_to_utf8_string],
           [libfdatetime_posix_time_copy_to_utf8_string_with_index],
           [libfdatetime_posix_time_copy_to_utf16_string],
           [libfdatetime_posix_time_copy_to_utf16_string_with_index],
           [libfdatetime_posix_time_copy_to_utf32_string],
           [libfdatetime_posix_time_copy_to_utf32_string_with_index],
           [libfdatetime_systemetime_initialize],
           [libfdatetime_systemetime_free],
           [libfdatetime_systemetime_copy_from_byte_stream],
           [libfdatetime_systemetime_copy_to_utf8_string],
           [libfdatetime_systemetime_copy_to_utf8_string_with_index],
           [libfdatetime_systemetime_copy_to_utf16_string],
           [libfdatetime_systemetime_copy_to_utf16_string_with_index],
           [libfdatetime_systemetime_copy_to_utf32_string],
           [libfdatetime_systemetime_copy_to_utf32_string_with_index]])

        ac_cv_libfdatetime_LIBADD="-lfdatetime"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libfdatetime])
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

  ac_cv_libfdatetime_CPPFLAGS="-I../libfdatetime -I\$(top_srcdir)/libfdatetime";
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

