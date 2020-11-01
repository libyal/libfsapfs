/*
 * Crypographic digest hash
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
#include <types.h>

#include "digest_hash.h"
#include "fsapfstools_libcerror.h"

/* Converts the digest hash to a printable string
 * Returns 1 if successful or -1 on error
 */
int digest_hash_copy_to_string(
     const uint8_t *digest_hash,
     size_t digest_hash_size,
     char *string,
     size_t string_size,
     libcerror_error_t **error )
{
	static char *function       = "digest_hash_copy_to_string";
	size_t string_iterator      = 0;
	size_t digest_hash_iterator = 0;
	uint8_t digest_digit        = 0;

	if( digest_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid digest hash.",
		 function );

		return( -1 );
	}
	if( digest_hash_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid digest hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* The string requires space for 2 characters per digest hash digit and a end of string
	 */
	if( string_size < ( ( 2 * digest_hash_size ) + 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: string too small.",
		 function );

		return( -1 );
	}
	for( digest_hash_iterator = 0;
	     digest_hash_iterator < digest_hash_size;
	     digest_hash_iterator++ )
	{
		digest_digit = digest_hash[ digest_hash_iterator ] / 16;

		if( digest_digit <= 9 )
		{
			string[ string_iterator++ ] = (char) ( (uint8_t) '0' + digest_digit );
		}
		else
		{
			string[ string_iterator++ ] = (char) ( (uint8_t) 'a' + ( digest_digit - 10 ) );
		}
		digest_digit = digest_hash[ digest_hash_iterator ] % 16;

		if( digest_digit <= 9 )
		{
			string[ string_iterator++ ] = (char) ( (uint8_t) '0' + digest_digit );
		}
		else
		{
			string[ string_iterator++ ] = (char) ( (uint8_t) 'a' + ( digest_digit - 10 ) );
		}
	}
	string[ string_iterator ] = 0;

	return( 1 );
}

