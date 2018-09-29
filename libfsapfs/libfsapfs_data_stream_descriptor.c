/*
 * The data stream descriptor functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_data_stream_descriptor.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_file_system.h"

/* Creates data stream descriptor
 * Make sure the value data_stream_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_data_stream_descriptor_initialize(
     libfsapfs_data_stream_descriptor_t **data_stream_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_data_stream_descriptor_initialize";

	if( data_stream_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream descriptor.",
		 function );

		return( -1 );
	}
	if( *data_stream_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data stream descriptor value already set.",
		 function );

		return( -1 );
	}
	*data_stream_descriptor = memory_allocate_structure(
	                           libfsapfs_data_stream_descriptor_t );

	if( *data_stream_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data stream descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_stream_descriptor,
	     0,
	     sizeof( libfsapfs_data_stream_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data stream descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_stream_descriptor != NULL )
	{
		memory_free(
		 *data_stream_descriptor );

		*data_stream_descriptor = NULL;
	}
	return( -1 );
}

/* Frees data stream descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_data_stream_descriptor_free(
     libfsapfs_data_stream_descriptor_t **data_stream_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_data_stream_descriptor_free";

	if( data_stream_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream descriptor.",
		 function );

		return( -1 );
	}
	if( *data_stream_descriptor != NULL )
	{
		memory_free(
		 *data_stream_descriptor );

		*data_stream_descriptor = NULL;
	}
	return( 1 );
}

/* Reads the data stream descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_data_stream_descriptor_read_data(
     libfsapfs_data_stream_descriptor_t *data_stream_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_data_stream_descriptor_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( data_stream_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream descriptor.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsapfs_file_system_data_stream_descriptor_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data stream descriptor data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_file_system_data_stream_descriptor_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_data_stream_descriptor_t *) data )->size,
		 value_64bit );
		libcnotify_printf(
		 "%s: size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_data_stream_descriptor_t *) data )->allocated_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: allocated size\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_data_stream_descriptor_t *) data )->default_encryption_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: default encryption identifier\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_data_stream_descriptor_t *) data )->total_bytes_written,
		 value_64bit );
		libcnotify_printf(
		 "%s: total bytes written\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_data_stream_descriptor_t *) data )->total_bytes_read,
		 value_64bit );
		libcnotify_printf(
		 "%s: total bytes read\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

