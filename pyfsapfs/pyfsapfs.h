/*
 * Python bindings module for libfsapfs (pyfsapfs)
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

#if !defined( _PYFSAPFS_H )
#define _PYFSAPFS_H

#include <common.h>
#include <types.h>

#include "pyfsapfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

PyObject *pyfsapfs_get_version(
           PyObject *self,
           PyObject *arguments );

PyObject *pyfsapfs_check_container_signature(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_check_container_signature_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_open_new_container(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_open_new_container_with_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pyfsapfs(
                void );
#else
PyMODINIT_FUNC initpyfsapfs(
                void );
#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSAPFS_H ) */

