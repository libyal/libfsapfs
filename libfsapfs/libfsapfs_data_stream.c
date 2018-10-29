/*
 * Data stream functions
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

#include <common.h>
#include <types.h>

#include "libfsapfs_buffer_data_handle.h"
#include "libfsapfs_compressed_data_handle.h"
#include "libfsapfs_data_block_data_handle.h"
#include "libfsapfs_data_stream.h"
#include "libfsapfs_file_extent.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_volume_data_handle.h"

/* Creates data stream from a buffer of data
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_data_stream_initialize_from_data(
     libfdata_stream_t **data_stream,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_buffer_data_handle_t *data_handle = NULL;
	static char *function                       = "libfsapfs_data_stream_initialize_from_data";
	int segment_index                           = 0;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream attribute.",
		 function );

		return( -1 );
	}
	if( libfsapfs_buffer_data_handle_initialize(
	     &data_handle,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create buffer data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_buffer_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsapfs_buffer_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsapfs_buffer_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     *data_stream,
	     &segment_index,
	     0,
	     0,
	     (size64_t) data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data as data stream segment.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_stream != NULL )
	{
		libfdata_stream_free(
		 data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfsapfs_buffer_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Creates data stream from file extents
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_data_stream_initialize_from_file_extents(
     libfdata_stream_t **data_stream,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_volume_data_handle_t *volume_data_handle,
     libcdata_array_t *file_extents,
     size64_t data_stream_size,
     libcerror_error_t **error )
{
	libfsapfs_data_block_data_handle_t *data_handle = NULL;
	libfsapfs_file_extent_t *file_extent                          = NULL;
	static char *function                                         = "libfsapfs_data_stream_initialize_from_file_extents";
	int extent_index                                              = 0;
	int number_of_extents                                         = 0;
	int segment_index                                             = 0;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream attribute.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libfsapfs_data_block_data_handle_initialize(
	     &data_handle,
	     io_handle,
	     volume_data_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_data_block_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsapfs_data_block_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsapfs_data_block_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libcdata_array_get_number_of_entries(
	     file_extents,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from array.",
		 function );

		goto on_error;
	}
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     file_extents,
		     extent_index,
		     (intptr_t **) &file_extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file extent: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( file_extent == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing file extent: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( libfdata_stream_append_segment(
		     *data_stream,
		     &segment_index,
		     0,
		     file_extent->block_number * io_handle->block_size,
		     file_extent->data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent: %d as data stream segment.",
			 function,
			 extent_index );

			goto on_error;
		}
	}
/* TODO handle mapped size of 0 */
	if( libfdata_stream_set_mapped_size(
	     *data_stream,
	     data_stream_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set mapped size.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_stream != NULL )
	{
		libfdata_stream_free(
		 data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfsapfs_data_block_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Creates data stream from a compressed data stream
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_data_stream_initialize_from_compressed_data_stream(
     libfdata_stream_t **data_stream,
     libfdata_stream_t *compressed_data_stream,
     size64_t uncompressed_data_size,
     int compression_method,
     libcerror_error_t **error )
{
	libfsapfs_compressed_data_handle_t *data_handle = NULL;
	static char *function                           = "libfsapfs_data_stream_initialize_from_compressed_data_stream";
	int segment_index                               = 0;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream attribute.",
		 function );

		return( -1 );
	}
	if( libfsapfs_compressed_data_handle_initialize(
	     &data_handle,
	     compressed_data_stream,
	     uncompressed_data_size,
	     compression_method,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compressed data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_compressed_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsapfs_compressed_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsapfs_compressed_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     *data_stream,
	     &segment_index,
	     0,
	     0,
	     uncompressed_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data as data stream segment.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_stream != NULL )
	{
		libfdata_stream_free(
		 data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfsapfs_compressed_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

