dnl Checks for tools required headers and functions
dnl
dnl Version: 20260601

dnl Function to determine how to link with MinGW binmode
AC_DEFUN([AX_TOOLS_CHECK_MINGW_BINMODE],
  [MINGW_BINMODE_LDFLAGS=""

  case "$host_os" in
    *mingw* | *cygwin*)
      AC_LANG_PUSH(C)

      AC_MSG_CHECKING([how to link with MinGW binmode])

      BACKUP_LDFLAGS="$LDFLAGS"

      LDFLAGS="$LDFLAGS -lbinmode"

      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM([], [])],
        [MINGW_BINMODE_LDFLAGS="-lbinmode"],
        [MINGW_BINMODE_LDFLAGS=""])

      LDFLAGS="$BACKUP_LDFLAGS"

      AS_IF(
        [test "x$MINGW_BINMODE_LDFLAGS" = "x"],
        [PATH_BINMODE_O=`$CC -print-file-name=binmode.o`

        AS_IF(
          [test -f "$PATH_BINMODE_O"],
          [MINGW_BINMODE_LDFLAGS="-Wl,$PATH_BINMODE_O"],
          [MINGW_BINMODE_LDFLAGS=""])

        AS_IF(
          [test "x$MINGW_BINMODE_LDFLAGS" = "x"],
          [AC_MSG_RESULT([undetermined])],
          [AC_MSG_RESULT([$MINGW_BINMODE_LDFLAGS])])
      ])
      ;;
    *)
      ;;
  esac
])

dnl Function to detect whether MinGW binmode support should be enabled
AC_DEFUN([AX_TOOLS_CHECK_ENABLE_MINGW_BINMODE],
  [AX_COMMON_ARG_ENABLE(
    [mingw-binmode],
    [mingw_binmode],
    [build with MinGW binmode],
    [no])

  AS_IF(
    [test "x$ac_cv_enable_mingw_binmode" != xno],
    [AX_TOOLS_CHECK_MINGW_BINMODE

    AS_IF(
      [test "x$MINGW_BINMODE_LDFLAGS" = xno],
      [AC_MSG_FAILURE(
        [Unable to build with functional MinGW binmode],
        [1])
      ])

    AC_DEFINE(
      [HAVE_MINGW_BINMODE],
      [1],
      [Define to 1 if MinGW binmode should be used.])

    LDFLAGS="$LDFLAGS $MINGW_BINMODE_LDFLAGS"
  ])
])

dnl Function to detect if fsapfstools dependencies are available
AC_DEFUN([AX_FSAPFSTOOLS_CHECK_LOCAL],
  [AC_CHECK_HEADERS([signal.h sys/signal.h unistd.h])

  AC_CHECK_FUNCS([close getopt setvbuf])

  AS_IF(
   [test "x$ac_cv_func_close" != xyes],
   [AC_MSG_FAILURE(
     [Missing function: close],
     [1])
  ])

  dnl Headers included in fsapfstools/fsapfsmount.c
  AC_CHECK_HEADERS([errno.h sys/time.h])

  dnl Functions included in fsapfstools/mount_file_system.c and fsapfstools/mount_file_entry.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_FUNCS([clock_gettime getegid geteuid time])
  ])

  AX_TOOLS_CHECK_ENABLE_MINGW_BINMODE
])

