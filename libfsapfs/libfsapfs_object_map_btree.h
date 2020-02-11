/*
 * The object map B-tree functions
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

#if !defined( _LIBFSAPFS_OBJECT_MAP_BTREE_H )
#define _LIBFSAPFS_OBJECT_MAP_BTREE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_btree_entry.h"
#include "libfsapfs_btree_node.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_object_map_descriptor.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_object_map_btree libfsapfs_object_map_btree_t;

struct libfsapfs_object_map_btree
{
	/* The IO handle
	 */
	libfsapfs_io_handle_t *io_handle;

	/* Data block vector
	 */
	libfdata_vector_t *data_block_vector;

	/* Data block cache
	 */
	libfcache_cache_t *data_block_cache;

	/* The node cache
	 */
	libfcache_cache_t *node_cache;

	/* Block number of B-tree root node
	 */
	uint64_t root_node_block_number;
};

int libfsapfs_object_map_btree_initialize(
     libfsapfs_object_map_btree_t **object_map_btree,
     libfsapfs_io_handle_t *io_handle,
     libfdata_vector_t *data_block_vector,
     uint64_t root_node_block_number,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_free(
     libfsapfs_object_map_btree_t **object_map_btree,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_get_root_node(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t root_node_block_number,
     libfsapfs_btree_node_t **root_node,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_get_sub_node(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t sub_node_block_number,
     libfsapfs_btree_node_t **sub_node,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_get_entry_from_node_by_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     libfsapfs_btree_node_t *node,
     uint64_t object_identifier,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_get_entry_by_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
     libfsapfs_btree_node_t **btree_node,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_get_descriptor_by_object_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
     libfsapfs_object_map_descriptor_t **descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_OBJECT_MAP_BTREE_H ) */

