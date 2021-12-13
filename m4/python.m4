dnl Functions for Python bindings
dnl
dnl Version: 20211114

dnl Function to check if the python binary is available
dnl "python${PYTHON_VERSION} python python# python#.#"
AC_DEFUN([AX_PROG_PYTHON],
  [AS_IF(
    [test "x${PYTHON_VERSION}" != x],
    [ax_python_progs="python${PYTHON_VERSION}"],
    [ax_python_progs="python python3 python3.11 python3.10 python3.9 python3.8 python3.7 python3.6 python3.5 python3.4 python3.3 python3.2 python3.1 python3.0 python2 python2.7 python2.6 python2.5"])
  AC_CHECK_PROGS(
    [PYTHON],
    [$ax_python_progs])
  AS_IF(
    [test "x${PYTHON}" != x],
    [ax_prog_python_version=`${PYTHON} -c "import sys; sys.stdout.write('%d.%d' % (sys.version_info[[0]], sys.version_info[[1]]))" 2>/dev/null`;
    ax_prog_python_platform=`${PYTHON} -c "import sys; sys.stdout.write(sys.platform)" 2>/dev/null`;
    AC_SUBST(
      [PYTHON_PLATFORM],
      [$ax_prog_python_platform])
    ],
    [AC_MSG_ERROR(
      [Unable to find python])
    ])
  AC_SUBST(
    [PYTHON],
    [$PYTHON])
  ])

dnl Function to check if the python2 binary is available
dnl "python2 python2.#"
AC_DEFUN([AX_PROG_PYTHON2],
  [ax_python2_progs="python2 python2.7 python2.6 python2.5"
  AC_CHECK_PROGS(
    [PYTHON2],
    [$ax_python2_progs])
  AS_IF(
    [test "x${PYTHON2}" != x],
    [ax_prog_python2_version=`${PYTHON2} -c "import sys; sys.stdout.write('%d.%d' % (sys.version_info[[0]], sys.version_info[[1]]))" 2>/dev/null`;
    AC_SUBST(
      [PYTHON2_VERSION],
      [$ax_prog_python2_version])

    ax_prog_python2_platform=`${PYTHON2} -c "import sys; sys.stdout.write(sys.platform)" 2>/dev/null`;
    AC_SUBST(
      [PYTHON2_PLATFORM],
      [$ax_prog_python2_platform])
    ],
    [AC_MSG_ERROR(
      [Unable to find python2])
    ])
  AC_SUBST(
    [PYTHON2],
    [$PYTHON2])
  ])

dnl Function to check if the python3 binary is available
dnl "python3 python3.#"
AC_DEFUN([AX_PROG_PYTHON3],
  [ax_python3_progs="python3 python3.11 python3.10 python3.9 python3.8 python3.7 python3.6 python3.5 python3.4 python3.3 python3.2 python3.1 python3.0"
  AC_CHECK_PROGS(
    [PYTHON3],
    [$ax_python3_progs])
  AS_IF(
    [test "x${PYTHON3}" != x],
    [ax_prog_python3_version=`${PYTHON3} -c "import sys; sys.stdout.write('%d.%d' % (sys.version_info[[0]], sys.version_info[[1]]))" 2>/dev/null`;
    AC_SUBST(
      [PYTHON3_VERSION],
      [$ax_prog_python3_version])

    ax_prog_python3_platform=`${PYTHON3} -c "import sys; sys.stdout.write(sys.platform)" 2>/dev/null`;
    AC_SUBST(
      [PYTHON3_PLATFORM],
      [$ax_prog_python3_platform])
    ],
    [AC_MSG_ERROR(
      [Unable to find python3])
    ])
  AC_SUBST(
    [PYTHON3],
    [$PYTHON3])
  ])

dnl Function to check if the python-config binary is available
dnl "python${PYTHON_VERSION}-config python-config"
AC_DEFUN([AX_PROG_PYTHON_CONFIG],
  [AS_IF(
    [test "x${PYTHON_CONFIG}" = x && test "x${PYTHON_VERSION}" != x],
    [AC_CHECK_PROGS(
      [PYTHON_CONFIG],
      [python${PYTHON_VERSION}-config])
    ])
  AS_IF(
    [test "x${PYTHON_CONFIG}" = x],
    [AC_CHECK_PROGS(
      [PYTHON_CONFIG],
      [python-config python3-config python3.11-config python3.10-config python3.9-config python3.8-config python3.7-config python3.6-config python3.5-config python3.4-config python3.3-config python3.2-config python3.1-config python3.0-config python2-config python2.7-config python2.6-config python2.5-config])
    ])
  AS_IF(
    [test "x${PYTHON_CONFIG}" = x],
    [AC_MSG_ERROR(
      [Unable to find python-config])
    ])
  AC_SUBST(
    [PYTHON_CONFIG],
    [$PYTHON_CONFIG])
  ])

