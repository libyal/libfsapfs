/*
 * Python object definition of the sequence and iterator object of extended attributes
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

#if !defined( _PYFSAPFS_EXTENDED_ATTRIBUTES_H )
#define _PYFSAPFS_EXTENDED_ATTRIBUTES_H

#include <common.h>
#include <types.h>

#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsapfs_extended_attributes pyfsapfs_extended_attributes_t;

struct pyfsapfs_extended_attributes
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyfsapfs_extended_attributes_type_object;

PyObject *pyfsapfs_extended_attributes_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyfsapfs_extended_attributes_init(
     pyfsapfs_extended_attributes_t *sequence_object );

void pyfsapfs_extended_attributes_free(
      pyfsapfs_extended_attributes_t *sequence_object );

Py_ssize_t pyfsapfs_extended_attributes_len(
            pyfsapfs_extended_attributes_t *sequence_object );

PyObject *pyfsapfs_extended_attributes_getitem(
           pyfsapfs_extended_attributes_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyfsapfs_extended_attributes_iter(
           pyfsapfs_extended_attributes_t *sequence_object );

PyObject *pyfsapfs_extended_attributes_iternext(
           pyfsapfs_extended_attributes_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSAPFS_EXTENDED_ATTRIBUTES_H ) */

