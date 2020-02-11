/*
 * The snapshot metadata functions
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

#if !defined( _LIBFSAPFS_SNAPSHOT_METADATA_H )
#define _LIBFSAPFS_SNAPSHOT_METADATA_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_snapshot_metadata libfsapfs_snapshot_metadata_t;

struct libfsapfs_snapshot_metadata
{
	/* Volume superblock block number
	 */
	uint64_t volume_superblock_block_number;

	/* Name size
	 */
	uint16_t name_size;

	/* Name
	 */
	uint8_t *name;
};

int libfsapfs_snapshot_metadata_initialize(
     libfsapfs_snapshot_metadata_t **snapshot_metadata,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_free(
     libfsapfs_snapshot_metadata_t **snapshot_metadata,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_read_key_data(
     libfsapfs_snapshot_metadata_t *snapshot_metadata,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_read_value_data(
     libfsapfs_snapshot_metadata_t *snapshot_metadata,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_get_utf8_name_size(
     libfsapfs_snapshot_metadata_t *snapshot_metadata,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_get_utf8_name(
     libfsapfs_snapshot_metadata_t *snapshot_metadata,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_get_utf16_name_size(
     libfsapfs_snapshot_metadata_t *snapshot_metadata,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_snapshot_metadata_get_utf16_name(
     libfsapfs_snapshot_metadata_t *snapshot_metadata,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_SNAPSHOT_METADATA_H ) */

