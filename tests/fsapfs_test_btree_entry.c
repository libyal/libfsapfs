/*
 * Library btree_entry type test program
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

#include "../libfsapfs/libfsapfs_btree_entry.h"

uint8_t fsapfs_test_btree_entry_key_data1[ 23 ] = {
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x0b, 0x14, 0xbe, 0x9c, 0x2e, 0x66, 0x73, 0x65,
	0x76, 0x65, 0x6e, 0x74, 0x73, 0x64, 0x00 };

uint8_t fsapfs_test_btree_entry_value_data1[ 18 ] = {
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0xe3, 0x85, 0x4a, 0x54, 0x55, 0x52, 0x15,
	0x04, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_btree_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_btree_entry_initialize(
     void )
{
	libcerror_error_t *error             = NULL;
	libfsapfs_btree_entry_t *btree_entry = NULL;
	int result                           = 0;

#if defined( HAVE_FSAPFS_TEST_MEMORY )
	int number_of_malloc_fail_tests      = 1;
	int number_of_memset_fail_tests      = 1;
	int test_number                      = 0;
#endif

	/* Test regular cases
	 */
	result = libfsapfs_btree_entry_initialize(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "btree_entry",
	 btree_entry );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_btree_entry_free(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "btree_entry",
	 btree_entry );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_btree_entry_initialize(
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

	btree_entry = (libfsapfs_btree_entry_t *) 0x12345678UL;

	result = libfsapfs_btree_entry_initialize(
	          &btree_entry,
	          &error );

	btree_entry = NULL;

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
		/* Test libfsapfs_btree_entry_initialize with malloc failing
		 */
		fsapfs_test_malloc_attempts_before_fail = test_number;

		result = libfsapfs_btree_entry_initialize(
		          &btree_entry,
		          &error );

		if( fsapfs_test_malloc_attempts_before_fail != -1 )
		{
			fsapfs_test_malloc_attempts_before_fail = -1;

			if( btree_entry != NULL )
			{
				libfsapfs_btree_entry_free(
				 &btree_entry,
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
			 "btree_entry",
			 btree_entry );

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
		/* Test libfsapfs_btree_entry_initialize with memset failing
		 */
		fsapfs_test_memset_attempts_before_fail = test_number;

		result = libfsapfs_btree_entry_initialize(
		          &btree_entry,
		          &error );

		if( fsapfs_test_memset_attempts_before_fail != -1 )
		{
			fsapfs_test_memset_attempts_before_fail = -1;

			if( btree_entry != NULL )
			{
				libfsapfs_btree_entry_free(
				 &btree_entry,
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
			 "btree_entry",
			 btree_entry );

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
	if( btree_entry != NULL )
	{
		libfsapfs_btree_entry_free(
		 &btree_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_btree_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_btree_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_btree_entry_free(
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

/* Tests the libfsapfs_btree_entry_set_key_data function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_btree_entry_set_key_data(
     void )
{
	libcerror_error_t *error             = NULL;
	libfsapfs_btree_entry_t *btree_entry = NULL;
	int result                           = 0;

	/* Initialize test
	 */
	result = libfsapfs_btree_entry_initialize(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "btree_entry",
	 btree_entry );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsapfs_btree_entry_set_key_data(
	          btree_entry,
	          fsapfs_test_btree_entry_key_data1,
	          23,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_btree_entry_set_key_data(
	          btree_entry,
	          fsapfs_test_btree_entry_key_data1,
	          23,
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
	result = libfsapfs_btree_entry_free(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "btree_entry",
	 btree_entry );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsapfs_btree_entry_initialize(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "btree_entry",
	 btree_entry );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_btree_entry_set_key_data(
	          NULL,
	          fsapfs_test_btree_entry_key_data1,
	          23,
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

	result = libfsapfs_btree_entry_set_key_data(
	          btree_entry,
	          NULL,
	          23,
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

	result = libfsapfs_btree_entry_set_key_data(
	          btree_entry,
	          fsapfs_test_btree_entry_key_data1,
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

#if defined( HAVE_FSAPFS_TEST_MEMORY )

	/* Test libfsapfs_btree_entry_set_key_data with malloc failing
	 */
	fsapfs_test_malloc_attempts_before_fail = 0;

	result = libfsapfs_btree_entry_set_key_data(
	          btree_entry,
	          fsapfs_test_btree_entry_key_data1,
	          23,
	          &error );

	if( fsapfs_test_malloc_attempts_before_fail != -1 )
	{
		fsapfs_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test libfsapfs_btree_entry_set_key_data with memcpy failing
	 */
	fsapfs_test_memcpy_attempts_before_fail = 0;

	result = libfsapfs_btree_entry_set_key_data(
	          btree_entry,
	          fsapfs_test_btree_entry_key_data1,
	          23,
	          &error );

	if( fsapfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsapfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfsapfs_btree_entry_free(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "btree_entry",
	 btree_entry );

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
	if( btree_entry != NULL )
	{
		libfsapfs_btree_entry_free(
		 &btree_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_btree_entry_set_value_data function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_btree_entry_set_value_data(
     void )
{
	libcerror_error_t *error             = NULL;
	libfsapfs_btree_entry_t *btree_entry = NULL;
	int result                           = 0;

	/* Initialize test
	 */
	result = libfsapfs_btree_entry_initialize(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "btree_entry",
	 btree_entry );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsapfs_btree_entry_set_value_data(
	          btree_entry,
	          fsapfs_test_btree_entry_value_data1,
	          18,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_btree_entry_set_value_data(
	          btree_entry,
	          fsapfs_test_btree_entry_value_data1,
	          18,
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
	result = libfsapfs_btree_entry_free(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "btree_entry",
	 btree_entry );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsapfs_btree_entry_initialize(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "btree_entry",
	 btree_entry );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_btree_entry_set_value_data(
	          NULL,
	          fsapfs_test_btree_entry_value_data1,
	          18,
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

	result = libfsapfs_btree_entry_set_value_data(
	          btree_entry,
	          NULL,
	          18,
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

	result = libfsapfs_btree_entry_set_value_data(
	          btree_entry,
	          fsapfs_test_btree_entry_value_data1,
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

#if defined( HAVE_FSAPFS_TEST_MEMORY )

	/* Test libfsapfs_btree_entry_set_value_data with malloc failing
	 */
	fsapfs_test_malloc_attempts_before_fail = 0;

	result = libfsapfs_btree_entry_set_value_data(
	          btree_entry,
	          fsapfs_test_btree_entry_value_data1,
	          18,
	          &error );

	if( fsapfs_test_malloc_attempts_before_fail != -1 )
	{
		fsapfs_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test libfsapfs_btree_entry_set_value_data with memcpy failing
	 */
	fsapfs_test_memcpy_attempts_before_fail = 0;

	result = libfsapfs_btree_entry_set_value_data(
	          btree_entry,
	          fsapfs_test_btree_entry_value_data1,
	          18,
	          &error );

	if( fsapfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsapfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfsapfs_btree_entry_free(
	          &btree_entry,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "btree_entry",
	 btree_entry );

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
	if( btree_entry != NULL )
	{
		libfsapfs_btree_entry_free(
		 &btree_entry,
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
	 "libfsapfs_btree_entry_initialize",
	 fsapfs_test_btree_entry_initialize );

	FSAPFS_TEST_RUN(
	 "libfsapfs_btree_entry_free",
	 fsapfs_test_btree_entry_free );

	FSAPFS_TEST_RUN(
	 "libfsapfs_btree_entry_set_key_data",
	 fsapfs_test_btree_entry_set_key_data );

	FSAPFS_TEST_RUN(
	 "libfsapfs_btree_entry_set_value_data",
	 fsapfs_test_btree_entry_set_value_data );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

