/*
 * The type definitions
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

#if !defined( _FSAPFS_TEST_TYPES_H )
#define _FSAPFS_TEST_TYPES_H

#include <common.h>
#include <types.h>

#include "fsapfs_test_libuna.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_test_unicode_case_folding_mapping fsapfs_test_unicode_case_folding_mapping_t;

struct fsapfs_test_unicode_case_folding_mapping
{
	/* The Unicode character
	 */
	libuna_unicode_character_t unicode_character;

	/* The character
	 */
	uint32_t character;
};

typedef struct fsapfs_test_unicode_decomposition_mapping fsapfs_test_unicode_decomposition_mapping_t;

struct fsapfs_test_unicode_decomposition_mapping
{
	/* The Unicode character
	 */
	libuna_unicode_character_t unicode_character;

	/* The number of characters
	 */
	uint8_t number_of_characters;

	/* The characters
	 */
	uint32_t characters[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_TEST_TYPES_H ) */

