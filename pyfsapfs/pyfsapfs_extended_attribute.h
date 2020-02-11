/*
 * Python object wrapper of libfsapfs_extended_attribute_t
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

#if !defined( _PYFSAPFS_EXTENDED_ATTRIBUTE_H )
#define _PYFSAPFS_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsapfs_extended_attribute pyfsapfs_extended_attribute_t;

struct pyfsapfs_extended_attribute
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsapfs extended attribute
	 */
	libfsapfs_extended_attribute_t *extended_attribute;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsapfs_extended_attribute_object_methods[];
extern PyTypeObject pyfsapfs_extended_attribute_type_object;

PyObject *pyfsapfs_extended_attribute_new(
           libfsapfs_extended_attribute_t *extended_attribute,
           PyObject *parent_object );

int pyfsapfs_extended_attribute_init(
     pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute );

void pyfsapfs_extended_attribute_free(
      pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute );

PyObject *pyfsapfs_extended_attribute_get_identifier(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfsapfs_extended_attribute_get_name(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfsapfs_extended_attribute_read_buffer(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_extended_attribute_read_buffer_at_offset(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_extended_attribute_seek_offset(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_extended_attribute_get_offset(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfsapfs_extended_attribute_get_size(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSAPFS_EXTENDED_ATTRIBUTE_H ) */

