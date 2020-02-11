/*
 * Library volume type test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfs_test_functions.h"
#include "fsapfs_test_getopt.h"
#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libclocale.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_libuna.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"

#include "../libfsapfs/libfsapfs_volume.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make fsapfs_test_volume generate verbose output
#define FSAPFS_TEST_VOLUME_VERBOSE
 */

#if !defined( LIBFSAPFS_HAVE_BFIO )

LIBFSAPFS_EXTERN \
int libfsapfs_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif /* !defined( LIBFSAPFS_HAVE_BFIO ) */

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_volume_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_volume_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfsapfs_volume_t *volume      = NULL;
	int result                      = 0;

#if defined( HAVE_FSAPFS_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
#ifdef TODO
/* TODO create file_io_handle of container */

	result = libfsapfs_volume_initialize(
	          &volume,
	          NULL,
	          NULL,
	          NULL,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_volume_free(
	          &volume,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* TODO */

	/* Test error cases
	 */
	result = libfsapfs_volume_initialize(
	          NULL,
	          NULL,
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

	volume = (libfsapfs_volume_t *) 0x12345678UL;

	result = libfsapfs_volume_initialize(
	          &volume,
	          NULL,
	          NULL,
	          NULL,
	          &error );

	volume = NULL;

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#ifdef TODO
/* TODO create file_io_handle of container */

#if defined( HAVE_FSAPFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsapfs_volume_initialize with malloc failing
		 */
		fsapfs_test_malloc_attempts_before_fail = test_number;

		result = libfsapfs_volume_initialize(
		          &volume,
		          NULL,
		          NULL,
		          NULL,
		          &error );

		if( fsapfs_test_malloc_attempts_before_fail != -1 )
		{
			fsapfs_test_malloc_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libfsapfs_volume_free(
				 &volume,
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
			 "volume",
			 volume );

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
		/* Test libfsapfs_volume_initialize with memset failing
		 */
		fsapfs_test_memset_attempts_before_fail = test_number;

		result = libfsapfs_volume_initialize(
		          &volume,
		          NULL,
		          NULL,
		          NULL,
		          &error );

		if( fsapfs_test_memset_attempts_before_fail != -1 )
		{
			fsapfs_test_memset_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libfsapfs_volume_free(
				 &volume,
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
			 "volume",
			 volume );

			FSAPFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) */

#endif /* TODO */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

/* Tests the libfsapfs_volume_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_volume_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_volume_free(
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

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle    = NULL;
	libcerror_error_t *error            = NULL;
	libfsapfs_volume_t *volume          = NULL;
	system_character_t *option_offset   = NULL;
	system_character_t *option_password = NULL;
	system_character_t *source          = NULL;
	system_integer_t option             = 0;
	size_t string_length                = 0;
	off64_t volume_offset               = 0;
	int result                          = 0;

	while( ( option = fsapfs_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "o:p:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (system_integer_t) 'o':
				option_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_password = optarg;

				break;
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
	if( option_offset != NULL )
	{
		string_length = system_string_length(
		                 option_offset );

		result = fsapfs_test_system_string_copy_from_64_bit_in_decimal(
		          option_offset,
		          string_length + 1,
		          (uint64_t *) &volume_offset,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FSAPFS_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( FSAPFS_TEST_VOLUME_VERBOSE )
	libfsapfs_notify_set_verbose(
	 1 );
	libfsapfs_notify_set_stream(
	 stderr,
	 NULL );
#endif

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

	FSAPFS_TEST_RUN(
	 "libfsapfs_volume_initialize",
	 fsapfs_test_volume_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	FSAPFS_TEST_RUN(
	 "libfsapfs_volume_free",
	 fsapfs_test_volume_free );

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_range_initialize(
		          &file_io_handle,
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

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_range_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_range_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FSAPFS_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libbfio_file_range_set(
		          file_io_handle,
		          volume_offset,
		          0,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FSAPFS_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libfsapfs_check_volume_signature_file_io_handle(
		          file_io_handle,
		          &error );

		FSAPFS_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( result != 0 )
	{
		/* Initialize test
		 */
/* TODO get first volume from container */

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

		/* TODO: add tests for libfsapfs_internal_volume_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

		/* TODO add tests for libfsapfs_volume_get_identifier */

		/* TODO add tests for libfsapfs_volume_get_utf8_name_size */

		/* TODO add tests for libfsapfs_volume_get_utf8_name */

		/* TODO add tests for libfsapfs_volume_get_utf16_name_size */

		/* TODO add tests for libfsapfs_volume_get_utf16_name */

		/* Clean up
		 */
		result = libfsapfs_volume_free(
		          &volume,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSAPFS_TEST_ASSERT_IS_NULL(
	         "volume",
	         volume );

	        FSAPFS_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

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
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

