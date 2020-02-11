/*
 * The container superblock functions
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

#if !defined( _LIBFSAPFS_CONTAINER_SUPERBLOCK_H )
#define _LIBFSAPFS_CONTAINER_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_container_superblock libfsapfs_container_superblock_t;

struct libfsapfs_container_superblock
{
	/* The object transaction identifier
	 */
	uint64_t object_transaction_identifier;

	/* The block size
	 */
	uint32_t block_size;

	/* The number of blocks
	 */
	uint64_t number_of_blocks;

	/* The incompatible features flags
	 */
	uint64_t incompatible_features_flags;

	/* The container identifier
	 */
	uint8_t container_identifier[ 16 ];

	/* The number of blocks of the checkpoint descriptor area
	 */
	uint32_t checkpoint_descriptor_area_number_of_blocks;

	/* The block number of the checkpoint descriptor area
	 */
	uint32_t checkpoint_descriptor_area_block_number;

	/* The space manager object identifier
	 */
	uint64_t space_manager_object_identifier;

	/* The object map block number
	 */
	uint64_t object_map_block_number;

	/* The reaper object identifier
	 */
	uint64_t reaper_object_identifier;

	/* The Fusion set identifier
	 */
	uint8_t fusion_set_identifier[ 16 ];

	/* The key bag block number
	 */
	uint64_t key_bag_block_number;

	/* The key bag number of blocks
	 */
	uint64_t key_bag_number_of_blocks;

	/* The number of volumes
	 */
	int number_of_volumes;

	/* The volume object identifier
	 */
	uint64_t volume_object_identifiers[ 100 ];

	/* The Fusion middle tree block number
	 */
	uint64_t fusion_middle_tree_block_number;
};

int libfsapfs_container_superblock_initialize(
     libfsapfs_container_superblock_t **container_superblock,
     libcerror_error_t **error );

int libfsapfs_container_superblock_free(
     libfsapfs_container_superblock_t **container_superblock,
     libcerror_error_t **error );

int libfsapfs_container_superblock_read_file_io_handle(
     libfsapfs_container_superblock_t *container_superblock,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsapfs_container_superblock_read_data(
     libfsapfs_container_superblock_t *container_superblock,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_container_superblock_get_container_identifier(
     libfsapfs_container_superblock_t *container_superblock,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_CONTAINER_SUPERBLOCK_H ) */

