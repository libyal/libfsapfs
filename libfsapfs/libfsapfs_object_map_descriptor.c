/*
 * The object map descriptor functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_object_map_descriptor.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_object_map.h"

/* Creates physical_map_entry
 * Make sure the value object_map_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_descriptor_initialize(
     libfsapfs_object_map_descriptor_t **object_map_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_map_descriptor_initialize";

	if( object_map_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map descriptor.",
		 function );

		return( -1 );
	}
	if( *object_map_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid object map descriptor value already set.",
		 function );

		return( -1 );
	}
	*object_map_descriptor = memory_allocate_structure(
	                          libfsapfs_object_map_descriptor_t );

	if( *object_map_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create object map descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *object_map_descriptor,
	     0,
	     sizeof( libfsapfs_object_map_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear object map descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *object_map_descriptor != NULL )
	{
		memory_free(
		 *object_map_descriptor );

		*object_map_descriptor = NULL;
	}
	return( -1 );
}

/* Frees object map descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_descriptor_free(
     libfsapfs_object_map_descriptor_t **object_map_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_map_descriptor_free";

	if( object_map_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map descriptor.",
		 function );

		return( -1 );
	}
	if( *object_map_descriptor != NULL )
	{
		memory_free(
		 *object_map_descriptor );

		*object_map_descriptor = NULL;
	}
	return( 1 );
}

/* Reads the object map descriptor B-tree key data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_descriptor_read_key_data(
     libfsapfs_object_map_descriptor_t *object_map_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_map_descriptor_read_key_data";

	if( object_map_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map descriptor.",
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
	if( ( data_size < sizeof( fsapfs_object_map_btree_key_t ) )
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
		 "%s: object map B-tree key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_object_map_btree_key_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_map_btree_key_t *) data )->object_identifier,
	 object_map_descriptor->identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_map_btree_key_t *) data )->object_transaction_identifier,
	 object_map_descriptor->transaction_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object identifier\t\t: %" PRIu64 "\n",
		 function,
		 object_map_descriptor->identifier );

		libcnotify_printf(
		 "%s: object transaction identifier\t: %" PRIu64 "\n",
		 function,
		 object_map_descriptor->transaction_identifier );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the object map descriptor B-tree value data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_descriptor_read_value_data(
     libfsapfs_object_map_descriptor_t *object_map_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_map_descriptor_read_value_data";

	if( object_map_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map descriptor.",
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
	if( ( data_size < sizeof( fsapfs_object_map_btree_value_t ) )
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
		 "%s: object map B-tree value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_object_map_btree_value_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_map_btree_value_t *) data )->object_flags,
	 object_map_descriptor->flags );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_map_btree_value_t *) data )->object_size,
	 object_map_descriptor->size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_map_btree_value_t *) data )->object_physical_address,
	 object_map_descriptor->physical_address );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object flags\t\t\t: 0x%04" PRIx32 "\n",
		 function,
		 object_map_descriptor->flags );

		libcnotify_printf(
		 "%s: object size\t\t\t: %" PRIu32 "\n",
		 function,
		 object_map_descriptor->size );

		libcnotify_printf(
		 "%s: object physical address\t: %" PRIu64 "\n",
		 function,
		 object_map_descriptor->physical_address );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

