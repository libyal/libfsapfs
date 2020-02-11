/*
 * Python object wrapper of libfsapfs_container_t
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

#if !defined( _PYFSAPFS_CONTAINER_H )
#define _PYFSAPFS_CONTAINER_H

#include <common.h>
#include <types.h>

#include "pyfsapfs_libbfio.h"
#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsapfs_container pyfsapfs_container_t;

struct pyfsapfs_container
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsapfs container
	 */
	libfsapfs_container_t *container;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyfsapfs_container_object_methods[];
extern PyTypeObject pyfsapfs_container_type_object;

int pyfsapfs_container_init(
     pyfsapfs_container_t *pyfsapfs_container );

void pyfsapfs_container_free(
      pyfsapfs_container_t *pyfsapfs_container );

PyObject *pyfsapfs_container_signal_abort(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments );

PyObject *pyfsapfs_container_open(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_container_open_file_object(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_container_close(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments );

PyObject *pyfsapfs_container_get_size(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments );

PyObject *pyfsapfs_container_get_identifier(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments );

PyObject *pyfsapfs_container_get_number_of_volumes(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments );

PyObject *pyfsapfs_container_get_volume_by_index(
           PyObject *pyfsapfs_container,
           int volume_index );

PyObject *pyfsapfs_container_get_volume(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_container_get_volumes(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSAPFS_CONTAINER_H ) */

