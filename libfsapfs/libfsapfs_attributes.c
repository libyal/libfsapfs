/*
 * (Extended) attribute functions
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
#include <types.h>

#include "libfsapfs_attribute_values.h"
#include "libfsapfs_attributes.h"
#include "libfsapfs_data_stream.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_file_extent.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfdata.h"

/* Retrieves the attribute value data file extents
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_attributes_get_file_extents(
     libfsapfs_attribute_values_t *attribute_values,
     libbfio_handle_t *file_io_handle,
     libfsapfs_file_system_btree_t *file_system_btree,
     uint64_t transaction_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attributes_get_file_extents";
	int result            = 0;

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( attribute_values->value_data_file_extents != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values - value data file extents value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( attribute_values->value_data_file_extents ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value data file extents array.",
		 function );

		goto on_error;
	}
	result = libfsapfs_file_system_btree_get_file_extents(
		  file_system_btree,
		  file_io_handle,
		  attribute_values->value_data_stream_identifier,
		  transaction_identifier,
		  attribute_values->value_data_file_extents,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data file extents from file system B-tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( attribute_values->value_data_file_extents != NULL )
	{
		libcdata_array_free(
		 &( attribute_values->value_data_file_extents ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the attribute value data stream
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_attributes_get_data_stream(
     libfsapfs_attribute_values_t *attribute_values,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfsapfs_file_system_btree_t *file_system_btree,
     uint64_t transaction_identifier,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attributes_get_data_stream";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( ( attribute_values->flags & 0x0001 ) != 0 )
	{
		if( attribute_values->value_data_file_extents == NULL )
		{
			if( libfsapfs_attributes_get_file_extents(
			     attribute_values,
			     file_io_handle,
			     file_system_btree,
			     transaction_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute value data file extents.",
				 function );

				return( -1 );
			}
		}
		if( libfsapfs_data_stream_initialize_from_file_extents(
		     data_stream,
		     io_handle,
		     encryption_context,
		     attribute_values->value_data_file_extents,
		     attribute_values->value_data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value data stream from file extents.",
			 function );

			return( -1 );
		}
	}
	else if( ( attribute_values->flags & 0x0002 ) != 0 )
	{
		if( libfsapfs_data_stream_initialize_from_data(
		     data_stream,
		     attribute_values->value_data,
		     attribute_values->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value data stream from data.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

