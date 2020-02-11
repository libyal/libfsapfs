/*
 * The volume superblock functions
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

#if !defined( _LIBFSAPFS_VOLUME_SUPERBLOCK_H )
#define _LIBFSAPFS_VOLUME_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_volume_superblock libfsapfs_volume_superblock_t;

struct libfsapfs_volume_superblock
{
	/* The incompatibility features flags
	 */
	uint64_t incompatibility_features_flags;

	/* The object map block number
	 */
	uint64_t object_map_block_number;

	/* The file system root object identifier
	 */
	uint64_t file_system_root_object_identifier;

	/* The extent-reference tree block number
	 */
	uint64_t extent_reference_tree_block_number;

	/* The snapshot metadata tree block number
	 */
	uint64_t snapshot_metadata_tree_block_number;

	/* The next file system object identifier
	 */
	uint64_t next_file_system_object_identifier;

	/* The volume identifier
	 */
	uint8_t volume_identifier[ 16 ];

	/* The volume flags
	 */
	uint64_t volume_flags;

	/* The volume name
	 */
	uint8_t volume_name[ 256 ];
};

int libfsapfs_volume_superblock_initialize(
     libfsapfs_volume_superblock_t **volume_superblock,
     libcerror_error_t **error );

int libfsapfs_volume_superblock_free(
     libfsapfs_volume_superblock_t **volume_superblock,
     libcerror_error_t **error );

int libfsapfs_volume_superblock_read_file_io_handle(
     libfsapfs_volume_superblock_t *volume_superblock,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsapfs_volume_superblock_read_data(
     libfsapfs_volume_superblock_t *volume_superblock,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_volume_superblock_get_volume_identifier(
     libfsapfs_volume_superblock_t *volume_superblock,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

int libfsapfs_volume_superblock_get_utf8_volume_name_size(
     libfsapfs_volume_superblock_t *volume_superblock,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_volume_superblock_get_utf8_volume_name(
     libfsapfs_volume_superblock_t *volume_superblock,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_volume_superblock_get_utf16_volume_name_size(
     libfsapfs_volume_superblock_t *volume_superblock,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_volume_superblock_get_utf16_volume_name(
     libfsapfs_volume_superblock_t *volume_superblock,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_VOLUME_SUPERBLOCK_H ) */

