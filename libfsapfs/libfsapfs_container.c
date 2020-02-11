/*
 * Container functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsapfs_checkpoint_map.h"
#include "libfsapfs_container.h"
#include "libfsapfs_container_data_handle.h"
#include "libfsapfs_container_key_bag.h"
#include "libfsapfs_container_reaper.h"
#include "libfsapfs_space_manager.h"
#include "libfsapfs_container_superblock.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_fusion_middle_tree.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_object.h"
#include "libfsapfs_object_map.h"
#include "libfsapfs_object_map_btree.h"
#include "libfsapfs_object_map_descriptor.h"
#include "libfsapfs_volume.h"

/* Creates a container
 * Make sure the value container is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_initialize(
     libfsapfs_container_t **container,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_initialize";

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
	if( *container != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container value already set.",
		 function );

		return( -1 );
	}
	internal_container = memory_allocate_structure(
	                      libfsapfs_internal_container_t );

	if( internal_container == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create container.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_container,
	     0,
	     sizeof( libfsapfs_internal_container_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear container.",
		 function );

		goto on_error;
	}
	if( libfsapfs_io_handle_initialize(
	     &( internal_container->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_container->read_write_lock ),
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
	*container = (libfsapfs_container_t *) internal_container;

	return( 1 );

on_error:
	if( internal_container != NULL )
	{
		memory_free(
		 internal_container );
	}
	return( -1 );
}

/* Frees a container
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_free(
     libfsapfs_container_t **container,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_free";
	int result                                         = 1;

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
	if( *container != NULL )
	{
		internal_container = (libfsapfs_internal_container_t *) *container;

		if( internal_container->file_io_handle != NULL )
		{
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
		}
		*container = NULL;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_container->read_write_lock ),
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
		if( libfsapfs_io_handle_free(
		     &( internal_container->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_container );
	}
	return( result );
}

/* Signals the container to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_signal_abort(
     libfsapfs_container_t *container,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_signal_abort";

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
	internal_container = (libfsapfs_internal_container_t *) container;

	if( internal_container->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid container - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_container->io_handle->abort = 1;

	return( 1 );
}

/* Opens a container
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_open(
     libfsapfs_container_t *container,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                   = NULL;
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_open";
	size_t filename_length                             = 0;

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
	internal_container = (libfsapfs_internal_container_t *) container;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFSAPFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSAPFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSAPFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = narrow_string_length(
	                   filename );

	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libfsapfs_container_open_file_io_handle(
	     container,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open container: %s.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_container->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_container->file_io_handle_created_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
        return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a container
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_open_wide(
     libfsapfs_container_t *container,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                   = NULL;
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_open_wide";
	size_t filename_length                             = 0;

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
	internal_container = (libfsapfs_internal_container_t *) container;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFSAPFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSAPFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSAPFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = wide_string_length(
	                   filename );

	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libfsapfs_container_open_file_io_handle(
	     container,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open container: %ls.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_container->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_container->file_io_handle_created_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
        return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a container using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_open_file_io_handle(
     libfsapfs_container_t *container,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_open_file_io_handle";
	int bfio_access_flags                              = 0;
	int file_io_handle_is_open                         = 0;
	int file_io_handle_opened_in_library               = 0;

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
	internal_container = (libfsapfs_internal_container_t *) container;

	if( internal_container->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container - file IO handle already set.",
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
	if( ( ( access_flags & LIBFSAPFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSAPFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSAPFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSAPFS_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open container.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
	if( libfsapfs_internal_container_open_read(
	     internal_container,
	     file_io_handle,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_container->file_io_handle                   = file_io_handle;
	internal_container->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_container->file_io_handle                   = NULL;
		internal_container->file_io_handle_opened_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );
	}
	return( -1 );
}

/* Closes a container
 * Returns 0 if successful or -1 on error
 */
