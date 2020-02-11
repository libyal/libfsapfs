/*
 * The data block data handle functions
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

#if !defined( _LIBFSAPFS_DATA_BLOCK_DATA_HANDLE_H )
#define _LIBFSAPFS_DATA_BLOCK_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_encryption_context.h"
#include "libfsapfs_file_system_data_handle.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_data_block_data_handle libfsapfs_data_block_data_handle_t;

struct libfsapfs_data_block_data_handle
{
	/* The current offset
	 */
	off64_t current_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The file system data handle
	 */
	libfsapfs_file_system_data_handle_t *file_system_data_handle;

	/* The data block vector
	 */
	libfdata_vector_t *data_block_vector;

	/* The data block cache
	 */
	libfcache_cache_t *data_block_cache;
};

int libfsapfs_data_block_data_handle_initialize(
     libfsapfs_data_block_data_handle_t **data_handle,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libcdata_array_t *file_extents,
     uint8_t is_sparse,
     libcerror_error_t **error );

int libfsapfs_data_block_data_handle_free(
     libfsapfs_data_block_data_handle_t **data_handle,
     libcerror_error_t **error );

ssize_t libfsapfs_data_block_data_handle_read_segment_data(
         libfsapfs_data_block_data_handle_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libfsapfs_data_block_data_handle_seek_segment_offset(
         libfsapfs_data_block_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_DATA_BLOCK_DATA_HANDLE_H ) */

