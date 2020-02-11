/*
 * Library name_hash functions test program
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
#include "fsapfs_test_macros.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_name_hash.h"

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_name_hash_calculate_from_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_name_hash_calculate_from_utf8_string(
     void )
{
        uint8_t nfc_utf8_string[ 5 ] = { 'T', 0xc3, 0xa9, 'S', 't' };
        uint8_t nfd_utf8_string[ 6 ] = { 'T', 'e', 0xcc, 0x81, 'S', 't' };
        uint8_t utf8_string[ 4 ]     = { 'T', 'e', 'S', 't' };
	libcerror_error_t *error     = NULL;
	uint32_t name_hash           = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	name_hash = 0;

	result = libfsapfs_name_hash_calculate_from_utf8_string(
	          &name_hash,
	          utf8_string,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x0000996aUL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name_hash = 0;

	result = libfsapfs_name_hash_calculate_from_utf8_string(
	          &name_hash,
	          nfc_utf8_string,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x00190453UL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name_hash = 0;

	result = libfsapfs_name_hash_calculate_from_utf8_string(
	          &name_hash,
	          nfd_utf8_string,
	          6,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x00190453UL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	name_hash = 0;

	result = libfsapfs_name_hash_calculate_from_utf8_string(
	          NULL,
	          utf8_string,
	          4,
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

	result = libfsapfs_name_hash_calculate_from_utf8_string(
	          &name_hash,
	          NULL,
	          4,
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

	result = libfsapfs_name_hash_calculate_from_utf8_string(
	          &name_hash,
	          utf8_string,
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

/* Tests the libfsapfs_name_hash_calculate_from_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_name_hash_calculate_from_utf16_string(
     void )
{
        uint16_t nfc_utf16_string[ 4 ] = { 'T', 0x00e9, 'S', 't' };
        uint16_t nfd_utf16_string[ 5 ] = { 'T', 'e', 0x0301, 'S', 't' };
        uint16_t utf16_string[ 4 ]     = { 'T', 'e', 'S', 't' };
	libcerror_error_t *error       = NULL;
	uint32_t name_hash             = 0;
	int result                     = 0;

	/* Test regular cases
	 */
	name_hash = 0;

	result = libfsapfs_name_hash_calculate_from_utf16_string(
	          &name_hash,
	          utf16_string,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x0000996aUL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name_hash = 0;

	result = libfsapfs_name_hash_calculate_from_utf16_string(
	          &name_hash,
	          nfc_utf16_string,
	          4,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x00190453UL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name_hash = 0;

	result = libfsapfs_name_hash_calculate_from_utf16_string(
	          &name_hash,
	          nfd_utf16_string,
	          5,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x00190453UL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	name_hash = 0;

	result = libfsapfs_name_hash_calculate_from_utf16_string(
	          NULL,
	          utf16_string,
	          4,
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

	result = libfsapfs_name_hash_calculate_from_utf16_string(
	          &name_hash,
	          NULL,
	          4,
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

	result = libfsapfs_name_hash_calculate_from_utf16_string(
	          &name_hash,
	          utf16_string,
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
	 "libfsapfs_name_hash_calculate_from_utf8_string",
	 fsapfs_test_name_hash_calculate_from_utf8_string );

	FSAPFS_TEST_RUN(
	 "libfsapfs_name_hash_calculate_from_utf16_string",
	 fsapfs_test_name_hash_calculate_from_utf16_string );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

