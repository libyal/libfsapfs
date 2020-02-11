/*
 * Library name functions test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_libuna.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_types.h"
#include "fsapfs_test_unused.h"
#include "fsapfs_test_unicode_case_folding_mappings.h"
#include "fsapfs_test_unicode_decomposition_mappings.h"

#include "../libfsapfs/libfsapfs_name.h"

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_name_case_folding_mappings
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_name_case_folding_mappings(
     void )
{
	fsapfs_test_unicode_case_folding_mapping_t *mapping = NULL;
	libuna_unicode_character_t unicode_character        = 0;
	int mapping_index                                   = 0;

	for( mapping_index = 0;
	     mapping_index < 1325;
	     mapping_index++ )
	{
		mapping           = &( fsapfs_test_unicode_case_folding_mappings[ mapping_index ] );
		unicode_character = mapping->unicode_character;

		libfsapfs_name_get_case_folding_mapping(
		 unicode_character );

		if( mapping->character != unicode_character )
		{
			goto on_error;
		}
	}
	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libfsapfs_name_decomposition_mappings
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_name_decomposition_mappings(
     void )
{
	libfsapfs_name_decomposition_mapping_t single_nfd_mapping = { 1, { 0 } };

	fsapfs_test_unicode_decomposition_mapping_t *mapping      = NULL;
	libfsapfs_name_decomposition_mapping_t *nfd_mapping       = NULL;
	libuna_unicode_character_t unicode_character              = 0;
	int character_index                                       = 0;
	int mapping_index                                         = 0;

	for( mapping_index = 0;
	     mapping_index < 30592;
	     mapping_index++ )
	{
		mapping           = &( fsapfs_test_unicode_nfd_mappings[ mapping_index ] );
		unicode_character = mapping->unicode_character;

		libfsapfs_name_get_decomposition_mapping(
		 unicode_character,
		 nfd_mapping,
		 single_nfd_mapping );

		if( mapping->number_of_characters != nfd_mapping->number_of_characters )
		{
			goto on_error;
		}
		for( character_index = 0;
		     character_index < mapping->number_of_characters;
		     character_index++ )
		{
			if( mapping->characters[ character_index ] != nfd_mapping->characters[ character_index ] )
			{
				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libfsapfs_name_compare_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_name_compare_with_utf8_string(
     void )
{
        uint8_t utf8_string_equal[ 6 ]   = { 'e', 'q', 'u', 'a', 'l', 0 };
        uint8_t utf8_string_greater[ 7 ] = { 'g', 'r', 'e', 'a', 't', 'e', 'r' };
        uint8_t utf8_string_less[ 4 ]    = { 'l', 'e', 's', 's' };
        uint8_t utf8_string_more[ 4 ]    = { 'm', 'o', 'r', 'e' };
	libcerror_error_t *error         = NULL;
	int result                       = 0;

	/* Test regular cases
	 */
	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "equal",
	          6,
	          utf8_string_equal,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "equal",
	          6,
	          utf8_string_equal,
	          6,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "great",
	          6,
	          utf8_string_greater,
	          7,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "less",
	          5,
	          utf8_string_more,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "more",
	          5,
	          utf8_string_less,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "lesser",
	          7,
	          utf8_string_less,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_name_compare_with_utf8_string(
	          NULL,
	          6,
	          utf8_string_equal,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "equal",
	          (size_t) SSIZE_MAX + 1,
	          utf8_string_equal,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "equal",
	          6,
	          NULL,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_name_compare_with_utf8_string(
	          (uint8_t *) "equal",
	          6,
	          utf8_string_equal,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsapfs_name_compare_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_name_compare_with_utf16_string(
     void )
{
        uint16_t utf16_string_equal[ 6 ]   = { 'e', 'q', 'u', 'a', 'l', 0 };
        uint16_t utf16_string_greater[ 7 ] = { 'g', 'r', 'e', 'a', 't', 'e', 'r' };
        uint16_t utf16_string_less[ 4 ]    = { 'l', 'e', 's', 's' };
        uint16_t utf16_string_more[ 4 ]    = { 'm', 'o', 'r', 'e' };
	libcerror_error_t *error           = NULL;
	int result                         = 0;

	/* Test regular cases
	 */
	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "equal",
	          6,
	          utf16_string_equal,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "equal",
	          6,
	          utf16_string_equal,
	          6,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "great",
	          6,
	          utf16_string_greater,
	          7,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "less",
	          5,
	          utf16_string_more,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "more",
	          5,
	          utf16_string_less,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "lesser",
	          7,
	          utf16_string_less,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_name_compare_with_utf16_string(
	          NULL,
	          6,
	          utf16_string_equal,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "equal",
	          (size_t) SSIZE_MAX + 1,
	          utf16_string_equal,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "equal",
	          6,
	          NULL,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_name_compare_with_utf16_string(
	          (uint8_t *) "equal",
	          6,
	          utf16_string_equal,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSAPFS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSAPFS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSAPFS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSAPFS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	FSAPFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSAPFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

	FSAPFS_TEST_RUN(
	 "libfsapfs_name_case_folding_mappings",
	 fsapfs_test_name_case_folding_mappings );

	FSAPFS_TEST_RUN(
	 "libfsapfs_name_decomposition_mappings",
	 fsapfs_test_name_decomposition_mappings );

	FSAPFS_TEST_RUN(
	 "libfsapfs_name_compare_with_utf8_string",
	 fsapfs_test_name_compare_with_utf8_string );

	FSAPFS_TEST_RUN(
	 "libfsapfs_name_compare_with_utf16_string",
	 fsapfs_test_name_compare_with_utf16_string );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

