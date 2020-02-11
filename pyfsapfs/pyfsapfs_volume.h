/*
 * Python object wrapper of libfsapfs_volume_t
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

#if !defined( _PYFSAPFS_VOLUME_H )
#define _PYFSAPFS_VOLUME_H

#include <common.h>
#include <types.h>

#include "pyfsapfs_libbfio.h"
#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsapfs_volume pyfsapfs_volume_t;

struct pyfsapfs_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsapfs volume
	 */
	libfsapfs_volume_t *volume;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsapfs_volume_object_methods[];
extern PyTypeObject pyfsapfs_volume_type_object;

PyObject *pyfsapfs_volume_new(
           libfsapfs_volume_t *volume,
           PyObject *parent_object );

int pyfsapfs_volume_init(
     pyfsapfs_volume_t *pyfsapfs_volume );

void pyfsapfs_volume_free(
      pyfsapfs_volume_t *pyfsapfs_volume );

PyObject *pyfsapfs_volume_open(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_volume_open_file_object(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_volume_close(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments );

PyObject *pyfsapfs_volume_unlock(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments );

PyObject *pyfsapfs_volume_get_size(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments );

PyObject *pyfsapfs_volume_get_identifier(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments );

PyObject *pyfsapfs_volume_get_name(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments );

PyObject *pyfsapfs_volume_is_locked(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments );

PyObject *pyfsapfs_volume_set_password(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_volume_set_recovery_password(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_volume_get_next_file_entry_identifier(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments );

PyObject *pyfsapfs_volume_get_file_entry_by_identifier(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_volume_get_root_directory(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments );

PyObject *pyfsapfs_volume_get_file_entry_by_path(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSAPFS_VOLUME_H ) */