dnl Function to check if the python2-config binary is available
AC_DEFUN([AX_PROG_PYTHON2_CONFIG],
  [AS_IF(
    [test "x${PYTHON2_CONFIG}" = x],
    [AC_CHECK_PROGS(
      [PYTHON2_CONFIG],
      [python2-config python2.7-config python2.6-config python2.5-config])
    ])
  AS_IF(
    [test "x${PYTHON2_CONFIG}" = x],
    [AC_MSG_ERROR(
      [Unable to find python2-config])
    ])
  AC_SUBST(
    [PYTHON2_CONFIG],
    [$PYTHON2_CONFIG])
  ])

dnl Function to check if the python3-config binary is available
AC_DEFUN([AX_PROG_PYTHON3_CONFIG],
  [AS_IF(
    [test "x${PYTHON3_CONFIG}" = x],
    [AC_CHECK_PROGS(
      [PYTHON3_CONFIG],
      [python3-config python3.11-config python3.10-config python3.9-config python3.8-config python3.7-config python3.6-config python3.5-config python3.4-config python3.3-config python3.2-config python3.1-config python3.0-config])
    ])
  AS_IF(
    [test "x${PYTHON3_CONFIG}" = x],
    [AC_MSG_ERROR(
      [Unable to find python3-config])
    ])
  AC_SUBST(
    [PYTHON3_CONFIG],
    [$PYTHON3_CONFIG])
  ])

dnl Function to detect if a Python build environment is available
AC_DEFUN([AX_PYTHON_CHECK],
  [AX_PROG_PYTHON
  AX_PROG_PYTHON_CONFIG

  AS_IF(
    [test "x${PYTHON_CONFIG}" != x],
    [dnl Check for Python includes
    PYTHON_INCLUDES=`${PYTHON_CONFIG} --includes 2>/dev/null`;

    AC_MSG_CHECKING(
      [for Python includes])
    AC_MSG_RESULT(
      [$PYTHON_INCLUDES])

    dnl Check for Python libraries
    PYTHON_LDFLAGS=`${PYTHON_CONFIG} --ldflags 2>/dev/null`;

    AC_MSG_CHECKING(
      [for Python libraries])
    AC_MSG_RESULT(
      [$PYTHON_LDFLAGS])

    dnl For CygWin add the -no-undefined linker flag
    AS_CASE(
      [$host_os],
      [cygwin*],[PYTHON_LDFLAGS="${PYTHON_LDFLAGS} -no-undefined"],
      [*],[])

    dnl Check for the existence of Python.h
    BACKUP_CPPFLAGS="${CPPFLAGS}"
    CPPFLAGS="${CPPFLAGS} ${PYTHON_INCLUDES}"

    AC_CHECK_HEADERS(
      [Python.h],
      [ac_cv_header_python_h=yes],
      [ac_cv_header_python_h=no])

    CPPFLAGS="${BACKUP_CPPFLAGS}"
  ])

  AS_IF(
    [test "x${ac_cv_header_python_h}" != xyes],
    [ac_cv_enable_python=no],
    [ac_cv_enable_python=${ax_prog_python_version}
    AC_SUBST(
      [PYTHON_CPPFLAGS],
      [$PYTHON_INCLUDES])

    AC_SUBST(
      [PYTHON_LDFLAGS],
      [$PYTHON_LDFLAGS])

    dnl Check for Python prefix
    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python_prefix="\${prefix}"],
      [ax_python_prefix=`${PYTHON_CONFIG} --prefix 2>/dev/null`])

    AC_SUBST(
      [PYTHON_PREFIX],
      [$ax_python_prefix])

    dnl Check for Python exec-prefix
    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python_exec_prefix="\${exec_prefix}"],
      [ax_python_exec_prefix=`${PYTHON_CONFIG} --exec-prefix 2>/dev/null`])

    AC_SUBST(
      [PYTHON_EXEC_PREFIX],
      [$ax_python_exec_prefix])

    dnl Check for Python library directory
    ax_python_pythondir_suffix=`${PYTHON} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(0, 0, prefix=''))" 2>/dev/null`;

    AS_IF(
      [test "x${ac_cv_with_pythondir}" = x || test "x${ac_cv_with_pythondir}" = xno],
      [AS_IF(
        [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
        [ax_python_pythondir="${ax_python_prefix}/${ax_python_pythondir_suffix}"],
        [ax_python_pythondir=`${PYTHON} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib()) " 2>/dev/null`])],
      [ax_python_pythondir=$ac_cv_with_pythondir])

    AC_SUBST(
      [pythondir],
      [$ax_python_pythondir])

    dnl Check for Python platform specific library directory
    ax_python_pyexecdir_suffix=`${PYTHON} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(1, 0, prefix=''))" 2>/dev/null`;
    ax_python_library_dir=`${PYTHON} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(True)) " 2>/dev/null`;

    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python_pyexecdir="${ax_python_exec_prefix}/${ax_python_pyexecdir_suffix}"],
      [ax_python_pyexecdir=$ax_python_library_dir])

    AC_SUBST(
      [pyexecdir],
      [$ax_python_pyexecdir])

    AC_SUBST(
      [PYTHON_LIBRARY_DIR],
      [$ax_python_pyexecdir_suffix])

    AC_SUBST(
      [PYTHON_PACKAGE_DIR],
      [$ax_python_library_dir])
    ])
  ])

