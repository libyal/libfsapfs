dnl Functions for pthread
dnl
dnl Version: 20130509

dnl Function to detect if pthread is available
AC_DEFUN([AX_PTHREAD_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_pthread" != x && test "x$ac_cv_with_pthread" != xno && test "x$ac_cv_with_pthread" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_pthread"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_pthread}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_pthread}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_pthread])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_pthread" = xno],
  [ac_cv_pthread=no],
  [dnl Check for headers
  AC_CHECK_HEADERS([pthread.h])

  AS_IF(
   [test "x$ac_cv_header_pthread_h" = xno],
   [ac_cv_pthread=no],
   [dnl Check for the individual functions
   ac_cv_pthread=pthread

   dnl Thread functions
   AC_CHECK_LIB(
    pthread,
    pthread_create,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_exit,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_join,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])

   dnl Condition functions
   AC_CHECK_LIB(
    pthread,
    pthread_cond_init,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_cond_destroy,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_cond_broadcast,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_cond_signal,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_cond_wait,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])

   dnl Mutex functions
   AC_CHECK_LIB(
    pthread,
    pthread_mutex_init,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_mutex_destroy,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_mutex_lock,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_mutex_trylock,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_mutex_unlock,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])

   dnl Read/Write lock functions
   AC_CHECK_LIB(
    pthread,
    pthread_rwlock_init,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_rwlock_destroy,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_rwlock_rdlock,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_rwlock_wrlock,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])
   AC_CHECK_LIB(
    pthread,
    pthread_rwlock_unlock,
    [ac_pthread_dummy=yes],
    [ac_cv_pthread=no])

   ac_cv_pthread_LIBADD="-lpthread";
   ])
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

