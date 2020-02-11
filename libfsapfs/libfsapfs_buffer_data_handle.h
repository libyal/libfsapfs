/*
 * The buffer data handle functions
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

#if !defined( _LIBFSAPFS_BUFFER_DATA_HANDLE_H )
#define _LIBFSAPFS_BUFFER_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_buffer_data_handle libfsapfs_buffer_data_handle_t;

struct libfsapfs_buffer_data_handle
{
	/* The current offset
	 */
	off64_t current_offset;

	/* The data
	 */
	const uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libfsapfs_buffer_data_handle_initialize(
     libfsapfs_buffer_data_handle_t **data_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_buffer_data_handle_free(
     libfsapfs_buffer_data_handle_t **data_handle,
     libcerror_error_t **error );

ssize_t libfsapfs_buffer_data_handle_read_segment_data(
         libfsapfs_buffer_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libfsapfs_buffer_data_handle_seek_segment_offset(
         libfsapfs_buffer_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_BUFFER_DATA_HANDLE_H ) */

