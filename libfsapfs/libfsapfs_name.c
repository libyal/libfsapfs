/*
 * Name functions
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

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libfsapfs_libcerror.h"
#include "libfsapfs_libuna.h"

/* Compares an UTF-8 string with a file entry name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsapfs_name_compare_with_utf8_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf8_character_t *utf8_string,
     size_t utf8_string_size,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	static char *function                             = "libfsapfs_name_compare_with_utf8_string";
	size_t name_index                                 = 0;
	size_t utf8_string_index                          = 0;
	libuna_unicode_character_t name_unicode_character = 0;
	libuna_unicode_character_t utf8_unicode_character = 0;

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 encoded name.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 encoded name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( name_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: missing UTF-8 encoded name value.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( name[ name_size - 1 ] == 0 )
	{
		name_size -= 1;
	}
	if( utf8_string[ utf8_string_size - 1 ] == 0 )
	{
		utf8_string_size -= 1;
	}
	while( ( name_index < name_size )
	    && ( utf8_string_index < utf8_string_size ) )
	{
		if( libuna_unicode_character_copy_from_utf8(
		     &name_unicode_character,
		     name,
		     name_size,
		     &name_index,
                     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from UTF-8 encoded name.",
			 function );

			return( -1 );
		}
		if( libuna_unicode_character_copy_from_utf8(
		     &utf8_unicode_character,
		     utf8_string,
		     utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_OUTPUT_FAILED,
			 "%s: unable to copy Unicode character from UTF-8 string.",
			 function );

			return( -1 );
		}
/* TODO add NFD support */
		if( use_case_folding != 0 )
		{
/* TODO add case folding */
			utf8_unicode_character = towlower( utf8_unicode_character );
			name_unicode_character = towlower( name_unicode_character );
		}
		if( utf8_unicode_character < name_unicode_character )
		{
			return( LIBUNA_COMPARE_LESS );
		}
		else if( utf8_unicode_character > name_unicode_character )
		{
			return( LIBUNA_COMPARE_GREATER );
		}
	}
	if( utf8_string_index < utf8_string_size )
	{
		return( LIBUNA_COMPARE_GREATER );
	}
	else if( name_index < name_size )
	{
		return( LIBUNA_COMPARE_LESS );
	}
	return( LIBUNA_COMPARE_EQUAL );
}

/* Compares an UTF-16 string with a file entry name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsapfs_name_compare_with_utf16_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf16_character_t *utf16_string,
     size_t utf16_string_size,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	static char *function                              = "libfsapfs_name_compare_with_utf16_string";
	size_t name_index                                  = 0;
	size_t utf16_string_index                          = 0;
	libuna_unicode_character_t name_unicode_character  = 0;
	libuna_unicode_character_t utf16_unicode_character = 0;

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 encoded name.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 encoded name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( name_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: missing UTF-8 encoded name value.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( name[ name_size - 1 ] == 0 )
	{
		name_size -= 1;
	}
	if( utf16_string[ utf16_string_size - 1 ] == 0 )
	{
		utf16_string_size -= 1;
	}
	while( ( name_index < name_size )
	    && ( utf16_string_index < utf16_string_size ) )
	{
		if( libuna_unicode_character_copy_from_utf8(
		     &name_unicode_character,
		     name,
		     name_size,
		     &name_index,
                     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from UTF-8 encoded name.",
			 function );

			return( -1 );
		}
		if( libuna_unicode_character_copy_from_utf16(
		     &utf16_unicode_character,
		     utf16_string,
		     utf16_string_size,
		     &utf16_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_OUTPUT_FAILED,
			 "%s: unable to copy Unicode character from UTF-16 string.",
			 function );

			return( -1 );
		}
/* TODO add NFD support */
		if( use_case_folding != 0 )
		{
/* TODO add case folding */
			name_unicode_character  = towlower( name_unicode_character );
			utf16_unicode_character = towlower( utf16_unicode_character );
		}
		if( utf16_unicode_character < name_unicode_character )
		{
			return( LIBUNA_COMPARE_LESS );
		}
		else if( utf16_unicode_character > name_unicode_character )
		{
			return( LIBUNA_COMPARE_GREATER );
		}
	}
	if( utf16_string_index < utf16_string_size )
	{
		return( LIBUNA_COMPARE_GREATER );
	}
	else if( name_index < name_size )
	{
		return( LIBUNA_COMPARE_LESS );
	}
	return( LIBUNA_COMPARE_EQUAL );
}

