/*
 * Bit-stream testing program
 *
 * Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libcnotify.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_bit_stream.h"

/* Define to make fsapfs_test_bit_stream generate verbose output
#define FSAPFS_TEST_BIT_STREAM_VERBOSE
 */

uint8_t fsapfs_test_bit_stream_data[ 16 ] = {
	0x78, 0xda, 0xbd, 0x59, 0x6d, 0x8f, 0xdb, 0xb8, 0x11, 0xfe, 0x7c, 0xfa, 0x15, 0xc4, 0x7e, 0xb9 };

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_bit_stream_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_bit_stream_initialize(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsapfs_bit_stream_t *bit_stream = NULL;
	int result                       = 0;

#if defined( HAVE_FSAPFS_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 1;
	int number_of_memset_fail_tests  = 1;
	int test_number                  = 0;
#endif

	/* Test regular cases
	 */
	result = libfsapfs_bit_stream_initialize(
	          &bit_stream,
	          fsapfs_test_bit_stream_data,
	          16,
	          0,
	          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "bit_stream",
	 bit_stream );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_bit_stream_free(
	          &bit_stream,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "bit_stream",
	 bit_stream );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_bit_stream_initialize(
	          NULL,
	          fsapfs_test_bit_stream_data,
	          16,
	          0,
	          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
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

	bit_stream = (libfsapfs_bit_stream_t *) 0x12345678UL;

	result = libfsapfs_bit_stream_initialize(
	          &bit_stream,
	          fsapfs_test_bit_stream_data,
	          16,
	          0,
	          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
	          &error );

	bit_stream = NULL;

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_bit_stream_initialize(
	          &bit_stream,
	          NULL,
	          16,
	          0,
	          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
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

	result = libfsapfs_bit_stream_initialize(
	          &bit_stream,
	          fsapfs_test_bit_stream_data,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
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

	result = libfsapfs_bit_stream_initialize(
	          &bit_stream,
	          fsapfs_test_bit_stream_data,
	          16,
	          (size_t) SSIZE_MAX + 1,
	          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
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

	result = libfsapfs_bit_stream_initialize(
	          &bit_stream,
	          fsapfs_test_bit_stream_data,
	          16,
	          0,
	          0xff,
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
		/* Test libfsapfs_bit_stream_initialize with malloc failing
		 */
		fsapfs_test_malloc_attempts_before_fail = test_number;

		result = libfsapfs_bit_stream_initialize(
		          &bit_stream,
		          fsapfs_test_bit_stream_data,
		          16,
		          0,
		          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
		          &error );

		if( fsapfs_test_malloc_attempts_before_fail != -1 )
		{
			fsapfs_test_malloc_attempts_before_fail = -1;

			if( bit_stream != NULL )
			{
				libfsapfs_bit_stream_free(
				 &bit_stream,
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
			 "bit_stream",
			 bit_stream );

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
		/* Test libfsapfs_bit_stream_initialize with memset failing
		 */
		fsapfs_test_memset_attempts_before_fail = test_number;

		result = libfsapfs_bit_stream_initialize(
		          &bit_stream,
		          fsapfs_test_bit_stream_data,
		          16,
		          0,
		          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
		          &error );

		if( fsapfs_test_memset_attempts_before_fail != -1 )
		{
			fsapfs_test_memset_attempts_before_fail = -1;

			if( bit_stream != NULL )
			{
				libfsapfs_bit_stream_free(
				 &bit_stream,
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
			 "bit_stream",
			 bit_stream );

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
	if( bit_stream != NULL )
	{
		libfsapfs_bit_stream_free(
		 &bit_stream,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_bit_stream_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_bit_stream_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_bit_stream_free(
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

/* Tests the libfsapfs_bit_stream_get_value function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_bit_stream_get_value(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsapfs_bit_stream_t *bit_stream = NULL;
	uint32_t value_32bit             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsapfs_bit_stream_initialize(
	          &bit_stream,
	          fsapfs_test_bit_stream_data,
	          16,
	          0,
	          LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsapfs_bit_stream_get_value(
	          bit_stream,
	          0,
	          &value_32bit,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "value_32bit",
	 value_32bit,
	 (uint32_t) 0x00000000UL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "bit_stream->byte_stream_offset",
	 bit_stream->byte_stream_offset,
	 (size_t) 0 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "bit_stream->bit_buffer",
	 bit_stream->bit_buffer,
	 (uint32_t) 0x00000000UL );

	FSAPFS_TEST_ASSERT_EQUAL_UINT8(
	 "bit_stream->bit_buffer_size",
	 bit_stream->bit_buffer_size,
	 (uint8_t) 0 );

	result = libfsapfs_bit_stream_get_value(
	          bit_stream,
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
	 (uint32_t) 0x00000008UL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "bit_stream->byte_stream_offset",
	 bit_stream->byte_stream_offset,
	 (size_t) 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "bit_stream->bit_buffer",
	 bit_stream->bit_buffer,
	 (uint32_t) 0x00000007UL );

	FSAPFS_TEST_ASSERT_EQUAL_UINT8(
	 "bit_stream->bit_buffer_size",
	 bit_stream->bit_buffer_size,
	 (uint8_t) 4 );

	result = libfsapfs_bit_stream_get_value(
	          bit_stream,
	          12,
	          &value_32bit,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "value_32bit",
	 value_32bit,
	 (uint32_t) 0x00000da7UL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "bit_stream->byte_stream_offset",
	 bit_stream->byte_stream_offset,
	 (size_t) 2 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "bit_stream->bit_buffer",
	 bit_stream->bit_buffer,
	 (uint32_t) 0x00000000UL );

	FSAPFS_TEST_ASSERT_EQUAL_UINT8(
	 "bit_stream->bit_buffer_size",
	 bit_stream->bit_buffer_size,
	 (uint8_t) 0 );

	result = libfsapfs_bit_stream_get_value(
	          bit_stream,
	          32,
	          &value_32bit,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "value_32bit",
	 value_32bit,
	 (uint32_t) 0x8f6d59bdUL );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "bit_stream->byte_stream_offset",
	 bit_stream->byte_stream_offset,
	 (size_t) 6 );

	FSAPFS_TEST_ASSERT_EQUAL_UINT32(
	 "bit_stream->bit_buffer",
	 bit_stream->bit_buffer,
	 (uint32_t) 0x00000000UL );

	FSAPFS_TEST_ASSERT_EQUAL_UINT8(
	 "bit_stream->bit_buffer_size",
	 bit_stream->bit_buffer_size,
	 (uint8_t) 0 );

	/* Test error cases
	 */
	result = libfsapfs_bit_stream_get_value(
	          NULL,
	          32,
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

	result = libfsapfs_bit_stream_get_value(
	          bit_stream,
	          64,
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

	result = libfsapfs_bit_stream_get_value(
	          bit_stream,
	          32,
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

	bit_stream->byte_stream_offset = 16;
        bit_stream->bit_buffer_size    = 0;

	result = libfsapfs_bit_stream_get_value(
	          bit_stream,
	          32,
	          &value_32bit,
	          &error );

	bit_stream->byte_stream_offset = 0;

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
	result = libfsapfs_bit_stream_free(
	          &bit_stream,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( bit_stream != NULL )
	{
		libfsapfs_bit_stream_free(
		 &bit_stream,
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

#if defined( HAVE_DEBUG_OUTPUT ) && defined( FSAPFS_TEST_BIT_STREAM_VERBOSE )
	libcnotify_verbose_set(
	 1 );
	libcnotify_stream_set(
	 stderr,
	 NULL );
#endif

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

	FSAPFS_TEST_RUN(
	 "libfsapfs_bit_stream_initialize",
	 fsapfs_test_bit_stream_initialize );

	FSAPFS_TEST_RUN(
	 "libfsapfs_bit_stream_free",
	 fsapfs_test_bit_stream_free );

	FSAPFS_TEST_RUN(
	 "libfsapfs_bit_stream_get_value",
	 fsapfs_test_bit_stream_get_value );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */
}

