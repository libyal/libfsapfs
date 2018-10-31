/*
 * The compressed data handle functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_compressed_data_handle.h"
#include "libfsapfs_compression.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_unused.h"

#define LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE	65536

/* Creates compressed data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_compressed_data_handle_initialize(
     libfsapfs_compressed_data_handle_t **data_handle,
     libfdata_stream_t *compressed_data_stream,
     size64_t uncompressed_data_size,
     int compression_method,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_compressed_data_handle_initialize";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle value already set.",
		 function );

		return( -1 );
	}
	if( compressed_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data stream.",
		 function );

		return( -1 );
	}
	if( ( compression_method != LIBFSAPFS_COMPRESSION_METHOD_NONE )
	 && ( compression_method != LIBFSAPFS_COMPRESSION_METHOD_DEFLATE )
	 && ( compression_method != LIBFSAPFS_COMPRESSION_METHOD_LZVN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method.",
		 function );

		return( -1 );
	}
	*data_handle = memory_allocate_structure(
	                libfsapfs_compressed_data_handle_t );

	if( *data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_handle,
	     0,
	     sizeof( libfsapfs_compressed_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		memory_free(
		 *data_handle );

		*data_handle = NULL;

		return( -1 );
	}
	( *data_handle )->compressed_segment_data = (uint8_t *) memory_allocate(
	                                                         sizeof( uint8_t ) * LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE );

	if( ( *data_handle )->compressed_segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed segment data.",
		 function );

		goto on_error;
	}
	( *data_handle )->segment_data = (uint8_t *) memory_allocate(
	                                              sizeof( uint8_t ) * LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE );

	if( ( *data_handle )->segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment data.",
		 function );

		goto on_error;
	}
	( *data_handle )->compressed_data_stream         = compressed_data_stream;
	( *data_handle )->current_compressed_block_index = (uint32_t) -1;
	( *data_handle )->uncompressed_data_size         = uncompressed_data_size;
	( *data_handle )->compression_method             = compression_method;

	return( 1 );

on_error:
	if( *data_handle != NULL )
	{
		if( ( *data_handle )->segment_data != NULL )
		{
			memory_free(
			 ( *data_handle )->segment_data );
		}
		if( ( *data_handle )->compressed_segment_data != NULL )
		{
			memory_free(
			 ( *data_handle )->compressed_segment_data );
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( -1 );
}

/* Frees data handle
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_compressed_data_handle_free(
     libfsapfs_compressed_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_compressed_data_handle_free";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		if( ( *data_handle )->segment_data != NULL )
		{
			memory_free(
			 ( *data_handle )->segment_data );
		}
		if( ( *data_handle )->compressed_segment_data != NULL )
		{
			memory_free(
			 ( *data_handle )->compressed_segment_data );
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( 1 );
}

/* Reads data from the current offset into a compressed
 * Callback for the data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsapfs_compressed_data_handle_read_segment_data(
         libfsapfs_compressed_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index LIBFSAPFS_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBFSAPFS_ATTRIBUTE_UNUSED,
         uint8_t read_flags LIBFSAPFS_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	static char *function           = "libfsapfs_compressed_data_handle_read_segment_data";
	off64_t data_stream_offset      = 0;
	size_t data_offset              = 0;
	size_t read_size                = 0;
	size_t segment_data_offset      = 0;
	ssize_t read_count              = 0;
	uint32_t compressed_block_index = 0;
	uint32_t compressed_block_size  = 0;
	uint32_t compressed_data_offset = 0;

	LIBFSAPFS_UNREFERENCED_PARAMETER( file_io_handle )
	LIBFSAPFS_UNREFERENCED_PARAMETER( segment_file_index )
	LIBFSAPFS_UNREFERENCED_PARAMETER( segment_flags )
	LIBFSAPFS_UNREFERENCED_PARAMETER( read_flags )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_index != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment data.",
		 function );

		return( -1 );
	}
	if( segment_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_handle->compressed_block_offsets == NULL )
	{
		read_count = libfdata_stream_read_buffer_at_offset(
		              data_handle->compressed_data_stream,
		              (intptr_t *) file_io_handle,
		              data_handle->compressed_segment_data,
		              4,
		              0,
		              0,
		              error );

		if( read_count != 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer at offset: 0 (0x00000000) from data stream.",
			 function );

			return( -1 );
		}
		if( memory_compare(
		     data_handle->compressed_segment_data,
		     "fpmc",
		     4 ) == 0 )
		{
			if( segment_data_size > (size_t) LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid segment data size value out of bounds.",
				 function );

				return( -1 );
			}
			data_handle->number_of_compressed_blocks = 1;

			data_handle->compressed_block_offsets = (off64_t *) memory_allocate(
			                                                     sizeof( off64_t ) * ( data_handle->number_of_compressed_blocks + 1 ) );

			if( data_handle->compressed_block_offsets == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create compressed block offsets.",
				 function );

				return( -1 );
			}
			data_handle->compressed_block_offsets[ 0 ] = 16;
			data_handle->compressed_block_offsets[ 1 ] = (off64_t) segment_data_size;
		}
		else
		{
			byte_stream_copy_to_uint32_little_endian(
			 data_handle->compressed_segment_data,
			 compressed_data_offset );

			if( ( compressed_data_offset <= 4 )
			 || ( compressed_data_offset >= LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid compressed data offset value out of bounds.",
				 function );

				return( -1 );
			}
			data_handle->number_of_compressed_blocks = ( compressed_data_offset / 4 ) - 1;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: number of compressed blocks\t: %" PRIu32 "\n",
				 function,
				 data_handle->number_of_compressed_blocks );
			}
#endif
			read_size = (size_t) compressed_data_offset - 4;

			read_count = libfdata_stream_read_buffer_at_offset(
			              data_handle->compressed_data_stream,
			              (intptr_t *) file_io_handle,
			              data_handle->compressed_segment_data,
			              read_size,
			              4,
			              0,
			              error );

			if( read_count != read_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read buffer at offset: 4 (0x00000004) from data stream.",
				 function );

				return( -1 );
			}
			data_handle->compressed_block_offsets = (off64_t *) memory_allocate(
			                                                     sizeof( off64_t ) * ( data_handle->number_of_compressed_blocks + 1 ) );

			if( data_handle->compressed_block_offsets == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create compressed block offsets.",
				 function );

				return( -1 );
			}
			segment_data_offset = 0;
			data_stream_offset  = (off64_t) compressed_data_offset;

			data_handle->compressed_block_offsets[ 0 ] = data_stream_offset;

			for( compressed_block_index = 1;
			     compressed_block_index <= data_handle->number_of_compressed_blocks;
			     compressed_block_index++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 &( data_handle->compressed_segment_data[ segment_data_offset ] ),
				 compressed_block_size );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: compressed block: %" PRIu32 " size\t: %" PRIu32 "\n",
					 function,
					 compressed_block_index,
					 compressed_block_size );
				}
#endif
				if( compressed_block_size > (uint32_t) LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid compressed block: %" PRIu32 " size value out of bounds.",
					 function,
					 compressed_block_size );

					return( -1 );
				}
				data_stream_offset += compressed_block_size;

				data_handle->compressed_block_offsets[ compressed_block_index ] = data_stream_offset;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "\n" );
			}
#endif
		}
	}
	if( data_handle->current_segment_offset >= data_handle->uncompressed_data_size )
	{
		return( 0 );
	}
	compressed_block_index = data_handle->current_segment_offset / LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE;
	segment_data_offset    = 0;
	data_offset            = (size_t) ( data_handle->current_segment_offset % LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE );

	while( segment_data_size > 0 )
	{
		if( compressed_block_index >= data_handle->number_of_compressed_blocks )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed block index value out of bounds.",
			 function );

			return( -1 );
		}
		if( data_handle->current_compressed_block_index != compressed_block_index )
		{
			data_stream_offset = data_handle->compressed_block_offsets[ compressed_block_index ];
			read_size          = (size_t) ( data_handle->compressed_block_offsets[ compressed_block_index + 1 ] - data_stream_offset );

			read_count = libfdata_stream_read_buffer_at_offset(
			              data_handle->compressed_data_stream,
			              (intptr_t *) file_io_handle,
			              (uint8_t *) data_handle->compressed_segment_data,
			              read_size,
			              data_stream_offset,
			              0,
			              error );

			if( read_count != read_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read buffer at offset: %" PRIi64 " (0x08%" PRIx64 ") from data stream.",
				 function,
				 data_stream_offset,
				 data_stream_offset );

				return( -1 );
			}
			data_handle->segment_data_size = LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE;

			if( libfsapfs_decompress_data(
			     data_handle->compressed_segment_data,
			     (size_t) read_count,
			     data_handle->compression_method,
			     data_handle->segment_data,
			     &( data_handle->segment_data_size ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to decompress data.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: uncompressed data:\n",
				 function );
				libcnotify_print_data(
				 data_handle->segment_data,
				 data_handle->segment_data_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			if( ( ( compressed_block_index + 1 ) != data_handle->number_of_compressed_blocks )
			 && ( data_handle->segment_data_size != LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid uncompressed segment data size value out of bounds.",
				 function );

				return( -1 );
			}
			data_handle->current_compressed_block_index = compressed_block_index;
		}
		if( data_offset >= data_handle->segment_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data offset value out of bounds.",
			 function );

			return( -1 );
		}
		read_size = data_handle->segment_data_size - data_offset;

		if( read_size > segment_data_size )
		{
			read_size = segment_data_size;
		}
		if( memory_copy(
		     &( segment_data[ segment_data_offset ] ),
		     &( data_handle->segment_data[ data_offset ] ),
		     read_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			return( -1 );
		}
		segment_data_size   -= read_size;
		segment_data_offset += read_size;
	}
	data_handle->current_segment_offset += segment_data_offset;

	return( (ssize_t) segment_data_offset );
}

/* Seeks a certain offset of the data
 * Callback for the data stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsapfs_compressed_data_handle_seek_segment_offset(
         libfsapfs_compressed_data_handle_t *data_handle,
         intptr_t *file_io_handle LIBFSAPFS_ATTRIBUTE_UNUSED,
         int segment_index,
         int segment_file_index LIBFSAPFS_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libfsapfs_compressed_data_handle_seek_segment_offset";

	LIBFSAPFS_UNREFERENCED_PARAMETER( file_io_handle )
	LIBFSAPFS_UNREFERENCED_PARAMETER( segment_file_index )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_index != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment offset value out of bounds.",
		 function );

		return( -1 );
	}
	data_handle->current_segment_offset = segment_offset;

	return( segment_offset );
}

