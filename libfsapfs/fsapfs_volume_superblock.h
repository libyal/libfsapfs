/*
 * The APFS volume superblock definition
 *
 * Copyright (C) 2018-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _FSAPFS_VOLUME_SUPERBLOCK_H )
#define _FSAPFS_VOLUME_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_volume_superblock fsapfs_volume_superblock_t;

typedef struct {
	uint8_t id[ 32 ];       // APFS_MODIFIED_NAMELEN == 32
	uint8_t timestamp[ 8 ]; // uint64_t
	uint8_t last_xid[ 8 ];  // xid_t aka uint64_t
} fsapfs_modified_by_t;

struct fsapfs_volume_superblock
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
	 * Contains: "APSB"
	 */
	uint8_t signature[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

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

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown5[ 8 ];

	/* The number of reserved blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_reserved_blocks[ 8 ];

	/* The number of quota blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_quota_blocks[ 8 ];

	/* The number of allocated blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_allocated_blocks[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown9[ 8 ];

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

	/* The file system root tree object type
	 * Consists of 4 bytes
	 */
	uint8_t file_system_root_tree_object_type[ 4 ];

	/* The extent-reference tree object type
	 * Consists of 4 bytes
	 */
	uint8_t extent_reference_tree_object_type[ 4 ];

	/* The snapshot metadata tree object type
	 * Consists of 4 bytes
	 */
	uint8_t snapshot_metadata_tree_object_type[ 4 ];

	/* The object map block number
	 * Consists of 8 bytes
	 */
	uint8_t object_map_block_number[ 8 ];

	/* The file system root object identifier
	 * Consists of 8 bytes
	 */
	uint8_t file_system_root_object_identifier[ 8 ];

	/* The extent-reference tree block number
	 * Consists of 8 bytes
	 */
	uint8_t extent_reference_tree_block_number[ 8 ];

	/* The snapshot metadata tree block number
	 * Consists of 8 bytes
	 */
	uint8_t snapshot_metadata_tree_block_number[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown20[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown21[ 8 ];

	/* The next file system object identifier
	 * Consists of 8 bytes
	 */
	uint8_t next_file_system_object_identifier[ 8 ];

	/* The number of files
	 * Consists of 8 bytes
	 */
	uint8_t number_of_files[ 8 ];

	/* The number of directories
	 * Consists of 8 bytes
	 */
	uint8_t number_of_directories[ 8 ];

	/* The number of symbolic links
	 * Consists of 8 bytes
	 */
	uint8_t number_of_symlinks[ 8 ];

	/* The number of other filesystem objects
	 * Consists of 8 bytes
	 */
	uint8_t number_of_other_objects[ 8 ];

	/* The number of snapshots
	 * Consists of 8 bytes
	 */
	uint8_t number_of_snapshots[ 8 ];

	/* The number of allocated blocks
	 * Consists of 8 bytes
	 */
	uint8_t total_blocks_allocated[ 8 ];

	/* The number of blocks freed
	 * Consists of 8 bytes
	 */
	uint8_t total_blocks_freed[ 8 ];

	/* The volume identifier
	 * Consists of 16 bytes
	 * Contains an UUID
	 */
	uint8_t volume_identifier[ 16 ];

	/* The volume (last) modification date and time
	 * Consists of 8 bytes
	 */
	uint8_t modification_time[ 8 ];

	/* The volume flags
	 * Consists of 8 bytes
	 */
	uint8_t volume_flags[ 8 ];

	/* "Formatted By" information
	 * Consists of 32 byte "name", 8 byte timestamp, 8 byte transaction id
	 */
	fsapfs_modified_by_t formatted_by;

	/* "Modified By"  information *8 (APFS_MAX_HIST)
	 * Each record consists of 32 byte "name", 8 byte timestamp, 8 byte transaction id
	 */
	fsapfs_modified_by_t modified_by[8];

	/* The volume name
	 * Consists of 256 bytes (APFS_VOLNAME_LEN)
	 */
	uint8_t volume_name[ 256 ];

	/* The next (available) document identifier
	 * Consists of 4 bytes
	 */
	uint8_t next_document_identifier[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown60[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown61[ 8 ];

	/* Unknown
	 * Consists of 32 bytes
	 */
	uint8_t unknown62[ 32 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_VOLUME_SUPERBLOCK_H ) */