dnl Function to detect if a Python 2 build environment is available
AC_DEFUN([AX_PYTHON2_CHECK],
  [AX_PROG_PYTHON2
  AX_PROG_PYTHON2_CONFIG

  AS_IF(
    [test "x${PYTHON2_CONFIG}" != x],
    [dnl Check for Python 2 includes
    PYTHON2_INCLUDES=`${PYTHON2_CONFIG} --includes 2>/dev/null`;

    AC_MSG_CHECKING(
      [for Python verison 2 includes])
    AC_MSG_RESULT(
      [$PYTHON2_INCLUDES])

    dnl Check for Python 2 libraries
    PYTHON2_LDFLAGS=`${PYTHON2_CONFIG} --ldflags 2>/dev/null`;

    AC_MSG_CHECKING(
      [for Python 2 libraries])
    AC_MSG_RESULT(
      [$PYTHON2_LDFLAGS])

    dnl For CygWin add the -no-undefined linker flag
    AS_CASE(
      [$host_os],
      [cygwin*],[PYTHON2_LDFLAGS="${PYTHON2_LDFLAGS} -no-undefined"],
      [*],[])

    dnl Check for the existence of Python.h
    BACKUP_CPPFLAGS="${CPPFLAGS}"
    CPPFLAGS="${CPPFLAGS} ${PYTHON2_INCLUDES}"

    AC_CHECK_HEADERS(
      [Python.h],
      [ac_cv_header_python2_h=yes],
      [ac_cv_header_python2_h=no])

    CPPFLAGS="${BACKUP_CPPFLAGS}"
  ])

  AS_IF(
    [test "x${ac_cv_header_python2_h}" != xyes],
    [ac_cv_enable_python2=no],
    [ac_cv_enable_python2=$PYTHON2_VERSION
    AC_SUBST(
      [PYTHON2_CPPFLAGS],
      [$PYTHON2_INCLUDES])

    AC_SUBST(
      [PYTHON2_LDFLAGS],
      [$PYTHON2_LDFLAGS])

    dnl Check for Python prefix
    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python2_prefix="\${prefix}"],
      [ax_python2_prefix=`${PYTHON2_CONFIG} --prefix 2>/dev/null`])

    AC_SUBST(
      [PYTHON2_PREFIX],
      [$ax_python2_prefix])

    dnl Check for Python exec-prefix
    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python2_exec_prefix="\${exec_prefix}"],
      [ax_python2_exec_prefix=`${PYTHON2_CONFIG} --exec-prefix 2>/dev/null`])

    AC_SUBST(
      [PYTHON2_EXEC_PREFIX],
      [$ax_python2_exec_prefix])

    dnl Check for Python 2 library directory
    ax_python2_pythondir_suffix=`${PYTHON2} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(0, 0, prefix=''))" 2>/dev/null`;

    AS_IF(
      [test "x${ac_cv_with_pythondir2}" = x || test "x${ac_cv_with_pythondir2}" = xno],
      [AS_IF(
        [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
        [ax_python2_pythondir="${ax_python_prefix}/${ax_python2_pythondir_suffix}"],
        [ax_python2_pythondir=`${PYTHON2} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib()) " 2>/dev/null`])],
      [ax_python2_pythondir=$ac_cv_with_pythondir2])

    AC_SUBST(
      [pythondir2],
      [$ax_python2_pythondir])

    dnl Check for Python 2 platform specific library directory
    ax_python2_pyexecdir_suffix=`${PYTHON2} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(1, 0, prefix=''))" 2>/dev/null`;
    ax_python2_library_dir=`${PYTHON2} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(True)) " 2>/dev/null`;

    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python2_pyexecdir="${ax_python2_exec_prefix}/${ax_python2_pyexecdir_suffix}"],
      [ax_python2_pyexecdir=$ax_python2_library_dir])

    AC_SUBST(
      [pyexecdir2],
      [$ax_python2_pyexecdir])

    AC_SUBST(
      [PYTHON2_LIBRARY_DIR],
      [$ax_python2_pyexecdir_suffix])

    AC_SUBST(
      [PYTHON2_PACKAGE_DIR],
      [$ax_python2_library_dir])
    ])
  ])

