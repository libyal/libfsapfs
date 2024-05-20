dnl Checks for libcfile required headers and functions
dnl
dnl Version: 20240514

dnl Function to detect if libcfile is available
dnl ac_libcfile_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCFILE_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcfile" = xno],
    [ac_cv_libcfile=no],
    [ac_cv_libcfile=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libcfile which returns "yes" and --with-libcfile= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libcfile" != x && test "x$ac_cv_with_libcfile" != xauto-detect && test "x$ac_cv_with_libcfile" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libcfile])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcfile],
          [libcfile >= 20160409],
          [ac_cv_libcfile=yes],
          [ac_cv_libcfile=check])
        ])
      AS_IF(
        [test "x$ac_cv_libcfile" = xyes && test "x$ac_cv_enable_wide_character_type" != xno],
        [AC_CACHE_CHECK(
         [whether libcfile/features.h defines LIBCFILE_HAVE_WIDE_CHARACTER_TYPE as 1],
         [ac_cv_header_libcfile_features_h_have_wide_character_type],
         [AC_LANG_PUSH(C)
         AC_COMPILE_IFELSE(
           [AC_LANG_PROGRAM(
             [[#include <libcfile/features.h>]],
             [[#if !defined( LIBCFILE_HAVE_WIDE_CHARACTER_TYPE ) || ( LIBCFILE_HAVE_WIDE_CHARACTER_TYPE != 1 )
#error LIBCFILE_HAVE_WIDE_CHARACTER_TYPE not defined
#endif]] )],
           [ac_cv_header_libcfile_features_h_have_wide_character_type=yes],
           [ac_cv_header_libcfile_features_h_have_wide_character_type=no])
         AC_LANG_POP(C)],
         [ac_cv_header_libcfile_features_h_have_wide_character_type=no])

        AS_IF(
          [test "x$ac_cv_header_libcfile_features_h_have_wide_character_type" = xno],
          [ac_cv_libcfile=no])
        ])
      AS_IF(
        [test "x$ac_cv_libcfile" = xyes],
        [ac_cv_libcfile_CPPFLAGS="$pkg_cv_libcfile_CFLAGS"
        ac_cv_libcfile_LIBADD="$pkg_cv_libcfile_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libcfile" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libcfile.h])

      AS_IF(
        [test "x$ac_cv_header_libcfile_h" = xno],
        [ac_cv_libcfile=no],
        [ac_cv_libcfile=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libcfile],
          [cfile],
          [[libcfile_get_version],
           [libcfile_file_initialize],
           [libcfile_file_free],
           [libcfile_file_open],
           [libcfile_file_open_with_error_code],
           [libcfile_file_close],
           [libcfile_file_read_buffer],
           [libcfile_file_read_buffer_with_error_code],
           [libcfile_file_write_buffer],
           [libcfile_file_write_buffer_with_error_code],
           [libcfile_file_seek_offset],
           [libcfile_file_resize],
           [libcfile_file_is_open],
           [libcfile_file_get_offset],
           [libcfile_file_get_size],
           [libcfile_file_is_device],
           [libcfile_file_io_control_read],
           [libcfile_file_io_control_read_with_error_code],
           [libcfile_file_exists],
           [libcfile_file_remove]])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AX_CHECK_LIB_FUNCTIONS(
            [libcfile],
            [cfile],
            [[libcfile_file_open_wide],
             [libcfile_file_open_wide_with_error_code],
             [libcfile_file_exists_wide],
             [libcfile_file_remove_wide]])
          ])

        ac_cv_libcfile_LIBADD="-lcfile"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libcfile])
    ])

  AS_IF(
    [test "x$ac_cv_libcfile" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCFILE],
      [1],
      [Define to 1 if you have the `cfile' library (-lcfile).])
    ])

  AS_IF(
    [test "x$ac_cv_libcfile" = xyes],
    [AC_SUBST(
      [HAVE_LIBCFILE],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCFILE],
      [0])
    ])
  ])

dnl Function to detect if posix_fadvise is available
AC_DEFUN([AX_LIBCFILE_CHECK_FUNC_POSIX_FADVISE],
  [AC_CHECK_FUNCS([posix_fadvise])

  AS_IF(
    [test "x$ac_cv_func_posix_fadvise" = xyes],
    [AC_MSG_CHECKING(
        [whether posix_fadvise can be linked])

      SAVE_CFLAGS="$CFLAGS"
      CFLAGS="$CFLAGS -Wall -Werror"
      AC_LANG_PUSH(C)

      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [[#include <fcntl.h>]],
          [[#if !defined( POSIX_FADV_SEQUENTIAL )
#define POSIX_FADV_SEQUENTIAL 2
#endif
posix_fadvise( 0, 0, 0, POSIX_FADV_SEQUENTIAL )]] )],
          [ac_cv_func_posix_fadvise=yes],
          [ac_cv_func_posix_fadvise=no])

      AC_LANG_POP(C)
      CFLAGS="$SAVE_CFLAGS"

      AS_IF(
        [test "x$ac_cv_func_posix_fadvise" = xyes],
        [AC_MSG_RESULT(
          [yes])
        AC_DEFINE(
          [HAVE_POSIX_FADVISE],
          [1],
          [Define to 1 if you have the posix_fadvise function.]) ],
        [AC_MSG_RESULT(
          [no]) ])
    ])
  ])

dnl Function to detect if libcfile dependencies are available
AC_DEFUN([AX_LIBCFILE_CHECK_LOCAL],
  [dnl Headers included in libcfile/libcfile_file.c and libcfile/libcfile_support.c
  AC_CHECK_HEADERS([errno.h stdio.h sys/stat.h])

  dnl Headers included in libcfile/libcfile_file.c
  AC_CHECK_HEADERS([cygwin/fs.h fcntl.h linux/fs.h sys/disk.h sys/disklabel.h sys/ioctl.h unistd.h])

  dnl File input/output functions used in libcfile/libcfile_file.c
  AC_CHECK_FUNCS([close fstat ftruncate ioctl lseek open read write])

  AS_IF(
    [test "x$ac_cv_func_close" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: close],
      [1])
    ])

  AX_LIBCFILE_CHECK_FUNC_POSIX_FADVISE

  AS_IF(
    [test "x$ac_cv_func_fstat" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: fstat],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_ftruncate" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: ftruncate],
      [1])
    ])

  AS_IF(
    [test x"$ac_cv_enable_winapi" = xno],
    [AS_IF(
      [test "x$ac_cv_func_ioctl" != xyes],
      [AC_MSG_FAILURE(
        [Missing function: ioctl],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_func_lseek" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: lseek],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_open" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: open],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_read" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: read],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_write" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: write],
      [1])
    ])

  dnl File input/output functions used in libcfile/libcfile_support.c
  AC_CHECK_FUNCS([stat unlink])

  AS_IF(
    [test "x$ac_cv_func_stat" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: stat],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_unlink" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: unlink],
      [1])
    ])

  ac_cv_libcfile_CPPFLAGS="-I../libcfile -I\$(top_srcdir)/libcfile";
  ac_cv_libcfile_LIBADD="../libcfile/libcfile.la";

  ac_cv_libcfile=local
  ])

dnl Function to detect how to enable libcfile
AC_DEFUN([AX_LIBCFILE_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libcfile],
    [libcfile],
    [search for libcfile in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBCFILE_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libcfile" != xyes],
    [AX_LIBCFILE_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBCFILE],
      [1],
      [Define to 1 if the local version of libcfile is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBCFILE],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCFILE],
    [test "x$ac_cv_libcfile" = xlocal])
  AS_IF(
    [test "x$ac_cv_libcfile_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCFILE_CPPFLAGS],
      [$ac_cv_libcfile_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcfile_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCFILE_LIBADD],
      [$ac_cv_libcfile_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcfile" = xyes],
    [AC_SUBST(
      [ax_libcfile_pc_libs_private],
      [-lcfile])
    ])

  AS_IF(
    [test "x$ac_cv_libcfile" = xyes],
    [AC_SUBST(
      [ax_libcfile_spec_requires],
      [libcfile])
    AC_SUBST(
      [ax_libcfile_spec_build_requires],
      [libcfile-devel])
    ])
  ])

