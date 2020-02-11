/*
 * The APFS key bag definitions
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

#if !defined( _FSAPFS_KEY_BAG_H )
#define _FSAPFS_KEY_BAG_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_key_bag_header fsapfs_key_bag_header_t;

struct fsapfs_key_bag_header
{
	/* The format version
	 * Consists of 2 bytes
	 */
	uint8_t format_version[ 2 ];

	/* The number of entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* The data size
	 * Consists of 4 bytes
	 */
	uint8_t data_size[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];
};

typedef struct fsapfs_key_bag_entry_header fsapfs_key_bag_entry_header_t;

struct fsapfs_key_bag_entry_header
{
	/* The identifier
	 * Consists of 16 bytes
	 * Contains an UUID
	 */
	uint8_t identifier[ 16 ];

	/* The entry type
	 * Consists of 2 bytes
	 */
	uint8_t entry_type[ 2 ];

	/* The entry data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

typedef struct fsapfs_key_bag_kek_metadata fsapfs_key_bag_kek_metadata_t;

struct fsapfs_key_bag_kek_metadata
{
	/* The encryption method
	 * Consists of 4 bytes
	 */
	uint8_t encryption_method[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown2;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown3;
};

typedef struct fsapfs_key_bag_extent fsapfs_key_bag_extent_t;

struct fsapfs_key_bag_extent
{
	/* The block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number[ 8 ];

	/* The number of blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_blocks[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_KEY_BAG_H ) */

