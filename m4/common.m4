dnl Checks for common headers and functions
dnl
dnl Version: 20181117

dnl Function to test if a certain feature was disabled
AC_DEFUN([AX_COMMON_ARG_DISABLE],
[
  AC_ARG_ENABLE(
    [$1],
    [AS_HELP_STRING(
      [--disable-$1],
      [$3])],
    [ac_cv_enable_$2=$enableval],
    [ac_cv_enable_$2="yes"])dnl

    AC_CACHE_CHECK(
      [whether to disable $3],
      [ac_cv_enable_$2],
      [ac_cv_enable_$2="yes"])dnl
])

dnl Function to test if a certain feature was enabled
AC_DEFUN([AX_COMMON_ARG_ENABLE],
[
  AC_ARG_ENABLE(
    [$1],
    [AS_HELP_STRING(
      [--enable-$1],
      [$3 [default=$4]])],
    [ac_cv_enable_$2=$enableval],
    [ac_cv_enable_$2=$4])dnl

    AC_CACHE_CHECK(
      [whether to enable $3],
      [ac_cv_enable_$2],
      [ac_cv_enable_$2=$4])dnl
])

dnl Function to test if the location of a certain feature was provided
AC_DEFUN([AX_COMMON_ARG_WITH],
[
  AC_ARG_WITH(
    [$1],
    [AS_HELP_STRING(
      [--with-$1[[=$5]]],
      [$3 [default=$4]])],
    [ac_cv_with_$2=$withval],
    [ac_cv_with_$2=$4])dnl

    AC_CACHE_CHECK(
      [whether to use $3],
      [ac_cv_with_$2],
      [ac_cv_with_$2=$4])dnl
])

dnl Function to detect whether shared libary support should be disabled
AC_DEFUN([AX_COMMON_CHECK_DISABLE_SHARED_LIBS],
[
  AX_COMMON_ARG_DISABLE(
    [shared-libs],
    [shared_libs],
    [disable shared library support])
])

dnl Function to detect whether debug output should be enabled
AC_DEFUN([AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT],
[
  AX_COMMON_ARG_ENABLE(
    [debug-output],
    [debug_output],
    [enable debug output],
    [no])

  AS_IF(
    [test "x$ac_cv_enable_debug_output" != xno ],
    [AC_DEFINE(
      [HAVE_DEBUG_OUTPUT],
      [1],
      [Define to 1 if debug output should be used.])

    ac_cv_enable_debug_output=yes])
])

dnl Function to detect whether static executables support should be enabled
AC_DEFUN([AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES],
[
  AX_COMMON_ARG_ENABLE(
    [static-executables],
    [static_executables],
    [build static executables (binaries)],
    [no])

  AS_IF(
    [test "x$ac_cv_enable_static_executables" != xno],
    [STATIC_LDFLAGS="-all-static";

    AC_SUBST(
      [STATIC_LDFLAGS])

    ac_cv_enable_static_executables=yes
    enable_shared=no])
])

dnl Function to detect whether verbose output should be enabled
AC_DEFUN([AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT],
[
  AX_COMMON_ARG_ENABLE(
    [verbose-output],
    [verbose_output],
    [enable verbose output],
    [no])

  AS_IF(
    [test "x$ac_cv_enable_verbose_output" != xno ],
    [AC_DEFINE(
      [HAVE_VERBOSE_OUTPUT],
      [1],
      [Define to 1 if verbose output should be used.])

    ac_cv_enable_verbose_output=yes])
])

dnl Function to detect whether static executables support should be enabled
AC_DEFUN([AX_COMMON_CHECK_ENABLE_WIDE_CHARACTER_TYPE],
[
  AX_COMMON_ARG_ENABLE(
    [wide-character-type],
    [wide_character_type],
    [enable wide character type support],
    [no])
])

dnl Function to detect whether WINAPI support should be enabled
AC_DEFUN([AX_COMMON_CHECK_ENABLE_WINAPI],
[
  AX_COMMON_ARG_ENABLE(
    [winapi],
    [winapi],
    [enable WINAPI support for cross-compilation],
    [auto-detect])

  AS_IF(
    [test "x$ac_cv_enable_winapi" = xauto-detect],
    [ac_common_check_winapi_target_string="$target"

    AS_IF(
      [test "x$ac_common_check_winapi_target_string" = x],
      [ac_common_check_winapi_target_string="$host"])

    AS_CASE(
      [$ac_common_check_winapi_target_string],
      [*mingw*],[AC_MSG_NOTICE(
        [detected MinGW enabling WINAPI support for cross-compilation])
        ac_cv_enable_winapi=yes],
      [*msys*],[AC_MSG_NOTICE(
        [detected MSYS enabling WINAPI support for cross-compilation])
        ac_cv_enable_winapi=yes],
      [*],[ac_cv_enable_winapi=no])
  ])
])

