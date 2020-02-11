/*
 * Input/Output (IO) handle functions
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
#include <memory.h>
#include <types.h>

#include "libfsapfs_io_handle.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_profiler.h"

const char fsapfs_container_signature[ 4 ] = "NXSB";
const char fsapfs_volume_signature[ 4 ]    = "APSB";

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_io_handle_initialize(
     libfsapfs_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_io_handle_initialize";

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
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libfsapfs_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libfsapfs_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		memory_free(
		 *io_handle );

		*io_handle = NULL;

		return( -1 );
	}
#if defined( HAVE_PROFILER )
	if( libfsapfs_profiler_initialize(
	     &( ( *io_handle )->profiler ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize profiler.",
		 function );

		goto on_error;
	}
	if( libfsapfs_profiler_open(
	     ( *io_handle )->profiler,
	     "profiler.csv",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open profiler.",
		 function );

		goto on_error;
	}
#endif /* defined( HAVE_PROFILER ) */

	( *io_handle )->bytes_per_sector = 512;
	( *io_handle )->block_size       = 4096;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
#if defined( HAVE_PROFILER )
		if( ( *io_handle )->profiler != NULL )
		{
			libfsapfs_profiler_free(
			 &( ( *io_handle )->profiler ),
			 NULL );
		}
#endif
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_io_handle_free(
     libfsapfs_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_io_handle_free";
	int result            = 1;

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
	if( *io_handle != NULL )
	{
#if defined( HAVE_PROFILER )
		if( libfsapfs_profiler_close(
		     ( *io_handle )->profiler,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close profiler.",
			 function );

			result = -1;
		}
		if( libfsapfs_profiler_free(
		     &( ( *io_handle )->profiler ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free profiler.",
			 function );

			result = -1;
		}
#endif /* defined( HAVE_PROFILER ) */

		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_io_handle_clear(
     libfsapfs_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function          = "libfsapfs_io_handle_clear";

#if defined( HAVE_PROFILER )
	libfsapfs_profiler_t *profiler = NULL;
#endif

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
#if defined( HAVE_PROFILER )
	profiler = io_handle->profiler;
#endif
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libfsapfs_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	io_handle->bytes_per_sector = 512;
	io_handle->block_size       = 4096;

#if defined( HAVE_PROFILER )
	io_handle->profiler = profiler;
#endif
	return( 1 );
}

