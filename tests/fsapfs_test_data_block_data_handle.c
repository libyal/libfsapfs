/*
 * Library data_block_data_handle type test program
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfs_test_functions.h"
#include "fsapfs_test_libbfio.h"
#include "fsapfs_test_libcdata.h"
#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_data_block_data_handle.h"
#include "../libfsapfs/libfsapfs_file_extent.h"
#include "../libfsapfs/libfsapfs_io_handle.h"

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_data_block_data_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_data_block_data_handle_initialize(
     void )
{
	libcdata_array_t *file_extents                             = NULL;
	libcerror_error_t *error                                   = NULL;
	libfsapfs_data_block_data_handle_t *data_block_data_handle = NULL;
	libfsapfs_file_extent_t *file_extent                       = NULL;
	libfsapfs_io_handle_t *io_handle                           = NULL;
	int entry_index                                            = 0;
	int result                                                 = 0;

#if defined( HAVE_FSAPFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                            = 1;
	int number_of_memset_fail_tests                            = 1;
	int test_number                                            = 0;
#endif

	/* Initialize test
	 */
	result = libfsapfs_io_handle_initialize(
	          &io_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->block_size = 4096;

	result = libcdata_array_initialize(
	          &file_extents,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_extents",
	 file_extents );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_file_extent_initialize(
	          &file_extent,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_extent",
	 file_extent );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_extent->physical_block_number = 1;
	file_extent->data_size             = 4096;

	result = libcdata_array_append_entry(
	          file_extents,
	          &entry_index,
	          (intptr_t *) file_extent,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_extent = NULL;

	/* Test regular cases
	 */
	result = libfsapfs_data_block_data_handle_initialize(
	          &data_block_data_handle,
	          io_handle,
	          NULL,
	          file_extents,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_block_data_handle",
	 data_block_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_data_block_data_handle_free(
	          &data_block_data_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "data_block_data_handle",
	 data_block_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_data_block_data_handle_initialize(
	          NULL,
	          io_handle,
	          NULL,
	          file_extents,
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

	data_block_data_handle = (libfsapfs_data_block_data_handle_t *) 0x12345678UL;

	result = libfsapfs_data_block_data_handle_initialize(
	          &data_block_data_handle,
	          io_handle,
	          NULL,
	          file_extents,
	          0,
	          &error );

	data_block_data_handle = NULL;

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_data_block_data_handle_initialize(
	          &data_block_data_handle,
	          NULL,
	          NULL,
	          file_extents,
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

	result = libfsapfs_data_block_data_handle_initialize(
	          &data_block_data_handle,
	          io_handle,
	          NULL,
	          NULL,
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

#if defined( HAVE_FSAPFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsapfs_data_block_data_handle_initialize with malloc failing
		 */
		fsapfs_test_malloc_attempts_before_fail = test_number;

		result = libfsapfs_data_block_data_handle_initialize(
		          &data_block_data_handle,
		          io_handle,
		          NULL,
		          file_extents,
		          0,
		          &error );

		if( fsapfs_test_malloc_attempts_before_fail != -1 )
		{
			fsapfs_test_malloc_attempts_before_fail = -1;

			if( data_block_data_handle != NULL )
			{
				libfsapfs_data_block_data_handle_free(
				 &data_block_data_handle,
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
			 "data_block_data_handle",
			 data_block_data_handle );

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
		/* Test libfsapfs_data_block_data_handle_initialize with memset failing
		 */
		fsapfs_test_memset_attempts_before_fail = test_number;

		result = libfsapfs_data_block_data_handle_initialize(
		          &data_block_data_handle,
		          io_handle,
		          NULL,
		          file_extents,
		          0,
		          &error );

		if( fsapfs_test_memset_attempts_before_fail != -1 )
		{
			fsapfs_test_memset_attempts_before_fail = -1;

			if( data_block_data_handle != NULL )
			{
				libfsapfs_data_block_data_handle_free(
				 &data_block_data_handle,
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
			 "data_block_data_handle",
			 data_block_data_handle );

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
	result = libcdata_array_free(
	          &file_extents,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "file_extents",
	 file_extents );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_io_handle_free(
	          &io_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( data_block_data_handle != NULL )
	{
		libfsapfs_data_block_data_handle_free(
		 &data_block_data_handle,
		 NULL );
	}
	if( file_extent != NULL )
	{
		libfsapfs_file_extent_free(
		 &file_extent,
		 NULL );
	}
	if( file_extents != NULL )
	{
		libcdata_array_free(
		 &file_extents,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsapfs_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_data_block_data_handle_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_data_block_data_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_data_block_data_handle_free(
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

/* Tests the libfsapfs_data_block_data_handle_read_segment_data function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_data_block_data_handle_read_segment_data(
     void )
{
	uint8_t segment_data[ 16 ];

	uint8_t expected_segment_data[ 16 ] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

	libbfio_handle_t *file_io_handle                           = NULL;
	libcdata_array_t *file_extents                             = NULL;
	libcerror_error_t *error                                   = NULL;
	libfsapfs_data_block_data_handle_t *data_block_data_handle = NULL;
	libfsapfs_file_extent_t *file_extent                       = NULL;
	libfsapfs_io_handle_t *io_handle                           = NULL;
	uint8_t *data_block_data                                   = NULL;
	size_t data_offset                                         = 0;
	ssize_t read_count                                         = 0;
	off64_t offset                                             = 0;
	uint8_t byte_value                                         = 0;
	int entry_index                                            = 0;
	int result                                                 = 0;

	/* Initialize test
	 */
	result = libfsapfs_io_handle_initialize(
	          &io_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->block_size = 4096;

	result = libcdata_array_initialize(
	          &file_extents,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_extents",
	 file_extents );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_file_extent_initialize(
	          &file_extent,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_extent",
	 file_extent );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_extent->physical_block_number = 1;
	file_extent->data_size             = 4096;

	result = libcdata_array_append_entry(
	          file_extents,
	          &entry_index,
	          (intptr_t *) file_extent,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_extent = NULL;

	result = libfsapfs_data_block_data_handle_initialize(
	          &data_block_data_handle,
	          io_handle,
	          NULL,
	          file_extents,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_block_data_handle",
	 data_block_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	data_block_data = (uint8_t *) memory_allocate(
	                               8192 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_block_data",
	 data_block_data );

	for( data_offset = 0;
	     data_offset < 8192;
	     data_offset++ )
	{
		data_block_data[ data_offset ] = byte_value;

		if( byte_value < 15 )
		{
			byte_value++;
		}
		else
		{
			byte_value = 0;
		}
	}
	result = fsapfs_test_open_file_io_handle(
	          &file_io_handle,
	          data_block_data,
	          8192,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              file_io_handle,
	              0,
	              0,
	              segment_data,
	              12,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) 12 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          segment_data,
	          expected_segment_data,
	          12 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Read buffer on last segment boundary
	 */
	offset = libfsapfs_data_block_data_handle_seek_segment_offset(
	          data_block_data_handle,
	          NULL,
	          0,
	          0,
	          4096 - 4,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 4096 - 4 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              file_io_handle,
	              0,
	              0,
	              segment_data,
	              8,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) 4 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          segment_data,
	          &( expected_segment_data[ 12 ] ),
	          4 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Read buffer beyond last segment
	 */
	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              file_io_handle,
	              0,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) 0 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsapfs_data_block_data_handle_free(
	          &data_block_data_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "data_block_data_handle",
	 data_block_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Create a new data block data handle to prevent the data block cache
	 * affecting the tests.
	 */
	result = libfsapfs_data_block_data_handle_initialize(
	          &data_block_data_handle,
	          io_handle,
	          NULL,
	          file_extents,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_block_data_handle",
	 data_block_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              NULL,
	              file_io_handle,
	              0,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	data_block_data_handle->current_offset = -1;

	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              file_io_handle,
	              0,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	data_block_data_handle->current_offset = 0;

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              file_io_handle,
	              -1,
	              0,
	              segment_data,
	              16,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              file_io_handle,
	              0,
	              0,
	              NULL,
	              16,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              file_io_handle,
	              0,
	              0,
	              segment_data,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSAPFS_TEST_MEMORY )
#if defined( OPTIMIZATION_DISABLED )
	/* Test libfsapfs_data_block_data_handle_read_segment_data with memcpy failing
	 */
	fsapfs_test_memcpy_attempts_before_fail = 0;

	read_count = libfsapfs_data_block_data_handle_read_segment_data(
	              data_block_data_handle,
	              file_io_handle,
	              0,
	              0,
	              segment_data,
	              12,
	              0,
	              0,
	              &error );

	if( fsapfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsapfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT64(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) */

	/* Clean up file IO handle
	 */
	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	memory_free(
	 data_block_data );

	data_block_data = NULL;

	/* Clean up
	 */
	result = libfsapfs_data_block_data_handle_free(
	          &data_block_data_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "data_block_data_handle",
	 data_block_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_array_free(
	          &file_extents,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "file_extents",
	 file_extents );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_io_handle_free(
	          &io_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( data_block_data != NULL )
	{
		memory_free(
		 data_block_data );
	}
	if( data_block_data_handle != NULL )
	{
		libfsapfs_data_block_data_handle_free(
		 &data_block_data_handle,
		 NULL );
	}
	if( file_extent != NULL )
	{
		libfsapfs_file_extent_free(
		 &file_extent,
		 NULL );
	}
	if( file_extents != NULL )
	{
		libcdata_array_free(
		 &file_extents,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsapfs_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_data_block_data_handle_seek_segment_offset function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_data_block_data_handle_seek_segment_offset(
     void )
{
	libcdata_array_t *file_extents                             = NULL;
	libcerror_error_t *error                                   = NULL;
	libfsapfs_data_block_data_handle_t *data_block_data_handle = NULL;
	libfsapfs_file_extent_t *file_extent                       = NULL;
	libfsapfs_io_handle_t *io_handle                           = NULL;
	off64_t offset                                             = 0;
	int entry_index                                            = 0;
	int result                                                 = 0;

	/* Initialize test
	 */
	result = libfsapfs_io_handle_initialize(
	          &io_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->block_size = 4096;

	result = libcdata_array_initialize(
	          &file_extents,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_extents",
	 file_extents );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_file_extent_initialize(
	          &file_extent,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_extent",
	 file_extent );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_extent->physical_block_number = 1;
	file_extent->data_size             = 4096;

	result = libcdata_array_append_entry(
	          file_extents,
	          &entry_index,
	          (intptr_t *) file_extent,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_extent = NULL;

	result = libfsapfs_data_block_data_handle_initialize(
	          &data_block_data_handle,
	          io_handle,
	          NULL,
	          file_extents,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_block_data_handle",
	 data_block_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	offset = libfsapfs_data_block_data_handle_seek_segment_offset(
	          data_block_data_handle,
	          NULL,
	          0,
	          0,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libfsapfs_data_block_data_handle_seek_segment_offset(
	          NULL,
	          NULL,
	          0,
	          0,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libfsapfs_data_block_data_handle_seek_segment_offset(
	          data_block_data_handle,
	          NULL,
	          -1,
	          0,
	          0,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libfsapfs_data_block_data_handle_seek_segment_offset(
	          data_block_data_handle,
	          NULL,
	          0,
	          0,
	          -1,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsapfs_data_block_data_handle_free(
	          &data_block_data_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "data_block_data_handle",
	 data_block_data_handle );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_array_free(
	          &file_extents,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "file_extents",
	 file_extents );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_io_handle_free(
	          &io_handle,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( data_block_data_handle != NULL )
	{
		libfsapfs_data_block_data_handle_free(
		 &data_block_data_handle,
		 NULL );
	}
	if( file_extent != NULL )
	{
		libfsapfs_file_extent_free(
		 &file_extent,
		 NULL );
	}
	if( file_extents != NULL )
	{
		libcdata_array_free(
		 &file_extents,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsapfs_io_handle_free(
		 &io_handle,
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
	 "libfsapfs_data_block_data_handle_initialize",
	 fsapfs_test_data_block_data_handle_initialize );

	FSAPFS_TEST_RUN(
	 "libfsapfs_data_block_data_handle_free",
	 fsapfs_test_data_block_data_handle_free );

	FSAPFS_TEST_RUN(
	 "libfsapfs_data_block_data_handle_read_segment_data",
	 fsapfs_test_data_block_data_handle_read_segment_data );

	FSAPFS_TEST_RUN(
	 "libfsapfs_data_block_data_handle_seek_segment_offset",
	 fsapfs_test_data_block_data_handle_seek_segment_offset );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