dnl Function to detect if a Python 3 build environment is available
AC_DEFUN([AX_PYTHON3_CHECK],
  [AX_PROG_PYTHON3
  AX_PROG_PYTHON3_CONFIG

  AS_IF(
    [test "x${PYTHON3_CONFIG}" != x],
    [dnl Check for Python 3 includes
    PYTHON3_INCLUDES=`${PYTHON3_CONFIG} --includes 2>/dev/null`;

    AC_MSG_CHECKING(
      [for Python verison 3 includes])
    AC_MSG_RESULT(
      [$PYTHON3_INCLUDES])

    dnl Check for Python 3 libraries
    PYTHON3_LDFLAGS=`${PYTHON3_CONFIG} --ldflags 2>/dev/null`;

    AC_MSG_CHECKING(
      [for Python 3 libraries])
    AC_MSG_RESULT(
      [$PYTHON3_LDFLAGS])

    dnl For CygWin add the -no-undefined linker flag
    AS_CASE(
      [$host_os],
      [cygwin*],[PYTHON3_LDFLAGS="${PYTHON3_LDFLAGS} -no-undefined"],
      [*],[])

    dnl Check for the existence of Python.h
    BACKUP_CPPFLAGS="${CPPFLAGS}"
    CPPFLAGS="${CPPFLAGS} ${PYTHON3_INCLUDES}"

    AC_CHECK_HEADERS(
      [Python.h],
      [ac_cv_header_python3_h=yes],
      [ac_cv_header_python3_h=no])

    CPPFLAGS="${BACKUP_CPPFLAGS}"
  ])

  AS_IF(
    [test "x${ac_cv_header_python3_h}" != xyes],
    [ac_cv_enable_python3=no],
    [ac_cv_enable_python3=$PYTHON3_VERSION
    AC_SUBST(
      [PYTHON3_CPPFLAGS],
      [$PYTHON3_INCLUDES])

    AC_SUBST(
      [PYTHON3_LDFLAGS],
      [$PYTHON3_LDFLAGS])

    dnl Check for Python prefix
    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python3_prefix="\${prefix}"],
      [ax_python3_prefix=`${PYTHON3_CONFIG} --prefix 2>/dev/null`])

    AC_SUBST(
      [PYTHON3_PREFIX],
      [$ax_python3_prefix])

    dnl Check for Python exec-prefix
    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python3_exec_prefix="\${exec_prefix}"],
      [ax_python3_exec_prefix=`${PYTHON3_CONFIG} --exec-prefix 2>/dev/null`])

    AC_SUBST(
      [PYTHON3_EXEC_PREFIX],
      [$ax_python3_exec_prefix])

    dnl Check for Python 3 library directory
    ax_python3_pythondir_suffix=`${PYTHON3} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(0, 0, prefix=''))" 2>/dev/null`;

    AS_IF(
      [test "x${ac_cv_with_pythondir3}" = x || test "x${ac_cv_with_pythondir3}" = xno],
      [AS_IF(
        [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
        [ax_python3_pythondir="${ax_python_prefix}/${ax_python3_pythondir_suffix}"],
        [ax_python3_pythondir=`${PYTHON3} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib()) " 2>/dev/null`])],
      [ax_python3_pythondir=$ac_cv_with_pythondir3])

    AC_SUBST(
      [pythondir3],
      [$ax_python3_pythondir])

    dnl Check for Python 3 platform specific library directory
    ax_python3_pyexecdir_suffix=`${PYTHON3} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(1, 0, prefix=''))" 2>/dev/null`;
    ax_python3_library_dir=`${PYTHON3} -c "import sys; import distutils.sysconfig; sys.stdout.write(distutils.sysconfig.get_python_lib(True)) " 2>/dev/null`;

    AS_IF(
      [test "x${ac_cv_with_pyprefix}" = x || test "x${ac_cv_with_pyprefix}" = xno],
      [ax_python3_pyexecdir="${ax_python3_exec_prefix}/${ax_python3_pyexecdir_suffix}"],
      [ax_python3_pyexecdir=$ax_python3_library_dir])

    AC_SUBST(
      [pyexecdir3],
      [$ax_python3_pyexecdir])

    AC_SUBST(
      [PYTHON3_LIBRARY_DIR],
      [$ax_python3_pyexecdir_suffix])

    AC_SUBST(
      [PYTHON3_PACKAGE_DIR],
      [$ax_python3_library_dir])
    ])
  ])

