/*
 * Encryption functions
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

#include "libfsapfs_encryption.h"
#include "libfsapfs_libcaes.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

/* Unwrap data using AES Key Wrap (RFC3394)
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_encryption_aes_key_unwrap(
     const uint8_t *key,
     size_t key_bit_size,
     const uint8_t *wrapped_data,
     size_t wrapped_data_size,
     uint8_t *unwrapped_data,
     size_t unwrapped_data_size,
     libcerror_error_t **error )
{
	uint8_t block_data[ 16 ];
	uint8_t vector_data[ 8 ];

	libcaes_context_t *aes_context = NULL;
	uint8_t *initialization_vector = NULL;
	static char *function          = "libfsapfs_encryption_aes_key_unwrap";
	size_t block_offset            = 0;
	size_t number_of_blocks        = 0;
	size_t block_index             = 0;
	int8_t round_index             = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid AES key.",
		 function);

		return( -1 );
	}
	if( ( key_bit_size != 128 )
	 && ( key_bit_size != 192 )
	 && ( key_bit_size != 256 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid AES key length value out of bounds.",
		 function);

		return( -1 );
	}
	if( wrapped_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wrapped data.",
		 function);

		return( -1 );
	}
	if( wrapped_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid wrapped data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( wrapped_data_size <= 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid wrapped data size value too small.",
		 function);

		return( -1 );
	}
	if( ( wrapped_data_size % 8 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid wrapped data size value not a multitude of 8.",
		 function);

		return( -1 );
	}
	if( unwrapped_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unwrapped data.",
		 function);

		return( -1 );
	}
	if( unwrapped_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid unwrapped data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( unwrapped_data_size < wrapped_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid unwrapped data size value too small.",
		 function);

		return( -1 );
	}
	if( libcaes_context_initialize(
	     &aes_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize AES context.",
		 function );

		goto on_error;
	}
	if( libcaes_context_set_key(
	     aes_context,
	     LIBCAES_CRYPT_MODE_DECRYPT,
	     key,
	     key_bit_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set key in AES context.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     unwrapped_data,
	     wrapped_data,
	     wrapped_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy wrapped data.",
		 function );

		goto on_error;
	}
	number_of_blocks = wrapped_data_size / 8;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of blocks: %" PRIzd "\n",
		 function,
		 number_of_blocks );

		libcnotify_printf(
		 "%s: wrapped data:\n",
		 function );
		libcnotify_print_data(
		 wrapped_data,
		 wrapped_data_size,
		 0 );
	}
#endif
/* TODO make this code more readable */
	initialization_vector = unwrapped_data;

	for( round_index = 5;
	     round_index >= 0;
	     round_index-- )
	{
		for( block_index = number_of_blocks - 1;
		     block_index > 0;
		     block_index-- )
		{
			block_offset = block_index * 8;

			byte_stream_copy_from_uint64_big_endian(
			 vector_data,
			 (uint64_t) ( round_index * ( number_of_blocks - 1 ) + block_index ) );

			vector_data[ 0 ] ^= initialization_vector[ 0 ];
			vector_data[ 1 ] ^= initialization_vector[ 1 ];
			vector_data[ 2 ] ^= initialization_vector[ 2 ];
			vector_data[ 3 ] ^= initialization_vector[ 3 ];
			vector_data[ 4 ] ^= initialization_vector[ 4 ];
			vector_data[ 5 ] ^= initialization_vector[ 5 ];
			vector_data[ 6 ] ^= initialization_vector[ 6 ];
			vector_data[ 7 ] ^= initialization_vector[ 7 ];

			if( memory_copy(
			     &( block_data[ 0 ] ),
			     vector_data,
			     8 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy vector data to block data.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     &( block_data[ 8 ] ),
			     &( unwrapped_data[ block_offset ] ),
			     8 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy unwrapped data to block data.",
				 function );

				goto on_error;
			}
			if( libcaes_crypt_ecb(
			     aes_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     block_data,
			     16,
			     block_data,
			     16,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
				 "%s: unable to decrypt block data.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     initialization_vector,
			     &( block_data[ 0 ] ),
			     8 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to decrypted block data to initialization vector.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     &( unwrapped_data[ block_offset ] ),
			     &( block_data[ 8 ] ),
			     8 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy decrypted block data to unwrapped data.",
				 function );

				goto on_error;
			}
		}
	}
	if( libcaes_context_free(
	     &aes_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free AES context.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unwrapped data:\n",
		 function );
		libcnotify_print_data(
		 unwrapped_data,
		 wrapped_data_size,
		 0 );
	}
#endif
	return( 1 );

on_error:
	if( aes_context != NULL )
	{
		libcaes_context_free(
		 &aes_context,
		 NULL);
	}
	return( -1 );
}

