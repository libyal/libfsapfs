dnl Functions for pthread
dnl
dnl Version: 20240513

dnl Function to detect if pthread is available
AC_DEFUN([AX_PTHREAD_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_pthread" = xno],
    [ac_cv_pthread=no],
    [ac_cv_pthread=check
    dnl Check if parameters were provided
    dnl For both --with-pthread which returns "yes" and --with-pthread= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_pthread" != x && test "x$ac_cv_with_pthread" != xauto-detect && test "x$ac_cv_with_pthread" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([pthread])])
    ])

    AS_IF(
      [test "x$ac_cv_pthread" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([pthread.h])

      AS_IF(
        [test "x$ac_cv_header_pthread_h" = xno],
        [ac_cv_pthread=no],
        [ac_cv_pthread=yes

        AX_CHECK_LIB_FUNCTIONS(
          [pthread],
          [pthread],
          [[pthread_create],
          [pthread_exit],
          [pthread_join],
          [pthread_cond_init],
          [pthread_cond_destroy],
          [pthread_cond_broadcast],
          [pthread_cond_signal],
          [pthread_cond_wait],
          [pthread_mutex_init],
          [pthread_mutex_destroy],
          [pthread_mutex_lock],
          [pthread_mutex_trylock],
          [pthread_mutex_unlock],
          [pthread_rwlock_init],
          [pthread_rwlock_destroy],
          [pthread_rwlock_rdlock],
          [pthread_rwlock_wrlock],
          [pthread_rwlock_unlock]])

        ac_cv_pthread_LIBADD="-lpthread"
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([pthread])
    ])

  AS_IF(
    [test "x$ac_cv_pthread" = xpthread],
    [AC_DEFINE(
      [HAVE_PTHREAD],
      [1],
      [Define to 1 if you have the 'pthread' library (-lpthread).])
    ])

  AS_IF(
    [test "x$ac_cv_pthread" != xno],
    [AC_SUBST(
      [HAVE_PTHREAD],
      [1]) ],
    [AC_SUBST(
      [HAVE_PTHREAD],
      [0])
    ])
  ])

dnl Function to detect how to enable pthread
AC_DEFUN([AX_PTHREAD_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [pthread],
    [pthread],
    [search for pthread in includedir and libdir or in the specified DIR, or no if not to use pthread],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_PTHREAD_CHECK_LIB

  AS_IF(
    [test "x$ac_cv_pthread_CPPFLAGS" != "x"],
    [AC_SUBST(
      [PTHREAD_CPPFLAGS],
      [$ac_cv_pthread_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_pthread_LIBADD" != "x"],
    [AC_SUBST(
      [PTHREAD_LIBADD],
      [$ac_cv_pthread_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_pthread" = xpthread],
    [AC_SUBST(
      [ax_pthread_pc_libs_private],
      [-lpthread])
    ])
  ])

