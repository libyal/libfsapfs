/*
 * Snapshot functions
 *
 * Copyright (C) 2018-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_snapshot.h"
#include "libfsapfs_snapshot_metadata_tree.h"

/* Creates a snapshot
 * Make sure the value snapshot is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_snapshot_initialize(
     libfsapfs_snapshot_t **snapshot,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsapfs_internal_snapshot_t *internal_snapshot = NULL;
	static char *function                            = "libfsapfs_snapshot_initialize";

	if( snapshot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid snapshot.",
		 function );

		return( -1 );
	}
	if( *snapshot != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid snapshot value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
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
	internal_snapshot = memory_allocate_structure(
	                     libfsapfs_internal_snapshot_t );

	if( internal_snapshot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create snapshot.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_snapshot,
	     0,
	     sizeof( libfsapfs_internal_snapshot_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear snapshot.",
		 function );

		goto on_error;
	}
/* TODO clone file_io_handle? */
	internal_snapshot->io_handle      = io_handle;
	internal_snapshot->file_io_handle = file_io_handle;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_snapshot->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*snapshot = (libfsapfs_snapshot_t *) internal_snapshot;

	return( 1 );

on_error:
	if( internal_snapshot != NULL )
	{
		memory_free(
		 internal_snapshot );
	}
	return( -1 );
}

/* Frees a snapshot
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_snapshot_free(
     libfsapfs_snapshot_t **snapshot,
     libcerror_error_t **error )
{
	libfsapfs_internal_snapshot_t *internal_snapshot = NULL;
	static char *function                            = "libfsapfs_snapshot_free";
	int result                                       = 1;

	if( snapshot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid snapshot.",
		 function );

		return( -1 );
	}
	if( *snapshot != NULL )
	{
		internal_snapshot = (libfsapfs_internal_snapshot_t *) *snapshot;
		*snapshot         = NULL;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_snapshot->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		memory_free(
		 internal_snapshot );
	}
	return( result );
}

/* Opens a snapshot for reading
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_internal_snapshot_open_read(
     libfsapfs_internal_snapshot_t *internal_snapshot,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_internal_snapshot_open_read";

	if( internal_snapshot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid snapshot.",
		 function );

		return( -1 );
	}
	if( internal_snapshot->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid snapshot - missing IO handle.",
		 function );

		return( -1 );
	}
/* TODO  */
	return( -1 );
}