int libfsapfs_container_close(
     libfsapfs_container_t *container,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_close";
	int result                                         = 0;

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
	internal_container = (libfsapfs_internal_container_t *) container;

	if( internal_container->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid container - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_container->file_io_handle_created_in_library != 0 )
		{
			if( libfsapfs_debug_print_read_offsets(
			     internal_container->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_container->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_container->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_container->file_io_handle_opened_in_library = 0;
	}
	if( internal_container->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_container->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_container->file_io_handle_created_in_library = 0;
	}
	internal_container->file_io_handle = NULL;

	if( libfsapfs_io_handle_clear(
	     internal_container->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( internal_container->superblock != NULL )
	{
		if( libfsapfs_container_superblock_free(
		     &( internal_container->superblock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free container superblock.",
			 function );

			result = -1;
		}
	}
	if( internal_container->fusion_middle_tree != NULL )
	{
		if( libfsapfs_fusion_middle_tree_free(
		     &( internal_container->fusion_middle_tree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free Fusion middle tree.",
			 function );

			result = -1;
		}
	}
	if( internal_container->checkpoint_map != NULL )
	{
		if( libfsapfs_checkpoint_map_free(
		     &( internal_container->checkpoint_map ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free checkpoint map.",
			 function );

			result = -1;
		}
	}
	if( libfdata_vector_free(
	     &( internal_container->data_block_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free data block vector.",
		 function );

		result = -1;
	}
	if( internal_container->object_map_btree != NULL )
	{
		if( libfsapfs_object_map_btree_free(
		     &( internal_container->object_map_btree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free object map B-tree.",
			 function );

			result = -1;
		}
	}
	if( internal_container->key_bag != NULL )
	{
		if( libfsapfs_container_key_bag_free(
		     &( internal_container->key_bag ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key bag.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Opens a container for reading
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_internal_container_open_read(
     libfsapfs_internal_container_t *internal_container,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsapfs_container_data_handle_t *container_data_handle    = NULL;
	libfsapfs_container_superblock_t *container_superblock      = NULL;
	libfsapfs_container_superblock_t *container_superblock_swap = NULL;
	libfsapfs_object_t *object                                  = NULL;
	libfsapfs_object_map_t *object_map                          = NULL;
	static char *function                                       = "libfsapfs_internal_container_open_read";
	uint64_t checkpoint_map_block_number                        = 0;
	uint64_t checkpoint_map_transaction_identifier              = 0;
	uint64_t metadata_block_index                               = 0;
	int element_index                                           = 0;
	int result                                                  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libfsapfs_checkpoint_map_t *checkpoint_map                  = NULL;
	libfsapfs_container_reaper_t *container_reaper              = NULL;
	libfsapfs_space_manager_t *space_manager                    = NULL;
	uint64_t reaper_block_number                                = 0;
	uint64_t space_manager_block_number                         = 0;
#endif

	if( internal_container == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container.",
		 function );

		return( -1 );
	}
	if( internal_container->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid container - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_container->superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container - superblock map value already set.",
		 function );

		return( -1 );
	}
	if( internal_container->fusion_middle_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container - Fusion middle tree value already set.",
		 function );

		return( -1 );
	}
	if( internal_container->checkpoint_map != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container - checkpoint map value already set.",
		 function );

		return( -1 );
	}
	if( internal_container->data_block_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container - data block vector already set.",
		 function );

		return( -1 );
	}
	if( internal_container->object_map_btree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container - object map B-tree value already set.",
		 function );

		return( -1 );
	}
	if( internal_container->key_bag != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container - key bag value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading container superblock:\n" );
	}
#endif
	if( libfsapfs_container_superblock_initialize(
	     &( internal_container->superblock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create container superblock.",
		 function );

		goto on_error;
	}
	if( libfsapfs_container_superblock_read_file_io_handle(
	     internal_container->superblock,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container superblock at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	internal_container->io_handle->block_size     = internal_container->superblock->block_size;
	internal_container->io_handle->container_size = (size64_t) internal_container->superblock->number_of_blocks * (size64_t) internal_container->io_handle->block_size;

#if !defined( HAVE_DEBUG_OUTPUT )
	if( ( internal_container->superblock->incompatible_features_flags & 0x0000000000000100UL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: Fusion drive not supported.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Scanning checkpoint descriptor area:\n" );
	}
#endif
	if( libfsapfs_object_initialize(
	     &object,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create object.",
		 function );

		goto on_error;
	}
	file_offset = internal_container->superblock->checkpoint_descriptor_area_block_number * internal_container->io_handle->block_size;

	for( metadata_block_index = 0;
	     metadata_block_index <= internal_container->superblock->checkpoint_descriptor_area_number_of_blocks;
	     metadata_block_index++ )
	{
		if( libfsapfs_object_read_file_io_handle(
		     object,
		     file_io_handle,
		     file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read object at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 file_offset,
			 file_offset );

			goto on_error;
		}
		switch( object->type )
		{
			case 0x4000000c:
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "Reading checkpoint map:\n" );

					if( libfsapfs_checkpoint_map_initialize(
					     &checkpoint_map,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create backup checkpoint map.",
						 function );

						goto on_error;
					}
					if( libfsapfs_checkpoint_map_read_file_io_handle(
					     checkpoint_map,
					     file_io_handle,
					     file_offset,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read backup checkpoint map at offset: %" PRIi64 " (0x%08" PRIx64 ").",
						 function,
						 file_offset,
						 file_offset );

						goto on_error;
					}
					if( libfsapfs_checkpoint_map_free(
					     &checkpoint_map,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free backup checkpoint map.",
						 function );

						goto on_error;
					}
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
				if( object->transaction_identifier > checkpoint_map_transaction_identifier )
				{
					checkpoint_map_block_number           = internal_container->superblock->checkpoint_descriptor_area_block_number + metadata_block_index;
					checkpoint_map_transaction_identifier = object->transaction_identifier;
				}
				break;

			case 0x80000001:
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "Reading container superblock:\n" );
				}
#endif
				if( libfsapfs_container_superblock_initialize(
				     &container_superblock,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create backup container superblock.",
					 function );

					goto on_error;
				}
				if( libfsapfs_container_superblock_read_file_io_handle(
				     container_superblock,
				     file_io_handle,
				     file_offset,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read backup container superblock at offset: %" PRIi64 " (0x%08" PRIx64 ").",
					 function,
					 file_offset,
					 file_offset );

					goto on_error;
				}
				if( container_superblock->object_transaction_identifier > internal_container->superblock->object_transaction_identifier )
				{
					container_superblock_swap      = internal_container->superblock;
					internal_container->superblock = container_superblock;
					container_superblock           = container_superblock_swap;
				}
				if( libfsapfs_container_superblock_free(
				     &container_superblock,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free backup container superblock.",
					 function );

					goto on_error;
				}
				break;

			default:
				break;
		}
		file_offset += internal_container->io_handle->block_size;
	}
	if( libfsapfs_object_free(
	     &object,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free object.",
		 function );

		goto on_error;
	}
	if( checkpoint_map_block_number == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing checkpoint map block number.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading checkpoint map:\n" );
	}
#endif
	if( libfsapfs_checkpoint_map_initialize(
	     &( internal_container->checkpoint_map ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create checkpoint map.",
		 function );

		goto on_error;
	}
	file_offset = checkpoint_map_block_number * internal_container->io_handle->block_size;

	if( libfsapfs_checkpoint_map_read_file_io_handle(
	     internal_container->checkpoint_map,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read checkpoint map at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_container->superblock->fusion_middle_tree_block_number != 0 )
		{
			libcnotify_printf(
			 "Reading Fusion middle tree:\n" );

			file_offset = internal_container->superblock->fusion_middle_tree_block_number * internal_container->io_handle->block_size;

			if( libfsapfs_fusion_middle_tree_initialize(
			     &( internal_container->fusion_middle_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create Fusion middle tree.",
				 function );

				goto on_error;
			}
			if( libfsapfs_fusion_middle_tree_read_file_io_handle(
			     internal_container->fusion_middle_tree,
			     file_io_handle,
			     file_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read Fusion middle tree at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 file_offset,
				 file_offset );

				goto on_error;
			}
		}
		if( internal_container->superblock->space_manager_object_identifier > 0 )
		{
			libcnotify_printf(
			 "Reading space manager:\n" );

			if( libfsapfs_checkpoint_map_get_physical_address_by_object_identifier(
			     internal_container->checkpoint_map,
			     internal_container->superblock->space_manager_object_identifier,
			     &space_manager_block_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine space manager block number from object identifier: 0x08%" PRIx64 ".",
				 function,
				 internal_container->superblock->space_manager_object_identifier );

				goto on_error;
			}
			file_offset = space_manager_block_number * internal_container->io_handle->block_size;

			if( libfsapfs_space_manager_initialize(
			     &space_manager,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create space manager.",
				 function );

				goto on_error;
			}
			if( libfsapfs_space_manager_read_file_io_handle(
			     space_manager,
			     file_io_handle,
			     file_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read space manager at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 file_offset,
				 file_offset );

				goto on_error;
			}
			if( libfsapfs_space_manager_free(
			     &space_manager,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free space manager.",
				 function );

				goto on_error;
			}
		}
		if( internal_container->superblock->reaper_object_identifier > 0 )
		{
			libcnotify_printf(
			 "Reading reaper:\n" );

			if( libfsapfs_checkpoint_map_get_physical_address_by_object_identifier(
			     internal_container->checkpoint_map,
			     internal_container->superblock->reaper_object_identifier,
			     &reaper_block_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine reaper block number from object identifier: 0x08%" PRIx64 ".",
				 function,
				 internal_container->superblock->reaper_object_identifier );

				goto on_error;
			}
			file_offset = reaper_block_number * internal_container->io_handle->block_size;

			if( libfsapfs_container_reaper_initialize(
			     &container_reaper,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create container reaper.",
				 function );

				goto on_error;
			}
			if( libfsapfs_container_reaper_read_file_io_handle(
			     container_reaper,
			     file_io_handle,
			     file_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read container reaper at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 file_offset,
				 file_offset );

				goto on_error;
			}
			if( libfsapfs_container_reaper_free(
			     &container_reaper,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free container reaper.",
				 function );

				goto on_error;
			}
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfsapfs_container_data_handle_initialize(
	     &container_data_handle,
	     internal_container->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create container data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_initialize(
	     &( internal_container->data_block_vector ),
	     (size64_t) internal_container->io_handle->block_size,
	     (intptr_t *) container_data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_container_data_handle_free,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsapfs_container_data_handle_read_data_block,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data block vector.",
		 function );

		goto on_error;
	}
	internal_container->container_data_handle = container_data_handle;
	container_data_handle                     = NULL;

	if( libfdata_vector_append_segment(
	     internal_container->data_block_vector,
	     &element_index,
	     0,
	     0,
	     internal_container->io_handle->container_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to data block vector.",
		 function );

		goto on_error;
	}
/* TODO refactor into function to read object map */
	if( internal_container->superblock->object_map_block_number == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing object map block number.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading object map:\n" );
	}
#endif
	file_offset = internal_container->superblock->object_map_block_number * internal_container->io_handle->block_size;

	if( libfsapfs_object_map_initialize(
	     &object_map,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create object map.",
		 function );

		goto on_error;
	}
	if( libfsapfs_object_map_read_file_io_handle(
	     object_map,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object map at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( object_map->btree_block_number == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing object map B-tree block number.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading object map B-tree:\n" );
	}
#endif
	if( libfsapfs_object_map_btree_initialize(
	     &( internal_container->object_map_btree ),
	     internal_container->io_handle,
	     internal_container->data_block_vector,
	     object_map->btree_block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create object map B-tree.",
		 function );

		goto on_error;
	}
	if( libfsapfs_object_map_free(
	     &object_map,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free object map.",
		 function );

		goto on_error;
	}
	if( ( internal_container->superblock->key_bag_block_number > 0 )
	 && ( internal_container->superblock->key_bag_number_of_blocks > 0 ) )
	{
		if( libfsapfs_container_key_bag_initialize(
		     &( internal_container->key_bag ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create container key bag.",
			 function );

			goto on_error;
		}
		file_offset = internal_container->superblock->key_bag_block_number * internal_container->io_handle->block_size;

		result = libfsapfs_container_key_bag_read_file_io_handle(
		          internal_container->key_bag,
		          internal_container->io_handle,
		          file_io_handle,
		          file_offset,
		          (size64_t) internal_container->superblock->key_bag_number_of_blocks * internal_container->io_handle->block_size,
		          internal_container->superblock->container_identifier,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read container key bag at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 file_offset,
			 file_offset );

			goto on_error;
		}
		else if( result == 0 )
		{
			internal_container->key_bag->is_locked = 1;
		}
	}
	return( 1 );

on_error:
	if( internal_container->key_bag != NULL )
	{
		libfsapfs_container_key_bag_free(
		 &( internal_container->key_bag ),
		 NULL );
	}
	if( internal_container->object_map_btree != NULL )
	{
		libfsapfs_object_map_btree_free(
		 &( internal_container->object_map_btree ),
		 NULL );
	}
	if( object_map != NULL )
	{
		libfsapfs_object_map_free(
		 &object_map,
		 NULL );
	}
	if( container_data_handle != NULL )
	{
		libfsapfs_container_data_handle_free(
		 &container_data_handle,
		 NULL );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( container_reaper != NULL )
	{
		libfsapfs_container_reaper_free(
		 &container_reaper,
		 NULL );
	}
	if( space_manager != NULL )
	{
		libfsapfs_space_manager_free(
		 &space_manager,
		 NULL );
	}
#endif
	if( internal_container->checkpoint_map != NULL )
	{
		libfsapfs_checkpoint_map_free(
		 &( internal_container->checkpoint_map ),
		 NULL );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( checkpoint_map != NULL )
	{
		libfsapfs_checkpoint_map_free(
		 &checkpoint_map,
		 NULL );
	}
	if( container_superblock != NULL )
	{
		libfsapfs_container_superblock_free(
		 &container_superblock,
		 NULL );
	}
#endif
	if( internal_container->fusion_middle_tree != NULL )
	{
		libfsapfs_fusion_middle_tree_free(
		 &( internal_container->fusion_middle_tree ),
		 NULL );
	}
	if( object != NULL )
	{
		libfsapfs_object_free(
		 &object,
		 NULL );
	}
	if( internal_container->superblock != NULL )
	{
		libfsapfs_container_superblock_free(
		 &( internal_container->superblock ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_get_size(
     libfsapfs_container_t *container,
     size64_t *size,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_get_size";

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
	internal_container = (libfsapfs_internal_container_t *) container;

	if( internal_container->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid container - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_container->superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid container - missing superblock.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*size = internal_container->io_handle->container_size;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the identifier
 * The identifier is an UUID stored in big-endian and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_get_identifier(
     libfsapfs_container_t *container,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_get_identifier";
	int result                                         = 1;

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
	internal_container = (libfsapfs_internal_container_t *) container;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsapfs_container_superblock_get_container_identifier(
	     internal_container->superblock,
	     uuid_data,
	     uuid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve container identifier.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if the container is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
int libfsapfs_container_is_locked(
     libfsapfs_container_t *container,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_is_locked";
	uint8_t is_locked                                  = 0;

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
	internal_container = (libfsapfs_internal_container_t *) container;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_container->key_bag != NULL )
	{
		is_locked = internal_container->key_bag->is_locked;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( is_locked );
}

/* Retrieves the number of volumes
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_get_number_of_volumes(
     libfsapfs_container_t *container,
     int *number_of_volumes,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container = NULL;
	static char *function                              = "libfsapfs_container_get_number_of_volumes";

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
	internal_container = (libfsapfs_internal_container_t *) container;

	if( internal_container->superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid container - missing superblock.",
		 function );

		return( -1 );
	}
	if( number_of_volumes == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of volumes.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*number_of_volumes = internal_container->superblock->number_of_volumes;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves a specific of volume
 * The volume reference must be freed after use with libfsapfs_volume_free
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_get_volume_by_index(
     libfsapfs_container_t *container,
     int volume_index,
     libfsapfs_volume_t **volume,
     libcerror_error_t **error )
{
	libfsapfs_internal_container_t *internal_container       = NULL;
	libfsapfs_object_map_descriptor_t *object_map_descriptor = NULL;
	static char *function                                    = "libfsapfs_container_get_volume_by_index";
	off64_t file_offset                                      = 0;

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
	internal_container = (libfsapfs_internal_container_t *) container;

	if( internal_container->superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid container - missing superblock.",
		 function );

		return( -1 );
	}
	if( ( volume_index < 0 )
	 || ( volume_index >= (int) internal_container->superblock->number_of_volumes ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid volume index value out of bounds.",
		 function );

		return( -1 );
	}
	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsapfs_object_map_btree_get_descriptor_by_object_identifier(
	     internal_container->object_map_btree,
	     internal_container->file_io_handle,
	     internal_container->superblock->volume_object_identifiers[ volume_index ],
	     &object_map_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve object map descriptor for volume object identifier: %" PRIu64 ".",
		 function,
		 internal_container->superblock->volume_object_identifiers[ volume_index ] );

		goto on_error;
	}
	if( object_map_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid object map descriptor.",
		 function );

		goto on_error;
	}
	if( libfsapfs_volume_initialize(
	     volume,
	     internal_container->io_handle,
	     internal_container->file_io_handle,
	     internal_container->key_bag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	file_offset = (off64_t) ( object_map_descriptor->physical_address * internal_container->io_handle->block_size );

	if( libfsapfs_object_map_descriptor_free(
	     &object_map_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free object map descriptor.",
		 function );

		goto on_error;
	}
	if( libfsapfs_internal_volume_open_read(
	     (libfsapfs_internal_volume_t *) *volume,
	     internal_container->file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %d.",
		 function,
		 volume_index );

		goto on_error;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_container->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( object_map_descriptor != NULL )
	{
		libfsapfs_object_map_descriptor_free(
		 &object_map_descriptor,
		 NULL );
	}
	if( *volume != NULL )
	{
		libfsapfs_volume_free(
		 volume,
		 NULL );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_container->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

