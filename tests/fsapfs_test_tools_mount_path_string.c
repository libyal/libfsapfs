/*
 * Tools mount_path_string functions test program
 *
 * Copyright (C) 2018-2025, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libcpath.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_unused.h"

#include "../fsapfstools/mount_path_string.h"

#if defined( WINAPI )
#define ESCAPE_CHARACTER (system_character_t) '^'
#else
#define ESCAPE_CHARACTER (system_character_t) '\\'
#endif

/* Tests the mount_path_string_copy_hexadecimal_to_integer_32_bit function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_tools_mount_path_string_copy_hexadecimal_to_integer_32_bit(
     void )
{
	system_character_t error_string[ 5 ] = { '2', '0', 'Z', '8', 0 };
	system_character_t string[ 5 ]       = { '2', '0', '2', '8', 0 };
	libcerror_error_t *error             = NULL;
	uint32_t value_32bit                 = 0;
	int result                           = 0;

	/* Test regular cases
	 */
	result = mount_path_string_copy_hexadecimal_to_integer_32_bit(
	          string,
	          4,
	          &value_32bit,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "value_32bit",
	 value_32bit,
	 (uint32_t) 0x2028 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = mount_path_string_copy_hexadecimal_to_integer_32_bit(
	          NULL,
	          4,
	          &value_32bit,
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

	result = mount_path_string_copy_hexadecimal_to_integer_32_bit(
	          string,
	          0,
	          &value_32bit,
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

	result = mount_path_string_copy_hexadecimal_to_integer_32_bit(
	          string,
	          (size_t) SSIZE_MAX + 1,
	          &value_32bit,
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

	result = mount_path_string_copy_hexadecimal_to_integer_32_bit(
	          string,
	          4,
	          NULL,
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

	result = mount_path_string_copy_hexadecimal_to_integer_32_bit(
	          error_string,
	          4,
	          &value_32bit,
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

/* Tests the mount_path_string_copy_from_file_entry_path function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_tools_mount_path_string_copy_from_file_entry_path(
     void )
{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	system_character_t file_entry_path3[ 5 ] = { 't', 'e', 0x2028, 't', 0 };
	system_character_t file_entry_path4[ 5 ] = { 't', 'e', '\\', 't', 0 };
	system_character_t file_entry_path5[ 5 ] = { 't', 'e', ESCAPE_CHARACTER, 't', 0 };
	system_character_t expected_path2[ 8 ]   = { 't', 'e', ESCAPE_CHARACTER, 'x', '0', '3', 't', 0 };
	system_character_t expected_path3[ 14 ]  = { 't', 'e', ESCAPE_CHARACTER, 'U', '0', '0', '0', '0', '2', '0', '2', '8', 't', 0 };
	system_character_t expected_path4[ 8 ]   = { 't', 'e', ESCAPE_CHARACTER, 'x', '5', 'c', 't', 0 };
	system_character_t expected_path5[ 6 ]   = { 't', 'e', ESCAPE_CHARACTER, ESCAPE_CHARACTER, 't', 0 };
#else
	system_character_t file_entry_path3[ 7 ] = { 't', 'e', 0xe2, 0x80, 0xa8, 't', 0 };
	system_character_t file_entry_path4[ 5 ] = { 't', 'e', LIBCPATH_SEPARATOR, 't', 0 };
	system_character_t file_entry_path5[ 5 ] = { 't', 'e', ESCAPE_CHARACTER, 't', 0 };
	system_character_t expected_path2[ 8 ]   = { 't', 'e', ESCAPE_CHARACTER, 'x', '0', '3', 't', 0 };
	system_character_t expected_path3[ 14 ]  = { 't', 'e', ESCAPE_CHARACTER, 'U', '0', '0', '0', '0', '2', '0', '2', '8', 't', 0 };
#if defined( WINAPI )
	system_character_t expected_path4[ 8 ]   = { 't', 'e', ESCAPE_CHARACTER, 'x', '5', 'c', 't', 0 };
#else
	system_character_t expected_path4[ 8 ]   = { 't', 'e', ESCAPE_CHARACTER, 'x', '2', 'f', 't', 0 };
#endif
	system_character_t expected_path5[ 6 ]   = { 't', 'e', ESCAPE_CHARACTER, ESCAPE_CHARACTER, 't', 0 };
#endif

	system_character_t file_entry_path1[ 5 ] = { 't', 'e', 's', 't', 0 };
	system_character_t file_entry_path2[ 5 ] = { 't', 'e', 0x03, 't', 0 };
	system_character_t expected_path1[ 5 ]   = { 't', 'e', 's', 't', 0 };
	libcerror_error_t *error                 = NULL;
	system_character_t *path                 = NULL;
	size_t path_size                         = 0;
	int result                               = 0;

	/* Test regular cases
	 */
	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          file_entry_path1,
	          4,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "path",
	 path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "path_size",
	 path_size,
	 (size_t) 41 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          path,
	          expected_path1,
	          5 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 path );

	path = NULL;

	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          file_entry_path2,
	          4,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "path",
	 path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "path_size",
	 path_size,
	 (size_t) 41 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          path,
	          expected_path2,
	          8 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 path );

	path = NULL;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          file_entry_path3,
	          4,
	          &error );
#else
	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          file_entry_path3,
	          6,
	          &error );
#endif
	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "path",
	 path );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "path_size",
	 path_size,
	 (size_t) 41 );
