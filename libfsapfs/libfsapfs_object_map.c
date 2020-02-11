/*
 * The object map functions
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

#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_object_map.h"

#include "fsapfs_object_map.h"

/* Creates a object map
 * Make sure the value object_map is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_initialize(
     libfsapfs_object_map_t **object_map,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_map_initialize";

	if( object_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map.",
		 function );

		return( -1 );
	}
	if( *object_map != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid object map value already set.",
		 function );

		return( -1 );
	}
	*object_map = memory_allocate_structure(
	               libfsapfs_object_map_t );

	if( *object_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create object map.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *object_map,
	     0,
	     sizeof( libfsapfs_object_map_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear object map.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *object_map != NULL )
	{
		memory_free(
		 *object_map );

		*object_map = NULL;
	}
	return( -1 );
}

/* Frees a object map
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_free(
     libfsapfs_object_map_t **object_map,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_map_free";

	if( object_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map.",
		 function );

		return( -1 );
	}
	if( *object_map != NULL )
	{
		memory_free(
		 *object_map );

		*object_map = NULL;
	}
	return( 1 );
}

/* Reads the object map
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_read_file_io_handle(
     libfsapfs_object_map_t *object_map,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	fsapfs_object_map_t object_map_data;

	static char *function = "libfsapfs_object_map_read_file_io_handle";
	ssize_t read_count    = 0;

	if( object_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading object map at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek object map offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &object_map_data,
	              sizeof( fsapfs_object_map_t ),
	              error );

	if( read_count != (ssize_t) sizeof( fsapfs_object_map_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object map data.",
		 function );

		return( -1 );
	}
	if( libfsapfs_object_map_read_data(
	     object_map,
	     (uint8_t *) &object_map_data,
	     sizeof( fsapfs_object_map_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object map data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the object map
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_read_data(
     libfsapfs_object_map_t *object_map,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsapfs_object_map_read_data";
	uint32_t object_subtype = 0;
	uint32_t object_type    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit    = 0;
	uint32_t value_32bit    = 0;
#endif

	if( object_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map.",
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
	if( ( data_size < sizeof( fsapfs_object_map_t ) )
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
		 "%s: object map data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_object_map_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_map_t *) data )->object_type,
	 object_type );

	if( object_type != 0x4000000bUL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 object_type );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_map_t *) data )->object_subtype,
	 object_subtype );

	if( object_subtype != 0x00000000UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 object_subtype );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_map_t *) data )->number_of_snapshots,
	 object_map->number_of_snapshots );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_map_t *) data )->btree_block_number,
	 object_map->btree_block_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_map_t *) data )->snapshots_btree_block_number,
	 object_map->snapshots_btree_block_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_map_t *) data )->most_recent_snapshot_identifier,
	 object_map->most_recent_snapshot_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_map_t *) data )->object_checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: object checksum\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_map_t *) data )->object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_map_t *) data )->object_transaction_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object transaction identifier\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object type\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_subtype );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_object_map_t *) data )->flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of snapshots\t\t\t: %" PRIu32 "\n",
		 function,
		 object_map->number_of_snapshots );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_object_map_t *) data )->btree_type,
		 value_32bit );
		libcnotify_printf(
		 "%s: B-tree type\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_object_map_t *) data )->snaphots_btree_type,
		 value_32bit );
		libcnotify_printf(
		 "%s: snapshots B-tree type\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: B-tree block number\t\t\t: %" PRIu64 "\n",
		 function,
		 object_map->btree_block_number );

		libcnotify_printf(
		 "%s: snapshots B-tree block number\t\t: %" PRIu64 "\n",
		 function,
		 object_map->snapshots_btree_block_number );

		libcnotify_printf(
		 "%s: most recent snapshot identifier\t\t: %" PRIu64 "\n",
		 function,
		 object_map->most_recent_snapshot_identifier );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_map_t *) data )->unknown2,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_map_t *) data )->unknown3,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

