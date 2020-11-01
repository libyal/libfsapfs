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

libfsapfs_name_decomposition_mapping_t special_case_folding_000000df = { 2, { 0x00000073, 0x00000073 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00000130 = { 2, { 0x00000069, 0x00000307 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00000149 = { 2, { 0x000002bc, 0x0000006e } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00000587 = { 2, { 0x00000565, 0x00000582 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001e9a = { 2, { 0x00000061, 0x000002be } };

libfsapfs_name_decomposition_mapping_t special_case_folding_00001f80[ 48 ] = {
	/* 00001f80 */ { 3, { 0x000003b1, 0x00000313, 0x000003b9 } },
	/* 00001f81 */ { 3, { 0x000003b1, 0x00000314, 0x000003b9 } },
	/* 00001f82 */ { 4, { 0x000003b1, 0x00000313, 0x00000300, 0x000003b9 } },
	/* 00001f83 */ { 4, { 0x000003b1, 0x00000314, 0x00000300, 0x000003b9 } },
	/* 00001f84 */ { 4, { 0x000003b1, 0x00000313, 0x00000301, 0x000003b9 } },
	/* 00001f85 */ { 4, { 0x000003b1, 0x00000314, 0x00000301, 0x000003b9 } },
	/* 00001f86 */ { 4, { 0x000003b1, 0x00000313, 0x00000342, 0x000003b9 } },
	/* 00001f87 */ { 4, { 0x000003b1, 0x00000314, 0x00000342, 0x000003b9 } },
	/* 00001f88 */ { 3, { 0x000003b1, 0x00000313, 0x000003b9 } },
	/* 00001f89 */ { 3, { 0x000003b1, 0x00000314, 0x000003b9 } },
	/* 00001f8a */ { 4, { 0x000003b1, 0x00000313, 0x00000300, 0x000003b9 } },
	/* 00001f8b */ { 4, { 0x000003b1, 0x00000314, 0x00000300, 0x000003b9 } },
	/* 00001f8c */ { 4, { 0x000003b1, 0x00000313, 0x00000301, 0x000003b9 } },
	/* 00001f8d */ { 4, { 0x000003b1, 0x00000314, 0x00000301, 0x000003b9 } },
	/* 00001f8e */ { 4, { 0x000003b1, 0x00000313, 0x00000342, 0x000003b9 } },
	/* 00001f8f */ { 4, { 0x000003b1, 0x00000314, 0x00000342, 0x000003b9 } },
	/* 00001f90 */ { 3, { 0x000003b7, 0x00000313, 0x000003b9 } },
	/* 00001f91 */ { 3, { 0x000003b7, 0x00000314, 0x000003b9 } },
	/* 00001f92 */ { 4, { 0x000003b7, 0x00000313, 0x00000300, 0x000003b9 } },
	/* 00001f93 */ { 4, { 0x000003b7, 0x00000314, 0x00000300, 0x000003b9 } },
	/* 00001f94 */ { 4, { 0x000003b7, 0x00000313, 0x00000301, 0x000003b9 } },
	/* 00001f95 */ { 4, { 0x000003b7, 0x00000314, 0x00000301, 0x000003b9 } },
	/* 00001f96 */ { 4, { 0x000003b7, 0x00000313, 0x00000342, 0x000003b9 } },
	/* 00001f97 */ { 4, { 0x000003b7, 0x00000314, 0x00000342, 0x000003b9 } },
	/* 00001f98 */ { 3, { 0x000003b7, 0x00000313, 0x000003b9 } },
	/* 00001f99 */ { 3, { 0x000003b7, 0x00000314, 0x000003b9 } },
	/* 00001f9a */ { 4, { 0x000003b7, 0x00000313, 0x00000300, 0x000003b9 } },
	/* 00001f9b */ { 4, { 0x000003b7, 0x00000314, 0x00000300, 0x000003b9 } },
	/* 00001f9c */ { 4, { 0x000003b7, 0x00000313, 0x00000301, 0x000003b9 } },
	/* 00001f9d */ { 4, { 0x000003b7, 0x00000314, 0x00000301, 0x000003b9 } },
	/* 00001f9e */ { 4, { 0x000003b7, 0x00000313, 0x00000342, 0x000003b9 } },
	/* 00001f9f */ { 4, { 0x000003b7, 0x00000314, 0x00000342, 0x000003b9 } },
	/* 00001fa0 */ { 3, { 0x000003c9, 0x00000313, 0x000003b9 } },
	/* 00001fa1 */ { 3, { 0x000003c9, 0x00000314, 0x000003b9 } },
	/* 00001fa2 */ { 4, { 0x000003c9, 0x00000313, 0x00000300, 0x000003b9 } },
	/* 00001fa3 */ { 4, { 0x000003c9, 0x00000314, 0x00000300, 0x000003b9 } },
	/* 00001fa4 */ { 4, { 0x000003c9, 0x00000313, 0x00000301, 0x000003b9 } },
	/* 00001fa5 */ { 4, { 0x000003c9, 0x00000314, 0x00000301, 0x000003b9 } },
	/* 00001fa6 */ { 4, { 0x000003c9, 0x00000313, 0x00000342, 0x000003b9 } },
	/* 00001fa7 */ { 4, { 0x000003c9, 0x00000314, 0x00000342, 0x000003b9 } },
	/* 00001fa8 */ { 3, { 0x000003c9, 0x00000313, 0x000003b9 } },
	/* 00001fa9 */ { 3, { 0x000003c9, 0x00000314, 0x000003b9 } },
	/* 00001faa */ { 4, { 0x000003c9, 0x00000313, 0x00000300, 0x000003b9 } },
	/* 00001fab */ { 4, { 0x000003c9, 0x00000314, 0x00000300, 0x000003b9 } },
	/* 00001fac */ { 4, { 0x000003c9, 0x00000313, 0x00000301, 0x000003b9 } },
	/* 00001fad */ { 4, { 0x000003c9, 0x00000314, 0x00000301, 0x000003b9 } },
	/* 00001fae */ { 4, { 0x000003c9, 0x00000313, 0x00000342, 0x000003b9 } },
	/* 00001faf */ { 4, { 0x000003c9, 0x00000314, 0x00000342, 0x000003b9 } }
};

libfsapfs_name_decomposition_mapping_t special_case_folding_00001fb2 = { 3, { 0x000003b1, 0x00000300, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fb3 = { 2, { 0x000003b1, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fb4 = { 3, { 0x000003b1, 0x00000301, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fb6 = { 2, { 0x000003b1, 0x00000342 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fb7 = { 3, { 0x000003b1, 0x00000342, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fbc = { 2, { 0x000003b1, 0x000003b9 } };

libfsapfs_name_decomposition_mapping_t special_case_folding_00001fc2 = { 3, { 0x000003b7, 0x00000300, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fc3 = { 2, { 0x000003b7, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fc4 = { 3, { 0x000003b7, 0x00000301, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fc6 = { 2, { 0x000003b7, 0x00000342 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fc7 = { 3, { 0x000003b7, 0x00000342, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001fcc = { 2, { 0x000003b7, 0x000003b9 } };

libfsapfs_name_decomposition_mapping_t special_case_folding_00001ff2 = { 3, { 0x000003c9, 0x00000300, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001ff3 = { 2, { 0x000003c9, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001ff4 = { 3, { 0x000003c9, 0x00000301, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001ff6 = { 2, { 0x000003c9, 0x00000342 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001ff7 = { 3, { 0x000003c9, 0x00000342, 0x000003b9 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_00001ffc = { 2, { 0x000003c9, 0x000003b9 } };

libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb00 = { 2, { 0x00000066, 0x00000066 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb01 = { 2, { 0x00000066, 0x00000069 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb02 = { 2, { 0x00000066, 0x0000006c } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb03 = { 3, { 0x00000066, 0x00000066, 0x00000069 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb04 = { 3, { 0x00000066, 0x00000066, 0x0000006c } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb05 = { 2, { 0x00000073, 0x00000074 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb06 = { 2, { 0x00000073, 0x00000074 } };

libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb13 = { 2, { 0x00000574, 0x00000576 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb14 = { 2, { 0x00000574, 0x00000565 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb15 = { 2, { 0x00000574, 0x0000056b } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb16 = { 2, { 0x0000057e, 0x00000576 } };
libfsapfs_name_decomposition_mapping_t special_case_folding_0000fb17 = { 2, { 0x00000574, 0x0000056d } };

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

			/* Overload the NFD mapping to handle special case folding
			 */
			if( ( unicode_character >= 0x00001f80UL )
			 && ( unicode_character <= 0x00001fafUL ) )
			{
				nfd_mapping = &( special_case_folding_00001f80[ unicode_character - 0x00001f80UL ] );
			}
			else switch( unicode_character )
			{
				case 0x000000dfUL:
					nfd_mapping = &special_case_folding_000000df;
					break;

				case 0x00000130UL:
					nfd_mapping = &special_case_folding_00000130;
					break;

				case 0x00000149UL:
					nfd_mapping = &special_case_folding_00000149;
					break;

				case 0x00000587UL:
					nfd_mapping = &special_case_folding_00000587;
					break;

				case 0x00001e9aUL:
					nfd_mapping = &special_case_folding_00001e9a;
					break;

				case 0x00001fb2UL:
					nfd_mapping = &special_case_folding_00001fb2;
					break;

				case 0x00001fb3UL:
					nfd_mapping = &special_case_folding_00001fb3;
					break;

				case 0x00001fb4UL:
					nfd_mapping = &special_case_folding_00001fb4;
					break;

				case 0x00001fb6UL:
					nfd_mapping = &special_case_folding_00001fb6;
					break;

				case 0x00001fb7UL:
					nfd_mapping = &special_case_folding_00001fb7;
					break;

				case 0x00001fbcUL:
					nfd_mapping = &special_case_folding_00001fbc;
					break;

				case 0x00001fc2UL:
					nfd_mapping = &special_case_folding_00001fc2;
					break;

				case 0x00001fc3UL:
					nfd_mapping = &special_case_folding_00001fc3;
					break;

				case 0x00001fc4UL:
					nfd_mapping = &special_case_folding_00001fc4;
					break;

				case 0x00001fc6UL:
					nfd_mapping = &special_case_folding_00001fc6;
					break;

				case 0x00001fc7UL:
					nfd_mapping = &special_case_folding_00001fc7;
					break;

				case 0x00001fccUL:
					nfd_mapping = &special_case_folding_00001fcc;
					break;

				case 0x00001ff2UL:
					nfd_mapping = &special_case_folding_00001ff2;
					break;

				case 0x00001ff3UL:
					nfd_mapping = &special_case_folding_00001ff3;
					break;

				case 0x00001ff4UL:
					nfd_mapping = &special_case_folding_00001ff4;
					break;

				case 0x00001ff6UL:
					nfd_mapping = &special_case_folding_00001ff6;
					break;

				case 0x00001ff7UL:
					nfd_mapping = &special_case_folding_00001ff7;
					break;

				case 0x00001ffcUL:
					nfd_mapping = &special_case_folding_00001ffc;
					break;

				case 0x0000fb00UL:
					nfd_mapping = &special_case_folding_0000fb00;
					break;

				case 0x0000fb01UL:
					nfd_mapping = &special_case_folding_0000fb01;
					break;

				case 0x0000fb02UL:
					nfd_mapping = &special_case_folding_0000fb02;
					break;

				case 0x0000fb03UL:
					nfd_mapping = &special_case_folding_0000fb03;
					break;

				case 0x0000fb04UL:
					nfd_mapping = &special_case_folding_0000fb04;
					break;

				case 0x0000fb05UL:
					nfd_mapping = &special_case_folding_0000fb05;
					break;

				case 0x0000fb06UL:
					nfd_mapping = &special_case_folding_0000fb06;
					break;

				case 0x0000fb13UL:
					nfd_mapping = &special_case_folding_0000fb13;
					break;

				case 0x0000fb14UL:
					nfd_mapping = &special_case_folding_0000fb14;
					break;

				case 0x0000fb15UL:
					nfd_mapping = &special_case_folding_0000fb15;
					break;

				case 0x0000fb16UL:
					nfd_mapping = &special_case_folding_0000fb16;
					break;

				case 0x0000fb17UL:
					nfd_mapping = &special_case_folding_0000fb17;
					break;

				default:
					libfsapfs_name_get_decomposition_mapping(
					 unicode_character,
					 nfd_mapping,
					 single_nfd_mapping );
					break;
			}
		}
		else
		{
			libfsapfs_name_get_decomposition_mapping(
			 unicode_character,
			 nfd_mapping,
			 single_nfd_mapping );
		}
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

			/* Overload the NFD mapping to handle special case folding
			 */
			if( ( unicode_character >= 0x00001f80UL )
			 && ( unicode_character <= 0x00001fafUL ) )
			{
				nfd_mapping = &( special_case_folding_00001f80[ unicode_character - 0x00001f80UL ] );
			}
			else switch( unicode_character )
			{
				case 0x000000dfUL:
					nfd_mapping = &special_case_folding_000000df;
					break;

				case 0x00000130UL:
					nfd_mapping = &special_case_folding_00000130;
					break;

				case 0x00000149UL:
					nfd_mapping = &special_case_folding_00000149;
					break;

				case 0x00000587UL:
					nfd_mapping = &special_case_folding_00000587;
					break;

				case 0x00001e9aUL:
					nfd_mapping = &special_case_folding_00001e9a;
					break;

				case 0x00001fb2UL:
					nfd_mapping = &special_case_folding_00001fb2;
					break;

				case 0x00001fb3UL:
					nfd_mapping = &special_case_folding_00001fb3;
					break;

				case 0x00001fb4UL:
					nfd_mapping = &special_case_folding_00001fb4;
					break;

				case 0x00001fb6UL:
					nfd_mapping = &special_case_folding_00001fb6;
					break;

				case 0x00001fb7UL:
					nfd_mapping = &special_case_folding_00001fb7;
					break;

				case 0x00001fbcUL:
					nfd_mapping = &special_case_folding_00001fbc;
					break;

				case 0x00001fc2UL:
					nfd_mapping = &special_case_folding_00001fc2;
					break;

				case 0x00001fc3UL:
					nfd_mapping = &special_case_folding_00001fc3;
					break;

				case 0x00001fc4UL:
					nfd_mapping = &special_case_folding_00001fc4;
					break;

				case 0x00001fc6UL:
					nfd_mapping = &special_case_folding_00001fc6;
					break;

				case 0x00001fc7UL:
					nfd_mapping = &special_case_folding_00001fc7;
					break;

				case 0x00001fccUL:
					nfd_mapping = &special_case_folding_00001fcc;
					break;

				case 0x00001ff2UL:
					nfd_mapping = &special_case_folding_00001ff2;
					break;

				case 0x00001ff3UL:
					nfd_mapping = &special_case_folding_00001ff3;
					break;

				case 0x00001ff4UL:
					nfd_mapping = &special_case_folding_00001ff4;
					break;

				case 0x00001ff6UL:
					nfd_mapping = &special_case_folding_00001ff6;
					break;

				case 0x00001ff7UL:
					nfd_mapping = &special_case_folding_00001ff7;
					break;

				case 0x00001ffcUL:
					nfd_mapping = &special_case_folding_00001ffc;
					break;

				case 0x0000fb00UL:
					nfd_mapping = &special_case_folding_0000fb00;
					break;

				case 0x0000fb01UL:
					nfd_mapping = &special_case_folding_0000fb01;
					break;

				case 0x0000fb02UL:
					nfd_mapping = &special_case_folding_0000fb02;
					break;

				case 0x0000fb03UL:
					nfd_mapping = &special_case_folding_0000fb03;
					break;

				case 0x0000fb04UL:
					nfd_mapping = &special_case_folding_0000fb04;
					break;

				case 0x0000fb05UL:
					nfd_mapping = &special_case_folding_0000fb05;
					break;

				case 0x0000fb06UL:
					nfd_mapping = &special_case_folding_0000fb06;
					break;

				case 0x0000fb13UL:
					nfd_mapping = &special_case_folding_0000fb13;
					break;

				case 0x0000fb14UL:
					nfd_mapping = &special_case_folding_0000fb14;
					break;

				case 0x0000fb15UL:
					nfd_mapping = &special_case_folding_0000fb15;
					break;

				case 0x0000fb16UL:
					nfd_mapping = &special_case_folding_0000fb16;
					break;

				case 0x0000fb17UL:
					nfd_mapping = &special_case_folding_0000fb17;
					break;

				default:
					libfsapfs_name_get_decomposition_mapping(
					 unicode_character,
					 nfd_mapping,
					 single_nfd_mapping );
					break;
			}
		}
		else
		{
			libfsapfs_name_get_decomposition_mapping(
			 unicode_character,
			 nfd_mapping,
			 single_nfd_mapping );
		}
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

