/*
 * The APFS snapshot metadata definitions
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

#if !defined( _FSAPFS_SNAPSHOT_METADATA_H )
#define _FSAPFS_SNAPSHOT_METADATA_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_snapshot_metadata_btree_key fsapfs_snapshot_metadata_btree_key_t;

struct fsapfs_snapshot_metadata_btree_key
{
	/* The key object identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_identifier[ 8 ];
};

typedef struct fsapfs_snapshot_metadata_btree_value fsapfs_snapshot_metadata_btree_value_t;

struct fsapfs_snapshot_metadata_btree_value
{
	/* The extent-reference tree block number
	 * Consists of 8 bytes
	 */
	uint8_t extent_reference_tree_block_number[ 8 ];

	/* The volume superblock block number
	 * Consists of 8 bytes
	 */
	uint8_t volume_superblock_block_number[ 8 ];

	/* The creation date and time
	 * Consists of 8 bytes
	 */
	uint8_t creation_time[ 8 ];

	/* The change date and time
	 * Consists of 8 bytes
	 */
	uint8_t change_time[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];

	/* The extent-reference tree object type
	 * Consists of 8 bytes
	 */
	uint8_t extent_reference_tree_object_type[ 4 ];

	/* The flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* The name size
	 * Consists of 2 bytes
	 */
	uint8_t name_size[ 2 ];

	/* The name string
	 * Contains an UTF-8 string with end-of-string character
	 */
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_SNAPSHOT_METADATA_H ) */

