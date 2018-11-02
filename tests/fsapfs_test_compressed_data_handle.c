/*
 * Library compressed_data_handle type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libfdata.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_compressed_data_handle.h"
#include "../libfsapfs/libfsapfs_data_stream.h"
#include "../libfsapfs/libfsapfs_definitions.h"

uint8_t fsapfs_test_compressed_data_handle_deflate_compressed_data1[ 40 ] = {
	0x66, 0x70, 0x6d, 0x63, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x78, 0x9c, 0x63, 0x60, 0x64, 0x62, 0x66, 0x61, 0x65, 0x63, 0xe7, 0xe0, 0xe4, 0xe2, 0xe6, 0xe1,
	0xe5, 0xe3, 0x07, 0x00, 0x02, 0xb8, 0x00, 0x79 };

uint8_t fsapfs_test_compressed_data_handle_deflate_uncompressed_data1[ 33 ] = {
	0x66, 0x70, 0x6d, 0x63, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

uint8_t fsapfs_test_compressed_data_handle_lzvn_compressed_data1[ 35 ] = {
	0x66, 0x70, 0x6d, 0x63, 0x07, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xe0, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
	0x0e, 0x0f, 0x06 };

uint8_t fsapfs_test_compressed_data_handle_lzvn_uncompressed_data1[ 33 ] = {
	0x66, 0x70, 0x6d, 0x63, 0x07, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

uint8_t fsapfs_test_compressed_data_handle_uncompressed_data1[ 16 ] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_compressed_data_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_compressed_data_handle_initialize(
     void )
{
	libcerror_error_t *error                                   = NULL;
	libfdata_stream_t *compressed_data_stream                  = NULL;
	libfsapfs_compressed_data_handle_t *compressed_data_handle = NULL;
	int result                                                 = 0;

#if defined( HAVE_FSAPFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                            = 1;
	int number_of_memset_fail_tests                            = 1;
	int test_number                                            = 0;
#endif

	/* Initialize test
	 */
	result = libfsapfs_data_stream_initialize_from_data(
	          &compressed_data_stream,
	          fsapfs_test_compressed_data_handle_lzvn_compressed_data1,
	          35,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_data_stream",
	 compressed_data_stream );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsapfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_data_stream,
	          16,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_compressed_data_handle_free(
	          &compressed_data_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_compressed_data_handle_initialize(
	          NULL,
	          compressed_data_stream,
	          16,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
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

	compressed_data_handle = (libfsapfs_compressed_data_handle_t *) 0x12345678UL;

	result = libfsapfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_data_stream,
	          16,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
	          &error );

	compressed_data_handle = NULL;

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          NULL,
	          16,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
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

	result = libfsapfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_data_stream,
	          16,
	          -1,
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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsapfs_compressed_data_handle_initialize with malloc failing
		 */
		fsapfs_test_malloc_attempts_before_fail = test_number;

		result = libfsapfs_compressed_data_handle_initialize(
		          &compressed_data_handle,
		          compressed_data_stream,
		          16,
		          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
		          &error );

		if( fsapfs_test_malloc_attempts_before_fail != -1 )
		{
			fsapfs_test_malloc_attempts_before_fail = -1;

			if( compressed_data_handle != NULL )
			{
				libfsapfs_compressed_data_handle_free(
				 &compressed_data_handle,
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
			 "compressed_data_handle",
			 compressed_data_handle );

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
		/* Test libfsapfs_compressed_data_handle_initialize with memset failing
		 */
		fsapfs_test_memset_attempts_before_fail = test_number;

		result = libfsapfs_compressed_data_handle_initialize(
		          &compressed_data_handle,
		          compressed_data_stream,
		          16,
		          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
		          &error );

		if( fsapfs_test_memset_attempts_before_fail != -1 )
		{
			fsapfs_test_memset_attempts_before_fail = -1;

			if( compressed_data_handle != NULL )
			{
				libfsapfs_compressed_data_handle_free(
				 &compressed_data_handle,
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
			 "compressed_data_handle",
			 compressed_data_handle );

			FSAPFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfdata_stream_free(
	          &compressed_data_stream,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "compressed_data_stream",
	 compressed_data_stream );

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
	if( compressed_data_handle != NULL )
	{
		libfsapfs_compressed_data_handle_free(
		 &compressed_data_handle,
		 NULL );
	}
	if( compressed_data_stream != NULL )
	{
		libfdata_stream_free(
		 &compressed_data_stream,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_compressed_data_handle_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_compressed_data_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_compressed_data_handle_free(
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
	 "libfsapfs_compressed_data_handle_initialize",
	 fsapfs_test_compressed_data_handle_initialize );

	FSAPFS_TEST_RUN(
	 "libfsapfs_compressed_data_handle_free",
	 fsapfs_test_compressed_data_handle_free );

/* TODO add tests for libfsapfs_compressed_data_handle_get_compressed_block_offsets */

/* TODO add tests for libfsapfs_compressed_data_handle_read_segment_data */

/* TODO add tests for libfsapfs_compressed_data_handle_seek_segment_offset */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

