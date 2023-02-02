/*
 * Tools signal functions test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_unused.h"

#include "../fsapfstools/fsapfstools_signal.h"

void fsapfs_test_tools_signal_handler_function(
      fsapfstools_signal_t signal FSAPFS_TEST_ATTRIBUTE_UNUSED )
{
	FSAPFS_TEST_UNREFERENCED_PARAMETER( signal )
}

#if defined( WINAPI )

/* Tests the fsapfstools_signal_handler function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_tools_signal_handler(
     void )
{
	BOOL result = 0;

	/* Test regular cases
	 */
	result = fsapfstools_signal_handler(
	          CTRL_C_EVENT );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 (int) TRUE );

	result = fsapfstools_signal_handler(
	          CTRL_LOGOFF_EVENT );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 (int) FALSE );

	return( 1 );

on_error:
	return( 0 );
}

#if defined( _MSC_VER )

	/* TODO add tests for fsapfstools_signal_initialize_memory_debug */

#endif /* defined( _MSC_VER ) */

#endif /* defined( WINAPI ) */

/* Tests the fsapfstools_signal_attach function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_tools_signal_attach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = fsapfstools_signal_attach(
	          fsapfs_test_tools_signal_handler_function,
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
	result = fsapfstools_signal_attach(
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

/* Tests the fsapfstools_signal_detach function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_tools_signal_detach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = fsapfstools_signal_detach(
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

#if defined( WINAPI )

	FSAPFS_TEST_RUN(
	 "fsapfstools_signal_handler",
	 fsapfs_test_tools_signal_handler )

#if defined( _MSC_VER )

	/* TODO add tests for fsapfstools_signal_initialize_memory_debug */

#endif /* defined( _MSC_VER ) */

#endif /* defined( WINAPI ) */

	FSAPFS_TEST_RUN(
	 "fsapfstools_signal_attach",
	 fsapfs_test_tools_signal_attach )

	FSAPFS_TEST_RUN(
	 "fsapfstools_signal_detach",
	 fsapfs_test_tools_signal_detach )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

