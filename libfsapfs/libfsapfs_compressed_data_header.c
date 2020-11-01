/*
 * The compressed data header functions
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

#include "libfsapfs_compressed_data_header.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_compressed_data.h"

/* Creates compressed data header
 * Make sure the value compressed_data_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_compressed_data_header_initialize(
     libfsapfs_compressed_data_header_t **compressed_data_header,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_compressed_data_header_initialize";

	if( compressed_data_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data header.",
		 function );

		return( -1 );
	}
	if( *compressed_data_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid compressed data header value already set.",
		 function );

		return( -1 );
	}
	*compressed_data_header = memory_allocate_structure(
	                           libfsapfs_compressed_data_header_t );

	if( *compressed_data_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed data header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *compressed_data_header,
	     0,
	     sizeof( libfsapfs_compressed_data_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear compressed data header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *compressed_data_header != NULL )
	{
		memory_free(
		 *compressed_data_header );

		*compressed_data_header = NULL;
	}
	return( -1 );
}

/* Frees compressed data header
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_compressed_data_header_free(
     libfsapfs_compressed_data_header_t **compressed_data_header,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_compressed_data_header_free";

	if( compressed_data_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data header.",
		 function );

		return( -1 );
	}
	if( *compressed_data_header != NULL )
	{
		memory_free(
		 *compressed_data_header );

		*compressed_data_header = NULL;
	}
	return( 1 );
}

/* Reads the compressed data header
 * Returns 1 if successful, 0 if the signature does not match or -1 on error
 */
int libfsapfs_compressed_data_header_read_data(
     libfsapfs_compressed_data_header_t *compressed_data_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_compressed_data_header_read_data";

	if( compressed_data_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data header.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsapfs_compressed_data_header_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: compressed data header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_compressed_data_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (fsapfs_compressed_data_header_t *) data )->signature,
	     "fpmc",
	     4 ) != 0 )
	{
		return( 0 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_compressed_data_header_t *) data )->compression_method,
	 compressed_data_header->compression_method );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_compressed_data_header_t *) data )->uncompressed_data_size,
	 compressed_data_header->uncompressed_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsapfs_compressed_data_header_t *) data )->signature[ 0 ],
		 ( (fsapfs_compressed_data_header_t *) data )->signature[ 1 ],
		 ( (fsapfs_compressed_data_header_t *) data )->signature[ 2 ],
		 ( (fsapfs_compressed_data_header_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: compression method\t\t: %" PRIu32 "\n",
		 function,
		 compressed_data_header->compression_method );

		libcnotify_printf(
		 "%s: uncompressed data size\t: %" PRIu64 "\n",
		 function,
		 compressed_data_header->uncompressed_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

