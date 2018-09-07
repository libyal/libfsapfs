/*
 * The APFS container superblock definition
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

	/* The object unknown1
	 * Consists of 8 bytes
	 */
	uint8_t object_unknown1[ 8 ];

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
	uint8_t block_size[ 8 ];

	/* The number of block
	 * Consists of 8 bytes
	 */
	uint8_t number_of_block[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_CONTAINER_SUPERBLOCK_H ) */

