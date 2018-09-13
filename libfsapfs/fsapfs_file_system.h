/*
 * The APFS file system definitions
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

#if !defined( _FSAPFS_FILE_SYSTEM_H )
#define _FSAPFS_FILE_SYSTEM_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_file_system_btree_key_common fsapfs_file_system_btree_key_common_t;

struct fsapfs_file_system_btree_key_common
{
	/* The file system identifier (FSID) and data type
	 * Consists of 8 bytes
	 */
	uint8_t file_system_identifier[ 8 ];
};

typedef struct fsapfs_file_system_btree_key_file_extent fsapfs_file_system_btree_key_file_extent_t;

struct fsapfs_file_system_btree_key_file_extent
{
	/* The file system identifier (FSID) and data type
	 * Consists of 8 bytes
	 */
	uint8_t file_system_identifier[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];
};

typedef struct fsapfs_file_system_btree_key_directory_record fsapfs_file_system_btree_key_directory_record_t;

struct fsapfs_file_system_btree_key_directory_record
{
	/* The file system identifier (FSID) and data type
	 * Consists of 8 bytes
	 */
	uint8_t file_system_identifier[ 8 ];

	/* The name size
	 * Consists of 1 byte
	 */
	uint8_t name_size[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The name string
	 * Contains an UTF-8 string with end-of-string character
	 */
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_FILE_SYSTEM_H ) */

