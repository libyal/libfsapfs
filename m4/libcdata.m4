dnl Checks for libcdata required headers and functions
dnl
dnl Version: 20191221

dnl Function to detect if libcdata is available
dnl ac_libcdata_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCDATA_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcdata" = xno],
    [ac_cv_libcdata=no],
    [ac_cv_libcdata=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libcdata" != x && test "x$ac_cv_with_libcdata" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libcdata"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libcdata}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libcdata}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libcdata],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcdata],
          [libcdata >= 20190112],
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
        [dnl Check for the individual functions
        ac_cv_libcdata=yes

        AC_CHECK_LIB(
          cdata,
          libcdata_get_version,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])

        dnl Array functions
        AC_CHECK_LIB(
          cdata,
          libcdata_array_initialize,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_free,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_empty,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_clone,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_resize,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_reverse,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_get_number_of_entries,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_get_entry_by_index,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_get_entry_by_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_set_entry_by_index,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_prepend_entry,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_append_entry,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_insert_entry,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_array_remove_entry,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])

        dnl Balanced tree functions
        AC_CHECK_LIB(
          cdata,
          libfdata_btree_initialize,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_btree_free,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_btree_get_number_of_values,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_btree_get_value_by_index,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_btree_get_value_by_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_btree_insert_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_btree_replace_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_btree_remove_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])

        dnl List functions
        AC_CHECK_LIB(
          cdata,
          libcdata_list_initialize,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_free,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_empty,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_clone,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_get_number_of_elements,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_get_first_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_get_last_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_get_element_by_index,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_get_value_by_index,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_prepend_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_prepend_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_append_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_append_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_insert_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_insert_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_remove_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])

        dnl List element functions
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_initialize,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_free,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_get_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_set_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_get_previous_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_set_previous_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_get_next_element,
          [ac_cv_libcdata_dummy=yes],
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_set_next_element,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_get_elements,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_list_element_set_elements,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
          [ac_cv_libcdata=no])

        dnl Range list functions
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_initialize,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_free,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_empty,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_clone,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_get_number_of_elements,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_insert_range,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_insert_range_list,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_remove_range,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_get_range_by_index,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_get_range_at_offset,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_range_is_present,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_range_list_get_spanning_range,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])

        dnl Tree node functions
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_initialize,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_free,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_empty,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_clone,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_get_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_set_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_get_parent_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_set_parent_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_get_previous_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_set_previous_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_get_next_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_set_next_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_get_nodes,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_set_nodes,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_append_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_append_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_insert_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_insert_value,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_replace_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_remove_node,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_get_number_of_sub_nodes,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_get_sub_node_by_index,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])
        AC_CHECK_LIB(
          cdata,
          libcdata_tree_node_get_leaf_node_list,
          [ac_cv_libcdata_dummy=yes],
          [ac_cv_libcdata=no])

        ac_cv_libcdata_LIBADD="-lcdata"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libcdata" != x && test "x$ac_cv_with_libcdata" != xauto-detect && test "x$ac_cv_libcdata" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libcdata in directory: $ac_cv_with_libcdata],
        [1])
      ])
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

  ac_cv_libcdata_CPPFLAGS="-I../libcdata";
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

