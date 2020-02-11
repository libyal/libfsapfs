/*
 * The APFS container superblock definition
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

#if !defined( _FSAPFS_CONTAINER_SUPERBLOCK_H )
#define _FSAPFS_CONTAINER_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_container_superblock fsapfs_container_superblock_t;

struct fsapfs_container_superblock
{
	/* The object checksum
	 * Consists of 8 bytes
	 */
	uint8_t object_checksum[ 8 ];

	/* The object identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_identifier[ 8 ];

	/* The object transaction identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_transaction_identifier[ 8 ];

	/* The object type
	 * Consists of 4 bytes
	 */
	uint8_t object_type[ 4 ];

	/* The object subtype
	 * Consists of 4 bytes
	 */
	uint8_t object_subtype[ 4 ];

	/* The file system signature
	 * Consists of 4 bytes
	 * Contains: "NXSB"
	 */
	uint8_t signature[ 4 ];

	/* The block size
	 * Consists of 4 bytes
	 */
	uint8_t block_size[ 4 ];

	/* The number of block
	 * Consists of 8 bytes
	 */
	uint8_t number_of_blocks[ 8 ];

	/* Compatible features flags
	 * Consists of 8 bytes
	 */
	uint8_t compatible_features_flags[ 8 ];

	/* Read only compatible features flags
	 * Consists of 8 bytes
	 */
	uint8_t read_only_compatible_features_flags[ 8 ];

	/* Incompatible features flags
	 * Consists of 8 bytes
	 */
	uint8_t incompatible_features_flags[ 8 ];

	/* The container identifier
	 * Consists of 16 bytes
	 * Contains an UUID
	 */
	uint8_t container_identifier[ 16 ];

	/* The next object identifier
	 * Consists of 8 bytes
	 */
	uint8_t next_object_identifier[ 8 ];

	/* The next transaction identifier
	 * Consists of 8 bytes
	 */
	uint8_t next_transaction_identifier[ 8 ];

	/* The checkpoint descriptor area number of blocks
	 * Consists of 4 bytes
	 */
	uint8_t checkpoint_descriptor_area_number_of_blocks[ 4 ];

	/* The checkpoint data area number of blocks
	 * Consists of 4 bytes
	 */
	uint8_t checkpoint_data_area_number_of_blocks[ 4 ];

	/* The checkpoint descriptor area block number
	 * Consists of 8 bytes
	 */
	uint8_t checkpoint_descriptor_area_block_number[ 8 ];

	/* The checkpoint data area block number
	 * Consists of 8 bytes
	 */
	uint8_t checkpoint_data_area_block_number[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown8[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown9[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown10[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown11[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown12[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown13[ 4 ];

	/* The space manager object identifier
	 * Consists of 8 bytes
	 */
	uint8_t space_manager_object_identifier[ 8 ];

	/* The object map block number
	 * Consists of 8 bytes
	 */
	uint8_t object_map_block_number[ 8 ];

	/* The reaper object identifier
	 * Consists of 8 bytes
	 */
	uint8_t reaper_object_identifier[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown17[ 4 ];

	/* The maximum number of volumes
	 * Consists of 4 bytes
	 */
	uint8_t maximum_number_of_volumes[ 4 ];

	/* The volume object identifiers
	 * Consists of 100 x 8 bytes
	 */
	uint8_t volume_object_identifiers[ 800 ];

	/* The counters
	 * Consists of 32 x 8 bytes
	 */
	uint8_t counters[ 256 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown20[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown21[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown22[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown23[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown24[ 8 ];

	/* The Fusion set identifier
	 * Consists of 16 bytes
	 * Contains an UUID
	 */
	uint8_t fusion_set_identifier[ 16 ];

	/* The key bag block number
	 * Consists of 8 bytes
	 */
	uint8_t key_bag_block_number[ 8 ];

	/* The key bag number of blocks
	 * Consists of 8 bytes
	 */
	uint8_t key_bag_number_of_blocks[ 8 ];

	/* Unknown
	 * Consists of 4 x 8 bytes
	 */
	uint8_t unknown29[ 32 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown30[ 8 ];

	/* The Fusion middle tree block number
	 * Consists of 8 bytes
	 */
	uint8_t fusion_middle_tree_block_number[ 8 ];

	/* The Fusion write-back cache object identifier
	 * Consists of 8 bytes
	 */
	uint8_t fusion_write_back_cache_object_identifier[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown33[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown34[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_CONTAINER_SUPERBLOCK_H ) */

