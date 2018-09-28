/*
 * The file system B-tree functions
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

#if !defined( _LIBFSAPFS_FILE_SYSTEM_BTREE_H )
#define _LIBFSAPFS_FILE_SYSTEM_BTREE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_btree_node.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_file_system_btree libfsapfs_file_system_btree_t;

struct libfsapfs_file_system_btree
{
	/* Data block vector
	 */
	libfdata_vector_t *data_block_vector;

	/* Data block cache
	 */
	libfcache_cache_t *data_block_cache;

	/* Block number of B-tree root node
	 */
	uint64_t root_node_block_number;
};

int libfsapfs_file_system_btree_initialize(
     libfsapfs_file_system_btree_t **file_system_btree,
     libfdata_vector_t *data_block_vector,
     libfcache_cache_t *data_block_cache,
     uint64_t root_node_block_number,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_free(
     libfsapfs_file_system_btree_t **file_system_btree,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_root_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t **root_node,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_inode(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libfsapfs_inode_t **inode,
     libcerror_error_t **error );

int libfsapfs_file_system_btree_get_inode_from_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libfsapfs_inode_t **inode,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_FILE_SYSTEM_BTREE_H ) */

