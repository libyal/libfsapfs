/*
 * Compression functions
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_ZLIB ) || defined( ZLIB_DLL )
#include <zlib.h>
#endif

#include "libfsapfs_compression.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_deflate.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_lzvn.h"

/* Decompresses data using the compression method
 * Returns 1 on success, 0 on failure or -1 on error
 */
int libfsapfs_decompress_data(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     int compression_method,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function              = "libfsapfs_decompress_data";
	int result                         = 0;

#if ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_UNCOMPRESS ) ) || defined( ZLIB_DLL )
	uLongf zlib_uncompressed_data_size = 0;
#endif

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == compressed_data )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data buffer equals uncompressed data buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	if( compression_method == LIBFSAPFS_COMPRESSION_METHOD_DEFLATE )
	{
		if( ( compressed_data_size >= 1 )
		 && ( compressed_data[ 0 ] == 0xff ) )
		{
			if( compressed_data_size > (size_t) SSIZE_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid compressed data size value exceeds maximum.",
				 function );

				return( -1 );
			}
			if( *uncompressed_data_size > (size_t) SSIZE_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid uncompressed data size value exceeds maximum.",
				 function );

				return( -1 );
			}
			if( ( compressed_data_size - 1 ) > *uncompressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compressed data size value exceeds uncompressed data size.",
				 function );

				return( -1 );
			}
			*uncompressed_data_size = compressed_data_size - 1;

			if( memory_copy(
			     uncompressed_data,
			     &( compressed_data[ 1 ] ),
			     *uncompressed_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to compressed to uncompressed data.",
				 function );

				return( -1 );
			}
			result = 1;
		}
		else
		{
#if ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_UNCOMPRESS ) ) || defined( ZLIB_DLL )
			if( compressed_data_size > (size_t) ULONG_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid compressed data size value exceeds maximum.",
				 function );

				return( -1 );
			}
			if( *uncompressed_data_size > (size_t) ULONG_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid uncompressed data size value exceeds maximum.",
				 function );

				return( -1 );
			}
			zlib_uncompressed_data_size = (uLongf) *uncompressed_data_size;

			result = uncompress(
				  (Bytef *) uncompressed_data,
				  &zlib_uncompressed_data_size,
				  (Bytef *) compressed_data,
				  (uLong) compressed_data_size );

			if( result == Z_OK )
			{
				*uncompressed_data_size = (size_t) zlib_uncompressed_data_size;

				result = 1;
			}
			else if( result == Z_DATA_ERROR )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unable to read compressed data: data error.\n",
					 function );
				}
#endif
				*uncompressed_data_size = 0;

				result = -1;
			}
			else if( result == Z_BUF_ERROR )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					"%s: unable to read compressed data: target buffer too small.\n",
					 function );
				}
#endif
				/* Estimate that a factor 2 enlargement should suffice
				 */
				*uncompressed_data_size *= 2;

				result = 0;
			}
			else if( result == Z_MEM_ERROR )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to read compressed data: insufficient memory.",
				 function );

				*uncompressed_data_size = 0;

				result = -1;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
				 "%s: zlib returned undefined error: %d.",
				 function,
				 result );

				*uncompressed_data_size = 0;

				result = -1;
			}
#else
			result = libfsapfs_deflate_decompress_zlib(
			          compressed_data,
			          compressed_data_size,
			          uncompressed_data,
			          uncompressed_data_size,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to decompress DEFLATE compressed data.",
				 function );

				return( -1 );
			}
#endif /* ( defined( HAVE_ZLIB ) && defined( HAVE_ZLIB_UNCOMPRESS ) ) || defined( ZLIB_DLL ) */
		}
	}
	else if( compression_method == LIBFSAPFS_COMPRESSION_METHOD_LZVN )
	{
		if( ( compressed_data_size >= 1 )
		 && ( compressed_data[ 0 ] == 0x06 ) )
		{
			if( compressed_data_size > (size_t) SSIZE_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid compressed data size value exceeds maximum.",
				 function );

				return( -1 );
			}
			if( *uncompressed_data_size > (size_t) SSIZE_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid uncompressed data size value exceeds maximum.",
				 function );

				return( -1 );
			}
			if( ( compressed_data_size - 1 ) > *uncompressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compressed data size value exceeds uncompressed data size.",
				 function );

				return( -1 );
			}
			*uncompressed_data_size = compressed_data_size - 1;

			if( memory_copy(
			     uncompressed_data,
			     &( compressed_data[ 1 ] ),
			     *uncompressed_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to compressed to uncompressed data.",
				 function );

				return( -1 );
			}
			result = 1;
		}
		else
		{
			result = libfsapfs_lzvn_decompress(
			          compressed_data,
			          compressed_data_size,
			          uncompressed_data,
			          uncompressed_data_size,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to decompress LZVN compressed data.",
				 function );

				return( -1 );
			}
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method.",
		 function );

		return( -1 );
	}
	return( result );
}

