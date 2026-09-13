/*
 * The key bag (packed) value functions
 *
 * Copyright (C) 2018-2026, Joachim Metz <joachim.metz@gmail.com>
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
#include "libfsapfs_key_bag_value.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfguid.h"

#include "fsapfs_key_bag.h"

/* Creates container physical_map_value
 * Make sure the value key_bag_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_value_initialize(
     libfsapfs_key_bag_value_t **key_bag_value,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_bag_value_initialize";

	if( key_bag_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag value.",
		 function );

		return( -1 );
	}
	if( *key_bag_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key bag value value already set.",
		 function );

		return( -1 );
	}
	*key_bag_value = memory_allocate_structure(
	                  libfsapfs_key_bag_value_t );

	if( *key_bag_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key bag value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_bag_value,
	     0,
	     sizeof( libfsapfs_key_bag_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key bag value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_bag_value != NULL )
	{
		memory_free(
		 *key_bag_value );

		*key_bag_value = NULL;
	}
	return( -1 );
}

/* Frees key bag value
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_value_free(
     libfsapfs_key_bag_value_t **key_bag_value,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_bag_value_free";

	if( key_bag_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag value.",
		 function );

		return( -1 );
	}
	if( *key_bag_value != NULL )
	{
		memory_free(
		 *key_bag_value );

		*key_bag_value = NULL;
	}
	return( 1 );
}

/* Reads the key bag value
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_bag_value_read_data(
     libfsapfs_key_bag_value_t *key_bag_value,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_bag_value_read_data";

	if( key_bag_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key bag value.",
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
	if( ( data_size < sizeof( fsapfs_key_bag_value_t ) )
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
		 "%s: key bag packed value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_key_bag_value_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	key_bag_value->tag = ( (fsapfs_key_bag_value_t *) data )->tag;

	if( ( data[1] & 0x80 ) == 0 )
	{
		key_bag_value->extended_size = 0;
	}
	else
	{
		key_bag_value->extended_size = data[1] & 0x7f;
	}
	if( key_bag_value->extended_size > ( data_size - sizeof( fsapfs_key_bag_value_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid extended size value out of bounds.",
		 function );

		return( -1 );
	}
	if( key_bag_value->extended_size == 0 )
	{
		key_bag_value->data_size = (uint16_t) data[1];
	}
	else if( key_bag_value->extended_size == 1 )
	{
		key_bag_value->data_size = (uint16_t) data[2];
	}
	else if( key_bag_value->extended_size == 2 )
	{
		byte_stream_copy_to_uint16_big_endian(
		 &( data[ 2 ] ),
		 key_bag_value->data_size );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported extended size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: tag\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 key_bag_value->tag );

		libcnotify_printf(
		 "%s: extended size\t\t\t: %" PRIu8 "\n",
		 function,
		 key_bag_value->extended_size );

		libcnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 key_bag_value->data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

