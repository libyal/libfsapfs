/*
 * The checkpoint map entry functions
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

#include "libfsapfs_checkpoint_map_entry.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_checkpoint_map.h"

/* Creates checkpoint map entry
 * Make sure the value checkpoint_map_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_checkpoint_map_entry_initialize(
     libfsapfs_checkpoint_map_entry_t **checkpoint_map_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_checkpoint_map_entry_initialize";

	if( checkpoint_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint map entry.",
		 function );

		return( -1 );
	}
	if( *checkpoint_map_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid checkpoint map entry value already set.",
		 function );

		return( -1 );
	}
	*checkpoint_map_entry = memory_allocate_structure(
	                         libfsapfs_checkpoint_map_entry_t );

	if( *checkpoint_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create checkpoint map entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *checkpoint_map_entry,
	     0,
	     sizeof( libfsapfs_checkpoint_map_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear checkpoint map entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *checkpoint_map_entry != NULL )
	{
		memory_free(
		 *checkpoint_map_entry );

		*checkpoint_map_entry = NULL;
	}
	return( -1 );
}

/* Frees checkpoint map entry
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_checkpoint_map_entry_free(
     libfsapfs_checkpoint_map_entry_t **checkpoint_map_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_checkpoint_map_entry_free";

	if( checkpoint_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint map entry.",
		 function );

		return( -1 );
	}
	if( *checkpoint_map_entry != NULL )
	{
		memory_free(
		 *checkpoint_map_entry );

		*checkpoint_map_entry = NULL;
	}
	return( 1 );
}

/* Reads the checkpoint map entry
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_checkpoint_map_entry_read_data(
     libfsapfs_checkpoint_map_entry_t *checkpoint_map_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_checkpoint_map_entry_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
#endif

	if( checkpoint_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checkpoint map entry.",
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
	if( ( data_size < sizeof( fsapfs_checkpoint_map_entry_t ) )
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
		 "%s: checkpoint map entry data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_checkpoint_map_entry_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_checkpoint_map_entry_t *) data )->object_identifier,
	 checkpoint_map_entry->object_identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_checkpoint_map_entry_t *) data )->physical_address,
	 checkpoint_map_entry->physical_address );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_checkpoint_map_entry_t *) data )->object_type,
		 value_32bit );
		libcnotify_printf(
		 "%s: object type\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_checkpoint_map_entry_t *) data )->object_subtype,
		 value_32bit );
		libcnotify_printf(
		 "%s: object subtype\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_checkpoint_map_entry_t *) data )->size,
		 value_32bit );
		libcnotify_printf(
		 "%s: size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_checkpoint_map_entry_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_checkpoint_map_entry_t *) data )->file_system_object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: file system object identifier\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object identifier\t\t: %" PRIu64 "\n",
		 function,
		 checkpoint_map_entry->object_identifier );

		libcnotify_printf(
		 "%s: physical address\t\t: %" PRIu64 "\n",
		 function,
		 checkpoint_map_entry->physical_address );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

