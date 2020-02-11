/*
 * Encryption functions
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

#include "libfsapfs_definitions.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_libcaes.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

/* Creates an encryption context
 * Make sure the value encryption context is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_encryption_context_initialize(
     libfsapfs_encryption_context_t **context,
     uint32_t method,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_encryption_context_initialize";

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid context value already set.",
		 function );

		return( -1 );
	}
	if( method != LIBFSAPFS_ENCRYPTION_METHOD_AES_128_XTS )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported method.",
		 function );

		return( -1 );
	}
	*context = memory_allocate_structure(
	            libfsapfs_encryption_context_t );

	if( *context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *context,
	     0,
	     sizeof( libfsapfs_encryption_context_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		memory_free(
		 *context );

		*context = NULL;

		return( -1 );
	}
	if( libcaes_tweaked_context_initialize(
	     &( ( *context )->decryption_context ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize decryption context.",
		 function );

		goto on_error;
	}
	( *context )->method = method;

	return( 1 );

on_error:
	if( *context != NULL )
	{
		if( ( *context )->decryption_context != NULL )
		{
			libcaes_tweaked_context_free(
			 &( ( *context )->decryption_context ),
			 NULL );
		}
		memory_free(
		 *context );

		*context = NULL;
	}
	return( -1 );
}

/* Frees an encryption context
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_encryption_context_free(
     libfsapfs_encryption_context_t **context,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_encryption_context_free";
	int result            = 1;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		if( libcaes_tweaked_context_free(
		     &( ( *context )->decryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable free decryption context.",
			 function );

			result = -1;
		}
		memory_free(
		 *context );

		*context = NULL;
	}
	return( result );
}

/* Sets the de- and encryption keys
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_encryption_context_set_keys(
     libfsapfs_encryption_context_t *context,
     const uint8_t *key,
     size_t key_size,
     const uint8_t *tweak_key,
     size_t tweak_key_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_encryption_context_set_keys";
	size_t key_bit_size   = 0;
	size_t key_byte_size  = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( tweak_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweak key.",
		 function );

		return( -1 );
	}
	if( tweak_key_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid tweak key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( context->method == LIBFSAPFS_ENCRYPTION_METHOD_AES_128_XTS )
	{
		key_byte_size = 16;
	}
	if( key_size < key_byte_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid key value too small.",
		 function );

		return( -1 );
	}
	if( tweak_key_size < key_byte_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid tweak key value too small.",
		 function );

		return( -1 );
	}
	key_bit_size = key_byte_size * 8;

	if( libcaes_tweaked_context_set_keys(
	     context->decryption_context,
	     LIBCAES_CRYPT_MODE_DECRYPT,
	     key,
	     key_bit_size,
	     tweak_key,
	     key_bit_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set keys in decryption context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* De- or encrypts a block of data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_encryption_context_crypt(
     libfsapfs_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t sector_number,
     uint16_t bytes_per_sector,
     libcerror_error_t **error )
{
	uint8_t tweak_value[ 16 ];

	static char *function = "libfsapfs_encryption_context_crypt";
	size_t data_offset    = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( mode != LIBFSAPFS_ENCRYPTION_CRYPT_MODE_DECRYPT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( input_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input data.",
		 function );

		return( -1 );
	}
	if( input_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( output_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output data.",
		 function );

		return( -1 );
	}
	if( output_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid output data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( output_data_size < input_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid output data size value smaller than input data size.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     tweak_value,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear tweak value.",
		 function );

		goto on_error;
	}
	for( data_offset = 0;
	     data_offset < input_data_size;
	     data_offset += bytes_per_sector )
	{
		byte_stream_copy_from_uint64_little_endian(
		 tweak_value,
		 sector_number );

		if( libcaes_crypt_xts(
		     context->decryption_context,
		     LIBCAES_CRYPT_MODE_DECRYPT,
		     tweak_value,
		     16,
		     &( input_data[ data_offset ] ),
		     bytes_per_sector,
		     &( output_data[ data_offset ] ),
		     bytes_per_sector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt data.",
			 function );

			goto on_error;
		}
		sector_number += 1;
	}
	return( 1 );

on_error:
	memory_set(
	 tweak_value,
	 0,
	 16 );

	return( -1 );
}

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

