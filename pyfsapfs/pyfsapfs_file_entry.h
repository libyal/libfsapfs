/*
 * Python object wrapper of libfsapfs_file_entry_t
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

#if !defined( _PYFSAPFS_FILE_ENTRY_H )
#define _PYFSAPFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsapfs_file_entry pyfsapfs_file_entry_t;

struct pyfsapfs_file_entry
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsapfs file entry
	 */
	libfsapfs_file_entry_t *file_entry;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsapfs_file_entry_object_methods[];
extern PyTypeObject pyfsapfs_file_entry_type_object;

PyObject *pyfsapfs_file_entry_new(
           libfsapfs_file_entry_t *file_entry,
           PyObject *parent_object );

int pyfsapfs_file_entry_init(
     pyfsapfs_file_entry_t *pyfsapfs_file_entry );

void pyfsapfs_file_entry_free(
      pyfsapfs_file_entry_t *pyfsapfs_file_entry );

PyObject *pyfsapfs_file_entry_get_identifier(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_parent_identifier(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_creation_time(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_creation_time_as_integer(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_modification_time(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_modification_time_as_integer(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_access_time(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_access_time_as_integer(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_inode_change_time(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_inode_change_time_as_integer(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_owner_identifier(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_group_identifier(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_file_mode(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_name(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_symbolic_link_target(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_number_of_extended_attributes(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_extended_attribute_by_index(
           PyObject *pyfsapfs_file_entry,
           int extended_attribute_index );

PyObject *pyfsapfs_file_entry_get_extended_attribute(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_file_entry_get_extended_attributes(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_has_extended_attribute_by_name(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_file_entry_get_extended_attribute_by_name(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_file_entry_get_number_of_sub_file_entries(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_sub_file_entry_by_index(
           PyObject *pyfsapfs_file_entry,
           int sub_file_entry_index );

PyObject *pyfsapfs_file_entry_get_sub_file_entry(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_file_entry_get_sub_file_entries(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_sub_file_entry_by_name(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_file_entry_read_buffer(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_file_entry_read_buffer_at_offset(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_file_entry_seek_offset(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsapfs_file_entry_get_offset(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_size(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_number_of_extents(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments );

PyObject *pyfsapfs_file_entry_get_extent_by_index(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           int extent_index );

PyObject *pyfsapfs_file_entry_get_extent(
           pyfsapfs_file_entry_t *pyfsapfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSAPFS_FILE_ENTRY_H ) */

