/*
 * The B-tree node functions
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

#if !defined( _LIBFSAPFS_BTREE_NODE_H )
#define _LIBFSAPFS_BTREE_NODE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_btree_entry.h"
#include "libfsapfs_btree_footer.h"
#include "libfsapfs_btree_node_header.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_btree_node libfsapfs_btree_node_t;

struct libfsapfs_btree_node
{
	/* The object type
	 */
	uint32_t object_type;

	/* The object subtype
	 */
	uint32_t object_subtype;

	/* The B-tree header
	 */
	libfsapfs_btree_node_header_t *node_header;

	/* The B-tree footer
	 */
	libfsapfs_btree_footer_t *footer;

	/* The B-tree entries array
	 */
	libcdata_array_t *entries_array;
};

int libfsapfs_btree_node_initialize(
     libfsapfs_btree_node_t **btree_node,
     libcerror_error_t **error );

int libfsapfs_btree_node_free(
     libfsapfs_btree_node_t **btree_node,
     libcerror_error_t **error );

int libfsapfs_btree_node_read_data(
     libfsapfs_btree_node_t *btree_node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_btree_node_read_object_data(
     libfsapfs_btree_node_t *btree_node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_btree_node_is_leaf_node(
     libfsapfs_btree_node_t *btree_node,
     libcerror_error_t **error );

int libfsapfs_btree_node_get_number_of_entries(
     libfsapfs_btree_node_t *btree_node,
     int *number_of_entries,
     libcerror_error_t **error );

int libfsapfs_btree_node_get_entry_by_index(
     libfsapfs_btree_node_t *btree_node,
     int entry_index,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_BTREE_NODE_H ) */

