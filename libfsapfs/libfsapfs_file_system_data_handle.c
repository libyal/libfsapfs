/*
 * The file system data handle functions
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

#include "libfsapfs_data_block.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_file_extent.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_profiler.h"
#include "libfsapfs_unused.h"
#include "libfsapfs_file_system_data_handle.h"

/* Creates file system data handle
 * Make sure the value file_system_data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_data_handle_initialize(
     libfsapfs_file_system_data_handle_t **file_system_data_handle,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libcdata_array_t *file_extents,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_system_data_handle_initialize";

	if( file_system_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system data handle.",
		 function );

		return( -1 );
	}
	if( *file_system_data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system data handle value already set.",
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
	*file_system_data_handle = memory_allocate_structure(
	                            libfsapfs_file_system_data_handle_t );

	if( *file_system_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system_data_handle,
	     0,
	     sizeof( libfsapfs_file_system_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system data handle.",
		 function );

		goto on_error;
	}
	( *file_system_data_handle )->io_handle          = io_handle;
	( *file_system_data_handle )->encryption_context = encryption_context;
	( *file_system_data_handle )->file_extents       = file_extents;

	return( 1 );

on_error:
	if( *file_system_data_handle != NULL )
	{
		memory_free(
		 *file_system_data_handle );

		*file_system_data_handle = NULL;
	}
	return( -1 );
}

/* Frees file system data handle
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_data_handle_free(
     libfsapfs_file_system_data_handle_t **file_system_data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_system_data_handle_free";

	if( file_system_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system data handle.",
		 function );

		return( -1 );
	}
	if( *file_system_data_handle != NULL )
	{
		memory_free(
		 *file_system_data_handle );

		*file_system_data_handle = NULL;
	}
	return( 1 );
}

/* Reads a data block
 * Callback function for a data block vector
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_data_handle_read_data_block(
     libfsapfs_file_system_data_handle_t *file_system_data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags LIBFSAPFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsapfs_data_block_t *data_block   = NULL;
	libfsapfs_file_extent_t *file_extent = NULL;
	static char *function                = "libfsapfs_file_system_data_handle_read_data_block";
	uint64_t encryption_identifier       = 0;
	int64_t file_extent_offset           = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp     = 0;
#endif

	LIBFSAPFS_UNREFERENCED_PARAMETER( read_flags );

	if( file_system_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system data handle.",
		 function );

		return( -1 );
	}
	if( file_system_data_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system data handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( element_data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfsapfs_data_block_initialize(
	     &data_block,
	     (size_t) element_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data block.",
		 function );

		goto on_error;
	}
#if defined( HAVE_PROFILER )
	if( file_system_data_handle->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_start_timing(
		     file_system_data_handle->io_handle->profiler,
		     &profiler_start_timestamp,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to start timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	if( ( element_data_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( libfsapfs_data_block_clear_data(
		     data_block,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to clear data block.",
			 function );

			goto on_error;
		}
	}
	else
	{
		encryption_identifier = element_data_offset / element_data_size;

		if( file_system_data_handle->file_extents != NULL )
		{
			if( libcdata_array_get_entry_by_index(
			     file_system_data_handle->file_extents,
			     element_data_file_index,
			     (intptr_t **) &file_extent,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file extent: %d.",
				 function,
				 element_data_file_index );

				goto on_error;
			}
			if( file_extent == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing file extent: %d.",
				 function,
				 element_data_file_index );

				goto on_error;
			}
			file_extent_offset    = (int64_t) encryption_identifier - (int64_t) file_extent->physical_block_number;
			encryption_identifier = file_extent->encryption_identifier + file_extent_offset;
		}
		if( libfsapfs_data_block_read(
		     data_block,
		     file_system_data_handle->io_handle,
		     file_system_data_handle->encryption_context,
		     file_io_handle,
		     element_data_offset,
		     encryption_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data block.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_PROFILER )
	if( file_system_data_handle->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_stop_timing(
		     file_system_data_handle->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     element_data_offset,
		     element_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to stop timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     element_index,
	     (intptr_t *) data_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_data_block_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data block as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_block != NULL )
	{
		libfsapfs_data_block_free(
		 &data_block,
		 NULL );
	}
	return( -1 );
}

