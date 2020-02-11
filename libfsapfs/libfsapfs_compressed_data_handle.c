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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_compressed_data_handle.h"
#include "libfsapfs_compression.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_libbfio.h"
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
	if( ( compression_method != LIBFSAPFS_COMPRESSION_METHOD_DEFLATE )
	 && ( compression_method != LIBFSAPFS_COMPRESSION_METHOD_LZVN )
	 && ( compression_method != LIBFSAPFS_COMPRESSION_METHOD_UNKNOWN5 ) )
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
	                                                         sizeof( uint8_t ) * ( LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE + 1 ) );

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
	if( compression_method != LIBFSAPFS_COMPRESSION_METHOD_UNKNOWN5 )
	{
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
		if( ( *data_handle )->compressed_block_offsets != NULL )
		{
			memory_free(
			 ( *data_handle )->compressed_block_offsets );
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( 1 );
}

/* Determines the compressed block offsets
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_compressed_data_handle_get_compressed_block_offsets(
     libfsapfs_compressed_data_handle_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function                     = "libfsapfs_compressed_data_handle_get_compressed_block_offsets";
	size64_t compressed_data_size             = 0;
	size_t compressed_block_descriptor_size   = 0;
	size_t read_size                          = 0;
	size_t segment_data_offset                = 0;
	ssize_t read_count                        = 0;
	uint32_t compressed_block_index           = 0;
	uint32_t compressed_block_offset          = 0;
	uint32_t compressed_descriptors_offset    = 0;
	uint32_t compressed_footer_offset         = 0;
	uint32_t compressed_footer_size           = 0;
	uint32_t previous_compressed_block_offset = 0;
	int compare_result                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                      = 0;
#endif

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
	if( data_handle->compressed_block_offsets != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle - compressed block offsets value already set.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_size(
	     data_handle->compressed_data_stream,
	     &compressed_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compressed data size.",
		 function );

		goto on_error;
	}
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

		goto on_error;
	}
	compare_result = memory_compare(
	                  data_handle->compressed_segment_data,
	                  "fpmc",
	                  4 );

	if( compare_result == 0 )
	{
		if( compressed_data_size > (size64_t) ( LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid segment data size value out of bounds.",
			 function );

			goto on_error;
		}
		data_handle->number_of_compressed_blocks = 1;
	}
	else if( data_handle->compression_method == LIBFSAPFS_COMPRESSION_METHOD_DEFLATE )
	{
		byte_stream_copy_to_uint32_big_endian(
		 data_handle->compressed_segment_data,
		 compressed_descriptors_offset );

		if( compressed_descriptors_offset != 0x00000100UL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed descriptors offset value out of bounds.",
			 function );

			goto on_error;
		}
		read_size = (size_t) compressed_descriptors_offset + 16 - 4;

		read_count = libfdata_stream_read_buffer_at_offset(
		              data_handle->compressed_data_stream,
		              (intptr_t *) file_io_handle,
		              &( data_handle->compressed_segment_data[ 4 ] ),
		              read_size,
		              4,
		              0,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read compressed header data at offset: 4 (0x00000004) from data stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed header data:\n",
			 function );
			libcnotify_print_data(
			 data_handle->compressed_segment_data,
			 read_size - 4,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed descriptors offset\t: 0x%08" PRIx32 "\n",
			 function,
			 compressed_descriptors_offset );

			byte_stream_copy_to_uint32_big_endian(
			 &( data_handle->compressed_segment_data[ 4 ] ),
			 compressed_footer_offset );
			libcnotify_printf(
			 "%s: compressed footer offset\t\t: 0x%08" PRIx32 "\n",
			 function,
			 compressed_footer_offset );

			byte_stream_copy_to_uint32_big_endian(
			 &( data_handle->compressed_segment_data[ 8 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: compressed data size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_big_endian(
			 &( data_handle->compressed_segment_data[ 12 ] ),
			 compressed_footer_size );
			libcnotify_printf(
			 "%s: compressed footer size\t\t: %" PRIu32 "\n",
			 function,
			 compressed_footer_size );

			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 &( data_handle->compressed_segment_data[ 16 ] ),
			 240,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			byte_stream_copy_to_uint32_little_endian(
			 &( data_handle->compressed_segment_data[ 256 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: compressed data size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		byte_stream_copy_to_uint32_little_endian(
		 &( data_handle->compressed_segment_data[ 260 ] ),
		 data_handle->number_of_compressed_blocks );

		if( data_handle->number_of_compressed_blocks > ( (uint32_t) UINT32_MAX / 8 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of compressed blocks value out of bounds.",
			 function );

			goto on_error;
		}
		segment_data_offset = 264;

		byte_stream_copy_to_uint32_little_endian(
		 &( data_handle->compressed_segment_data[ segment_data_offset ] ),
		 compressed_block_offset );

		segment_data_offset += 4;

		compressed_descriptors_offset   += 4;
		compressed_block_descriptor_size = 8;
	}
	else if( data_handle->compression_method == LIBFSAPFS_COMPRESSION_METHOD_LZVN )
	{
		segment_data_offset = 0;

		byte_stream_copy_to_uint32_little_endian(
		 &( data_handle->compressed_segment_data[ segment_data_offset ] ),
		 compressed_block_offset );

		segment_data_offset += 4;

		compressed_block_descriptor_size = 4;

		if( ( compressed_block_offset <= 0x00000004UL )
		 || ( compressed_block_offset >= ( LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE + 1 ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of compressed blocks value out of bounds.",
			 function );

			goto on_error;
		}
		data_handle->number_of_compressed_blocks = compressed_block_offset / 4;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of compressed blocks\t: %" PRIu32 "\n",
		 function,
		 data_handle->number_of_compressed_blocks );
	}
#endif
#if ( SIZEOF_SIZE_T <= 4 )
	if( (size_t) data_handle->number_of_compressed_blocks > ( (size_t) ( SSIZE_MAX / 4 ) - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of compressed blocks value exceeds maximum.",
		 function );

		goto on_error;
	}
#endif
	data_handle->compressed_block_offsets = (uint32_t *) memory_allocate(
	                                                      sizeof( uint32_t ) * ( data_handle->number_of_compressed_blocks + 1 ) );

	if( data_handle->compressed_block_offsets == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed block offsets.",
		 function );

		goto on_error;
	}
	if( compare_result == 0 )
	{
		data_handle->compressed_block_offsets[ compressed_block_index++ ] = 16;
		previous_compressed_block_offset                                  = 16;
	}
	else
	{
		read_size = ( (size_t) data_handle->number_of_compressed_blocks - 1 ) * compressed_block_descriptor_size;

		read_count = libfdata_stream_read_buffer_at_offset(
		              data_handle->compressed_data_stream,
		              (intptr_t *) file_io_handle,
		              &( data_handle->compressed_segment_data[ segment_data_offset ] ),
		              read_size,
		              segment_data_offset,
		              0,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read compressed block descriptors data at offset: %" PRIzd " (0x%08" PRIzx ") from data stream.",
			 function,
			 segment_data_offset,
			 segment_data_offset );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed block descriptors data:\n",
			 function );
			libcnotify_print_data(
			 &( data_handle->compressed_segment_data[ segment_data_offset - compressed_block_descriptor_size ] ),
			 read_size + compressed_block_descriptor_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed block: % 2" PRIu32 " offset\t: 0x%08" PRIx32 " (0x%08" PRIx32 ")\n",
			 function,
			 compressed_block_index,
			 compressed_block_offset,
			 compressed_block_offset + compressed_descriptors_offset );
		}
#endif
		if( ( compressed_block_offset <= compressed_block_descriptor_size )
		 || ( compressed_block_offset >= ( LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE + 1 ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed block offset: %" PRIu32 " (0x%08" PRIx32 ") value out of bounds.",
			 function,
			 compressed_block_offset,
			 compressed_block_offset );

			goto on_error;
		}
		compressed_block_offset += compressed_descriptors_offset;

		data_handle->compressed_block_offsets[ 0 ] = compressed_block_offset;
		previous_compressed_block_offset           = compressed_block_offset;

		if( data_handle->compression_method == LIBFSAPFS_COMPRESSION_METHOD_DEFLATE )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 &( data_handle->compressed_segment_data[ segment_data_offset ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: compressed block: % 2" PRIu32 " size\t: %" PRIu32 "\n",
				 function,
				 compressed_block_index,
				 value_32bit );
			}
#endif
			segment_data_offset += 4;
		}
		for( compressed_block_index = 1;
		     compressed_block_index < data_handle->number_of_compressed_blocks;
		     compressed_block_index++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( data_handle->compressed_segment_data[ segment_data_offset ] ),
			 compressed_block_offset );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: compressed block: % 2" PRIu32 " offset\t: 0x%08" PRIx32 " (0x%08" PRIx32 ")\n",
				 function,
				 compressed_block_index,
				 compressed_block_offset,
				 compressed_block_offset + compressed_descriptors_offset );
			}
#endif
			segment_data_offset += 4;

			compressed_block_offset += compressed_descriptors_offset;

			if( ( previous_compressed_block_offset > compressed_block_offset )
			 || ( ( compressed_block_offset - previous_compressed_block_offset ) > (uint32_t) ( LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE + 1 ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid compressed block offset: %" PRIu32 " (0x%08" PRIx32 ") value out of bounds.",
				 function,
				 compressed_block_offset,
				 compressed_block_offset );

				goto on_error;
			}
			data_handle->compressed_block_offsets[ compressed_block_index ] = compressed_block_offset;
			previous_compressed_block_offset                                = compressed_block_offset;

			if( data_handle->compression_method == LIBFSAPFS_COMPRESSION_METHOD_DEFLATE )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					byte_stream_copy_to_uint32_little_endian(
					 &( data_handle->compressed_segment_data[ segment_data_offset ] ),
					 value_32bit );
					libcnotify_printf(
					 "%s: compressed block: % 2" PRIu32 " size\t: %" PRIu32 "\n",
					 function,
					 compressed_block_index,
					 value_32bit );
				}
#endif
				segment_data_offset += 4;
			}
		}
	}
	if( ( previous_compressed_block_offset > compressed_data_size )
	 || ( ( compressed_data_size - previous_compressed_block_offset ) > (uint32_t) ( LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE + 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compressed block offset: %" PRIu32 " (0x%08" PRIx32 ") value out of bounds.",
		 function,
		 previous_compressed_block_offset,
		 previous_compressed_block_offset );

		goto on_error;
	}
	data_handle->compressed_block_offsets[ compressed_block_index ] = (uint32_t) compressed_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( data_handle->compression_method == LIBFSAPFS_COMPRESSION_METHOD_DEFLATE )
	{
		if( compressed_footer_size > (uint32_t) ( LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed footer size value out of bounds.",
			 function );

			goto on_error;
		}
		read_count = libfdata_stream_read_buffer_at_offset(
		              data_handle->compressed_data_stream,
		              (intptr_t *) file_io_handle,
		              data_handle->compressed_segment_data,
		              (size_t) compressed_footer_size,
		              (off64_t) compressed_footer_offset,
		              0,
		              error );

		if( read_count != (ssize_t) compressed_footer_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read compressed footer data at offset: %" PRIu32 " (0x08%" PRIx32 ") from data stream.",
			 function,
			 compressed_footer_offset,
			 compressed_footer_offset );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed footer data:\n",
			 function );
			libcnotify_print_data(
			 data_handle->compressed_segment_data,
			 (size_t) compressed_footer_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	}
	else if( data_handle->compression_method == LIBFSAPFS_COMPRESSION_METHOD_UNKNOWN5 )
	{
		if( compressed_data_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed data size value out of bounds.",
			 function );

			goto on_error;
		}
		read_size = (size_t) compressed_data_size - 16;

		read_count = libfdata_stream_read_buffer_at_offset(
		              data_handle->compressed_data_stream,
		              (intptr_t *) file_io_handle,
		              data_handle->compressed_segment_data,
		              read_size,
		              16,
		              0,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read unknown data at offset: 16 (0x00000010) from data stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unknown data:\n",
			 function );
			libcnotify_print_data(
			 data_handle->compressed_segment_data,
			 read_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	}
	return( 1 );

on_error:
	if( data_handle->compressed_block_offsets != NULL )
	{
		memory_free(
		 data_handle->compressed_block_offsets );

		data_handle->compressed_block_offsets = NULL;
	}
	return( -1 );
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
	static char *function             = "libfsapfs_compressed_data_handle_read_segment_data";
	size_t data_offset                = 0;
	size_t read_size                  = 0;
	size_t segment_data_offset        = 0;
	ssize_t read_count                = 0;
	off64_t data_stream_offset        = 0;
	off64_t uncompressed_block_offset = 0;
	uint32_t compressed_block_index   = 0;

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
		if( libfsapfs_compressed_data_handle_get_compressed_block_offsets(
		     data_handle,
		     (libbfio_handle_t *) file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine compressed block offsets.",
			 function );

			return( -1 );
		}
	}
	if( (size64_t) data_handle->current_segment_offset >= data_handle->uncompressed_data_size )
	{
		return( 0 );
	}
	if( data_handle->compression_method == LIBFSAPFS_COMPRESSION_METHOD_UNKNOWN5 )
	{
		if( (size64_t) segment_data_size > ( data_handle->uncompressed_data_size - data_handle->current_segment_offset ) )
		{
			read_size = (size_t) ( data_handle->uncompressed_data_size - data_handle->current_segment_offset );
		}
		else
		{
			read_size = segment_data_size;
		}
		if( memory_set(
		     segment_data,
		     0,
		     read_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segment data.",
			 function );

			return( -1 );
		}
		data_handle->current_segment_offset += read_size;

		return( (ssize_t) read_size );
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

			if( read_count != (ssize_t) read_size )
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
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: compressed block data:\n",
				 function );
				libcnotify_print_data(
				 data_handle->compressed_segment_data,
				 read_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
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
				 "%s: uncompressed block data:\n",
				 function );
				libcnotify_print_data(
				 data_handle->segment_data,
				 data_handle->segment_data_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			uncompressed_block_offset = ( compressed_block_index + 1 ) * LIBFSAPFS_COMPRESSED_DATA_HANDLE_BLOCK_SIZE;

			if( ( (size64_t) uncompressed_block_offset < data_handle->uncompressed_data_size )
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
		data_offset          = 0;
		segment_data_size   -= read_size;
		segment_data_offset += read_size;

		compressed_block_index++;
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