dnl Function to detect whether printf conversion specifier "%jd" is available
AC_DEFUN([AX_COMMON_CHECK_FUNC_PRINTF_JD],
[
  AC_MSG_CHECKING(
    [whether printf supports the conversion specifier "%jd"])

  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -Wall -Werror"
  AC_LANG_PUSH(C)

  dnl First try to see if compilation and linkage without a parameter succeeds
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <stdio.h>]],
      [[printf( "%jd" ); ]] )],
    [ac_cv_cv_have_printf_jd=no],
    [ac_cv_cv_have_printf_jd=yes])

  dnl Second try to see if compilation and linkage with a parameter succeeds
  AS_IF(
    [test "x$ac_cv_cv_have_printf_jd" = xyes],
    [AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <sys/types.h>
#include <stdio.h>]],
        [[printf( "%jd", (off_t) 10 ); ]] )],
      [ac_cv_cv_have_printf_jd=yes],
      [ac_cv_cv_have_printf_jd=no])
  ])

  dnl Third try to see if the program runs correctly
  AS_IF(
    [test "x$ac_cv_cv_have_printf_jd" = xyes],
    [AC_RUN_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <sys/types.h>
#include <stdio.h>]],
        [[char string[ 3 ];
if( snprintf( string, 3, "%jd", (off_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
      [ac_cv_cv_have_printf_jd=yes],
      [ac_cv_cv_have_printf_jd=no],
      [ac_cv_cv_have_printf_jd=undetermined])
  ])

  AC_LANG_POP(C)
  CFLAGS="$SAVE_CFLAGS"

  AS_IF(
    [test "x$ac_cv_cv_have_printf_jd" = xyes],
    [AC_MSG_RESULT(
      [yes])
    AC_DEFINE(
      [HAVE_PRINTF_JD],
      [1],
      [Define to 1 whether printf supports the conversion specifier "%jd".]) ],
    [AC_MSG_RESULT(
      [$ac_cv_cv_have_printf_jd])
  ])
])

dnl Function to detect whether printf conversion specifier "%zd" is available
AC_DEFUN([AX_COMMON_CHECK_FUNC_PRINTF_ZD],
[
  AC_MSG_CHECKING(
    [whether printf supports the conversion specifier "%zd"])

  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -Wall -Werror"
  AC_LANG_PUSH(C)

  dnl First try to see if compilation and linkage without a parameter succeeds
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <stdio.h>]],
      [[printf( "%zd" ); ]] )],
    [ac_cv_cv_have_printf_zd=no],
    [ac_cv_cv_have_printf_zd=yes])

  dnl Second try to see if compilation and linkage with a parameter succeeds
  AS_IF(
    [test "x$ac_cv_cv_have_printf_zd" = xyes],
    [AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <sys/types.h>
#include <stdio.h>]],
        [[printf( "%zd", (size_t) 10 ); ]] )],
      [ac_cv_cv_have_printf_zd=yes],
      [ac_cv_cv_have_printf_zd=no])
  ])

  dnl Third try to see if the program runs correctly
  AS_IF(
    [test "x$ac_cv_cv_have_printf_zd" = xyes],
    [AC_RUN_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <sys/types.h>
#include <stdio.h>]],
        [[char string[ 3 ];
if( snprintf( string, 3, "%zd", (size_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
      [ac_cv_cv_have_printf_zd=yes],
      [ac_cv_cv_have_printf_zd=no],
      [ac_cv_cv_have_printf_zd=undetermined])
  ])

  AC_LANG_POP(C)
  CFLAGS="$SAVE_CFLAGS"

  AS_IF(
    [test "x$ac_cv_cv_have_printf_zd" = xyes],
    [AC_MSG_RESULT(
      [yes])
    AC_DEFINE(
      [HAVE_PRINTF_ZD],
      [1],
      [Define to 1 whether printf supports the conversion specifier "%zd".]) ],
    [AC_MSG_RESULT(
      [$ac_cv_cv_have_printf_zd])
  ])
])

dnl Function to detect if common dependencies are available
AC_DEFUN([AX_COMMON_CHECK_LOCAL],
[
  dnl Headers included in common/common.h
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xyes],
    [AC_CHECK_HEADERS([windows.h])

    AS_IF(
      [test "x$ac_cv_header_windows_h" = xno],
      [AC_MSG_FAILURE(
        [Missing header: windows.h header is required to compile with winapi support],
        [1])
    ])
  ])

  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_HEADERS([libintl.h])
  ])

  dnl Headers included in common/types.h
  AC_CHECK_HEADERS([limits.h])

  dnl Headers included in common/memory.h and common/narrow_string.h
  AC_CHECK_HEADERS([stdlib.h string.h])

  dnl Headers included in common/wide_string.h
  AC_CHECK_HEADERS([wchar.h wctype.h])

  dnl File stream functions used in common/file_stream.h
  AC_CHECK_FUNCS([fclose feof fgets fopen fread fseeko fseeko64 fwrite vfprintf])

  AS_IF(
    [test "x$ac_cv_func_fclose" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: fclose],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_feof" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: feof],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_fgets" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: fgets],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_fopen" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: fopen],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_fread" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: fread],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_fseeko" != xyes && test "x$ac_cv_func_fseeko64" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: fseeko and fseeko64],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_fwrite" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: fwrite],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_vfprintf" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: vfprintf],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_enable_wide_character_type" != xno],
    [AC_CHECK_FUNCS([fgetws])

    AS_IF(
      [test "x$ac_cv_func_fgetws" != xyes],
      [AC_MSG_FAILURE(
        [Missing function: fgetws],
        [1])
    ])
  ])

  dnl Memory functions used in common/memory.h
  AC_CHECK_FUNCS([free malloc memcmp memcpy memset realloc])

  AS_IF(
    [test "x$ac_cv_func_free" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: free],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_malloc" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: malloc],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_memcmp" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: memcmp],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_memcpy" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: memcpy],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_memset" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: memset],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_realloc" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: realloc],
      [1])
  ])

  dnl Narrow character string functions used in common/narrow_string.h
  AC_CHECK_FUNCS([memchr memrchr snprintf sscanf strcasecmp strchr strlen strncasecmp strncmp strncpy strnicmp strrchr strstr vsnprintf])

  AS_IF(
    [test "x$ac_cv_func_memchr" != xyes && test "x$ac_cv_func_strchr" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: memchr and strchr],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_memcmp" != xyes && test "x$ac_cv_func_strncmp" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: memcmp and strncmp],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_memcpy" != xyes && test "x$ac_cv_func_strncpy" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: memcpy and strncpy],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_memrchr" = xyes],
    [AC_CHECK_DECLS([memrchr])

    AS_IF(
      [test "x$ac_cv_decl_memrchr" != xyes],
      [ac_cv_func_memrchr=no])
  ])

  AS_IF(
    [test "x$ac_cv_func_memrchr" != xyes && test "x$ac_cv_func_strrchr" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: strrchr and memrchr],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_snprintf" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: snprintf],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_sscanf" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: sscanf],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_strlen" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: strlen],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_strcasecmp" != xyes && test "x$ac_cv_func_strncasecmp" != xyes && test "x$ac_cv_func_strnicmp" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: strncasecmp, strcasecmp and strnicmp],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_strstr" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: strstr],
      [1])
  ])

  AS_IF(
    [test "x$ac_cv_func_vsnprintf" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: vsnprintf],
      [1])
  ])

  dnl Wide character string functions used in common/wide_string.h
  AS_IF(
    [test "x$ac_cv_enable_wide_character_type" != xno],
    [AC_CHECK_FUNCS([swprintf towlower wcscasecmp wcschr wcslen wcsncasecmp wcsncmp wcsncpy wcsnicmp wcsrchr wcsstr wmemchr wmemcmp wmemcpy wmemrchr])

    AS_IF(
      [test "x$ac_cv_func_swprintf" != xyes],
      [AC_MSG_FAILURE(
        [Missing function: swprintf],
        [1])
    ])

    AS_IF(
      [test "x$ac_cv_func_wmemchr" != xyes && test "x$ac_cv_func_wcschr" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: wmemchr and wcschr],
        [1])
    ])

    AS_IF(
      [test "x$ac_cv_func_wmemcmp" != xyes && test "x$ac_cv_func_wcsncmp" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: wmemcmp and wcsncmp],
        [1])
    ])

    AS_IF(
      [test "x$ac_cv_func_wmemcpy" != xyes && test "x$ac_cv_func_wcsncpy" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: wmemcpy and wcsncpy],
        [1])
    ])

    AS_IF(
      [test "x$ac_cv_func_wmemrchr" != xyes && test "x$ac_cv_func_wcsrchr" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: wmemrchr and wcsrchr],
        [1])
    ])

    AS_IF(
      [test "x$ac_cv_func_wcslen" != xyes],
      [AC_MSG_FAILURE(
        [Missing function: wcslen],
        [1])
    ])

    AS_IF(
      [test "x$ac_cv_func_wcsncasecmp" != xyes && test "x$ac_cv_func_wcscasecmp" != xyes && test "x$ac_cv_func_wcsnicmp" != xyes && test "x$ac_cv_func_towlower" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: wcsncasecmp, wcscasecmp, wcsnicmp and towlower],
        [1])
    ])

    AS_IF(
      [test "x$ac_cv_func_wcsstr" != xyes],
      [AC_MSG_FAILURE(
        [Missing function: wcsstr],
        [1])
    ])
  ])

  dnl Check for printf conversion specifier support
  AX_COMMON_CHECK_FUNC_PRINTF_JD
  AX_COMMON_CHECK_FUNC_PRINTF_ZD
])

