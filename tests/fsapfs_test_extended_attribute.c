/*
 * Library extended_attribute type test program
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

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_extended_attribute.h"

#define FSAPFS_TEST_EXTENDED_ATTRIBUTE_READ_BUFFER_SIZE	4096

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

/* Tests the libfsapfs_extended_attribute_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_extended_attribute_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_extended_attribute_free(
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

	/* TODO: add tests for libfsapfs_extended_attribute_initialize */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	FSAPFS_TEST_RUN(
	 "libfsapfs_extended_attribute_free",
	 fsapfs_test_extended_attribute_free );

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

	/* TODO: add tests for libfsapfs_internal_extended_attribute_free */

	/* TODO: add tests for libfsapfs_extended_attribute_read_key_data */

	/* TODO: add tests for libfsapfs_extended_attribute_read_value_data */

	/* TODO: add tests for libfsapfs_extended_attribute_get_data_stream */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	/* TODO: add tests for libfsapfs_extended_attribute_get_identifier */

	/* TODO: add tests for libfsapfs_extended_attribute_get_utf8_name_size */

	/* TODO: add tests for libfsapfs_extended_attribute_get_utf8_name */

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

	/* TODO: add tests for libfsapfs_extended_attribute_compare_name_with_utf8_string */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	/* TODO: add tests for libfsapfs_extended_attribute_get_utf16_name_size */

	/* TODO: add tests for libfsapfs_extended_attribute_get_utf16_name */

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

	/* TODO: add tests for libfsapfs_extended_attribute_compare_name_with_utf16_string */

	/* TODO: add tests for libfsapfs_internal_extended_attribute_get_file_extents */

	/* TODO: add tests for libfsapfs_internal_extended_attribute_get_data_stream */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	/* TODO: add tests for libfsapfs_extended_attribute_read_buffer */

	/* TODO: add tests for libfsapfs_extended_attribute_read_buffer_at_offset */

	/* TODO: add tests for libfsapfs_extended_attribute_seek_offset */

	/* TODO: add tests for libfsapfs_extended_attribute_get_offset */

	/* TODO: add tests for libfsapfs_extended_attribute_get_size */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

