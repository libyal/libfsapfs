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

	/* The volume identifier
	 * Consists of 16 bytes
	 * Contains an UUID
	 */
	uint8_t volume_identifier[ 16 ];

	/* The next object identifier
	 * Consists of 8 bytes
	 */
	uint8_t next_object_identifier[ 8 ];

	/* The unknown3
	 * Consists of 8 bytes
	 */
	uint8_t unknown3[ 8 ];

	/* The unknown4
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* The unknown5
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* The unknown6
	 * Consists of 8 bytes
	 */
	uint8_t unknown6[ 8 ];

	/* The unknown7
	 * Consists of 8 bytes
	 */
	uint8_t unknown7[ 8 ];

	/* The unknown8
	 * Consists of 4 bytes
	 */
	uint8_t unknown8[ 4 ];

	/* The unknown9
	 * Consists of 4 bytes
	 */
	uint8_t unknown9[ 4 ];

	/* The unknown10
	 * Consists of 4 bytes
	 */
	uint8_t unknown10[ 4 ];

	/* The unknown11
	 * Consists of 4 bytes
	 */
	uint8_t unknown11[ 4 ];

	/* The unknown12
	 * Consists of 4 bytes
	 */
	uint8_t unknown12[ 4 ];

	/* The unknown13
	 * Consists of 4 bytes
	 */
	uint8_t unknown13[ 4 ];

	/* The space manager object identifier
	 * Consists of 8 bytes
	 */
	uint8_t space_manager_object_identifier[ 8 ];

	/* The object map object identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_map_object_identifier[ 8 ];

	/* The reaper object identifier
	 * Consists of 8 bytes
	 */
	uint8_t reaper_object_identifier[ 8 ];

	/* The unknown17
	 * Consists of 4 bytes
	 */
	uint8_t unknown17[ 4 ];

	/* The unknown18
	 * Consists of 4 bytes
	 */
	uint8_t unknown18[ 4 ];

	/* The volume object identifiers
	 * Consists of 100 x 8 bytes
	 */
	uint8_t volume_object_identifiers[ 800 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_VOLUME_SUPERBLOCK_H ) */

