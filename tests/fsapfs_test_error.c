/*
 * Library error functions test program
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
#include "fsapfs_test_unused.h"

/* Tests the libfsapfs_error_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_error_free(
     void )
{
	/* Test invocation of function only
	 */
	libfsapfs_error_free(
	 NULL );

	return( 1 );
}

/* Tests the libfsapfs_error_fprint function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_error_fprint(
     void )
{
	/* Test invocation of function only
	 */
	libfsapfs_error_fprint(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libfsapfs_error_sprint function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_error_sprint(
     void )
{
	/* Test invocation of function only
	 */
	libfsapfs_error_sprint(
	 NULL,
	 NULL,
	 0 );

	return( 1 );
}

/* Tests the libfsapfs_error_backtrace_fprint function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_error_backtrace_fprint(
     void )
{
	/* Test invocation of function only
	 */
	libfsapfs_error_backtrace_fprint(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libfsapfs_error_backtrace_sprint function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_error_backtrace_sprint(
     void )
{
	/* Test invocation of function only
	 */
	libfsapfs_error_backtrace_sprint(
	 NULL,
	 NULL,
	 0 );

	return( 1 );
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

	FSAPFS_TEST_RUN(
	 "libfsapfs_error_free",
	 fsapfs_test_error_free );

	FSAPFS_TEST_RUN(
	 "libfsapfs_error_fprint",
	 fsapfs_test_error_fprint );

	FSAPFS_TEST_RUN(
	 "libfsapfs_error_sprint",
	 fsapfs_test_error_sprint );

	FSAPFS_TEST_RUN(
	 "libfsapfs_error_backtrace_fprint",
	 fsapfs_test_error_backtrace_fprint );

	FSAPFS_TEST_RUN(
	 "libfsapfs_error_backtrace_sprint",
	 fsapfs_test_error_backtrace_sprint );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