dnl Function to determine the prefix of pythondir
AC_DEFUN([AX_PYTHON_CHECK_PYPREFIX],
  [AX_COMMON_ARG_WITH(
    [pyprefix],
    [pyprefix],
    [use `python-config --prefix' to determine the prefix of pythondir instead of --prefix],
    [no],
    [no])
])

dnl Function to detect if to enable Python
AC_DEFUN([AX_PYTHON_CHECK_ENABLE],
  [AX_PYTHON_CHECK_PYPREFIX

  AX_COMMON_ARG_ENABLE(
    [python],
    [python],
    [build Python bindings],
    [no])
  AX_COMMON_ARG_WITH(
    [pythondir],
    [pythondir],
    [use to specify the Python directory (pythondir)],
    [no],
    [no])

  AX_COMMON_ARG_ENABLE(
    [python2],
    [python2],
    [build Python 2 bindings],
    [no])
  AX_COMMON_ARG_WITH(
    [pythondir2],
    [pythondir2],
    [use to specify the Python 2 directory (pythondir2)],
    [no],
    [no])

  AX_COMMON_ARG_ENABLE(
    [python3],
    [python3],
    [build Python 3 bindings],
    [no])
  AX_COMMON_ARG_WITH(
    [pythondir3],
    [pythondir3],
    [use to specify the Python 3 directory (pythondir3)],
    [no],
    [no])

  AS_IF(
    [test "x${ac_cv_enable_python}" != xno],
    [AX_PYTHON_CHECK])

  AS_IF(
    [test "x${ac_cv_enable_python2}" != xno],
    [AX_PYTHON2_CHECK])

  AS_IF(
    [test "x${ac_cv_enable_python3}" != xno],
    [AX_PYTHON3_CHECK])

  AM_CONDITIONAL(
    HAVE_PYTHON,
    [test "x${ac_cv_enable_python}" != xno])

  AM_CONDITIONAL(
    HAVE_PYTHON2,
    [test "x${ac_cv_enable_python2}" != xno])

  AM_CONDITIONAL(
    HAVE_PYTHON3,
    [test "x${ac_cv_enable_python3}" != xno])

  AM_CONDITIONAL(
    HAVE_PYTHON_TESTS,
    [test "x${ac_cv_enable_python}" != xno || test "x${ac_cv_enable_python2}" != xno || test "x${ac_cv_enable_python3}" != xno])

  AS_IF(
    [test "x${ac_cv_enable_python}" = xno],
    [AS_IF(
      [test "x${ac_cv_enable_python2}" != xno || test "x${ac_cv_enable_python3}" != xno],
      [AS_IF(
        [test "x${ac_cv_enable_python2}" != xno],
        [ac_cv_enable_python=${ac_cv_enable_python2}],
        [ac_cv_enable_python=""])
      AS_IF(
        [test "x${ac_cv_enable_python3}" != xno],
        [AS_IF(
          [test "x${ac_cv_enable_python}" != x],
          [ac_cv_enable_python="${ac_cv_enable_python}, "])
        ac_cv_enable_python="${ac_cv_enable_python}${ac_cv_enable_python3}"])
      ])
    ])
  ])
])

