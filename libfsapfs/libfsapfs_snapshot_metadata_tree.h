/*
 * The snapshot metadata tree functions
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

#if !defined( _LIBFSAPFS_SNAPSHOT_METADATA_TREE_H )
#define _LIBFSAPFS_SNAPSHOT_METADATA_TREE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_btree_entry.h"
#include "libfsapfs_btree_node.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_object_map_btree.h"
#include "libfsapfs_snapshot_metadata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_snapshot_metadata_tree libfsapfs_snapshot_metadata_tree_t;

struct libfsapfs_snapshot_metadata_tree
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

	/* The volume object map B-tree
	 */
	libfsapfs_object_map_btree_t *object_map_btree;

	/* Block number of B-tree root node
	 */
	uint64_t root_node_block_number;
};

int libfsapfs_snapshot_metadata_tree_initialize(
     libfsapfs_snapshot_metadata_tree_t **snapshot_metadata_tree,
     libfsapfs_io_handle_t *io_handle,
     libfdata_vector_t *data_block_vector,
     libfsapfs_object_map_btree_t *object_map_btree,
     uint64_t root_node_block_number,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_free(
     libfsapfs_snapshot_metadata_tree_t **snapshot_metadata_tree,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_sub_node_block_number_from_entry(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_entry_t *entry,
     uint64_t *sub_node_block_number,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_root_node(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libbfio_handle_t *file_io_handle,
     uint64_t root_node_block_number,
     libfsapfs_btree_node_t **root_node,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_sub_node(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libbfio_handle_t *file_io_handle,
     uint64_t sub_node_block_number,
     libfsapfs_btree_node_t **sub_node,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_entry_from_node_by_identifier(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libfsapfs_btree_node_t *node,
     uint64_t object_identifier,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_entry_by_identifier(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
     libfsapfs_btree_node_t **btree_node,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_metadata_by_object_identifier(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
     libfsapfs_snapshot_metadata_t **metadata,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_snapshots_from_leaf_node(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libfsapfs_btree_node_t *node,
     libcdata_array_t *snapshots,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_snapshots_from_branch_node(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     libcdata_array_t *snapshots,
     int recursion_depth,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_tree_get_snapshots(
     libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree,
     libbfio_handle_t *file_io_handle,
     libcdata_array_t *snapshots,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_SNAPSHOT_METADATA_TREE_H ) */

