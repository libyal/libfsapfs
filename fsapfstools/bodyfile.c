/*
 * Bodyfile functions
 *
 * Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "bodyfile.h"
#include "fsapfstools_libcerror.h"
#include "fsapfstools_libfsapfs.h"
#include "fsapfstools_libuna.h"

/* Copies the path from a comparable file entry path
 * Returns 1 if successful or -1 on error
 */
int bodyfile_path_string_copy_from_file_entry_path(
     system_character_t **path,
     size_t *path_size,
     const system_character_t *file_entry_path,
     size_t file_entry_path_length,
     libcerror_error_t **error )
{
	system_character_t *safe_path                = NULL;
	static char *function                        = "bodyfile_path_string_copy_from_file_entry_path";
	libuna_unicode_character_t unicode_character = 0;
	system_character_t escape_character          = 0;
	size_t file_entry_path_index                 = 0;
	size_t path_index                            = 0;
	size_t safe_path_size                        = 0;
	int print_count                              = 0;
	int result                                   = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	if( path_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path size.",
		 function );

		return( -1 );
	}
	if( file_entry_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry path.",
		 function );

		return( -1 );
	}
	if( ( file_entry_path_length == 0 )
	 || ( file_entry_path_length > (size_t) ( SSIZE_MAX - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file entry path length value out of bounds.",
		 function );

		return( -1 );
	}
	/* Note that there is a worst-case of a 1 to 10 ratio for each escaped character.
	 */
	if( file_entry_path_length > (size_t) ( ( SSIZE_MAX - 1 ) / ( sizeof( system_character_t ) * 10 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	safe_path_size = ( file_entry_path_length * 10 ) + 1;

	safe_path = system_string_allocate(
	             safe_path_size );

	if( safe_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create path.",
		 function );

		goto on_error;
	}
	escape_character = (system_character_t) '\\';

	while( file_entry_path_index < file_entry_path_length )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_unicode_character_copy_from_utf16(
		          &unicode_character,
		          (libuna_utf16_character_t *) file_entry_path,
		          file_entry_path_length,
		          &file_entry_path_index,
		          error );
#else
		result = libuna_unicode_character_copy_from_utf8(
		          &unicode_character,
		          (libuna_utf8_character_t *) file_entry_path,
		          file_entry_path_length,
		          &file_entry_path_index,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from value string.",
			 function );

			goto on_error;
		}
		/* Replace by \x##:
		 *   Control characters ([U+0-U+1f, U+7f-U+9f])
		 */
		if( ( unicode_character <= 0x1f )
		 || ( ( unicode_character >= 0x7f )
		  &&  ( unicode_character <= 0x9f ) ) )
		{
			print_count = system_string_sprintf(
			               &( safe_path[ path_index ] ),
			               safe_path_size - path_index,
			               _SYSTEM_STRING( "%" PRIc_SYSTEM "x%02" PRIx32 "" ),
			               escape_character,
			               unicode_character );

			if( print_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy escaped Unicode character to path.",
				 function );

				goto on_error;
			}
			path_index += print_count;
		}
		/* Replace by \U########:
		 *   Unicode surrogate characters ([U+d800-U+dfff])
		 *   Undefined Unicode characters ([
		 *       U+fdd0-U+fddf, U+fffe-U+ffff, U+1fffe-U+1ffff, U+2fffe-U+2ffff,
		 *       U+3fffe-U+3ffff, U+4fffe-U+4ffff, U+5fffe-U+5ffff, U+6fffe-U+6ffff,
		 *       U+7fffe-U+7ffff, U+8fffe-U+8ffff, U+9fffe-U+9ffff, U+afffe-U+affff,
		 *       U+bfffe-U+bffff, U+cfffe-U+cffff, U+dfffe-U+dffff, U+efffe-U+effff,
		 *       U+ffffe-U+fffff, U+10fffe-U+ffffffff])
		 *   Observed unprintable characters ([
		 *       U+2028, U+2029, U+e000, U+f8ff, U+f0000, U+ffffd, U+100000, U+10fffd])
		 */
		else if( ( ( unicode_character >= 0x00002028UL )
		       &&  ( unicode_character <= 0x00002029UL ) )
		      || ( ( unicode_character >= 0x0000d800UL )
		       &&  ( unicode_character <= 0x0000dfffUL ) )
		      || ( unicode_character == 0x0000e000UL )
		      || ( unicode_character == 0x0000f8ffUL )
		      || ( ( unicode_character >= 0x0000fdd0UL )
		       &&  ( unicode_character <= 0x0000fddfUL ) )
		      || ( ( ( unicode_character & 0x0000ffffUL ) >= 0x0000fffeUL )
		       &&  ( ( unicode_character & 0x0000ffffUL ) <= 0x0000ffffUL ) )
		      || ( unicode_character == 0x000f0000UL )
		      || ( unicode_character == 0x000ffffdUL )
		      || ( unicode_character == 0x00100000UL )
		      || ( unicode_character >= 0x0010fffdUL ) )
		{
			print_count = system_string_sprintf(
			               &( safe_path[ path_index ] ),
			               safe_path_size - path_index,
			               _SYSTEM_STRING( "%" PRIc_SYSTEM "U%08" PRIx32 "" ),
			               escape_character,
			               unicode_character );

			if( print_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy escaped Unicode character to path.",
				 function );

				goto on_error;
			}
			path_index += print_count;
		}
		/* Replace:
		 *   Escape character (\) by \\
		 *   Bodyfile value seperator (|) by \|
		 */
                else if( ( unicode_character == (libuna_unicode_character_t) escape_character )
                      || ( unicode_character == '|' ) )
		{
			if( ( path_index + 2 ) > safe_path_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid path index value out of bounds.",
				 function );

				goto on_error;
			}
			safe_path[ path_index++ ] = escape_character;
			safe_path[ path_index++ ] = (system_character_t) unicode_character;
		}
		else
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_unicode_character_copy_to_utf16(
			          unicode_character,
			          (libuna_utf16_character_t *) safe_path,
			          safe_path_size,
			          &path_index,
			          error );
#else
			result = libuna_unicode_character_copy_to_utf8(
			          unicode_character,
			          (libuna_utf8_character_t *) safe_path,
			          safe_path_size,
			          &path_index,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy Unicode character to path.",
				 function );

				goto on_error;
			}
		}
	}
	safe_path[ path_index ] = 0;

	*path      = safe_path;
	*path_size = safe_path_size;

	return( 1 );

on_error:
	if( safe_path != NULL )
	{
		memory_free(
		 safe_path );
	}
	return( -1 );
}

