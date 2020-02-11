/*
 * The python header wrapper
 *
 * Copyright (C) 2018-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _PYFSAPFS_PYTHON_H )
#define _PYFSAPFS_PYTHON_H

#include <common.h>

#if PY_MAJOR_VERSION < 3

/* Fix defines in pyconfig.h
 */
#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE

/* Fix defines in pyport.h
 */
#undef HAVE_FSTAT
#undef HAVE_STAT
#undef HAVE_SSIZE_T
#undef HAVE_INT32_T
#undef HAVE_UINT32_T
#undef HAVE_INT64_T
#undef HAVE_UINT64_T

#endif /* PY_MAJOR_VERSION < 3 */

#include <Python.h>

/* Python compatibility macros
 */
#if !defined( PyMODINIT_FUNC )
#if PY_MAJOR_VERSION >= 3
#define PyMODINIT_FUNC PyObject *
#else
#define PyMODINIT_FUNC void
#endif
#endif /* !defined( PyMODINIT_FUNC ) */

#if !defined( PyVarObject_HEAD_INIT )
#define PyVarObject_HEAD_INIT( type, size ) \
	PyObject_HEAD_INIT( type ) \
	size,

#endif /* !defined( PyVarObject_HEAD_INIT ) */

#if PY_MAJOR_VERSION >= 3
#define Py_TPFLAGS_HAVE_ITER		0
#endif

#if !defined( Py_TYPE )
#define Py_TYPE( object ) \
	( ( (PyObject *) object )->ob_type )

#endif /* !defined( Py_TYPE ) */

#endif /* !defined( _PYFSAPFS_PYTHON_H ) */

