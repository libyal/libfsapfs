/*
 * The compressed data handle functions
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

#if !defined( _LIBFSAPFS_COMPRESSED_DATA_HANDLE_H )
#define _LIBFSAPFS_COMPRESSED_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_compressed_data_handle libfsapfs_compressed_data_handle_t;

struct libfsapfs_compressed_data_handle
{
	/* The current segment offset
	 */
	off64_t current_segment_offset;

	/* The compressed data steam
	 */
	libfdata_stream_t *compressed_data_stream;

	/* The uncompressed data size
	 */
	size64_t uncompressed_data_size;

	/* The compression method
	 */
	int compression_method;

	/* The current compressed block index
	 */
	uint32_t current_compressed_block_index;

	/* The compressed segment data
	 */
	uint8_t *compressed_segment_data;

	/* The (uncompressed) segment data
	 */
	uint8_t *segment_data;

	/* The (uncompressed) segment data size
	 */
	size_t segment_data_size;

	/* The number of compressed blocks
	 */
	uint32_t number_of_compressed_blocks;

	/* The compressed block offsets
	 */
	uint32_t *compressed_block_offsets;
};

int libfsapfs_compressed_data_handle_initialize(
     libfsapfs_compressed_data_handle_t **data_handle,
     libfdata_stream_t *compressed_data_stream,
     size64_t uncompressed_data_size,
     int compression_method,
     libcerror_error_t **error );

int libfsapfs_compressed_data_handle_free(
     libfsapfs_compressed_data_handle_t **data_handle,
     libcerror_error_t **error );

int libfsapfs_compressed_data_handle_get_compressed_block_offsets(
     libfsapfs_compressed_data_handle_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

ssize_t libfsapfs_compressed_data_handle_read_segment_data(
         libfsapfs_compressed_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libfsapfs_compressed_data_handle_seek_segment_offset(
         libfsapfs_compressed_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_COMPRESSED_DATA_HANDLE_H ) */

