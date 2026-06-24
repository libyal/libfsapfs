/*
 * Library compressed_data_handle type test program
 *
 * Copyright (C) 2018-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libfdata.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_compressed_data_handle.h"
#include "../libfsapfs/libfsapfs_data_stream.h"
#include "../libfsapfs/libfsapfs_definitions.h"

uint8_t fsapfs_test_compressed_data_handle_lzvn_compressed_data1[ 35 ] = {
	0x66, 0x70, 0x6d, 0x63, 0x07, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xe0, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
	0x0e, 0x0f, 0x06 };

/* Type 9 (stored xattr): fpmc header (method=9, uncompressed_size=16) + 0xCC sentinel + 16 bytes raw */
uint8_t fsapfs_test_compressed_data_handle_stored_compressed_data1[ 33 ] = {
	0x66, 0x70, 0x6d, 0x63, 0x09, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xcc, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

/* Type 10 (stored rsrc): 1-chunk LZVN-style table + 0xCC sentinel + 16 bytes raw.
 * table_size=8 => (N+1)*4 for N=1 chunk; chunk_end[0]=25 (absolute offset from rsrc start). */
uint8_t fsapfs_test_compressed_data_handle_stored_rsrc_compressed_data1[ 25 ] = {
	0x08, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00,
	0xcc, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

/* Expected plaintext for both stored test streams (the 0xCC sentinel and any table stripped) */
uint8_t fsapfs_test_compressed_data_handle_stored_uncompressed_data1[ 16 ] = {
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
	int number_of_malloc_fail_tests                            = 3;
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

/* Tests the libfsapfs_compressed_data_handle_get_compressed_block_offsets function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_compressed_data_handle_get_compressed_block_offsets(
     void )
{
	libcerror_error_t *error                                   = NULL;
	libfdata_stream_t *compressed_data_stream                  = NULL;
	libfsapfs_compressed_data_handle_t *compressed_data_handle = NULL;
	int result                                                 = 0;

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

	/* Test regular cases
	 */
	result = libfsapfs_compressed_data_handle_get_compressed_block_offsets(
	          compressed_data_handle,
	          NULL,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test STORED (type 9 xattr): single fpmc block, 0xCC sentinel + raw payload
	 */
	{
		libfdata_stream_t *stored_stream                                      = NULL;
		libfsapfs_compressed_data_handle_t *stored_compressed_data_handle     = NULL;

		result = libfsapfs_data_stream_initialize_from_data(
		          &stored_stream,
		          fsapfs_test_compressed_data_handle_stored_compressed_data1,
		          33,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "stored_stream",
		 stored_stream );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libfsapfs_compressed_data_handle_initialize(
		          &stored_compressed_data_handle,
		          stored_stream,
		          16,
		          LIBFSAPFS_COMPRESSION_METHOD_STORED,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "stored_compressed_data_handle",
		 stored_compressed_data_handle );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libfsapfs_compressed_data_handle_get_compressed_block_offsets(
		          stored_compressed_data_handle,
		          NULL,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libfsapfs_compressed_data_handle_free(
		          &stored_compressed_data_handle,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		result = libfdata_stream_free(
		          &stored_stream,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );
	}

	/* Test STORED (type 10 rsrc): 4-byte LE chunk table (same path as LZVN), 0xCC chunks
	 */
	{
		libfdata_stream_t *stored_rsrc_stream                                  = NULL;
		libfsapfs_compressed_data_handle_t *stored_rsrc_compressed_data_handle = NULL;

		result = libfsapfs_data_stream_initialize_from_data(
		          &stored_rsrc_stream,
		          fsapfs_test_compressed_data_handle_stored_rsrc_compressed_data1,
		          25,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "stored_rsrc_stream",
		 stored_rsrc_stream );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libfsapfs_compressed_data_handle_initialize(
		          &stored_rsrc_compressed_data_handle,
		          stored_rsrc_stream,
		          16,
		          LIBFSAPFS_COMPRESSION_METHOD_STORED,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "stored_rsrc_compressed_data_handle",
		 stored_rsrc_compressed_data_handle );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libfsapfs_compressed_data_handle_get_compressed_block_offsets(
		          stored_rsrc_compressed_data_handle,
		          NULL,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libfsapfs_compressed_data_handle_free(
		          &stored_rsrc_compressed_data_handle,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		result = libfdata_stream_free(
		          &stored_rsrc_stream,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );
	}

	/* Test error cases
	 */
	result = libfsapfs_compressed_data_handle_get_compressed_block_offsets(
	          NULL,
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

/* Tests the libfsapfs_compressed_data_handle_read_segment_data function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_compressed_data_handle_read_segment_data(
     void )
{
	uint8_t segment_data[ 32 ];

	libcerror_error_t *error                                   = NULL;
	libfdata_stream_t *compressed_data_stream                  = NULL;
	libfsapfs_compressed_data_handle_t *compressed_data_handle = NULL;
	ssize_t read_count                                         = 0;
	int result                                                 = 0;

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

	/* Test regular cases
	 */

	/* Test STORED: read 16 bytes of 0xCC-prefixed plaintext via the fpmc single-block path
	 */
	{
		uint8_t stored_segment_data[ 16 ];

		libfdata_stream_t *stored_stream                                  = NULL;
		libfsapfs_compressed_data_handle_t *stored_compressed_data_handle = NULL;

		result = libfsapfs_data_stream_initialize_from_data(
		          &stored_stream,
		          fsapfs_test_compressed_data_handle_stored_compressed_data1,
		          33,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		result = libfsapfs_compressed_data_handle_initialize(
		          &stored_compressed_data_handle,
		          stored_stream,
		          16,
		          LIBFSAPFS_COMPRESSION_METHOD_STORED,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		read_count = libfsapfs_compressed_data_handle_read_segment_data(
		              stored_compressed_data_handle,
		              NULL,
		              0,
		              0,
		              stored_segment_data,
		              16,
		              0,
		              0,
		              &error );

		FSAPFS_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 16 );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = memory_compare(
		          stored_segment_data,
		          fsapfs_test_compressed_data_handle_stored_uncompressed_data1,
		          16 );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		result = libfsapfs_compressed_data_handle_free(
		          &stored_compressed_data_handle,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		result = libfdata_stream_free(
		          &stored_stream,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );
	}
	/* Test STORED rsrc: read 16 bytes of 0xCC-prefixed plaintext via the chunk table path
	 */
	{
		uint8_t stored_rsrc_segment_data[ 16 ];

		libfdata_stream_t *stored_rsrc_stream                                  = NULL;
		libfsapfs_compressed_data_handle_t *stored_rsrc_compressed_data_handle = NULL;

		result = libfsapfs_data_stream_initialize_from_data(
		          &stored_rsrc_stream,
		          fsapfs_test_compressed_data_handle_stored_rsrc_compressed_data1,
		          25,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		result = libfsapfs_compressed_data_handle_initialize(
		          &stored_rsrc_compressed_data_handle,
		          stored_rsrc_stream,
		          16,
		          LIBFSAPFS_COMPRESSION_METHOD_STORED,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		read_count = libfsapfs_compressed_data_handle_read_segment_data(
		              stored_rsrc_compressed_data_handle,
		              NULL,
		              0,
		              0,
		              stored_rsrc_segment_data,
		              16,
		              0,
		              0,
		              &error );

		FSAPFS_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 16 );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = memory_compare(
		          stored_rsrc_segment_data,
		          fsapfs_test_compressed_data_handle_stored_uncompressed_data1,
		          16 );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		result = libfsapfs_compressed_data_handle_free(
		          &stored_rsrc_compressed_data_handle,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		result = libfdata_stream_free(
		          &stored_rsrc_stream,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );
	}

	/* Test error cases
	 */
	read_count = libfsapfs_compressed_data_handle_read_segment_data(
	              NULL,
	              NULL,
	              0,
	              0,
	              segment_data,
	              32,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsapfs_compressed_data_handle_read_segment_data(
	              compressed_data_handle,
	              NULL,
	              -1,
	              0,
	              segment_data,
	              32,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsapfs_compressed_data_handle_read_segment_data(
	              compressed_data_handle,
	              NULL,
	              0,
	              0,
	              NULL,
	              32,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfsapfs_compressed_data_handle_read_segment_data(
	              compressed_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              0,
	              &error );

	FSAPFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
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

/* Tests the libfsapfs_compressed_data_handle_seek_segment_offset function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_compressed_data_handle_seek_segment_offset(
     void )
{
	libcerror_error_t *error                                   = NULL;
	libfdata_stream_t *compressed_data_stream                  = NULL;
	libfsapfs_compressed_data_handle_t *compressed_data_handle = NULL;
	off64_t offset                                             = 0;
	int result                                                 = 0;

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

	/* Test regular cases
	 */
	offset = libfsapfs_compressed_data_handle_seek_segment_offset(
	          compressed_data_handle,
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
	offset = libfsapfs_compressed_data_handle_seek_segment_offset(
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

	offset = libfsapfs_compressed_data_handle_seek_segment_offset(
	          compressed_data_handle,
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

	offset = libfsapfs_compressed_data_handle_seek_segment_offset(
	          compressed_data_handle,
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

	FSAPFS_TEST_RUN(
	 "libfsapfs_compressed_data_handle_get_compressed_block_offsets",
	 fsapfs_test_compressed_data_handle_get_compressed_block_offsets );

	FSAPFS_TEST_RUN(
	 "libfsapfs_compressed_data_handle_read_segment_data",
	 fsapfs_test_compressed_data_handle_read_segment_data );

	FSAPFS_TEST_RUN(
	 "libfsapfs_compressed_data_handle_seek_segment_offset",
	 fsapfs_test_compressed_data_handle_seek_segment_offset );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */
}

