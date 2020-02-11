/*
 * The B-tree node header functions
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

#if !defined( _LIBFSAPFS_BTREE_NODE_HEADER_H )
#define _LIBFSAPFS_BTREE_NODE_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_btree_node_header libfsapfs_btree_node_header_t;

struct libfsapfs_btree_node_header
{
	/* The flags
	 */
	uint16_t flags;

	/* The level
	 */
	uint16_t level;

	/* The number of keys
	 */
	uint16_t number_of_keys;

	/* The entries data offset
	 */
	uint16_t entries_data_offset;

	/* The entries data size
	 */
	uint16_t entries_data_size;

	/* The unused data offset
	 */
	uint16_t unused_data_offset;

	/* The unused data size
	 */
	uint16_t unused_data_size;
};

int libfsapfs_btree_node_header_initialize(
     libfsapfs_btree_node_header_t **btree_node_header,
     libcerror_error_t **error );

int libfsapfs_btree_node_header_free(
     libfsapfs_btree_node_header_t **btree_node_header,
     libcerror_error_t **error );

int libfsapfs_btree_node_header_read_data(
     libfsapfs_btree_node_header_t *btree_node_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_BTREE_NODE_HEADER_H ) */

