/*
 * The data block vector functions
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
#include <types.h>

#include "libfsapfs_file_extent.h"
#include "libfsapfs_file_system_data_handle.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfdata.h"

/* Creates a data block vector
 * Make sure the value vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_data_block_vector_initialize(
     libfdata_vector_t **vector,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_file_system_data_handle_t *data_handle,
     libcdata_array_t *file_extents,
     uint8_t is_sparse,
     libcerror_error_t **error )
{
	libfdata_vector_t *safe_vector       = NULL;
	libfsapfs_file_extent_t *file_extent = NULL;
	static char *function                = "libfsapfs_data_block_vector_initialize";
	uint64_t logical_offset              = 0;
	uint32_t segment_flags               = 0;
	int extent_index                     = 0;
	int number_of_extents                = 0;
	int segment_index                    = 0;

	if( vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid vector.",
		 function );

		return( -1 );
	}
	if( *vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid vector value already set.",
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
	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_initialize(
	     &safe_vector,
	     (size64_t) io_handle->block_size,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_system_data_handle_free,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsapfs_file_system_data_handle_read_data_block,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
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
	if( libcdata_array_get_number_of_entries(
	     file_extents,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from array.",
		 function );

		goto on_error;
	}
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     file_extents,
		     extent_index,
		     (intptr_t **) &file_extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file extent: %d.",
			 function,
			 extent_index );

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
			 extent_index );

			goto on_error;
		}
		if( ( is_sparse == 0 )
		 || ( file_extent->physical_block_number != 0 ) )
		{
			if( file_extent->logical_offset != logical_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid file extent: %d - logical offset value out of bounds.",
				 function,
				 extent_index );

				goto on_error;
			}
		}
		segment_flags = 0;

		if( ( is_sparse != 0 )
		 && ( file_extent->physical_block_number == 0 ) )
		{
			segment_flags = LIBFDATA_RANGE_FLAG_IS_SPARSE;
		}
		if( libfdata_vector_append_segment(
		     safe_vector,
		     &segment_index,
		     extent_index,
		     file_extent->physical_block_number * io_handle->block_size,
		     file_extent->data_size,
		     segment_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent: %d as data block vector segment.",
			 function,
			 extent_index );

			goto on_error;
		}
		logical_offset += file_extent->data_size;
	}
	*vector = safe_vector;

	return( 1 );

on_error:
	if( safe_vector != NULL )
	{
		libfdata_vector_free(
		 &safe_vector,
		 NULL );
	}
	return( -1 );
}