#else
	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "path_size",
	 path_size,
	 (size_t) 61 );
#endif
	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          path,
	          expected_path3,
	          14 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 path );

	path = NULL;

	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          file_entry_path4,
	          4,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "path",
	 path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "path_size",
	 path_size,
	 (size_t) 41 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          path,
	          expected_path4,
	          8 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 path );

	path = NULL;

	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          file_entry_path5,
	          4,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "path",
	 path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "path_size",
	 path_size,
	 (size_t) 41 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          path,
	          expected_path5,
	          6 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 path );

	path = NULL;

	/* Test error cases
	 */
	result = mount_path_string_copy_from_file_entry_path(
	          NULL,
	          &path_size,
	          file_entry_path1,
	          4,
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

	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          NULL,
	          file_entry_path1,
	          4,
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

	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          NULL,
	          4,
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

	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          file_entry_path1,
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

	result = mount_path_string_copy_from_file_entry_path(
	          &path,
	          &path_size,
	          file_entry_path1,
	          (size_t) SSIZE_MAX + 1,
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
	if( path != NULL )
	{
		memory_free(
		 path );
	}
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the mount_path_string_copy_to_file_entry_path function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_tools_mount_path_string_copy_to_file_entry_path(
     void )
{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	system_character_t expected_file_entry_path3[ 6 ] = { LIBFSAPFS_SEPARATOR, 't', 'e', 0x2028, 't', 0 };
	system_character_t expected_file_entry_path4[ 6 ] = { LIBFSAPFS_SEPARATOR, 't', 'e', ESCAPE_CHARACTER, 't', 0 };
	system_character_t path2[ 9 ]                     = { LIBCPATH_SEPARATOR, 't', 'e', ESCAPE_CHARACTER, 'x', '0', '3', 't', 0 };
	system_character_t path3[ 15 ]                    = { LIBCPATH_SEPARATOR, 't', 'e', ESCAPE_CHARACTER, 'U', '0', '0', '0', '0', '2', '0', '2', '8', 't', 0 };
	system_character_t path4[ 7 ]                     = { LIBCPATH_SEPARATOR, 't', 'e', ESCAPE_CHARACTER, ESCAPE_CHARACTER, 't', 0 };
#else
	system_character_t expected_file_entry_path3[ 8 ] = { LIBFSAPFS_SEPARATOR, 't', 'e', 0xe2, 0x80, 0xa8, 't', 0 };
	system_character_t expected_file_entry_path4[ 6 ] = { LIBFSAPFS_SEPARATOR, 't', 'e', ESCAPE_CHARACTER, 't', 0 };
	system_character_t path2[ 9 ]                     = { LIBCPATH_SEPARATOR, 't', 'e', ESCAPE_CHARACTER, 'x', '0', '3', 't', 0 };
	system_character_t path3[ 15 ]                    = { LIBCPATH_SEPARATOR, 't', 'e', ESCAPE_CHARACTER, 'U', '0', '0', '0', '0', '2', '0', '2', '8', 't', 0 };
	system_character_t path4[ 7 ]                     = { LIBCPATH_SEPARATOR, 't', 'e', ESCAPE_CHARACTER, ESCAPE_CHARACTER, 't', 0 };
#endif

	system_character_t expected_file_entry_path1[ 6 ] = { LIBFSAPFS_SEPARATOR, 't', 'e', 's', 't', 0 };
	system_character_t expected_file_entry_path2[ 6 ] = { LIBFSAPFS_SEPARATOR, 't', 'e', 0x03, 't', 0 };
	system_character_t expected_file_entry_path5[ 6 ] = { LIBFSAPFS_SEPARATOR, 't', 'e', LIBFSAPFS_SEPARATOR, 't', 0 };
	system_character_t path1[ 6 ]                     = { LIBCPATH_SEPARATOR, 't', 'e', 's', 't', 0 };
	system_character_t path5[ 6 ]                     = { LIBCPATH_SEPARATOR, 't', 'e', LIBCPATH_SEPARATOR, 't', 0 };
	libcerror_error_t *error                          = NULL;
	system_character_t *file_entry_path               = NULL;
	size_t file_entry_path_size                       = 0;
	int result                                        = 0;

	/* Test regular cases
	 */
	result = mount_path_string_copy_to_file_entry_path(
	          path1,
	          5,
	          &file_entry_path,
	          &file_entry_path_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_entry_path",
	 file_entry_path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "file_entry_path_size",
	 file_entry_path_size,
	 (size_t) 6 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          file_entry_path,
	          expected_file_entry_path1,
	          6 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 file_entry_path );

	file_entry_path = NULL;

	result = mount_path_string_copy_to_file_entry_path(
	          path2,
	          8,
	          &file_entry_path,
	          &file_entry_path_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_entry_path",
	 file_entry_path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "file_entry_path_size",
	 file_entry_path_size,
	 (size_t) 9 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          file_entry_path,
	          expected_file_entry_path2,
	          6 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 file_entry_path );

	file_entry_path = NULL;

	result = mount_path_string_copy_to_file_entry_path(
	          path3,
	          14,
	          &file_entry_path,
	          &file_entry_path_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_entry_path",
	 file_entry_path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "file_entry_path_size",
	 file_entry_path_size,
	 (size_t) 15 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = memory_compare(
	          file_entry_path,
	          expected_file_entry_path3,
	          6 );
#else
	result = memory_compare(
	          file_entry_path,
	          expected_file_entry_path3,
	          8 );
#endif
	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 file_entry_path );

	file_entry_path = NULL;

	result = mount_path_string_copy_to_file_entry_path(
	          path4,
	          6,
	          &file_entry_path,
	          &file_entry_path_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_entry_path",
	 file_entry_path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "file_entry_path_size",
	 file_entry_path_size,
	 (size_t) 7 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          file_entry_path,
	          expected_file_entry_path4,
	          6 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 file_entry_path );

	file_entry_path = NULL;

	result = mount_path_string_copy_to_file_entry_path(
	          path5,
	          6,
	          &file_entry_path,
	          &file_entry_path_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_entry_path",
	 file_entry_path );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "file_entry_path_size",
	 file_entry_path_size,
	 (size_t) 7 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          file_entry_path,
	          expected_file_entry_path5,
	          6 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 file_entry_path );

	file_entry_path = NULL;

	/* Test error cases
	 */
	result = mount_path_string_copy_to_file_entry_path(
	          NULL,
	          5,
	          &file_entry_path,
	          &file_entry_path_size,
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

	result = mount_path_string_copy_to_file_entry_path(
	          path1,
	          0,
	          &file_entry_path,
	          &file_entry_path_size,
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

	result = mount_path_string_copy_to_file_entry_path(
	          path1,
	          (size_t) SSIZE_MAX + 1,
	          &file_entry_path,
	          &file_entry_path_size,
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

	result = mount_path_string_copy_to_file_entry_path(
	          path1,
	          5,
	          NULL,
	          &file_entry_path_size,
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

	result = mount_path_string_copy_to_file_entry_path(
	          path1,
	          5,
	          &file_entry_path,
	          NULL,
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
	if( file_entry_path != NULL )
	{
		memory_free(
		 file_entry_path );
	}
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

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

	FSAPFS_TEST_RUN(
	 "mount_path_string_copy_hexadecimal_to_integer_32_bit",
	 fsapfs_test_tools_mount_path_string_copy_hexadecimal_to_integer_32_bit )

	FSAPFS_TEST_RUN(
	 "mount_path_string_copy_from_file_entry_path",
	 fsapfs_test_tools_mount_path_string_copy_from_file_entry_path )

	FSAPFS_TEST_RUN(
	 "mount_path_string_copy_to_file_entry_path",
	 fsapfs_test_tools_mount_path_string_copy_to_file_entry_path )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

