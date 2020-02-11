/*
 * The key bag header functions
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
#include "libfsapfs_key_bag_header.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfguid.h"

#include "fsapfs_key_bag.h"

/* Creates container physical_map_header
 * Make sure the value key_bag_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_header_initialize(
     libfsapfs_key_bag_header_t **key_bag_header,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_bag_header_initialize";

	if( key_bag_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag header.",
		 function );

		return( -1 );
	}
	if( *key_bag_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key bag header value already set.",
		 function );

		return( -1 );
	}
	*key_bag_header = memory_allocate_structure(
	                   libfsapfs_key_bag_header_t );

	if( *key_bag_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key bag header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_bag_header,
	     0,
	     sizeof( libfsapfs_key_bag_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key bag header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_bag_header != NULL )
	{
		memory_free(
		 *key_bag_header );

		*key_bag_header = NULL;
	}
	return( -1 );
}

/* Frees key bag header
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_header_free(
     libfsapfs_key_bag_header_t **key_bag_header,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_bag_header_free";

	if( key_bag_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag header.",
		 function );

		return( -1 );
	}
	if( *key_bag_header != NULL )
	{
		memory_free(
		 *key_bag_header );

		*key_bag_header = NULL;
	}
	return( 1 );
}

/* Reads the key bag header
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_header_read_data(
     libfsapfs_key_bag_header_t *key_bag_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsapfs_key_bag_header_read_data";
	uint16_t format_version = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit    = 0;
#endif

	if( key_bag_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag header.",
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
	if( ( data_size < sizeof( fsapfs_key_bag_header_t ) )
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
		 "%s: key bag header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_key_bag_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_key_bag_header_t *) data )->format_version,
	 format_version );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_key_bag_header_t *) data )->number_of_entries,
	 key_bag_header->number_of_entries );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_key_bag_header_t *) data )->data_size,
	 key_bag_header->data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: format version\t\t\t: %" PRIu16 "\n",
		 function,
		 format_version );

		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu16 "\n",
		 function,
		 key_bag_header->number_of_entries );

		libcnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 key_bag_header->data_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_key_bag_header_t *) data )->unknown1,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( format_version != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: %" PRIu16 ".",
		 function,
		 format_version );

		return( -1 );
	}
	return( 1 );
}

