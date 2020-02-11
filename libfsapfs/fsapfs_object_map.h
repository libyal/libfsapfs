/*
 * The APFS object map definitions
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

#if !defined( _FSAPFS_OBJECT_MAP_H )
#define _FSAPFS_OBJECT_MAP_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_object_map fsapfs_object_map_t;

struct fsapfs_object_map
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

	/* The flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* The number of snapshots
	 * Consists of 4 bytes
	 */
	uint8_t number_of_snapshots[ 4 ];

	/* The B-tree type
	 * Consists of 4 bytes
	 */
	uint8_t btree_type[ 4 ];

	/* The snapshots B-tree type
	 * Consists of 4 bytes
	 */
	uint8_t snaphots_btree_type[ 4 ];

	/* The B-tree block number
	 * Consists of 8 bytes
	 */
	uint8_t btree_block_number[ 8 ];

	/* The snapshots B-tree block number
	 * Consists of 8 bytes
	 */
	uint8_t snapshots_btree_block_number[ 8 ];

	/* The most recent snapshot object identifier
	 * Consists of 8 bytes
	 */
	uint8_t most_recent_snapshot_identifier[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown3[ 8 ];
};

typedef struct fsapfs_object_map_btree_key fsapfs_object_map_btree_key_t;

struct fsapfs_object_map_btree_key
{
	/* The key object identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_identifier[ 8 ];

	/* The key object transaction identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_transaction_identifier[ 8 ];
};

typedef struct fsapfs_object_map_btree_value fsapfs_object_map_btree_value_t;

struct fsapfs_object_map_btree_value
{
	/* The object flags
	 * Consists of 4 bytes
	 */
	uint8_t object_flags[ 4 ];

	/* The object size
	 * Consists of 4 bytes
	 */
	uint8_t object_size[ 4 ];

	/* The object physical address
	 * Consists of 8 bytes
	 */
	uint8_t object_physical_address[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_OBJECT_MAP_H ) */

