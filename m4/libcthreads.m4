dnl Checks for libcthreads required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libcthreads is available
dnl ac_libcthreads_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCTHREADS_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcthreads" = xno],
    [ac_cv_libcthreads=no],
    [ac_cv_libcthreads=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libcthreads" != x && test "x$ac_cv_with_libcthreads" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libcthreads"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libcthreads}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libcthreads}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libcthreads],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcthreads],
          [libcthreads >= 20160404],
          [ac_cv_libcthreads=yes],
          [ac_cv_libcthreads=check])
        ])
      AS_IF(
        [test "x$ac_cv_libcthreads" = xyes],
        [ac_cv_libcthreads_CPPFLAGS="$pkg_cv_libcthreads_CFLAGS"
        ac_cv_libcthreads_LIBADD="$pkg_cv_libcthreads_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libcthreads" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libcthreads.h])

      AS_IF(
        [test "x$ac_cv_header_libcthreads_h" = xno],
        [ac_cv_libcthreads=no],
        [dnl Check for the individual functions
        ac_cv_libcthreads=yes

        AC_CHECK_LIB(
          cthreads,
          libcthreads_get_version,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        dnl Thread functions
        AC_CHECK_LIB(
          cthreads,
          libcthreads_thread_create,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_thread_join,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        dnl Thread attributes functions
        AC_CHECK_LIB(
          cthreads,
          libcthreads_thread_attributes_initialize,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_thread_attributes_free,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        dnl Condition functions
        AC_CHECK_LIB(
          cthreads,
          libcthreads_condition_initialize,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_condition_free,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_condition_broadcast,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_condition_signal,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_condition_wait,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        dnl Lock functions
        AC_CHECK_LIB(
          cthreads,
          libcthreads_lock_initialize,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_lock_free,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_lock_grab,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_lock_release,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        dnl Mutex functions
        AC_CHECK_LIB(
          cthreads,
          libcthreads_mutex_initialize,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_mutex_free,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_mutex_grab,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_mutex_try_grab,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_mutex_release,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        dnl Read/Write lock functions
        AC_CHECK_LIB(
          cthreads,
          libcthreads_read_write_lock_initialize,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_read_write_lock_free,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_read_write_lock_grab_for_read,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_read_write_lock_grab_for_write,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_read_write_lock_release_for_read,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_read_write_lock_release_for_write,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        dnl Queue functions
        AC_CHECK_LIB(
          cthreads,
          libcthreads_queue_initialize,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_queue_free,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_queue_type_pop,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_queue_pop,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_queue_try_push,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_queue_push,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_queue_push_sorted,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        dnl Thread pool functions
        AC_CHECK_LIB(
          cthreads,
          libcthreads_thread_pool_create,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_thread_pool_push,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_thread_pool_push_sorted,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])
        AC_CHECK_LIB(
          cthreads,
          libcthreads_thread_pool_join,
          [ac_cv_libcthreads_dummy=yes],
          [ac_cv_libcthreads=no])

        ac_cv_libcthreads_LIBADD="-lcthreads"])
      ])

    AS_IF(
      [test "x$ac_cv_with_libcthreads" != x && test "x$ac_cv_with_libcthreads" != xauto-detect && test "x$ac_cv_libcthreads" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libcthreads in directory: $ac_cv_with_libcthreads],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libcthreads" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCTHREADS],
      [1],
      [Define to 1 if you have the `cthreads' library (-lcthreads).])
    ])

  AS_IF(
    [test "x$ac_cv_libcthreads" = xyes],
    [ac_cv_libcthreads_multi_threading=libcthreads],
    [ac_cv_libcthreads_multi_threading=no])

  AS_IF(
    [test "x$ac_cv_libcthreads" = xyes],
    [AC_SUBST(
      [HAVE_LIBCTHREADS],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCTHREADS],
      [0])
    ])
  ])

dnl Function to detect if libcthreads dependencies are available
AC_DEFUN([AX_LIBCTHREADS_CHECK_LOCAL],
  [AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [dnl Check for enabling pthread support
    AX_PTHREAD_CHECK_ENABLE
      ac_cv_libcthreads_multi_threading=$ac_cv_pthread],
    [ac_cv_libcthreads_multi_threading="winapi"])

  AS_IF(
    [test "x$ac_cv_libcthreads_multi_threading" != xno],
    [ac_cv_libcthreads_CPPFLAGS="-I../libcthreads";
    ac_cv_libcthreads_LIBADD="../libcthreads/libcthreads.la";

    ac_cv_libcthreads=local],
    [ac_cv_libcthreads=no])
  ])

dnl Function to detect how to enable libcthreads
AC_DEFUN([AX_LIBCTHREADS_CHECK_ENABLE],
  [AX_COMMON_ARG_ENABLE(
    [multi-threading-support],
    [multi_threading_support],
    [enable multi-threading support],
    [yes])
  AX_COMMON_ARG_WITH(
    [libcthreads],
    [libcthreads],
    [search for libcthreads in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  AS_IF(
    [test "x$ac_cv_enable_multi_threading_support" = xno],
    [ac_cv_libcthreads="no"
    ac_cv_libcthreads_multi_threading="no"],
    [dnl Check for a shared library version
    AX_LIBCTHREADS_CHECK_LIB

    dnl Check if the dependencies for the local library version
    AS_IF(
      [test "x$ac_cv_libcthreads" != xyes],
      [AX_LIBCTHREADS_CHECK_LOCAL

      AC_DEFINE(
        [HAVE_LOCAL_LIBCTHREADS],
        [1],
        [Define to 1 if the local version of libcthreads is used.])
      AC_SUBST(
        [HAVE_LOCAL_LIBCTHREADS],
        [1])
      ])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCTHREADS],
    [test "x$ac_cv_libcthreads" = xlocal])
  AS_IF(
    [test "x$ac_cv_libcthreads_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCTHREADS_CPPFLAGS],
      [$ac_cv_libcthreads_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcthreads_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCTHREADS_LIBADD],
      [$ac_cv_libcthreads_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcthreads" != xno],
    [AC_DEFINE(
      [HAVE_MULTI_THREAD_SUPPORT],
      [1],
      [Define to 1 if multi thread support should be used.])
    AC_SUBST(
      [HAVE_MULTI_THREAD_SUPPORT],
      [1]) ],
    [AC_SUBST(
      [HAVE_MULTI_THREAD_SUPPORT],
      [0])
    ])

  AS_IF(
    [test "x$ac_cv_libcthreads" = xyes],
    [AC_SUBST(
      [ax_libcthreads_pc_libs_private],
      [-lcthreads])
    ])

  AS_IF(
    [test "x$ac_cv_libcthreads" = xyes],
    [AC_SUBST(
      [ax_libcthreads_spec_requires],
      [libcthreads])
    AC_SUBST(
      [ax_libcthreads_spec_build_requires],
      [libcthreads-devel])
    ])
  ])

