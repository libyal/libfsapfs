dnl Checks for libfcache required headers and functions
dnl
dnl Version: 20230115

dnl Function to detect if libfcache is available
dnl ac_libfcache_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFCACHE_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfcache" = xno],
    [ac_cv_libfcache=no],
    [ac_cv_libfcache=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libfcache" != x && test "x$ac_cv_with_libfcache" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libfcache"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfcache}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfcache}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfcache],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfcache],
          [libfcache >= 20191109],
          [ac_cv_libfcache=yes],
          [ac_cv_libfcache=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfcache" = xyes],
        [ac_cv_libfcache_CPPFLAGS="$pkg_cv_libfcache_CFLAGS"
        ac_cv_libfcache_LIBADD="$pkg_cv_libfcache_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfcache" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfcache.h])

      AS_IF(
        [test "x$ac_cv_header_libfcache_h" = xno],
        [ac_cv_libfcache=no],
        [dnl Check for the individual functions
        ac_cv_libfcache=yes

        AC_CHECK_LIB(
          fcache,
          libfcache_get_version,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])

        dnl Cache functions
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_initialize,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_free,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_clone,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_clear,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_get_number_of_entries,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_get_number_of_cache_values,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_clear_value_by_index,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_get_value_by_identifier,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_get_value_by_index,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_set_value_by_identifier,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_set_value_by_index,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])

        dnl Cache value functions
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_value_free,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_value_clear,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_value_get_identifier,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_value_set_identifier,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_value_get_value,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])
        AC_CHECK_LIB(
          fcache,
          libfcache_cache_value_set_value,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])

        dnl Date time functions
        AC_CHECK_LIB(
          fcache,
          libfcache_date_time_get_timestamp,
          [ac_cv_libfcache_dummy=yes],
          [ac_cv_libfcache=no])

        ac_cv_libfcache_LIBADD="-lfcache"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libfcache" != x && test "x$ac_cv_with_libfcache" != xauto-detect && test "x$ac_cv_libfcache" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libfcache in directory: $ac_cv_with_libfcache],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfcache" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFCACHE],
      [1],
      [Define to 1 if you have the `fcache' library (-lfcache).])
    ])

  AS_IF(
    [test "x$ac_cv_libfcache" = xyes],
    [AC_SUBST(
      [HAVE_LIBFCACHE],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFCACHE],
      [0])
    ])
  ])

dnl Function to detect if libfcache dependencies are available
AC_DEFUN([AX_LIBFCACHE_CHECK_LOCAL],
  [dnl Types used in libfcache/libfcache_date_time.h
  AC_STRUCT_TM

  dnl Headers included in libfcache/libfcache_date_time.h
  AC_CHECK_HEADERS([sys/time.h])

  dnl Date and time functions used in libfcache/libfcache_date_time.h
  AC_CHECK_FUNCS([clock_gettime time])

  AS_IF(
    [test "x$ac_cv_func_time" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: time],
      [1])
    ])

  ac_cv_libfcache_CPPFLAGS="-I../libfcache";
  ac_cv_libfcache_LIBADD="../libfcache/libfcache.la";

  ac_cv_libfcache=local
  ])

dnl Function to detect how to enable libfcache
AC_DEFUN([AX_LIBFCACHE_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfcache],
    [libfcache],
    [search for libfcache in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFCACHE_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfcache" != xyes],
    [AX_LIBFCACHE_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFCACHE],
      [1],
      [Define to 1 if the local version of libfcache is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFCACHE],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFCACHE],
    [test "x$ac_cv_libfcache" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfcache_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFCACHE_CPPFLAGS],
      [$ac_cv_libfcache_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfcache_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFCACHE_LIBADD],
      [$ac_cv_libfcache_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfcache" = xyes],
    [AC_SUBST(
      [ax_libfcache_pc_libs_private],
      [-lfcache])
    ])

  AS_IF(
    [test "x$ac_cv_libfcache" = xyes],
    [AC_SUBST(
      [ax_libfcache_spec_requires],
      [libfcache])
    AC_SUBST(
      [ax_libfcache_spec_build_requires],
      [libfcache-devel])
    ])
  ])
