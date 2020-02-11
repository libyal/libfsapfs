/*
 * The file system B-tree functions
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

#if !defined( _LIBFSAPFS_FILE_SYSTEM_BTREE_H )
#define _LIBFSAPFS_FILE_SYSTEM_BTREE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_btree_node.h"
#include "libfsapfs_directory_record.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_object_map_btree.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_file_system_btree libfsapfs_file_system_btree_t;

struct libfsapfs_file_system_btree
{
	/* The IO handle
	 */
	libfsapfs_io_handle_t *io_handle;

	/* The encryption context
	 */
	libfsapfs_encryption_context_t *encryption_context;

	/* The data block vector
	 */
	libfdata_vector_t *data_block_vector;

	/* The data block cache
	 */
	libfcache_cache_t *data_block_cache;

	/* The node cache
	 */
	libfcache_cache_t *node_cache;

	/* The volume object map B-tree
	 */
	libfsapfs_object_map_btree_t *object_map_btree;

	/* The block number of B-tree root node
	 */
	uint64_t root_node_block_number;

	/* Flag to indicate case folding should be used
	 */
	uint8_t use_case_folding;
};

int libfsapfs_file_system_btree_initialize(
     libfsapfs_file_system_btree_t **file_system_btree,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfdata_vector_t *data_block_vector,
     libfsapfs_object_map_btree_t *object_map_btree,
     uint64_t root_node_block_number,
     uint8_t use_case_folding,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_free(
     libfsapfs_file_system_btree_t **file_system_btree,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_entry_t *entry,
     uint64_t *sub_node_block_number,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_root_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t root_node_block_number,
     libfsapfs_btree_node_t **root_node,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_sub_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t sub_node_block_number,
     libfsapfs_btree_node_t **sub_node,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_entry_from_node_by_identifier(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     uint8_t data_type,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_entry_by_identifier(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     uint8_t data_type,
     libfsapfs_btree_node_t **btree_node,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf8_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t name_hash,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf8_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t name_hash,
     libfsapfs_directory_record_t **directory_record,
     int recursion_depth,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf16_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t name_hash,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf16_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t name_hash,
     libfsapfs_directory_record_t **directory_record,
     int recursion_depth,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_directory_entries_from_leaf_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_directory_entries_from_branch_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     libcdata_array_t *directory_entries,
     int recursion_depth,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_directory_entries(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_extended_attributes_from_leaf_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *extended_attributes,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_extended_attributes_from_branch_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *extended_attributes,
     int recursion_depth,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_extended_attributes(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     libcdata_array_t *extended_attributes,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_file_extents_from_leaf_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *file_extents,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_file_extents_from_branch_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *file_extents,
     int recursion_depth,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_file_extents(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libcdata_array_t *file_extents,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_inode_by_identifier(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libfsapfs_inode_t **inode,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_inode_by_utf8_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_inode_t **inode,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_inode_by_utf8_path(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_inode_t **inode,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_inode_by_utf16_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_inode_t **inode,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_inode_by_utf16_path(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_inode_t **inode,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_FILE_SYSTEM_BTREE_H ) */

