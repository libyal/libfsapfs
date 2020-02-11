/*
 * Data stream functions
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

#if !defined( _LIBFSAPFS_DATA_STREAM_H )
#define _LIBFSAPFS_DATA_STREAM_H

#include <common.h>
#include <types.h>

#include "libfsapfs_encryption_context.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsapfs_data_stream_initialize_from_data(
     libfdata_stream_t **data_stream,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_data_stream_initialize_from_file_extents(
     libfdata_stream_t **data_stream,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libcdata_array_t *file_extents,
     size64_t data_stream_size,
     uint8_t is_sparse,
     libcerror_error_t **error );

int libfsapfs_data_stream_initialize_from_compressed_data_stream(
     libfdata_stream_t **data_stream,
     libfdata_stream_t *compressed_data_stream,
     size64_t uncompressed_data_size,
     int compression_method,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_DATA_STREAM_H ) */

