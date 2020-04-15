/*
 * Library snapshot_metadata type test program
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
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_snapshot_metadata.h"

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_snapshot_metadata_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_snapshot_metadata_initialize(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfsapfs_snapshot_metadata_t *snapshot_metadata = NULL;
	int result                                       = 0;

#if defined( HAVE_FSAPFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                  = 1;
	int number_of_memset_fail_tests                  = 1;
	int test_number                                  = 0;
#endif

	/* Test regular cases
	 */
	result = libfsapfs_snapshot_metadata_initialize(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_snapshot_metadata_free(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_snapshot_metadata_initialize(
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

	snapshot_metadata = (libfsapfs_snapshot_metadata_t *) 0x12345678UL;

	result = libfsapfs_snapshot_metadata_initialize(
	          &snapshot_metadata,
	          &error );

	snapshot_metadata = NULL;

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSAPFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsapfs_snapshot_metadata_initialize with malloc failing
		 */
		fsapfs_test_malloc_attempts_before_fail = test_number;

		result = libfsapfs_snapshot_metadata_initialize(
		          &snapshot_metadata,
		          &error );

		if( fsapfs_test_malloc_attempts_before_fail != -1 )
		{
			fsapfs_test_malloc_attempts_before_fail = -1;

			if( snapshot_metadata != NULL )
			{
				libfsapfs_snapshot_metadata_free(
				 &snapshot_metadata,
				 NULL );
			}
		}
		else
		{
			FSAPFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSAPFS_TEST_ASSERT_IS_NULL(
			 "snapshot_metadata",
			 snapshot_metadata );

			FSAPFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfsapfs_snapshot_metadata_initialize with memset failing
		 */
		fsapfs_test_memset_attempts_before_fail = test_number;

		result = libfsapfs_snapshot_metadata_initialize(
		          &snapshot_metadata,
		          &error );

		if( fsapfs_test_memset_attempts_before_fail != -1 )
		{
			fsapfs_test_memset_attempts_before_fail = -1;

			if( snapshot_metadata != NULL )
			{
				libfsapfs_snapshot_metadata_free(
				 &snapshot_metadata,
				 NULL );
			}
		}
		else
		{
			FSAPFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSAPFS_TEST_ASSERT_IS_NULL(
			 "snapshot_metadata",
			 snapshot_metadata );

			FSAPFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( snapshot_metadata != NULL )
	{
		libfsapfs_snapshot_metadata_free(
		 &snapshot_metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_snapshot_metadata_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_snapshot_metadata_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_snapshot_metadata_free(
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
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsapfs_snapshot_metadata_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_snapshot_metadata_get_utf8_name_size(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfsapfs_snapshot_metadata_t *snapshot_metadata = NULL;
	size_t utf8_string_size                          = 0;
	int result                                       = 0;

	/* Initialize test
	 */
	result = libfsapfs_snapshot_metadata_initialize(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement
	result = libfsapfs_snapshot_metadata_get_utf8_name_size(
	          snapshot_metadata,
	          &utf8_string_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsapfs_snapshot_metadata_get_utf8_name_size(
	          NULL,
	          &utf8_string_size,
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

	result = libfsapfs_snapshot_metadata_get_utf8_name_size(
	          snapshot_metadata,
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

	/* Clean up
	 */
	result = libfsapfs_snapshot_metadata_free(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( snapshot_metadata != NULL )
	{
		libfsapfs_snapshot_metadata_free(
		 &snapshot_metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_snapshot_metadata_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_snapshot_metadata_get_utf8_name(
     void )
{
	uint8_t utf8_string[ 512 ];

	libcerror_error_t *error                         = NULL;
	libfsapfs_snapshot_metadata_t *snapshot_metadata = NULL;
	int result                                       = 0;

	/* Initialize test
	 */
	result = libfsapfs_snapshot_metadata_initialize(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement
	result = libfsapfs_snapshot_metadata_get_utf8_name(
	          snapshot_metadata,
	          utf8_string,
	          512,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsapfs_snapshot_metadata_get_utf8_name(
	          NULL,
	          utf8_string,
	          512,
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

	result = libfsapfs_snapshot_metadata_get_utf8_name(
	          snapshot_metadata,
	          NULL,
	          512,
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

	result = libfsapfs_snapshot_metadata_get_utf8_name(
	          snapshot_metadata,
	          utf8_string,
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

	result = libfsapfs_snapshot_metadata_get_utf8_name(
	          snapshot_metadata,
	          utf8_string,
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

	/* Clean up
	 */
	result = libfsapfs_snapshot_metadata_free(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( snapshot_metadata != NULL )
	{
		libfsapfs_snapshot_metadata_free(
		 &snapshot_metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_snapshot_metadata_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_snapshot_metadata_get_utf16_name_size(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfsapfs_snapshot_metadata_t *snapshot_metadata = NULL;
	size_t utf16_string_size                         = 0;
	int result                                       = 0;

	/* Initialize test
	 */
	result = libfsapfs_snapshot_metadata_initialize(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement
	result = libfsapfs_snapshot_metadata_get_utf16_name_size(
	          snapshot_metadata,
	          &utf16_string_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsapfs_snapshot_metadata_get_utf16_name_size(
	          NULL,
	          &utf16_string_size,
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

	result = libfsapfs_snapshot_metadata_get_utf16_name_size(
	          snapshot_metadata,
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

	/* Clean up
	 */
	result = libfsapfs_snapshot_metadata_free(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( snapshot_metadata != NULL )
	{
		libfsapfs_snapshot_metadata_free(
		 &snapshot_metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_snapshot_metadata_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_snapshot_metadata_get_utf16_name(
     void )
{
	uint16_t utf16_string[ 512 ];

	libcerror_error_t *error                         = NULL;
	libfsapfs_snapshot_metadata_t *snapshot_metadata = NULL;
	int result                                       = 0;

	/* Initialize test
	 */
	result = libfsapfs_snapshot_metadata_initialize(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement
	result = libfsapfs_snapshot_metadata_get_utf16_name(
	          snapshot_metadata,
	          utf16_string,
	          512,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libfsapfs_snapshot_metadata_get_utf16_name(
	          NULL,
	          utf16_string,
	          512,
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

	result = libfsapfs_snapshot_metadata_get_utf16_name(
	          snapshot_metadata,
	          NULL,
	          512,
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

	result = libfsapfs_snapshot_metadata_get_utf16_name(
	          snapshot_metadata,
	          utf16_string,
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

	result = libfsapfs_snapshot_metadata_get_utf16_name(
	          snapshot_metadata,
	          utf16_string,
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

	/* Clean up
	 */
	result = libfsapfs_snapshot_metadata_free(
	          &snapshot_metadata,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "snapshot_metadata",
	 snapshot_metadata );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( snapshot_metadata != NULL )
	{
		libfsapfs_snapshot_metadata_free(
		 &snapshot_metadata,
		 NULL );
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
	 "libfsapfs_snapshot_metadata_initialize",
	 fsapfs_test_snapshot_metadata_initialize );

	FSAPFS_TEST_RUN(
	 "libfsapfs_snapshot_metadata_free",
	 fsapfs_test_snapshot_metadata_free );

	/* TODO: add tests for libfsapfs_snapshot_metadata_read_key_data */

	/* TODO: add tests for libfsapfs_snapshot_metadata_read_value_data */

	FSAPFS_TEST_RUN(
	 "libfsapfs_snapshot_metadata_get_utf8_name_size",
	 fsapfs_test_snapshot_metadata_get_utf8_name_size );

	FSAPFS_TEST_RUN(
	 "libfsapfs_snapshot_metadata_get_utf8_name",
	 fsapfs_test_snapshot_metadata_get_utf8_name );

	FSAPFS_TEST_RUN(
	 "libfsapfs_snapshot_metadata_get_utf16_name_size",
	 fsapfs_test_snapshot_metadata_get_utf16_name_size );

	FSAPFS_TEST_RUN(
	 "libfsapfs_snapshot_metadata_get_utf16_name",
	 fsapfs_test_snapshot_metadata_get_utf16_name );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

