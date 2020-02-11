/*
 * The APFS file system definitions
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

typedef struct fsapfs_file_system_btree_key_directory_record fsapfs_file_system_btree_key_directory_record_t;

struct fsapfs_file_system_btree_key_directory_record
{
	/* The file system identifier (FSID) and data type
	 * Consists of 8 bytes
	 */
	uint8_t file_system_identifier[ 8 ];

	/* The name size
	 * Consists of 2 bytes
	 */
	uint8_t name_size[ 2 ];

	/* The name string
	 * Contains an UTF-8 string with end-of-string character
	 */
};

typedef struct fsapfs_file_system_btree_key_directory_record_with_hash fsapfs_file_system_btree_key_directory_record_with_hash_t;

struct fsapfs_file_system_btree_key_directory_record_with_hash
{
	/* The file system identifier (FSID) and data type
	 * Consists of 8 bytes
	 */
	uint8_t file_system_identifier[ 8 ];

	/* The name size and hash
	 * Consists of 4 bytes
	 */
	uint8_t name_size_and_hash[ 4 ];

	/* The name string
	 * Contains an UTF-8 string with end-of-string character
	 */
};

typedef struct fsapfs_file_system_btree_key_extended_attribute fsapfs_file_system_btree_key_extended_attribute_t;

struct fsapfs_file_system_btree_key_extended_attribute
{
	/* The file system identifier (FSID) and data type
	 * Consists of 8 bytes
	 */
	uint8_t file_system_identifier[ 8 ];

	/* The name size
	 * Consists of 2 bytes
	 */
	uint8_t name_size[ 2 ];

	/* The name string
	 * Contains an UTF-8 string with end-of-string character
	 */
};

typedef struct fsapfs_file_system_btree_key_file_extent fsapfs_file_system_btree_key_file_extent_t;

struct fsapfs_file_system_btree_key_file_extent
{
	/* The file system identifier (FSID) and data type
	 * Consists of 8 bytes
	 */
	uint8_t file_system_identifier[ 8 ];

	/* The logical address
	 * Consists of 8 bytes
	 */
	uint8_t logical_address[ 8 ];
};

typedef struct fsapfs_file_system_btree_value_directory_record fsapfs_file_system_btree_value_directory_record_t;

struct fsapfs_file_system_btree_value_directory_record
{
	/* The file system identifier (FSID)
	 * Consists of 8 bytes
	 */
	uint8_t file_system_identifier[ 8 ];

	/* The added date and time
	 * Consists of 8 bytes
	 */
	uint8_t added_time[ 8 ];

	/* The directory entry flags
	 * Consists of 2 bytes
	 */
	uint8_t directory_entry_flags[ 2 ];

	/* Extended fields
	 */
};

typedef struct fsapfs_file_system_btree_value_extended_attribute fsapfs_file_system_btree_value_extended_attribute_t;

struct fsapfs_file_system_btree_value_extended_attribute
{
	/* The flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The data
	 */
};

typedef struct fsapfs_file_system_extended_attribute_data_stream fsapfs_file_system_extended_attribute_data_stream_t;

struct fsapfs_file_system_extended_attribute_data_stream
{
	/* The data stream identifier
	 * Consists of 8 bytes
	 */
	uint8_t data_stream_identifier[ 8 ];

	/* The used size
	 * Consists of 8 bytes
	 */
	uint8_t used_size[ 8 ];

	/* The allocated size
	 * Consists of 8 bytes
	 */
	uint8_t allocated_size[ 8 ];

	/* The encryption identifier
	 * Consists of 8 bytes
	 */
	uint8_t encryption_identifier[ 8 ];

	/* The number of bytes written
	 * Consists of 8 bytes
	 */
	uint8_t number_of_bytes_written[ 8 ];

	/* The number of bytes read
	 * Consists of 8 bytes
	 */
	uint8_t number_of_bytes_read[ 8 ];
};

typedef struct fsapfs_file_system_extended_attribute_compression_header fsapfs_file_system_extended_attribute_compression_header_t;

struct fsapfs_file_system_extended_attribute_compression_header
{
	/* The signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* The compression method
	 * Consists of 4 bytes
	 */
	uint8_t compression_method[ 4 ];

	/* The uncompressed data size
	 * Consists of 8 bytes
	 */
	uint8_t uncompressed_data_size[ 8 ];
};

typedef struct fsapfs_file_system_btree_value_file_extent fsapfs_file_system_btree_value_file_extent_t;

struct fsapfs_file_system_btree_value_file_extent
{
	/* The data size and flags
	 * Consists of 8 bytes
	 */
	uint8_t data_size_and_flags[ 8 ];

	/* The physical block number
	 * Consists of 8 bytes
	 */
	uint8_t physical_block_number[ 8 ];

	/* The encryption identifier
	 * Consists of 8 bytes
	 */
	uint8_t encryption_identifier[ 8 ];
};

typedef struct fsapfs_file_system_btree_value_inode fsapfs_file_system_btree_value_inode_t;

struct fsapfs_file_system_btree_value_inode
{
	/* The parent file system identifier (FSID)
	 * Consists of 8 bytes
	 */
	uint8_t parent_identifier[ 8 ];

	/* The data stream file system identifier (FSID)
	 * Consists of 8 bytes
	 */
	uint8_t data_stream_identifier[ 8 ];

	/* The modification date and time
	 * Consists of 8 bytes
	 */
	uint8_t modification_time[ 8 ];

	/* The creation date and time
	 * Consists of 8 bytes
	 */
	uint8_t creation_time[ 8 ];

	/* The inode change date and time
	 * Consists of 8 bytes
	 */
	uint8_t inode_change_time[ 8 ];

	/* The access date and time
	 * Consists of 8 bytes
	 */
	uint8_t access_time[ 8 ];

	/* The inode flags
	 * Consists of 8 bytes
	 */
	uint8_t inode_flags[ 8 ];

	/* The number of children
	 * Consists of 4 bytes
	 */
	uint8_t number_of_children[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* The BSD flags
	 * Consists of 4 bytes
	 */
	uint8_t bsd_flags[ 4 ];

	/* The owner user identifier (UID)
	 * Consists of 4 bytes
	 */
	uint8_t owner_identifier[ 4 ];

	/* The group identifier (GID)
	 * Consists of 4 bytes
	 */
	uint8_t group_identifier[ 4 ];

	/* The file mode
	 * Consists of 2 bytes
	 */
	uint8_t file_mode[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown4[ 8 ];

	/* Extended fields
	 */
};

typedef struct fsapfs_file_system_data_stream_attribute fsapfs_file_system_data_stream_attribute_t;

struct fsapfs_file_system_data_stream_attribute
{
	/* The used size
	 * Consists of 8 bytes
	 */
	uint8_t used_size[ 8 ];

	/* The allocated size
	 * Consists of 8 bytes
	 */
	uint8_t allocated_size[ 8 ];

	/* The encryption identifier
	 * Consists of 8 bytes
	 */
	uint8_t encryption_identifier[ 8 ];

	/* The number of bytes written
	 * Consists of 8 bytes
	 */
	uint8_t number_of_bytes_written[ 8 ];

	/* The number of bytes read
	 * Consists of 8 bytes
	 */
	uint8_t number_of_bytes_read[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_FILE_SYSTEM_H ) */

