/*
 * The B-tree root functions
 *
 * Copyright (C) 2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSAPFS_BTREE_ROOT_H )
#define _LIBFSAPFS_BTREE_ROOT_H

#include <common.h>
#include <types.h>

#include "libfsapfs_btree_entry.h"
#include "libfsapfs_btree_footer.h"
#include "libfsapfs_btree_header.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_btree_root libfsapfs_btree_root_t;

struct libfsapfs_btree_root
{
	/* The object type
	 */
	uint32_t object_type;

	/* The object subtype
	 */
	uint32_t object_subtype;

	/* The B-tree header
	 */
	libfsapfs_btree_header_t *header;

	/* The B-tree footer
	 */
	libfsapfs_btree_footer_t *footer;

	/* The B-tree entries array
	 */
	libcdata_array_t *entries_array;
};

int libfsapfs_btree_root_initialize(
     libfsapfs_btree_root_t **btree_root,
     libcerror_error_t **error );

int libfsapfs_btree_root_free(
     libfsapfs_btree_root_t **btree_root,
     libcerror_error_t **error );

int libfsapfs_btree_root_read_data(
     libfsapfs_btree_root_t *btree_root,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_btree_root_read_object_data(
     libfsapfs_btree_root_t *btree_root,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_btree_root_get_number_of_entries(
     libfsapfs_btree_root_t *btree_root,
     int *number_of_entries,
     libcerror_error_t **error );

int libfsapfs_btree_root_get_entry_by_index(
     libfsapfs_btree_root_t *btree_root,
     int entry_index,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_BTREE_ROOT_H ) */

