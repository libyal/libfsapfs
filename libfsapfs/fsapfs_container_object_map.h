/*
 * The APFS container object map definition
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

#if !defined( _FSAPFS_CONTAINER_OBJECT_MAP_H )
#define _FSAPFS_CONTAINER_OBJECT_MAP_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_container_object_map fsapfs_container_object_map_t;

struct fsapfs_container_object_map
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

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* The object map B-tree block number
	 * Consists of 8 bytes
	 */
	uint8_t object_map_btree_block_number[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown6[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown7[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_CONTAINER_OBJECT_MAP_H ) */

