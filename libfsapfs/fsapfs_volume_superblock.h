/*
 * The APFS volume superblock definition
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

#if !defined( _FSAPFS_VOLUME_SUPERBLOCK_H )
#define _FSAPFS_VOLUME_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_volume_superblock fsapfs_volume_superblock_t;

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

	/* The object version
	 * Consists of 8 bytes
	 */
	uint8_t object_version[ 8 ];

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

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown8[ 8 ];

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

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown13[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown14[ 4 ];

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

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown18[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown19[ 8 ];

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

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown25[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown26[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown27[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown28[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown29[ 8 ];

	/* The volume identifier
	 * Consists of 16 bytes
	 * Contains an UUID
	 */
	uint8_t volume_identifier[ 16 ];

/* TODO add values starting with apfs_last_mod_time */
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_VOLUME_SUPERBLOCK_H ) */

