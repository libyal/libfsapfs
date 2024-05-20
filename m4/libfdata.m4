dnl Functions for libfdata
dnl
dnl Version: 20240520

dnl Function to detect if libfdata is available
dnl ac_libfdata_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFDATA_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfdata" = xno],
    [ac_cv_libfdata=no],
    [ac_cv_libfdata=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libfdata which returns "yes" and --with-libfdata= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libfdata" != x && test "x$ac_cv_with_libfdata" != xauto-detect && test "x$ac_cv_with_libfdata" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libfdata])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfdata],
          [libfdata >= 20201129],
          [ac_cv_libfdata=yes],
          [ac_cv_libfdata=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfdata" = xyes],
        [ac_cv_libfdata_CPPFLAGS="$pkg_cv_libfdata_CFLAGS"
        ac_cv_libfdata_LIBADD="$pkg_cv_libfdata_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfdata" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfdata.h])

      AS_IF(
        [test "x$ac_cv_header_libfdata_h" = xno],
        [ac_cv_libfdata=no],
        [ac_cv_libfdata=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libfdata],
          [fdata],
          [[libfdata_get_version],
           [libfdata_area_initialize],
           [libfdata_area_free],
           [libfdata_area_clone],
           [libfdata_area_empty],
           [libfdata_area_resize],
           [libfdata_area_get_number_of_segments],
           [libfdata_area_get_segment_by_index],
           [libfdata_area_set_segment_by_index],
           [libfdata_area_prepend_segment],
           [libfdata_area_append_segment],
           [libfdata_area_get_element_data_size],
           [libfdata_area_get_element_value_at_offset],
           [libfdata_area_set_element_value_at_offset],
           [libfdata_area_get_size],
           [libfdata_list_initialize],
           [libfdata_list_free],
           [libfdata_list_clone],
           [libfdata_list_empty],
           [libfdata_list_resize],
           [libfdata_list_reverse],
           [libfdata_list_get_number_of_elements],
           [libfdata_list_get_list_element_by_index],
           [libfdata_list_get_element_by_index],
           [libfdata_list_set_element_by_index],
           [libfdata_list_prepend_element],
           [libfdata_list_append_element],
           [libfdata_list_append_list],
           [libfdata_list_is_element_set],
           [libfdata_list_get_element_mapped_range],
           [libfdata_list_get_mapped_offset],
           [libfdata_list_set_mapped_offset],
           [libfdata_list_get_mapped_size_by_index],
           [libfdata_list_set_mapped_size_by_index],
           [libfdata_list_get_element_by_index_with_mapped_size],
           [libfdata_list_set_element_by_index_with_mapped_size],
           [libfdata_list_append_element_with_mapped_size],
           [libfdata_list_get_element_index_at_offset],
           [libfdata_list_get_list_element_at_offset],
           [libfdata_list_get_element_at_offset],
           [libfdata_list_cache_element_value],
           [libfdata_list_get_element_value_by_index],
           [libfdata_list_get_element_value_at_offset],
           [libfdata_list_set_element_value_by_index],
           [libfdata_list_set_element_value_at_offset],
           [libfdata_list_get_size],
           [libfdata_list_element_get_mapped_size],
           [libfdata_list_element_set_mapped_size],
           [libfdata_list_element_get_element_value],
           [libfdata_list_element_set_element_value],
           [libfdata_list_element_get_mapped_size],
           [libfdata_list_element_set_mapped_size],
           [libfdata_list_element_get_element_value],
           [libfdata_list_element_set_element_value],
           [libfdata_stream_initialize],
           [libfdata_stream_free],
           [libfdata_stream_clone],
           [libfdata_stream_empty],
           [libfdata_stream_resize],
           [libfdata_stream_reverse],
           [libfdata_stream_get_number_of_segments],
           [libfdata_stream_get_segment_by_index],
           [libfdata_stream_set_segment_by_index],
           [libfdata_stream_prepend_segment],
           [libfdata_stream_append_segment],
           [libfdata_stream_set_mapped_size],
           [libfdata_stream_get_segment_mapped_range],
           [libfdata_stream_get_segment_index_at_offset],
           [libfdata_stream_get_segment_at_offset],
           [libfdata_stream_read_buffer],
           [libfdata_stream_read_buffer_at_offset],
           [libfdata_stream_write_buffer],
           [libfdata_stream_seek_offset],
           [libfdata_stream_get_offset],
           [libfdata_stream_get_size],
           [libfdata_range_list_initialize],
           [libfdata_range_list_free],
           [libfdata_range_list_clone],
           [libfdata_range_list_empty],
           [libfdata_range_list_get_list_element_at_offset],
           [libfdata_range_list_get_element_at_offset],
           [libfdata_range_list_insert_element],
           [libfdata_range_list_get_element_value_at_offset],
           [libfdata_range_list_set_element_value_at_offset],
           [libfdata_vector_initialize],
           [libfdata_vector_free],
           [libfdata_vector_clone],
           [libfdata_vector_empty],
           [libfdata_vector_resize],
           [libfdata_vector_get_number_of_segments],
           [libfdata_vector_get_segment_by_index],
           [libfdata_vector_set_segment_by_index],
           [libfdata_vector_prepend_segment],
           [libfdata_vector_append_segment],
           [libfdata_vector_get_element_data_size],
           [libfdata_vector_get_number_of_elements],
           [libfdata_vector_get_element_index_at_offset],
           [libfdata_vector_get_element_value_by_index],
           [libfdata_vector_get_element_value_at_offset],
           [libfdata_vector_set_element_value_by_index],
           [libfdata_vector_get_size]])

        ac_cv_libfdata_LIBADD="-lfdata"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libfdata])
    ])

  AS_IF(
    [test "x$ac_cv_libfdata" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFDATA],
      [1],
      [Define to 1 if you have the `fdata' library (-lfdata).])
    ])

  AS_IF(
    [test "x$ac_cv_libfdata" = xyes],
    [AC_SUBST(
      [HAVE_LIBFDATA],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFDATA],
      [0])
    ])
  ])

dnl Function to detect if libfdata dependencies are available
AC_DEFUN([AX_LIBFDATA_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfdata_CPPFLAGS="-I../libfdata -I\$(top_srcdir)/libfdata";
  ac_cv_libfdata_LIBADD="../libfdata/libfdata.la";

  ac_cv_libfdata=local
  ])

dnl Function to detect how to enable libfdata
AC_DEFUN([AX_LIBFDATA_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfdata],
    [libfdata],
    [search for libfdata in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFDATA_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfdata" != xyes],
    [AX_LIBFDATA_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFDATA],
      [1],
      [Define to 1 if the local version of libfdata is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFDATA],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFDATA],
    [test "x$ac_cv_libfdata" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfdata_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFDATA_CPPFLAGS],
      [$ac_cv_libfdata_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfdata_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFDATA_LIBADD],
      [$ac_cv_libfdata_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfdata" = xyes],
    [AC_SUBST(
      [ax_libfdata_pc_libs_private],
      [-lfdata])
    ])

  AS_IF(
    [test "x$ac_cv_libfdata" = xyes],
    [AC_SUBST(
      [ax_libfdata_spec_requires],
      [libfdata])
    AC_SUBST(
      [ax_libfdata_spec_build_requires],
      [libfdata-devel])
    ])
  ])
