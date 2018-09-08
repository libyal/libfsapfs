/*
 * Container functions
 *
 * Copyright (C) 2018, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsapfs_container_object_map.h"
#include "libfsapfs_container_physical_map.h"
#include "libfsapfs_container_reaper.h"
#include "libfsapfs_container_space_manager.h"
#include "libfsapfs_container_superblock.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_container.h"

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
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     narrow_string_length(
	      filename ) + 1,
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
	internal_container->file_io_handle_created_in_library = 1;

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
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     wide_string_length(
	      filename ) + 1,
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
	internal_container->file_io_handle_created_in_library = 1;

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
	if( libfsapfs_container_open_read(
	     internal_container,
	     file_io_handle,
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
	internal_container->file_io_handle                   = file_io_handle;
	internal_container->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

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
	if( internal_container->physical_map != NULL )
	{
		if( libfsapfs_container_physical_map_free(
		     &( internal_container->physical_map ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free container physical map.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Opens a container for reading
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_open_read(
     libfsapfs_internal_container_t *internal_container,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsapfs_container_object_map_t *container_object_map       = NULL;
	libfsapfs_container_superblock_t *container_superblock       = NULL;
	static char *function                                        = "libfsapfs_internal_container_open_read";
	off64_t file_offset                                          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libfsapfs_container_reaper_t *container_reaper               = NULL;
	libfsapfs_container_space_manager_t *container_space_manager = NULL;
	uint64_t reaper_block_number                                 = 0;
	uint64_t space_manager_block_number                          = 0;
#endif

	if( internal_container == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal container.",
		 function );

		return( -1 );
	}
	if( internal_container->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal container - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_container->superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal container - superblock map value already set.",
		 function );

		return( -1 );
	}
	if( internal_container->physical_map != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal container - physical map value already set.",
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
	internal_container->block_size       = internal_container->superblock->block_size;
	internal_container->number_of_blocks = internal_container->superblock->number_of_blocks;

	file_offset += internal_container->block_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading container physical map:\n" );
	}
#endif
	if( libfsapfs_container_physical_map_initialize(
	     &( internal_container->physical_map ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create container physical map.",
		 function );

		goto on_error;
	}
	if( libfsapfs_container_physical_map_read_file_io_handle(
	     internal_container->physical_map,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container physical map at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	file_offset += internal_container->block_size;

/* TODO read previous copies of superblock and physical map */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading backup container superblock:\n" );
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
/* TODO compare backup superblock with superblock */

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_container->superblock->space_manager_object_identifier > 0 )
		{
			libcnotify_printf(
			 "Reading space manager:\n" );

			if( libfsapfs_container_physical_map_get_physical_address_by_object_identifier(
			     internal_container->physical_map,
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
			file_offset = space_manager_block_number * internal_container->block_size;

			if( libfsapfs_container_space_manager_initialize(
			     &container_space_manager,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create container space manager.",
				 function );

				goto on_error;
			}
			if( libfsapfs_container_space_manager_read_file_io_handle(
			     container_space_manager,
			     file_io_handle,
			     file_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read container space manager at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 file_offset,
				 file_offset );

				goto on_error;
			}
			if( libfsapfs_container_space_manager_free(
			     &container_space_manager,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free container space manager.",
				 function );

				goto on_error;
			}
		}
		if( internal_container->superblock->reaper_object_identifier > 0 )
		{
			libcnotify_printf(
			 "Reading reaper:\n" );

			if( libfsapfs_container_physical_map_get_physical_address_by_object_identifier(
			     internal_container->physical_map,
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
			file_offset = reaper_block_number * internal_container->block_size;

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

	if( internal_container->superblock->object_map_block_number > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading object map:\n" );
		}
#endif
		file_offset = internal_container->superblock->object_map_block_number * internal_container->block_size;

		if( libfsapfs_container_object_map_initialize(
		     &container_object_map,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create container object map.",
			 function );

			goto on_error;
		}
		if( libfsapfs_container_object_map_read_file_io_handle(
		     container_object_map,
		     file_io_handle,
		     file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read container object map at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 file_offset,
			 file_offset );

			goto on_error;
		}
		if( libfsapfs_container_object_map_free(
		     &container_object_map,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free container object map.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( container_object_map != NULL )
	{
		libfsapfs_container_object_map_free(
		 &container_object_map,
		 NULL );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( container_reaper != NULL )
	{
		libfsapfs_container_reaper_free(
		 &container_reaper,
		 NULL );
	}
	if( container_space_manager != NULL )
	{
		libfsapfs_container_space_manager_free(
		 &container_space_manager,
		 NULL );
	}
#endif
	if( internal_container->physical_map != NULL )
	{
		libfsapfs_container_physical_map_free(
		 &( internal_container->physical_map ),
		 NULL );
	}
	if( container_superblock != NULL )
	{
		libfsapfs_container_superblock_free(
		 &container_superblock,
		 NULL );
	}
	return( -1 );
}

