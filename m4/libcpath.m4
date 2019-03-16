dnl Checks for libcpath required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libcpath is available
dnl ac_libcpath_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCPATH_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcpath" = xno],
    [ac_cv_libcpath=no],
    [ac_cv_libcpath=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libcpath" != x && test "x$ac_cv_with_libcpath" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libcpath"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libcpath}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libcpath}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libcpath],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcpath],
          [libcpath >= 20180716],
          [ac_cv_libcpath=yes],
          [ac_cv_libcpath=check])
        ])
      AS_IF(
        [test "x$ac_cv_libcpath" = xyes && test "x$ac_cv_enable_wide_character_type" != xno],
        [AC_CACHE_CHECK(
         [whether libcpath/features.h defines LIBCPATH_HAVE_WIDE_CHARACTER_TYPE as 1],
         [ac_cv_header_libcpath_features_h_have_wide_character_type],
         [AC_LANG_PUSH(C)
         AC_COMPILE_IFELSE(
           [AC_LANG_PROGRAM(
             [[#include <libcpath/features.h>]],
             [[#if !defined( LIBCPATH_HAVE_WIDE_CHARACTER_TYPE ) || ( LIBCPATH_HAVE_WIDE_CHARACTER_TYPE != 1 )
#error LIBCPATH_HAVE_WIDE_CHARACTER_TYPE not defined
#endif]] )],
           [ac_cv_header_libcpath_features_h_have_wide_character_type=yes],
           [ac_cv_header_libcpath_features_h_have_wide_character_type=no])
         AC_LANG_POP(C)],
         [ac_cv_header_libcpath_features_h_have_wide_character_type=no])

        AS_IF(
          [test "x$ac_cv_header_libcpath_features_h_have_wide_character_type" = xno],
          [ac_cv_libcpath=no])
        ])
      AS_IF(
        [test "x$ac_cv_libcpath" = xyes],
        [ac_cv_libcpath_CPPFLAGS="$pkg_cv_libcpath_CFLAGS"
        ac_cv_libcpath_LIBADD="$pkg_cv_libcpath_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libcpath" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libcpath.h])

      AS_IF(
        [test "x$ac_cv_header_libcpath_h" = xno],
        [ac_cv_libcpath=no],
        [dnl Check for the individual functions
        ac_cv_libcpath=yes

        AC_CHECK_LIB(
          cpath,
          libcpath_get_version,
          [ac_cv_libcpath_dummy=yes],
          [ac_cv_libcpath=no])

        dnl Path functions
        AC_CHECK_LIB(
          cpath,
          libcpath_path_change_directory,
          [ac_cv_libcpath_dummy=yes],
          [ac_cv_libcpath=no])
        AC_CHECK_LIB(
          cpath,
          libcpath_path_get_current_working_directory,
          [ac_cv_libcpath_dummy=yes],
          [ac_cv_libcpath=no])
        AC_CHECK_LIB(
          cpath,
          libcpath_path_get_full_path,
          [ac_cv_libcpath_dummy=yes],
          [ac_cv_libcpath=no])
        AC_CHECK_LIB(
          cpath,
          libcpath_path_get_sanitized_filename,
          [ac_cv_libcpath_dummy=yes],
          [ac_cv_libcpath=no])
        AC_CHECK_LIB(
          cpath,
          libcpath_path_get_sanitized_path,
          [ac_cv_libcpath_dummy=yes],
          [ac_cv_libcpath=no])
        AC_CHECK_LIB(
          cpath,
          libcpath_path_join,
          [ac_cv_libcpath_dummy=yes],
          [ac_cv_libcpath=no])
        AC_CHECK_LIB(
          cpath,
          libcpath_path_make_directory,
          [ac_cv_libcpath_dummy=yes],
          [ac_cv_libcpath=no])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AC_CHECK_LIB(
            cpath,
            libcpath_path_change_directory_wide,
            [ac_cv_libcpath_dummy=yes],
            [ac_cv_libcpath=no])
          AC_CHECK_LIB(
            cpath,
            libcpath_path_get_current_working_directory_wide,
            [ac_cv_libcpath_dummy=yes],
            [ac_cv_libcpath=no])
          AC_CHECK_LIB(
            cpath,
            libcpath_path_get_full_path_wide,
            [ac_cv_libcpath_dummy=yes],
            [ac_cv_libcpath=no])
          AC_CHECK_LIB(
            cpath,
            libcpath_path_get_sanitized_filename_wide,
            [ac_cv_libcpath_dummy=yes],
            [ac_cv_libcpath=no])
          AC_CHECK_LIB(
            cpath,
            libcpath_path_get_sanitized_path_wide,
            [ac_cv_libcpath_dummy=yes],
            [ac_cv_libcpath=no])
          AC_CHECK_LIB(
            cpath,
            libcpath_path_join_wide,
            [ac_cv_libcpath_dummy=yes],
            [ac_cv_libcpath=no])
          AC_CHECK_LIB(
            cpath,
            libcpath_path_make_directory_wide,
            [ac_cv_libcpath_dummy=yes],
            [ac_cv_libcpath=no])
          ])

        ac_cv_libcpath_LIBADD="-lcpath"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libcpath" != x && test "x$ac_cv_with_libcpath" != xauto-detect && test "x$ac_cv_libcpath" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libcpath in directory: $ac_cv_with_libcpath],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libcpath" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCPATH],
      [1],
      [Define to 1 if you have the `cpath' library (-lcpath).])
    ])

  AS_IF(
    [test "x$ac_cv_libcpath" = xyes],
    [AC_SUBST(
      [HAVE_LIBCPATH],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCPATH],
      [0])
    ])
  ])

dnl Function to detect if mkdir is available
dnl Also checks how to use mkdir
AC_DEFUN([AX_LIBCPATH_CHECK_FUNC_MKDIR],
  [AC_CHECK_FUNCS([mkdir])

  AS_IF(
    [test "x$ac_cv_func_mkdir" = xyes],
    [AC_MSG_CHECKING(
      [how to use mkdir])

    SAVE_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS -Wall -Werror"
    AC_LANG_PUSH(C)

    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <sys/stat.h>
#include <sys/types.h>]],
        [[mkdir( "", 0 )]] )],
        [AC_MSG_RESULT(
          [with additional mode argument])
        ac_cv_cv_mkdir_mode=yes],
        [ac_cv_cv_mkdir_mode=no])

    AS_IF(
      [test "x$ac_cv_cv_mkdir_mode" = xno],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [[#include <io.h>]],
          [[mkdir( "" )]] )],
        [AC_MSG_RESULT(
          [with single argument])
        ac_cv_cv_mkdir=yes],
        [ac_cv_cv_mkdir=no])
      ])

    AC_LANG_POP(C)
    CFLAGS="$SAVE_CFLAGS"

    AS_IF(
      [test "x$ac_cv_cv_mkdir_mode" = xno && test "x$ac_cv_cv_mkdir" = xno],
      [AC_MSG_WARN(
        [unknown])
      ac_cv_func_mkdir=no])

    AS_IF(
      [test "x$ac_cv_func_mkdir" = xyes],
      [AC_DEFINE(
        [HAVE_MKDIR],
        [1],
        [Define to 1 if you have the mkdir function.])
      ])

    AS_IF(
      [test "x$ac_cv_cv_mkdir_mode" = xyes],
      [AC_DEFINE(
        [HAVE_MKDIR_MODE],
        [1],
        [Define to 1 if you have the mkdir function with a second mode argument.])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_func_mkdir" = xno],
    [AC_MSG_FAILURE(
      [Missing function: mkdir],
      [1])
    ])
  ])

dnl Function to detect if libcpath dependencies are available
AC_DEFUN([AX_LIBCPATH_CHECK_LOCAL],
  [dnl Headers included in libcpath/libcpath_path.h
  AC_CHECK_HEADERS([errno.h sys/stat.h sys/syslimits.h])

  dnl Path functions used in libcpath/libcpath_path.h
  AC_CHECK_FUNCS([chdir getcwd])

  AS_IF(
    [test "x$ac_cv_func_chdir" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: chdir],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_getcwd" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: getcwd],
      [1])
    ])

  AX_LIBCPATH_CHECK_FUNC_MKDIR

  ac_cv_libcpath_CPPFLAGS="-I../libcpath";
  ac_cv_libcpath_LIBADD="../libcpath/libcpath.la";

  ac_cv_libcpath=local
  ])

dnl Function to detect how to enable libcpath
AC_DEFUN([AX_LIBCPATH_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libcpath],
    [libcpath],
    [search for libcpath in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBCPATH_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libcpath" != xyes],
    [AX_LIBCPATH_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBCPATH],
      [1],
      [Define to 1 if the local version of libcpath is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBCPATH],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCPATH],
    [test "x$ac_cv_libcpath" = xlocal])
  AS_IF(
    [test "x$ac_cv_libcpath_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCPATH_CPPFLAGS],
      [$ac_cv_libcpath_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcpath_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCPATH_LIBADD],
      [$ac_cv_libcpath_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcpath" = xyes],
    [AC_SUBST(
      [ax_libcpath_pc_libs_private],
      [-lcpath])
    ])

  AS_IF(
    [test "x$ac_cv_libcpath" = xyes],
    [AC_SUBST(
      [ax_libcpath_spec_requires],
      [libcpath])
    AC_SUBST(
      [ax_libcpath_spec_build_requires],
      [libcpath-devel])
    ])
  ])

