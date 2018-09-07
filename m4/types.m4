dnl Functions for type definitions
dnl
dnl Version: 20180727

dnl Function to detect if type definitions are available
AC_DEFUN([AX_TYPES_CHECK_LOCAL],
 [AS_IF(
  [test "x$ac_cv_enable_winapi" = xyes],
  [ac_cv_enable_wide_character_type=yes])

 AS_IF(
  [test "x$ac_cv_enable_wide_character_type" = xyes],
  [AC_DEFINE(
   [HAVE_WIDE_CHARACTER_TYPE],
   [1],
   [Define to 1 if wide character type should be used.])
  AC_SUBST(
   [HAVE_WIDE_CHARACTER_TYPE],
   [1]) ],
  [AC_SUBST(
   [HAVE_WIDE_CHARACTER_TYPE],
   [0])
  ])

 AC_CHECK_HEADERS([sys/types.h inttypes.h stdint.h wchar.h])

 AS_IF(
  [test "x$ac_cv_header_sys_types_h" = xyes],
  [AC_SUBST(
   [HAVE_SYS_TYPES_H],
   [1])],
  [AC_SUBST(
   [HAVE_SYS_TYPES_H],
   [0])
  ])

 AS_IF(
  [test "x$ac_cv_header_inttypes_h" = xyes],
  [AC_SUBST(
   [HAVE_INTTYPES_H],
   [1])],
  [AC_SUBST(
   [HAVE_INTTYPES_H],
   [0])
  ])

 AS_IF(
  [test "x$ac_cv_header_stdint_h" = xyes],
  [AC_SUBST(
   [HAVE_STDINT_H],
   [1])],
  [AC_SUBST(
   [HAVE_STDINT_H],
   [0])
  ])

 AS_IF(
  [test "x$ac_cv_header_wchar_h" = xyes],
  [AC_SUBST(
   [HAVE_WCHAR_H],
   [1]) ],
  [AC_SUBST(
   [HAVE_WCHAR_H],
   [0])
  ])

 AC_TYPE_MODE_T
 AC_TYPE_OFF_T
 AC_TYPE_SIZE_T

 AC_CHECK_TYPE(
  [size32_t],
  [AC_SUBST(
   [HAVE_SIZE32_T],
   [1])],
  [AC_SUBST(
   [HAVE_SIZE32_T],
   [0])
  ])

 AC_CHECK_TYPE(
  [ssize32_t],
  [AC_SUBST(
   [HAVE_SSIZE32_T],
   [1])],
  [AC_SUBST(
   [HAVE_SSIZE32_T],
   [0])
  ])

 AC_CHECK_TYPE(
  [size64_t],
  [AC_SUBST(
   [HAVE_SIZE64_T],
   [1])],
  [AC_SUBST(
   [HAVE_SIZE64_T],
   [0])
  ])

 AC_CHECK_TYPE(
  [ssize64_t],
  [AC_SUBST(
   [HAVE_SSIZE64_T],
   [1])],
  [AC_SUBST(
   [HAVE_SSIZE64_T],
   [0])
  ])

 AC_CHECK_TYPE(
  [off64_t],
  [AC_SUBST(
   [HAVE_OFF64_T],
   [1])],
  [AC_SUBST(
   [HAVE_OFF64_T],
   [0])
  ])

 AC_CHECK_TYPE([ssize_t])
 AC_CHECK_TYPE([u64])

 AC_CHECK_SIZEOF([int])
 AC_CHECK_SIZEOF([long])
 AC_CHECK_SIZEOF([off_t])
 AC_CHECK_SIZEOF([size_t])

 AS_IF(
  [test "x$ac_cv_header_wchar_h" = xyes],
  [AC_CHECK_SIZEOF([wchar_t])])
 ])

