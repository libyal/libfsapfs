/*
 * The volume data handle functions
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
#include <types.h>

#include "libfsapfs_data_block.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_unused.h"
#include "libfsapfs_volume_data_handle.h"

/* Creates container physical_map_entry
 * Make sure the value volume_data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_data_handle_initialize(
     libfsapfs_volume_data_handle_t **volume_data_handle,
     libfsapfs_io_handle_t *io_handle,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_volume_data_handle_initialize";

	if( volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume data handle.",
		 function );

		return( -1 );
	}
	if( *volume_data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume data handle value already set.",
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
	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( ( volume_master_key_size != 0 )
	 && ( volume_master_key_size != 32 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported volume master key.",
		 function );

		return( -1 );
	}
	*volume_data_handle = memory_allocate_structure(
	                       libfsapfs_volume_data_handle_t );

	if( *volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_data_handle,
	     0,
	     sizeof( libfsapfs_volume_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume data handle.",
		 function );

		memory_free(
		 *volume_data_handle );

		*volume_data_handle = NULL;

		return( -1 );
	}
	if( volume_master_key_size != 0 )
	{
		if( libfsapfs_encryption_context_initialize(
		     &( ( *volume_data_handle )->encryption_context ),
		     LIBFSAPFS_ENCRYPTION_METHOD_AES_128_XTS,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize encryption context.",
			 function );

			goto on_error;
		}
		if( libfsapfs_encryption_context_set_keys(
		     ( *volume_data_handle )->encryption_context,
		     volume_master_key,
		     16,
		     &( volume_master_key[ 16 ] ),
		     16,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set keys in encryption context.",
			 function );

			goto on_error;
		}
	}
	( *volume_data_handle )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *volume_data_handle != NULL )
	{
		if( ( *volume_data_handle )->encryption_context != NULL )
		{
			libfsapfs_encryption_context_free(
			 &( ( *volume_data_handle )->encryption_context ),
			 NULL );
		}
		memory_free(
		 *volume_data_handle );

		*volume_data_handle = NULL;
	}
	return( -1 );
}

/* Frees volume data handle
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_data_handle_free(
     libfsapfs_volume_data_handle_t **volume_data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_volume_data_handle_free";
	int result            = 1;

	if( volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume data handle.",
		 function );

		return( -1 );
	}
	if( *volume_data_handle != NULL )
	{
		if( ( *volume_data_handle )->encryption_context != NULL )
		{
			if( libfsapfs_encryption_context_free(
			     &( ( *volume_data_handle )->encryption_context ),
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free encryption context.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *volume_data_handle );

		*volume_data_handle = NULL;
	}
	return( result );
}

/* Reads a sector
 * Callback function for the volume vector
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_data_handle_read_sector(
     libfsapfs_volume_data_handle_t *volume_data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index LIBFSAPFS_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags LIBFSAPFS_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBFSAPFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsapfs_data_block_t *data_block = NULL;
	static char *function              = "libfsapfs_volume_data_handle_read_sector";

	LIBFSAPFS_UNREFERENCED_PARAMETER( element_data_file_index );
	LIBFSAPFS_UNREFERENCED_PARAMETER( element_data_flags );
	LIBFSAPFS_UNREFERENCED_PARAMETER( read_flags );

	if( volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume data handle.",
		 function );

		return( -1 );
	}
	if( volume_data_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume data handle - missing IO handle.",
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
	if( libfsapfs_data_block_read(
	     data_block,
	     volume_data_handle->io_handle,
	     volume_data_handle->encryption_context,
	     file_io_handle,
	     element_data_offset,
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
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
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

