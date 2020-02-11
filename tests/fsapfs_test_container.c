/*
 * Library container type test program
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
#include "fsapfs_test_libbfio.h"
#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libclocale.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_libuna.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"

#include "../libfsapfs/libfsapfs_container.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make fsapfs_test_container generate verbose output
#define FSAPFS_TEST_CONTAINER_VERBOSE
 */

#if !defined( LIBFSAPFS_HAVE_BFIO )

LIBFSAPFS_EXTERN \
int libfsapfs_check_container_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_open_file_io_handle(
     libfsapfs_container_t *container,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsapfs_error_t **error );

#endif /* !defined( LIBFSAPFS_HAVE_BFIO ) */

/* Creates and opens a source container
 * Returns 1 if successful or -1 on error
 */
int fsapfs_test_container_open_source(
     libfsapfs_container_t **container,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "fsapfs_test_container_open_source";
	int result            = 0;

	if( container == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_initialize(
	     container,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize container.",
		 function );

		goto on_error;
	}
	result = libfsapfs_container_open_file_io_handle(
	          *container,
	          file_io_handle,
	          LIBFSAPFS_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open container.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *container != NULL )
	{
		libfsapfs_container_free(
		 container,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source container
 * Returns 1 if successful or -1 on error
 */
int fsapfs_test_container_close_source(
     libfsapfs_container_t **container,
     libcerror_error_t **error )
{
	static char *function = "fsapfs_test_container_close_source";
	int result            = 0;

	if( container == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_close(
	     *container,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close container.",
		 function );

		result = -1;
	}
	if( libfsapfs_container_free(
	     container,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free container.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libfsapfs_container_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_initialize(
     void )
{
	libcerror_error_t *error         = NULL;
	libfsapfs_container_t *container = NULL;
	int result                       = 0;

#if defined( HAVE_FSAPFS_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 1;
	int number_of_memset_fail_tests  = 1;
	int test_number                  = 0;
#endif

	/* Test regular cases
	 */
	result = libfsapfs_container_initialize(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "container",
	 container );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_container_free(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "container",
	 container );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsapfs_container_initialize(
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

	container = (libfsapfs_container_t *) 0x12345678UL;

	result = libfsapfs_container_initialize(
	          &container,
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

	container = NULL;

#if defined( HAVE_FSAPFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsapfs_container_initialize with malloc failing
		 */
		fsapfs_test_malloc_attempts_before_fail = test_number;

		result = libfsapfs_container_initialize(
		          &container,
		          &error );

		if( fsapfs_test_malloc_attempts_before_fail != -1 )
		{
			fsapfs_test_malloc_attempts_before_fail = -1;

			if( container != NULL )
			{
				libfsapfs_container_free(
				 &container,
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
			 "container",
			 container );

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
		/* Test libfsapfs_container_initialize with memset failing
		 */
		fsapfs_test_memset_attempts_before_fail = test_number;

		result = libfsapfs_container_initialize(
		          &container,
		          &error );

		if( fsapfs_test_memset_attempts_before_fail != -1 )
		{
			fsapfs_test_memset_attempts_before_fail = -1;

			if( container != NULL )
			{
				libfsapfs_container_free(
				 &container,
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
			 "container",
			 container );

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
	if( container != NULL )
	{
		libfsapfs_container_free(
		 &container,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_container_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_container_free(
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

/* Tests the libfsapfs_container_open function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error         = NULL;
	libfsapfs_container_t *container = NULL;
	int result                       = 0;

	/* Initialize test
	 */
	result = fsapfs_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_container_initialize(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "container",
	 container );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libfsapfs_container_open(
	          container,
	          narrow_source,
	          LIBFSAPFS_OPEN_READ,
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
	result = libfsapfs_container_open(
	          container,
	          narrow_source,
	          LIBFSAPFS_OPEN_READ,
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
	result = libfsapfs_container_free(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "container",
	 container );

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
	if( container != NULL )
	{
		libfsapfs_container_free(
		 &container,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libfsapfs_container_open_wide function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error         = NULL;
	libfsapfs_container_t *container = NULL;
	int result                       = 0;

	/* Initialize test
	 */
	result = fsapfs_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_container_initialize(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "container",
	 container );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libfsapfs_container_open_wide(
	          container,
	          wide_source,
	          LIBFSAPFS_OPEN_READ,
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
	result = libfsapfs_container_open_wide(
	          container,
	          wide_source,
	          LIBFSAPFS_OPEN_READ,
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
	result = libfsapfs_container_free(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "container",
	 container );

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
	if( container != NULL )
	{
		libfsapfs_container_free(
		 &container,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libfsapfs_container_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_open_file_io_handle(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libfsapfs_container_t *container = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
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
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
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

	result = libfsapfs_container_initialize(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "container",
	 container );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsapfs_container_open_file_io_handle(
	          container,
	          file_io_handle,
	          LIBFSAPFS_OPEN_READ,
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
	result = libfsapfs_container_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBFSAPFS_OPEN_READ,
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

	result = libfsapfs_container_open_file_io_handle(
	          container,
	          NULL,
	          LIBFSAPFS_OPEN_READ,
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

	result = libfsapfs_container_open_file_io_handle(
	          container,
	          file_io_handle,
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

	/* Test open when already opened
	 */
	result = libfsapfs_container_open_file_io_handle(
	          container,
	          file_io_handle,
	          LIBFSAPFS_OPEN_READ,
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
	result = libfsapfs_container_free(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "container",
	 container );

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( container != NULL )
	{
		libfsapfs_container_free(
		 &container,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_container_close function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_container_close(
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

/* Tests the libfsapfs_container_open and libfsapfs_container_close functions
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error         = NULL;
	libfsapfs_container_t *container = NULL;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfsapfs_container_initialize(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "container",
	 container );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_container_open_wide(
	          container,
	          source,
	          LIBFSAPFS_OPEN_READ,
	          &error );
#else
	result = libfsapfs_container_open(
	          container,
	          source,
	          LIBFSAPFS_OPEN_READ,
	          &error );
#endif

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_container_close(
	          container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_container_open_wide(
	          container,
	          source,
	          LIBFSAPFS_OPEN_READ,
	          &error );
#else
	result = libfsapfs_container_open(
	          container,
	          source,
	          LIBFSAPFS_OPEN_READ,
	          &error );
#endif

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsapfs_container_close(
	          container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsapfs_container_free(
	          &container,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "container",
	 container );

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
	if( container != NULL )
	{
		libfsapfs_container_free(
		 &container,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsapfs_container_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_signal_abort(
     libfsapfs_container_t *container )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsapfs_container_signal_abort(
	          container,
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
	result = libfsapfs_container_signal_abort(
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

/* Tests the libfsapfs_container_get_size function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_get_size(
     libfsapfs_container_t *container )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsapfs_container_get_size(
	          container,
	          &size,
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
	result = libfsapfs_container_get_size(
	          NULL,
	          &size,
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

	result = libfsapfs_container_get_size(
	          container,
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

/* Tests the libfsapfs_container_get_number_of_volumes function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_container_get_number_of_volumes(
     libfsapfs_container_t *container )
{
	libcerror_error_t *error = NULL;
	int number_of_volumes    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsapfs_container_get_number_of_volumes(
	          container,
	          &number_of_volumes,
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
	result = libfsapfs_container_get_number_of_volumes(
	          NULL,
	          &number_of_volumes,
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

	result = libfsapfs_container_get_number_of_volumes(
	          container,
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
	libbfio_handle_t *file_io_handle  = NULL;
	libcerror_error_t *error          = NULL;
	libfsapfs_container_t *container  = NULL;
	system_character_t *option_offset = NULL;
	system_character_t *source        = NULL;
	system_integer_t option           = 0;
	size_t string_length              = 0;
	off64_t volume_offset             = 0;
	int result                        = 0;

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
#if defined( HAVE_DEBUG_OUTPUT ) && defined( FSAPFS_TEST_CONTAINER_VERBOSE )
	libfsapfs_notify_set_verbose(
	 1 );
	libfsapfs_notify_set_stream(
	 stderr,
	 NULL );
#endif

	FSAPFS_TEST_RUN(
	 "libfsapfs_container_initialize",
	 fsapfs_test_container_initialize );

	FSAPFS_TEST_RUN(
	 "libfsapfs_container_free",
	 fsapfs_test_container_free );

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

		result = libfsapfs_check_container_signature_file_io_handle(
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
	if( ( result != 0 )
	 && ( volume_offset == 0 ) )
	{
		FSAPFS_TEST_RUN_WITH_ARGS(
		 "libfsapfs_container_open",
		 fsapfs_test_container_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		FSAPFS_TEST_RUN_WITH_ARGS(
		 "libfsapfs_container_open_wide",
		 fsapfs_test_container_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		FSAPFS_TEST_RUN_WITH_ARGS(
		 "libfsapfs_container_open_file_io_handle",
		 fsapfs_test_container_open_file_io_handle,
		 source );

		FSAPFS_TEST_RUN(
		 "libfsapfs_container_close",
		 fsapfs_test_container_close );

		FSAPFS_TEST_RUN_WITH_ARGS(
		 "libfsapfs_container_open_close",
		 fsapfs_test_container_open_close,
		 source );
	}
	if( result != 0 )
	{
		/* Initialize test
		 */
		result = fsapfs_test_container_open_source(
		          &container,
		          file_io_handle,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	         "container",
	         container );

	        FSAPFS_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		FSAPFS_TEST_RUN_WITH_ARGS(
		 "libfsapfs_container_signal_abort",
		 fsapfs_test_container_signal_abort,
		 container );

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

		/* TODO: add tests for libfsapfs_internal_container_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

		FSAPFS_TEST_RUN_WITH_ARGS(
		 "libfsapfs_container_get_size",
		 fsapfs_test_container_get_size,
		 container );

		/* TODO: add tests for libfsapfs_container_get_identifier */

		FSAPFS_TEST_RUN_WITH_ARGS(
		 "libfsapfs_container_get_number_of_volumes",
		 fsapfs_test_container_get_number_of_volumes,
		 container );

		/* TODO: add tests for libfsapfs_container_get_volume_by_index */

		/* Clean up
		 */
		result = fsapfs_test_container_close_source(
		          &container,
		          &error );

		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		FSAPFS_TEST_ASSERT_IS_NULL(
	         "container",
	         container );

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

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( container != NULL )
	{
		fsapfs_test_container_close_source(
		 &container,
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

