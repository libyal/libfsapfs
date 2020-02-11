/*
 * Password functions
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
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libhmac.h"
#include "libfsapfs_password.h"

/* Compute a PBKDF2-derived key from the given input.
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_password_pbkdf2(
     const uint8_t *password,
     size_t password_size,
     const uint8_t *salt,
     size_t salt_size,
     uint32_t number_of_iterations,
     uint8_t *output_data,
     size_t output_data_size,
     libcerror_error_t **error )
{
	uint8_t hash_buffer[ LIBHMAC_SHA256_HASH_SIZE ];

	uint8_t *data_buffer       = NULL;
	uint8_t *output_ptr        = NULL;
	static char *function      = "libfsapfs_password_pbkdf2";
	size_t block_offset        = 0;
	size_t data_buffer_size    = 0;
	size_t hash_size           = LIBHMAC_SHA256_HASH_SIZE;
	size_t remaining_data_size = 0;
	uint32_t block_index       = 0;
	uint32_t byte_index        = 0;
	uint32_t number_of_blocks  = 0;
	uint32_t password_iterator = 0;
	int result                 = 0;

	if( password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid password.",
		 function );

		return( -1 );
	}
	if( password_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid password size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( salt == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid salt.",
		 function );

		return( -1 );
	}
	if( salt_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid salt size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( number_of_iterations == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of iterations value zero or less.",
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
	if( memory_set(
	     output_data,
	     0,
	     output_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to clear output data.",
		 function );

		goto on_error;
	}
/* TODO add bounds check */
	number_of_blocks    = output_data_size / hash_size;
	remaining_data_size = output_data_size - ( number_of_blocks * hash_size );
	data_buffer_size    = salt_size + 4;

	data_buffer = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * data_buffer_size );

	if( data_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data buffer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     data_buffer,
	     0,
	     data_buffer_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to clear data buffer.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     data_buffer,
	     salt,
	     salt_size ) == NULL)
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy salt into data buffer.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: password:\n",
		 function );
		libcnotify_print_data(
		 password,
		 password_size,
		 0 );

		libcnotify_printf(
		 "%s: data buffer:\n",
		 function );
		libcnotify_print_data(
		 data_buffer,
		 data_buffer_size,
		 0 );

		libcnotify_printf(
		 "%s: number of iterations\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_iterations );

		libcnotify_printf(
		 "%s: number of blocks\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_blocks );

		libcnotify_printf(
		 "%s: remaining data size\t\t\t\t: %" PRIzd "\n",
		 function,
		 remaining_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif
	for( block_index = 0;
	     block_index < number_of_blocks;
	     block_index++ )
	{
		output_ptr = &( output_data[ block_offset ] );

		byte_stream_copy_from_uint32_big_endian(
		 &( data_buffer[ salt_size ] ),
		 block_index + 1 );

               	result = libhmac_sha256_calculate_hmac(
		          password,
		          password_size,
		          data_buffer,
		          data_buffer_size,
		          hash_buffer,
		          hash_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compute initial hmac for block %d.",
			 function,
			 block_index );

			goto on_error;
		}
		if( memory_copy(
		     output_ptr,
		     hash_buffer,
		     hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy hash buffer into output data.",
			 function );

			goto on_error;
		}
		for( password_iterator = 0;
		     password_iterator < number_of_iterations - 1;
		     password_iterator++ )
		{
			result = libhmac_sha256_calculate_hmac(
			          password,
			          password_size,
			          hash_buffer,
			          hash_size,
			          hash_buffer,
			          hash_size,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compute initial hmac for block %d.",
				 function,
				 block_index);

				goto on_error;
			}
			for( byte_index = 0;
			     byte_index < hash_size;
			     byte_index++ )
			{
				output_ptr[ byte_index ] ^= hash_buffer[ byte_index ];
			}
		}
		block_offset += hash_size;
	}
	if( remaining_data_size > 0 )
	{
		output_ptr = &( output_data[ block_offset ] );

		byte_stream_copy_from_uint32_big_endian(
		 &( data_buffer[ salt_size ] ),
		 block_index + 1 );

		result = libhmac_sha256_calculate_hmac(
		          password,
		          password_size,
		          data_buffer,
		          data_buffer_size,
		          hash_buffer,
		          hash_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compute initial hmac for block %d.",
			 function,
			 block_index);

			goto on_error;
		}
		if( memory_copy(
		     output_ptr,
		     hash_buffer,
		     remaining_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy hash buffer into output data in last block.",
			 function );

			goto on_error;
		}
		for( password_iterator = 0;
		     password_iterator < number_of_iterations - 1;
		     password_iterator++ )
		{
			result = libhmac_sha256_calculate_hmac(
			          password,
			          password_size,
			          hash_buffer,
			          hash_size,
			          hash_buffer,
			          hash_size,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compute initial hmac for block %d.",
				 function,
				 block_index );

				goto on_error;
			}
			for( byte_index = 0;
			     byte_index < remaining_data_size;
			     byte_index++ )
			{
				output_ptr[ byte_index ] ^= hash_buffer[ byte_index ];
			}
		}
	}
	if( data_buffer != NULL )
	{
		memory_free(
		 data_buffer );
	}
	return( 1 );

on_error:
	if( data_buffer != NULL )
	{
		memory_free(
		 data_buffer );
	}
	return( -1 );
}

