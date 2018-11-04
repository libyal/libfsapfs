/*
 * Name hash functions
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

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
	uint8_t nfd_character_index                               = 0;

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
/* TODO add case folding */
			unicode_character = towlower( unicode_character );
		}
		if( ( unicode_character >= 0x000000c0UL )
		 && ( unicode_character <= 0x0000017fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000000c0[ unicode_character - 0x000000c0UL ] );
		}
		else if( ( unicode_character >= 0x000001a0UL )
		      && ( unicode_character <= 0x00000237UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000001a0[ unicode_character - 0x000001a0UL ] );
		}
		else if( ( unicode_character >= 0x00000340UL )
		      && ( unicode_character <= 0x000003d7UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000340[ unicode_character - 0x00000340UL ] );
		}
		else if( ( unicode_character >= 0x00000400UL )
		      && ( unicode_character <= 0x000004ffUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000400[ unicode_character - 0x00000400UL ] );
		}
		else if( ( unicode_character >= 0x00000620UL )
		      && ( unicode_character <= 0x000006d7UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000620[ unicode_character - 0x00000620UL ] );
		}
		else if( ( unicode_character >= 0x00000928UL )
		      && ( unicode_character <= 0x000009dfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000928[ unicode_character - 0x00000928UL ] );
		}
		else if( ( unicode_character >= 0x00000a30UL )
		      && ( unicode_character <= 0x00000a5fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000a30[ unicode_character - 0x00000a30UL ] );
		}
		else if( ( unicode_character >= 0x00000b48UL )
		      && ( unicode_character <= 0x00000bcfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000b48[ unicode_character - 0x00000b48UL ] );
		}
		else if( ( unicode_character >= 0x00000c48UL )
		      && ( unicode_character <= 0x00000ccfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000c48[ unicode_character - 0x00000c48UL ] );
		}
		else if( ( unicode_character >= 0x00000d48UL )
		      && ( unicode_character <= 0x00000ddfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000d48[ unicode_character - 0x00000d48UL ] );
		}
		else if( ( unicode_character >= 0x00000f40UL )
		      && ( unicode_character <= 0x00001027UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000f40[ unicode_character - 0x00000f40UL ] );
		}
		else if( ( unicode_character >= 0x00001b00UL )
		      && ( unicode_character <= 0x00001b47UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00001b00[ unicode_character - 0x00001b00UL ] );
		}
		else if( ( unicode_character >= 0x00001e00UL )
		      && ( unicode_character <= 0x00002007UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00001e00[ unicode_character - 0x00001e00UL ] );
		}
		else if( ( unicode_character >= 0x00002120UL )
		      && ( unicode_character <= 0x000021cfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002120[ unicode_character - 0x00002120UL ] );
		}
		else if( ( unicode_character >= 0x00002200UL )
		      && ( unicode_character <= 0x0000232fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002200[ unicode_character - 0x00002200UL ] );
		}
		else if( ( unicode_character >= 0x00002ad8UL )
		      && ( unicode_character <= 0x00002adfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002ad8[ unicode_character - 0x00002ad8UL ] );
		}
		else if( ( unicode_character >= 0x00003048UL )
		      && ( unicode_character <= 0x000030ffUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00003048[ unicode_character - 0x00003048UL ] );
		}
		else if( ( unicode_character >= 0x0000f900UL )
		      && ( unicode_character <= 0x0000fadfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0000f900[ unicode_character - 0x0000f900UL ] );
		}
		else if( ( unicode_character >= 0x0000fb18UL )
		      && ( unicode_character <= 0x0000fb4fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0000fb18[ unicode_character - 0x0000fb18UL ] );
		}
		else if( ( unicode_character >= 0x00011098UL )
		      && ( unicode_character <= 0x000110afUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011098[ unicode_character - 0x00011098UL ] );
		}
		else if( ( unicode_character >= 0x00011128UL )
		      && ( unicode_character <= 0x0001112fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011128[ unicode_character - 0x00011128UL ] );
		}
		else if( ( unicode_character >= 0x00011348UL )
		      && ( unicode_character <= 0x0001134fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011348[ unicode_character - 0x00011348UL ] );
		}
		else if( ( unicode_character >= 0x000114b8UL )
		      && ( unicode_character <= 0x000114bfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000114b8[ unicode_character - 0x000114b8UL ] );
		}
		else if( ( unicode_character >= 0x000115b8UL )
		      && ( unicode_character <= 0x000115bfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000115b8[ unicode_character - 0x000115b8UL ] );
		}
		else if( ( unicode_character >= 0x0001d158UL )
		      && ( unicode_character <= 0x0001d167UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0001d158[ unicode_character - 0x0001d158UL ] );
		}
		else if( ( unicode_character >= 0x0001d1b8UL )
		      && ( unicode_character <= 0x0001d1c7UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0001d1b8[ unicode_character - 0x0001d1b8UL ] );
		}
		else if( ( unicode_character >= 0x0002f800UL )
		      && ( unicode_character <= 0x0002fa1fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0002f800[ unicode_character - 0x0002f800UL ] );
		}
		else
		{
			single_nfd_mapping.characters[ 0 ] = unicode_character;

			nfd_mapping = &single_nfd_mapping;
		}
		for( nfd_character_index = 0;
		     nfd_character_index < nfd_mapping->number_of_characters;
		     nfd_character_index++ )
		{
			unicode_character = nfd_mapping->characters[ nfd_character_index ];

			checksum_table_index = ( calculated_checksum ^ ( unicode_character & 0x000000ffUL ) ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

			checksum_table_index = ( calculated_checksum ^ ( unicode_character & 0x000000ffUL ) ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

			checksum_table_index = ( calculated_checksum ^ ( unicode_character & 0x000000ffUL ) ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

			checksum_table_index = ( calculated_checksum ^ ( unicode_character & 0x000000ffUL ) ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
		}
	}
	*name_hash = calculated_checksum & 0x003fffffUL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
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
	uint8_t nfd_character_index                               = 0;

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
/* TODO add case folding */
			unicode_character = towlower( unicode_character );
		}
		if( ( unicode_character >= 0x000000c0UL )
		 && ( unicode_character <= 0x0000017fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000000c0[ unicode_character - 0x000000c0UL ] );
		}
		else if( ( unicode_character >= 0x000001a0UL )
		      && ( unicode_character <= 0x00000237UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000001a0[ unicode_character - 0x000001a0UL ] );
		}
		else if( ( unicode_character >= 0x00000340UL )
		      && ( unicode_character <= 0x000003d7UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000340[ unicode_character - 0x00000340UL ] );
		}
		else if( ( unicode_character >= 0x00000400UL )
		      && ( unicode_character <= 0x000004ffUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000400[ unicode_character - 0x00000400UL ] );
		}
		else if( ( unicode_character >= 0x00000620UL )
		      && ( unicode_character <= 0x000006d7UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000620[ unicode_character - 0x00000620UL ] );
		}
		else if( ( unicode_character >= 0x00000928UL )
		      && ( unicode_character <= 0x000009dfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000928[ unicode_character - 0x00000928UL ] );
		}
		else if( ( unicode_character >= 0x00000a30UL )
		      && ( unicode_character <= 0x00000a5fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000a30[ unicode_character - 0x00000a30UL ] );
		}
		else if( ( unicode_character >= 0x00000b48UL )
		      && ( unicode_character <= 0x00000bcfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000b48[ unicode_character - 0x00000b48UL ] );
		}
		else if( ( unicode_character >= 0x00000c48UL )
		      && ( unicode_character <= 0x00000ccfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000c48[ unicode_character - 0x00000c48UL ] );
		}
		else if( ( unicode_character >= 0x00000d48UL )
		      && ( unicode_character <= 0x00000ddfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000d48[ unicode_character - 0x00000d48UL ] );
		}
		else if( ( unicode_character >= 0x00000f40UL )
		      && ( unicode_character <= 0x00001027UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000f40[ unicode_character - 0x00000f40UL ] );
		}
		else if( ( unicode_character >= 0x00001b00UL )
		      && ( unicode_character <= 0x00001b47UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00001b00[ unicode_character - 0x00001b00UL ] );
		}
		else if( ( unicode_character >= 0x00001e00UL )
		      && ( unicode_character <= 0x00002007UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00001e00[ unicode_character - 0x00001e00UL ] );
		}
		else if( ( unicode_character >= 0x00002120UL )
		      && ( unicode_character <= 0x000021cfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002120[ unicode_character - 0x00002120UL ] );
		}
		else if( ( unicode_character >= 0x00002200UL )
		      && ( unicode_character <= 0x0000232fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002200[ unicode_character - 0x00002200UL ] );
		}
		else if( ( unicode_character >= 0x00002ad8UL )
		      && ( unicode_character <= 0x00002adfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002ad8[ unicode_character - 0x00002ad8UL ] );
		}
		else if( ( unicode_character >= 0x00003048UL )
		      && ( unicode_character <= 0x000030ffUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00003048[ unicode_character - 0x00003048UL ] );
		}
		else if( ( unicode_character >= 0x0000f900UL )
		      && ( unicode_character <= 0x0000fadfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0000f900[ unicode_character - 0x0000f900UL ] );
		}
		else if( ( unicode_character >= 0x0000fb18UL )
		      && ( unicode_character <= 0x0000fb4fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0000fb18[ unicode_character - 0x0000fb18UL ] );
		}
		else if( ( unicode_character >= 0x00011098UL )
		      && ( unicode_character <= 0x000110afUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011098[ unicode_character - 0x00011098UL ] );
		}
		else if( ( unicode_character >= 0x00011128UL )
		      && ( unicode_character <= 0x0001112fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011128[ unicode_character - 0x00011128UL ] );
		}
		else if( ( unicode_character >= 0x00011348UL )
		      && ( unicode_character <= 0x0001134fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011348[ unicode_character - 0x00011348UL ] );
		}
		else if( ( unicode_character >= 0x000114b8UL )
		      && ( unicode_character <= 0x000114bfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000114b8[ unicode_character - 0x000114b8UL ] );
		}
		else if( ( unicode_character >= 0x000115b8UL )
		      && ( unicode_character <= 0x000115bfUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000115b8[ unicode_character - 0x000115b8UL ] );
		}
		else if( ( unicode_character >= 0x0001d158UL )
		      && ( unicode_character <= 0x0001d167UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0001d158[ unicode_character - 0x0001d158UL ] );
		}
		else if( ( unicode_character >= 0x0001d1b8UL )
		      && ( unicode_character <= 0x0001d1c7UL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0001d1b8[ unicode_character - 0x0001d1b8UL ] );
		}
		else if( ( unicode_character >= 0x0002f800UL )
		      && ( unicode_character <= 0x0002fa1fUL ) )
		{
			nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0002f800[ unicode_character - 0x0002f800UL ] );
		}
		else
		{
			single_nfd_mapping.characters[ 0 ] = unicode_character;

			nfd_mapping = &single_nfd_mapping;
		}
		for( nfd_character_index = 0;
		     nfd_character_index < nfd_mapping->number_of_characters;
		     nfd_character_index++ )
		{
			unicode_character = nfd_mapping->characters[ nfd_character_index ];

			checksum_table_index = ( calculated_checksum ^ ( unicode_character & 0x000000ffUL ) ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

			checksum_table_index = ( calculated_checksum ^ ( unicode_character & 0x000000ffUL ) ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

			checksum_table_index = ( calculated_checksum ^ ( unicode_character & 0x000000ffUL ) ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
			unicode_character  >>= 8;

			checksum_table_index = ( calculated_checksum ^ ( unicode_character & 0x000000ffUL ) ) & 0x000000ffUL;
			calculated_checksum  = libfsapfs_checksum_crc32_table[ checksum_table_index ] ^ ( calculated_checksum >> 8 );
		}
	}
	*name_hash = calculated_checksum & 0x003fffffUL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
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

