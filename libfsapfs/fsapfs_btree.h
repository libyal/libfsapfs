/*
 * The APFS B-tree definition
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

#if !defined( _FSAPFS_BTREE_H )
#define _FSAPFS_BTREE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_btree_header fsapfs_btree_header_t;

struct fsapfs_btree_header
{
	/* The flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* The level
	 * Consists of 2 bytes
	 */
	uint8_t level[ 2 ];

	/* The number of keys
	 * Consists of 4 bytes
	 */
	uint8_t number_of_keys[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown4[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown5[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown6[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown7[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown8[ 2 ];
};

typedef struct fsapfs_btree_footer fsapfs_btree_footer_t;

struct fsapfs_btree_footer
{
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

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown6[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown7[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown8[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_BTREE_H ) */

