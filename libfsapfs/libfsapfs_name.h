/*
 * Name functions
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

#if !defined( _LIBFSAPFS_NAME_H )
#define _LIBFSAPFS_NAME_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"
#include "libfsapfs_libuna.h"
#include "libfsapfs_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_name_decomposition_mapping libfsapfs_name_decomposition_mapping_t;

struct libfsapfs_name_decomposition_mapping
{
	/* The number of characters
	 */
	uint8_t number_of_characters;

	/* The characters
	 */
	uint32_t characters[ 4 ];
};

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x00000040[ 528 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x00000340[ 536 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x000010a0[ 48 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x000013f8[ 8 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x00001c80[ 16 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x00001e00[ 512 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x00002120[ 104 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x000024b0[ 32 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x00002c00[ 248 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x0000a640[ 96 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x0000a720[ 152 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x0000ab70[ 80 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x0000ff20[ 32 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x00010400[ 40 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x000104b0[ 40 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x00010c80[ 56 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x000118a0[ 32 ];

LIBFSAPFS_EXTERN_VARIABLE \
uint32_t libfsapfs_name_case_folding_mappings_0x0001e900[ 40 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x000000c0[ 192 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x000001a0[ 152 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000340[ 152 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000400[ 256 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000620[ 184 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000928[ 184 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000a30[ 48 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000b48[ 136 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000c48[ 136 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000d48[ 152 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00000f40[ 232 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00001b00[ 72 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00001e00[ 520 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00002120[ 176 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00002200[ 304 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00002ad8[ 8 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00003048[ 184 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x0000f900[ 480 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x0000fb18[ 56 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00011098[ 24 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00011128[ 8 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x00011348[ 8 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x000114b8[ 8 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x000115b8[ 8 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x0001d158[ 16 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x0001d1b8[ 16 ];

LIBFSAPFS_EXTERN_VARIABLE \
libfsapfs_name_decomposition_mapping_t libfsapfs_name_decomposition_mappings_0x0002f800[ 544 ];

#define libfsapfs_name_get_case_folding_mapping( unicode_character ) \
	if( ( unicode_character >= 0x00000040UL ) \
	 && ( unicode_character <= 0x0000024fUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x00000040[ unicode_character - 0x00000040UL ]; \
	} \
	else if( ( unicode_character >= 0x00000340UL ) \
	      && ( unicode_character <= 0x00000557UL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x00000340[ unicode_character - 0x00000340UL ]; \
	} \
	else if( ( unicode_character >= 0x000010a0UL ) \
	      && ( unicode_character <= 0x000010cfUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x000010a0[ unicode_character - 0x000010a0UL ]; \
	} \
	else if( ( unicode_character >= 0x000013f8UL ) \
	      && ( unicode_character <= 0x000013ffUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x000013f8[ unicode_character - 0x000013f8UL ]; \
	} \
	else if( ( unicode_character >= 0x00001c80UL ) \
	      && ( unicode_character <= 0x00001c8fUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x00001c80[ unicode_character - 0x00001c80UL ]; \
	} \
	else if( ( unicode_character >= 0x00001e00UL ) \
	      && ( unicode_character <= 0x00001fffUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x00001e00[ unicode_character - 0x00001e00UL ]; \
	} \
	else if( ( unicode_character >= 0x00002120UL ) \
	      && ( unicode_character <= 0x00002187UL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x00002120[ unicode_character - 0x00002120UL ]; \
	} \
	else if( ( unicode_character >= 0x000024b0UL ) \
	      && ( unicode_character <= 0x000024cfUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x000024b0[ unicode_character - 0x000024b0UL ]; \
	} \
	else if( ( unicode_character >= 0x00002c00UL ) \
	      && ( unicode_character <= 0x00002cf7UL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x00002c00[ unicode_character - 0x00002c00UL ]; \
	} \
	else if( ( unicode_character >= 0x0000a640UL ) \
	      && ( unicode_character <= 0x0000a69fUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x0000a640[ unicode_character - 0x0000a640UL ]; \
	} \
	else if( ( unicode_character >= 0x0000a720UL ) \
	      && ( unicode_character <= 0x0000a7b7UL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x0000a720[ unicode_character - 0x0000a720UL ]; \
	} \
	else if( ( unicode_character >= 0x0000ab70UL ) \
	      && ( unicode_character <= 0x0000abbf ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x0000ab70[ unicode_character - 0x0000ab70UL ]; \
	} \
	else if( ( unicode_character >= 0x0000ff20UL ) \
	      && ( unicode_character <= 0x0000ff3fUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x0000ff20[ unicode_character - 0x0000ff20UL ]; \
	} \
	else if( ( unicode_character >= 0x00010400UL ) \
	      && ( unicode_character <= 0x00010427UL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x00010400[ unicode_character - 0x00010400UL ]; \
	} \
	else if( ( unicode_character >= 0x000104b0UL ) \
	      && ( unicode_character <= 0x000104d7UL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x000104b0[ unicode_character - 0x000104b0UL ]; \
	} \
	else if( ( unicode_character >= 0x00010c80UL ) \
	      && ( unicode_character <= 0x00010cb7UL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x00010c80[ unicode_character - 0x00010c80UL ]; \
	} \
	else if( ( unicode_character >= 0x000118a0UL ) \
	      && ( unicode_character <= 0x000118bfUL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x000118a0[ unicode_character - 0x000118a0UL ]; \
	} \
	else if( ( unicode_character >= 0x0001e900UL ) \
	      && ( unicode_character <= 0x0001e927UL ) ) \
	{ \
		unicode_character = libfsapfs_name_case_folding_mappings_0x0001e900[ unicode_character - 0x0001e900UL ]; \
	}

#define libfsapfs_name_get_decomposition_mapping( unicode_character, nfd_mapping, single_nfd_mapping ) \
	if( ( unicode_character >= 0x000000c0UL ) \
	 && ( unicode_character <= 0x0000017fUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000000c0[ unicode_character - 0x000000c0UL ] ); \
	} \
	else if( ( unicode_character >= 0x000001a0UL ) \
	      && ( unicode_character <= 0x00000237UL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000001a0[ unicode_character - 0x000001a0UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000340UL ) \
	      && ( unicode_character <= 0x000003d7UL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000340[ unicode_character - 0x00000340UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000400UL ) \
	      && ( unicode_character <= 0x000004ffUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000400[ unicode_character - 0x00000400UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000620UL ) \
	      && ( unicode_character <= 0x000006d7UL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000620[ unicode_character - 0x00000620UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000928UL ) \
	      && ( unicode_character <= 0x000009dfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000928[ unicode_character - 0x00000928UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000a30UL ) \
	      && ( unicode_character <= 0x00000a5fUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000a30[ unicode_character - 0x00000a30UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000b48UL ) \
	      && ( unicode_character <= 0x00000bcfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000b48[ unicode_character - 0x00000b48UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000c48UL ) \
	      && ( unicode_character <= 0x00000ccfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000c48[ unicode_character - 0x00000c48UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000d48UL ) \
	      && ( unicode_character <= 0x00000ddfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000d48[ unicode_character - 0x00000d48UL ] ); \
	} \
	else if( ( unicode_character >= 0x00000f40UL ) \
	      && ( unicode_character <= 0x00001027UL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00000f40[ unicode_character - 0x00000f40UL ] ); \
	} \
	else if( ( unicode_character >= 0x00001b00UL ) \
	      && ( unicode_character <= 0x00001b47UL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00001b00[ unicode_character - 0x00001b00UL ] ); \
	} \
	else if( ( unicode_character >= 0x00001e00UL ) \
	      && ( unicode_character <= 0x00002007UL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00001e00[ unicode_character - 0x00001e00UL ] ); \
	} \
	else if( ( unicode_character >= 0x00002120UL ) \
	      && ( unicode_character <= 0x000021cfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002120[ unicode_character - 0x00002120UL ] ); \
	} \
	else if( ( unicode_character >= 0x00002200UL ) \
	      && ( unicode_character <= 0x0000232fUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002200[ unicode_character - 0x00002200UL ] ); \
	} \
	else if( ( unicode_character >= 0x00002ad8UL ) \
	      && ( unicode_character <= 0x00002adfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00002ad8[ unicode_character - 0x00002ad8UL ] ); \
	} \
	else if( ( unicode_character >= 0x00003048UL ) \
	      && ( unicode_character <= 0x000030ffUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00003048[ unicode_character - 0x00003048UL ] ); \
	} \
	else if( ( unicode_character >= 0x0000f900UL ) \
	      && ( unicode_character <= 0x0000fadfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0000f900[ unicode_character - 0x0000f900UL ] ); \
	} \
	else if( ( unicode_character >= 0x0000fb18UL ) \
	      && ( unicode_character <= 0x0000fb4fUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0000fb18[ unicode_character - 0x0000fb18UL ] ); \
	} \
	else if( ( unicode_character >= 0x00011098UL ) \
	      && ( unicode_character <= 0x000110afUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011098[ unicode_character - 0x00011098UL ] ); \
	} \
	else if( ( unicode_character >= 0x00011128UL ) \
	      && ( unicode_character <= 0x0001112fUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011128[ unicode_character - 0x00011128UL ] ); \
	} \
	else if( ( unicode_character >= 0x00011348UL ) \
	      && ( unicode_character <= 0x0001134fUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x00011348[ unicode_character - 0x00011348UL ] ); \
	} \
	else if( ( unicode_character >= 0x000114b8UL ) \
	      && ( unicode_character <= 0x000114bfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000114b8[ unicode_character - 0x000114b8UL ] ); \
	} \
	else if( ( unicode_character >= 0x000115b8UL ) \
	      && ( unicode_character <= 0x000115bfUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x000115b8[ unicode_character - 0x000115b8UL ] ); \
	} \
	else if( ( unicode_character >= 0x0001d158UL ) \
	      && ( unicode_character <= 0x0001d167UL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0001d158[ unicode_character - 0x0001d158UL ] ); \
	} \
	else if( ( unicode_character >= 0x0001d1b8UL ) \
	      && ( unicode_character <= 0x0001d1c7UL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0001d1b8[ unicode_character - 0x0001d1b8UL ] ); \
	} \
	else if( ( unicode_character >= 0x0002f800UL ) \
	      && ( unicode_character <= 0x0002fa1fUL ) ) \
	{ \
		nfd_mapping = &( libfsapfs_name_decomposition_mappings_0x0002f800[ unicode_character - 0x0002f800UL ] ); \
	} \
	else \
	{ \
		single_nfd_mapping.characters[ 0 ] = unicode_character; \
 \
		nfd_mapping = &single_nfd_mapping; \
	} \

int libfsapfs_name_compare_with_utf8_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf8_character_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libcerror_error_t **error );

int libfsapfs_name_compare_with_utf16_string(
     const uint8_t *name,
     size_t name_size,
     const libuna_utf16_character_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_NAME_H ) */

