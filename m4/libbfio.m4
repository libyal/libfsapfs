dnl Checks for libbfio required headers and functions
dnl
dnl Version: 20240518

dnl Function to detect if libbfio is available
dnl ac_libbfio_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBBFIO_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libbfio" = xno],
    [ac_cv_libbfio=no],
    [ac_cv_libbfio=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libbfio which returns "yes" and --with-libbfio= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libbfio" != x && test "x$ac_cv_with_libbfio" != xauto-detect && test "x$ac_cv_with_libbfio" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libbfio])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libbfio],
          [libbfio >= 20201125],
          [ac_cv_libbfio=yes],
          [ac_cv_libbfio=check])
        ])
      AS_IF(
        [test "x$ac_cv_libbfio" = xyes && test "x$ac_cv_enable_wide_character_type" != xno],
        [AC_CACHE_CHECK(
         [whether libbfio/features.h defines LIBBFIO_HAVE_WIDE_CHARACTER_TYPE as 1],
         [ac_cv_header_libbfio_features_h_have_wide_character_type],
         [AC_LANG_PUSH(C)
         AC_COMPILE_IFELSE(
           [AC_LANG_PROGRAM(
             [[#include <libbfio/features.h>]],
             [[#if !defined( LIBBFIO_HAVE_WIDE_CHARACTER_TYPE ) || ( LIBBFIO_HAVE_WIDE_CHARACTER_TYPE != 1 )
#error LIBBFIO_HAVE_WIDE_CHARACTER_TYPE not defined
#endif]] )],
           [ac_cv_header_libbfio_features_h_have_wide_character_type=yes],
           [ac_cv_header_libbfio_features_h_have_wide_character_type=no])
         AC_LANG_POP(C)],
         [ac_cv_header_libbfio_features_h_have_wide_character_type=no])

        AS_IF(
          [test "x$ac_cv_header_libbfio_features_h_have_wide_character_type" = xno],
          [ac_cv_libbfio=no])
        ])
      AS_IF(
        [test "x$ac_cv_libbfio" = xyes],
        [ac_cv_libbfio_CPPFLAGS="$pkg_cv_libbfio_CFLAGS"
        ac_cv_libbfio_LIBADD="$pkg_cv_libbfio_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libbfio" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libbfio.h])

      AS_IF(
        [test "x$ac_cv_header_libbfio_h" = xno],
        [ac_cv_libbfio=no],
        [ac_cv_libbfio=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libbfio],
          [bfio],
          [[libbfio_get_version],
           [libbfio_handle_free],
           [libbfio_handle_open],
           [libbfio_handle_close],
           [libbfio_handle_exists],
           [libbfio_handle_read_buffer],
           [libbfio_handle_read_buffer_at_offset],
           [libbfio_handle_write_buffer],
           [libbfio_handle_write_buffer_at_offset],
           [libbfio_handle_seek_offset],
           [libbfio_handle_is_open],
           [libbfio_handle_get_offset],
           [libbfio_handle_get_size],
           [libbfio_handle_set_track_offsets_read],
           [libbfio_handle_get_number_of_offsets_read],
           [libbfio_handle_get_offset_read],
           [libbfio_file_initialize],
           [libbfio_file_get_name_size],
           [libbfio_file_get_name],
           [libbfio_file_set_name],
           [libbfio_pool_initialize],
           [libbfio_pool_free],
           [libbfio_pool_clone],
           [libbfio_pool_get_number_of_handles],
           [libbfio_pool_get_handle],
           [libbfio_pool_set_handle],
           [libbfio_pool_append_handle],
           [libbfio_pool_remove_handle],
           [libbfio_pool_get_maximum_number_of_open_handles],
           [libbfio_pool_set_maximum_number_of_open_handles],
           [libbfio_pool_open],
           [libbfio_pool_reopen],
           [libbfio_pool_close],
           [libbfio_pool_close_all],
           [libbfio_pool_read_buffer],
           [libbfio_pool_read_buffer_at_offset],
           [libbfio_pool_write_buffer],
           [libbfio_pool_write_buffer_at_offset],
           [libbfio_pool_seek_offset],
           [libbfio_pool_get_offset],
           [libbfio_pool_get_size],
           [libbfio_file_pool_open]])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AX_CHECK_LIB_FUNCTIONS(
            [libbfio],
            [bfio],
            [[libbfio_file_get_name_size_wide],
             [libbfio_file_get_name_wide],
             [libbfio_file_set_name_wide],
             [libbfio_file_pool_open_wide]])
          ])

        ac_cv_libbfio_LIBADD="-lbfio"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libbfio])
    ])

  AS_IF(
    [test "x$ac_cv_libbfio" = xyes],
    [AC_DEFINE(
      [HAVE_LIBBFIO],
      [1],
      [Define to 1 if you have the `bfio' library (-lbfio).])
    ])

  AS_IF(
    [test "x$ac_cv_libbfio" = xyes],
    [AC_SUBST(
      [HAVE_LIBBFIO],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBBFIO],
      [0])
    ])
  ])

dnl Function to detect if libbfio dependencies are available
AC_DEFUN([AX_LIBBFIO_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libbfio_CPPFLAGS="-I../libbfio -I\$(top_srcdir)/libbfio";
  ac_cv_libbfio_LIBADD="../libbfio/libbfio.la";

  ac_cv_libbfio=local
  ])

dnl Function to detect how to enable libbfio
AC_DEFUN([AX_LIBBFIO_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libbfio],
    [libbfio],
    [search for libbfio in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBBFIO_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libbfio" != xyes],
    [AX_LIBBFIO_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBBFIO],
      [1],
      [Define to 1 if the local version of libbfio is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBBFIO],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBBFIO],
    [test "x$ac_cv_libbfio" = xlocal])
  AS_IF(
    [test "x$ac_cv_libbfio_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBBFIO_CPPFLAGS],
      [$ac_cv_libbfio_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libbfio_LIBADD" != "x"],
    [AC_SUBST(
      [LIBBFIO_LIBADD],
      [$ac_cv_libbfio_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libbfio" = xyes],
    [AC_SUBST(
      [ax_libbfio_pc_libs_private],
      [-lbfio])
    ])

  AS_IF(
    [test "x$ac_cv_libbfio" = xyes],
    [AC_SUBST(
      [ax_libbfio_spec_requires],
      [libbfio])
    AC_SUBST(
      [ax_libbfio_spec_build_requires],
      [libbfio-devel])
    ])
  ])

