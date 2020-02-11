/*
 * The key bag entry functions
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

#include "libfsapfs_debug.h"
#include "libfsapfs_key_bag_entry.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfguid.h"

#include "fsapfs_key_bag.h"

/* Creates key bag entry
 * Make sure the value key_bag_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_entry_initialize(
     libfsapfs_key_bag_entry_t **key_bag_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_bag_entry_initialize";

	if( key_bag_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag entry.",
		 function );

		return( -1 );
	}
	if( *key_bag_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key bag entry value already set.",
		 function );

		return( -1 );
	}
	*key_bag_entry = memory_allocate_structure(
	                  libfsapfs_key_bag_entry_t );

	if( *key_bag_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key bag entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_bag_entry,
	     0,
	     sizeof( libfsapfs_key_bag_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key bag entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_bag_entry != NULL )
	{
		memory_free(
		 *key_bag_entry );

		*key_bag_entry = NULL;
	}
	return( -1 );
}

/* Frees key bag entry
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_entry_free(
     libfsapfs_key_bag_entry_t **key_bag_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_bag_entry_free";

	if( key_bag_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag entry.",
		 function );

		return( -1 );
	}
	if( *key_bag_entry != NULL )
	{
		if( ( *key_bag_entry )->data != NULL )
		{
			memory_free(
			 ( *key_bag_entry )->data );
		}
		memory_free(
		 *key_bag_entry );

		*key_bag_entry = NULL;
	}
	return( 1 );
}

/* Reads the key bag entry
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_entry_read_data(
     libfsapfs_key_bag_entry_t *key_bag_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_bag_entry_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( key_bag_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag entry.",
		 function );

		return( -1 );
	}
	if( key_bag_entry->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key bag entry - data value already set.",
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
	if( ( data_size < sizeof( fsapfs_key_bag_entry_header_t ) )
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
		 "%s: key bag entry header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_key_bag_entry_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_copy(
	     key_bag_entry->identifier,
	     ( (fsapfs_key_bag_entry_header_t *) data )->identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_key_bag_entry_header_t *) data )->entry_type,
	 key_bag_entry->type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_key_bag_entry_header_t *) data )->data_size,
	 key_bag_entry->data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfsapfs_debug_print_guid_value(
		     function,
		     "identifier\t\t\t\t",
		     ( (fsapfs_key_bag_entry_header_t *) data )->identifier,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print UUID value.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: entry type\t\t\t\t: %" PRIu16 "\n",
		 function,
		 key_bag_entry->type );

		libcnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 key_bag_entry->data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_key_bag_entry_header_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( key_bag_entry->data_size > ( data_size - sizeof( fsapfs_key_bag_entry_header_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry data size value out of bounds.",
		 function );

		goto on_error;
	}
	key_bag_entry->data = (uint8_t *) memory_allocate(
	                                   sizeof( uint8_t ) * key_bag_entry->data_size );

	if( key_bag_entry->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key bag entry data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     key_bag_entry->data,
	     &( data[ sizeof( fsapfs_key_bag_entry_header_t ) ] ),
	     (size_t) key_bag_entry->data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key bag entry data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key bag entry data:\n",
		 function );
		libcnotify_print_data(
		 key_bag_entry->data,
		 (size_t) key_bag_entry->data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	key_bag_entry->size = sizeof( fsapfs_key_bag_entry_header_t ) + key_bag_entry->data_size;

	return( 1 );

on_error:
	if( key_bag_entry->data != NULL )
	{
		memory_free(
		 key_bag_entry->data );

		key_bag_entry->data = NULL;
	}
	return( -1 );
}

