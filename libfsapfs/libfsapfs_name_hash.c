/*
 * Name hash functions
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

#include "libfsapfs_checksum.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libuna.h"
#include "libfsapfs_name.h"

/* Calculates the name hash of an UTF-8 formatted string
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_name_hash_calculate_from_utf8_string(
     uint32_t *name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	libfsapfs_name_decomposition_mapping_t single_nfd_mapping = { 1, { 0 } };

	libfsapfs_name_decomposition_mapping_t *nfd_mapping       = NULL;
	static char *function                                     = "libfsapfs_name_hash_calculate_from_utf8_string";
	libuna_unicode_character_t unicode_character              = 0;
	size_t utf8_string_index                                  = 0;
	uint32_t calculated_checksum                              = 0;
	uint32_t checksum_table_index                             = 0;
	uint8_t byte_value                                        = 0;
	uint8_t nfd_character_index                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t utf32_stream[ 512 ];

	size_t utf32_stream_index                                 = 0;
#endif

	if( name_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name hash.",
		 function );

		return( -1 );
	}
	if( libfsapfs_checksum_crc32_table_computed == 0 )
	{
		libfsapfs_checksum_initialize_crc32_table(
		 0x82f63b78UL );
	}
	calculated_checksum = 0xffffffffUL;

	while( utf8_string_index < utf8_string_length )
	{
		if( libuna_unicode_character_copy_from_utf8(
		     &unicode_character,
		     utf8_string,
		     utf8_string_length,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from UTF-8 string.",
			 function );

			return( -1 );
		}
		if( unicode_character == 0 )
		{
			break;
		}
		if( use_case_folding != 0 )
		{
			libfsapfs_name_get_case_folding_mapping(
			 unicode_character );
		}
		libfsapfs_name_get_decomposition_mapping(
		 unicode_character,
		 nfd_mapping,
		 single_nfd_mapping );

		for( nfd_character_index = 0;
		     nfd_character_index < nfd_mapping->number_of_characters;
		     nfd_character_index++ )
		{
			unicode_character = nfd_mapping->characters[ nfd_character_index ];

			byte_value           = (uint8_t) ( unicode_character & 0x000000ffUL );
			checksum_table_index = ( calculated_checksum ^ (uint32_t) byte_value ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

#if defined( HAVE_DEBUG_OUTPUT )
			if( utf32_stream_index < 512 )
			{
				utf32_stream[ utf32_stream_index++ ] = byte_value;
			}
#endif

			byte_value           = (uint8_t) ( unicode_character & 0x000000ffUL );
			checksum_table_index = ( calculated_checksum ^ (uint32_t) byte_value ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

#if defined( HAVE_DEBUG_OUTPUT )
			if( utf32_stream_index < 512 )
			{
				utf32_stream[ utf32_stream_index++ ] = byte_value;
			}
#endif

			byte_value           = (uint8_t) ( unicode_character & 0x000000ffUL );
			checksum_table_index = ( calculated_checksum ^ (uint32_t) byte_value ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

#if defined( HAVE_DEBUG_OUTPUT )
			if( utf32_stream_index < 512 )
			{
				utf32_stream[ utf32_stream_index++ ] = byte_value;
			}
#endif

			byte_value           = (uint8_t) ( unicode_character & 0x000000ffUL );
			checksum_table_index = ( calculated_checksum ^ (uint32_t) byte_value ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );

#if defined( HAVE_DEBUG_OUTPUT )
			if( utf32_stream_index < 512 )
			{
				utf32_stream[ utf32_stream_index++ ] = byte_value;
			}
#endif
		}
	}
	*name_hash = calculated_checksum & 0x003fffffUL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: UTF-32 stream data:\n",
		 function );
		libcnotify_print_data(
		 utf32_stream,
		 utf32_stream_index,
		 0 );

		libcnotify_printf(
		 "%s: CRC-32 checkum\t\t: 0x%08" PRIx32 "\n",
		 function,
		 calculated_checksum );

		libcnotify_printf(
		 "%s: name hash\t\t: 0x%06" PRIx32 "\n",
		 function,
		 *name_hash );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Calculates the name hash of an UTF-16 formatted string
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_name_hash_calculate_from_utf16_string(
     uint32_t *name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	libfsapfs_name_decomposition_mapping_t single_nfd_mapping = { 1, { 0 } };

	libfsapfs_name_decomposition_mapping_t *nfd_mapping       = NULL;
	static char *function                                     = "libfsapfs_name_hash_calculate_from_utf16_string";
	libuna_unicode_character_t unicode_character              = 0;
	size_t utf16_string_index                                 = 0;
	uint32_t calculated_checksum                              = 0;
	uint32_t checksum_table_index                             = 0;
	uint8_t byte_value                                        = 0;
	uint8_t nfd_character_index                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t utf32_stream[ 512 ];

	size_t utf32_stream_index                                 = 0;
#endif

	if( name_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name hash.",
		 function );

		return( -1 );
	}
	if( libfsapfs_checksum_crc32_table_computed == 0 )
	{
		libfsapfs_checksum_initialize_crc32_table(
		 0x82f63b78UL );
	}
	calculated_checksum = 0xffffffffUL;

	while( utf16_string_index < utf16_string_length )
	{
		if( libuna_unicode_character_copy_from_utf16(
		     &unicode_character,
		     utf16_string,
		     utf16_string_length,
		     &utf16_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from UTF-16 string.",
			 function );

			return( -1 );
		}
		if( unicode_character == 0 )
		{
			break;
		}
		if( use_case_folding != 0 )
		{
			libfsapfs_name_get_case_folding_mapping(
			 unicode_character );
		}
		libfsapfs_name_get_decomposition_mapping(
		 unicode_character,
		 nfd_mapping,
		 single_nfd_mapping );

		for( nfd_character_index = 0;
		     nfd_character_index < nfd_mapping->number_of_characters;
		     nfd_character_index++ )
		{
			unicode_character = nfd_mapping->characters[ nfd_character_index ];

			byte_value           = (uint8_t) ( unicode_character & 0x000000ffUL );
			checksum_table_index = ( calculated_checksum ^ (uint32_t) byte_value ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

#if defined( HAVE_DEBUG_OUTPUT )
			if( utf32_stream_index < 512 )
			{
				utf32_stream[ utf32_stream_index++ ] = byte_value;
			}
#endif

			byte_value           = (uint8_t) ( unicode_character & 0x000000ffUL );
			checksum_table_index = ( calculated_checksum ^ (uint32_t) byte_value ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

#if defined( HAVE_DEBUG_OUTPUT )
			if( utf32_stream_index < 512 )
			{
				utf32_stream[ utf32_stream_index++ ] = byte_value;
			}
#endif

			byte_value           = (uint8_t) ( unicode_character & 0x000000ffUL );
			checksum_table_index = ( calculated_checksum ^ (uint32_t) byte_value ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

#if defined( HAVE_DEBUG_OUTPUT )
			if( utf32_stream_index < 512 )
			{
				utf32_stream[ utf32_stream_index++ ] = byte_value;
			}
#endif

			byte_value           = (uint8_t) ( unicode_character & 0x000000ffUL );
			checksum_table_index = ( calculated_checksum ^ (uint32_t) byte_value ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );

#if defined( HAVE_DEBUG_OUTPUT )
			if( utf32_stream_index < 512 )
			{
				utf32_stream[ utf32_stream_index++ ] = byte_value;
			}
#endif
		}
	}
	*name_hash = calculated_checksum & 0x003fffffUL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: UTF-32 stream data:\n",
		 function );
		libcnotify_print_data(
		 utf32_stream,
		 utf32_stream_index,
		 0 );

		libcnotify_printf(
		 "%s: CRC-32 checkum\t\t: 0x%08" PRIx32 "\n",
		 function,
		 calculated_checksum );

		libcnotify_printf(
		 "%s: name hash\t\t: 0x%06" PRIx32 "\n",
		 function,
		 *name_hash );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

