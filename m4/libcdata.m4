dnl Checks for libcdata required headers and functions
dnl
dnl Version: 20240514

dnl Function to detect if libcdata is available
dnl ac_libcdata_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCDATA_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcdata" = xno],
    [ac_cv_libcdata=no],
    [ac_cv_libcdata=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libcdata which returns "yes" and --with-libcdata= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libcdata" != x && test "x$ac_cv_with_libcdata" != xauto-detect && test "x$ac_cv_with_libcdata" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libcdata])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcdata],
          [libcdata >= 20230108],
          [ac_cv_libcdata=yes],
          [ac_cv_libcdata=check])
        ])
      AS_IF(
        [test "x$ac_cv_libcdata" = xyes],
        [ac_cv_libcdata_CPPFLAGS="$pkg_cv_libcdata_CFLAGS"
        ac_cv_libcdata_LIBADD="$pkg_cv_libcdata_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libcdata" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libcdata.h])

      AS_IF(
        [test "x$ac_cv_header_libcdata_h" = xno],
        [ac_cv_libcdata=no],
        [ac_cv_libcdata=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libcdata],
          [cdata],
          [[libcdata_get_version],
           [libcdata_array_initialize],
           [libcdata_array_free],
           [libcdata_array_empty],
           [libcdata_array_clone],
           [libcdata_array_resize],
           [libcdata_array_reverse],
           [libcdata_array_get_number_of_entries],
           [libcdata_array_get_entry_by_index],
           [libcdata_array_get_entry_by_value],
           [libcdata_array_set_entry_by_index],
           [libcdata_array_prepend_entry],
           [libcdata_array_append_entry],
           [libcdata_array_insert_entry],
           [libcdata_array_remove_entry],
           [libcdata_btree_initialize],
           [libcdata_btree_free],
           [libcdata_btree_get_number_of_values],
           [libcdata_btree_get_value_by_index],
           [libcdata_btree_get_value_by_value],
           [libcdata_btree_insert_value],
           [libcdata_btree_replace_value],
           [libcdata_btree_remove_value],
           [libcdata_list_initialize],
           [libcdata_list_free],
           [libcdata_list_empty],
           [libcdata_list_clone],
           [libcdata_list_get_number_of_elements],
           [libcdata_list_get_first_element],
           [libcdata_list_get_last_element],
           [libcdata_list_get_element_by_index],
           [libcdata_list_get_value_by_index],
           [libcdata_list_prepend_element],
           [libcdata_list_prepend_value],
           [libcdata_list_append_element],
           [libcdata_list_append_value],
           [libcdata_list_insert_element],
           [libcdata_list_insert_element_with_existing],
           [libcdata_list_insert_value],
           [libcdata_list_insert_value_with_existing],
           [libcdata_list_remove_element],
           [libcdata_list_element_initialize],
           [libcdata_list_element_free],
           [libcdata_list_element_get_value],
           [libcdata_list_element_set_value],
           [libcdata_list_element_get_previous_element],
           [libcdata_list_element_set_previous_element],
           [libcdata_list_element_get_next_element],
           [libcdata_list_element_get_elements],
           [libcdata_list_element_set_elements],
           [libcdata_range_list_initialize],
           [libcdata_range_list_free],
           [libcdata_range_list_empty],
           [libcdata_range_list_clone],
           [libcdata_range_list_get_number_of_elements],
           [libcdata_range_list_insert_range],
           [libcdata_range_list_insert_range_list],
           [libcdata_range_list_remove_range],
           [libcdata_range_list_get_range_by_index],
           [libcdata_range_list_get_range_at_offset],
           [libcdata_range_list_range_is_present],
           [libcdata_range_list_range_has_overlapping_range],
           [libcdata_range_list_get_spanning_range],
           [libcdata_tree_node_initialize],
           [libcdata_tree_node_free],
           [libcdata_tree_node_empty],
           [libcdata_tree_node_clone],
           [libcdata_tree_node_get_value],
           [libcdata_tree_node_set_value],
           [libcdata_tree_node_get_parent_node],
           [libcdata_tree_node_set_parent_node],
           [libcdata_tree_node_get_previous_node],
           [libcdata_tree_node_set_previous_node],
           [libcdata_tree_node_get_next_node],
           [libcdata_tree_node_set_next_node],
           [libcdata_tree_node_get_nodes],
           [libcdata_tree_node_set_nodes],
           [libcdata_tree_node_append_node],
           [libcdata_tree_node_append_value],
           [libcdata_tree_node_insert_node],
           [libcdata_tree_node_insert_value],
           [libcdata_tree_node_replace_node],
           [libcdata_tree_node_remove_node],
           [libcdata_tree_node_get_number_of_sub_nodes],
           [libcdata_tree_node_get_sub_node_by_index],
           [libcdata_tree_node_get_leaf_node_list]])

        ac_cv_libcdata_LIBADD="-lcdata"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libcdata])
    ])

  AS_IF(
    [test "x$ac_cv_libcdata" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCDATA],
      [1],
      [Define to 1 if you have the `cdata' library (-lcdata).])
    ])

  AS_IF(
    [test "x$ac_cv_libcdata" = xyes],
    [AC_SUBST(
      [HAVE_LIBCDATA],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCDATA],
      [0])
    ])
  ])

dnl Function to detect if libcdata dependencies are available
AC_DEFUN([AX_LIBCDATA_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libcdata_CPPFLAGS="-I../libcdata -I\$(top_srcdir)/libcdata";
  ac_cv_libcdata_LIBADD="../libcdata/libcdata.la";

  ac_cv_libcdata=local
  ])

dnl Function to detect how to enable libcdata
AC_DEFUN([AX_LIBCDATA_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libcdata],
    [libcdata],
    [search for libcdata in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBCDATA_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libcdata" != xyes],
    [AX_LIBCDATA_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBCDATA],
      [1],
      [Define to 1 if the local version of libcdata is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBCDATA],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCDATA],
    [test "x$ac_cv_libcdata" = xlocal])
  AS_IF(
    [test "x$ac_cv_libcdata_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCDATA_CPPFLAGS],
      [$ac_cv_libcdata_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcdata_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCDATA_LIBADD],
      [$ac_cv_libcdata_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcdata" = xyes],
    [AC_SUBST(
      [ax_libcdata_pc_libs_private],
      [-lcdata])
    ])

  AS_IF(
    [test "x$ac_cv_libcdata" = xyes],
    [AC_SUBST(
      [ax_libcdata_spec_requires],
      [libcdata])
    AC_SUBST(
      [ax_libcdata_spec_build_requires],
      [libcdata-devel])
    ])
  ])

