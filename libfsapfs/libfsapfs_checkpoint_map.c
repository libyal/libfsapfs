/*
 * The checkpoint map functions
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

#include "libfsapfs_checkpoint_map.h"
#include "libfsapfs_checkpoint_map_entry.h"
#include "libfsapfs_checksum.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_checkpoint_map.h"

/* Creates a checkpoint map
 * Make sure the value checkpoint_map is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_checkpoint_map_initialize(
     libfsapfs_checkpoint_map_t **checkpoint_map,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_checkpoint_map_initialize";

	if( checkpoint_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint map.",
		 function );

		return( -1 );
	}
	if( *checkpoint_map != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid checkpoint map value already set.",
		 function );

		return( -1 );
	}
	*checkpoint_map = memory_allocate_structure(
	                   libfsapfs_checkpoint_map_t );

	if( *checkpoint_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create checkpoint map.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *checkpoint_map,
	     0,
	     sizeof( libfsapfs_checkpoint_map_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear checkpoint map.",
		 function );

		memory_free(
		 *checkpoint_map );

		*checkpoint_map = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *checkpoint_map )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *checkpoint_map != NULL )
	{
		memory_free(
		 *checkpoint_map );

		*checkpoint_map = NULL;
	}
	return( -1 );
}

/* Frees a checkpoint map
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_checkpoint_map_free(
     libfsapfs_checkpoint_map_t **checkpoint_map,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_checkpoint_map_free";
	int result            = 1;

	if( checkpoint_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint map.",
		 function );

		return( -1 );
	}
	if( *checkpoint_map != NULL )
	{
		if( libcdata_array_free(
		     &( ( *checkpoint_map )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_checkpoint_map_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free entries array.",
			 function );

			result = -1;
		}
		memory_free(
		 *checkpoint_map );

		*checkpoint_map = NULL;
	}
	return( result );
}

/* Reads the checkpoint map
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_checkpoint_map_read_file_io_handle(
     libfsapfs_checkpoint_map_t *checkpoint_map,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t checkpoint_map_data[ 4096 ];

	static char *function = "libfsapfs_checkpoint_map_read_file_io_handle";
	ssize_t read_count    = 0;

	if( checkpoint_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint map.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading checkpoint map at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek checkpoint map offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &checkpoint_map_data,
	              4096,
	              error );

	if( read_count != (ssize_t) 4096 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read checkpoint map data.",
		 function );

		return( -1 );
	}
	if( libfsapfs_checkpoint_map_read_data(
	     checkpoint_map,
	     (uint8_t *) &checkpoint_map_data,
	     4096,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read checkpoint map data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the checkpoint map
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_checkpoint_map_read_data(
     libfsapfs_checkpoint_map_t *checkpoint_map,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_checkpoint_map_entry_t *map_entry = NULL;
	static char *function                       = "libfsapfs_checkpoint_map_read_data";
	size_t data_offset                          = 0;
	uint64_t calculated_checksum                = 0;
	uint64_t stored_checksum                    = 0;
	uint32_t map_entry_index                    = 0;
	uint32_t number_of_map_entries              = 0;
	uint32_t object_subtype                     = 0;
	uint32_t object_type                        = 0;
	int entry_index                             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                        = 0;
	uint32_t value_32bit                        = 0;
#endif

	if( checkpoint_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint map.",
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
	if( ( data_size < sizeof( fsapfs_checkpoint_map_t ) )
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
		 "%s: checkpoint map data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_checkpoint_map_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_checkpoint_map_t *) data )->object_checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_checkpoint_map_t *) data )->object_type,
	 object_type );

	if( object_type != 0x4000000cUL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 object_type );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_checkpoint_map_t *) data )->object_subtype,
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

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_checkpoint_map_t *) data )->number_of_entries,
	 number_of_map_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object checksum\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 stored_checksum );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_checkpoint_map_t *) data )->object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_checkpoint_map_t *) data )->object_transaction_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object transaction identifier\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object type\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_subtype );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_checkpoint_map_t *) data )->flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
		libfsapfs_debug_print_checkpoint_flags(
		 value_32bit );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_map_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfsapfs_checksum_calculate_fletcher64(
	     &calculated_checksum,
	     &( data[ 8 ] ),
	     data_size - 8,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate Fletcher-64 checksum.",
		 function );

		goto on_error;
	}
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in checksum ( 0x%08" PRIx64 " != 0x%08" PRIx64 " ).\n",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	data_offset = 40;

	if( number_of_map_entries > 101 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of map entries value out of bounds.",
		 function );

		goto on_error;
	}
	for( map_entry_index = 0;
	     map_entry_index < number_of_map_entries;
	     map_entry_index++ )
	{
		if( libfsapfs_checkpoint_map_entry_initialize(
		     &map_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create checkpoint map entry.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %" PRIu32 "\n",
			 function,
			 map_entry_index );
		}
#endif
		if( libfsapfs_checkpoint_map_entry_read_data(
		     map_entry,
		     &( data[ data_offset ] ),
		     data_size - data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read checkpoint map entry: %" PRIu32 ".",
			 function,
			 map_entry_index );

			goto on_error;
		}
		data_offset += sizeof( fsapfs_checkpoint_map_entry_t );

		if( libcdata_array_append_entry(
		     checkpoint_map->entries_array,
		     &entry_index,
		     (intptr_t *) map_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append map entry: %" PRIu32 " to array.",
			 function,
			 map_entry_index );

			goto on_error;
		}
		map_entry = NULL;
	}
	return( 1 );

on_error:
	if( map_entry != NULL )
	{
		libfsapfs_checkpoint_map_entry_free(
		 &map_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the physical address of a specific object identifier
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libfsapfs_checkpoint_map_get_physical_address_by_object_identifier(
     libfsapfs_checkpoint_map_t *checkpoint_map,
     uint64_t object_identifier,
     uint64_t *physical_address,
     libcerror_error_t **error )
{
	libfsapfs_checkpoint_map_entry_t *map_entry = NULL;
	static char *function                       = "libfsapfs_checkpoint_map_get_physical_address_by_object_identifier";
	int entry_index                             = 0;
	int number_of_entries                       = 0;

	if( checkpoint_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint map.",
		 function );

		return( -1 );
	}
	if( physical_address == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid physical address.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     checkpoint_map->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from array.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     checkpoint_map->entries_array,
		     entry_index,
		     (intptr_t **) &map_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from array.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( map_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( object_identifier == map_entry->object_identifier )
		{
			*physical_address = map_entry->physical_address;

			return( 1 );
		}
		map_entry = NULL;
	}
	return( 0 );
}

