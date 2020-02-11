/*
 * The B-tree entry functions
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

#if !defined( _LIBFSAPFS_BTREE_ENTRY_H )
#define _LIBFSAPFS_BTREE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_btree_entry libfsapfs_btree_entry_t;

struct libfsapfs_btree_entry
{
	/* The key data
	 */
	const uint8_t *key_data;

	/* The key data size
	 */
	size_t key_data_size;

	/* The value data
	 */
	const uint8_t *value_data;

	/* The value data size
	 */
	size_t value_data_size;
};

int libfsapfs_btree_entry_initialize(
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

int libfsapfs_btree_entry_free(
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

int libfsapfs_btree_entry_set_key_data(
     libfsapfs_btree_entry_t *btree_entry,
     const uint8_t *key_data,
     size_t key_data_size,
     libcerror_error_t **error );

int libfsapfs_btree_entry_set_value_data(
     libfsapfs_btree_entry_t *btree_entry,
     const uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_BTREE_ENTRY_H ) */

