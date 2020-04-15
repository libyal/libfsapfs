/*
 * Library snapshot_metadata_tree type test program
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
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_snapshot_metadata_tree.h"

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_snapshot_metadata_tree_free function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_snapshot_metadata_tree_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsapfs_snapshot_metadata_tree_free(
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

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_initialize */

	FSAPFS_TEST_RUN(
	 "libfsapfs_snapshot_metadata_tree_free",
	 fsapfs_test_snapshot_metadata_tree_free );

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_sub_node_block_number_from_entry */

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_root_node */

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_sub_node */

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_entry_from_node_by_identifier */

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_entry_by_identifier */

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_metadata_by_object_identifier */

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_snapshots_from_leaf_node */

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_snapshots_from_branch_node */

	/* TODO: add tests for libfsapfs_snapshot_metadata_tree_get_snapshots */

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

