/*
 * Library fusion_middle_tree type test program
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

#include "fsapfs_test_functions.h"
#include "fsapfs_test_libbfio.h"
#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_fusion_middle_tree.h"

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_fusion_middle_tree_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_fusion_middle_tree_initialize(
     void )
{
	libcerror_error_t *error                           = NULL;
	libfsapfs_fusion_middle_tree_t *fusion_middle_tree = NULL;
	int result                                         = 0;

#if defined( HAVE_FSAPFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                    = 1;
	int number_of_memset_fail_tests                    = 1;
	int test_number                                    = 0;
#endif

	/* Test regular cases
	 */
	result = libfsapfs_fusion_middle_tree_initialize(
	          &fusion_middle_tree,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "fusion_middle_tree",
	 fusion_middle_tree );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_fusion_middle_tree_free(
	          &fusion_middle_tree,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "fusion_middle_tree",
	 fusion_middle_tree );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_fusion_middle_tree_initialize(
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

	fusion_middle_tree = (libfsapfs_fusion_middle_tree_t *) 0x12345678UL;

	result = libfsapfs_fusion_middle_tree_initialize(
	          &fusion_middle_tree,
	          &error );

	fusion_middle_tree = NULL;

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
		/* Test libfsapfs_fusion_middle_tree_initialize with malloc failing
		 */
		fsapfs_test_malloc_attempts_before_fail = test_number;

		result = libfsapfs_fusion_middle_tree_initialize(
		          &fusion_middle_tree,
		          &error );

		if( fsapfs_test_malloc_attempts_before_fail != -1 )
		{
			fsapfs_test_malloc_attempts_before_fail = -1;

			if( fusion_middle_tree != NULL )
			{
				libfsapfs_fusion_middle_tree_free(
				 &fusion_middle_tree,
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
			 "fusion_middle_tree",
			 fusion_middle_tree );

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
		/* Test libfsapfs_fusion_middle_tree_initialize with memset failing
		 */
		fsapfs_test_memset_attempts_before_fail = test_number;

		result = libfsapfs_fusion_middle_tree_initialize(
		          &fusion_middle_tree,
		          &error );

		if( fsapfs_test_memset_attempts_before_fail != -1 )
		{
			fsapfs_test_memset_attempts_before_fail = -1;

			if( fusion_middle_tree != NULL )
			{
				libfsapfs_fusion_middle_tree_free(
				 &fusion_middle_tree,
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
			 "fusion_middle_tree",
			 fusion_middle_tree );

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
	if( fusion_middle_tree != NULL )
	{
		libfsapfs_fusion_middle_tree_free(
		 &fusion_middle_tree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_fusion_middle_tree_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_fusion_middle_tree_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_fusion_middle_tree_free(
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
	 "libfsapfs_fusion_middle_tree_initialize",
	 fsapfs_test_fusion_middle_tree_initialize );

	FSAPFS_TEST_RUN(
	 "libfsapfs_fusion_middle_tree_free",
	 fsapfs_test_fusion_middle_tree_free );

	/* TODO: add tests for libfsapfs_fusion_middle_tree_read_file_io_handle */

	/* TODO: add tests for libfsapfs_fusion_middle_tree_read_data */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

