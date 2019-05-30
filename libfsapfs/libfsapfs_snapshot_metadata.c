/*
 * The snapshot metadata functions
 *
 * Copyright (C) 2018-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsapfs_debug.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfdatetime.h"
#include "libfsapfs_snapshot_metadata.h"

#include "fsapfs_snapshot_metadata.h"

/* Creates physical_map_entry
 * Make sure the value snapshot_metadata is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_snapshot_metadata_initialize(
     libfsapfs_snapshot_metadata_t **snapshot_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_snapshot_metadata_initialize";

	if( snapshot_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid snapshot metadata.",
		 function );

		return( -1 );
	}
	if( *snapshot_metadata != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid snapshot metadata value already set.",
		 function );

		return( -1 );
	}
	*snapshot_metadata = memory_allocate_structure(
	                      libfsapfs_snapshot_metadata_t );

	if( *snapshot_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create snapshot metadata.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *snapshot_metadata,
	     0,
	     sizeof( libfsapfs_snapshot_metadata_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear snapshot metadata.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *snapshot_metadata != NULL )
	{
		memory_free(
		 *snapshot_metadata );

		*snapshot_metadata = NULL;
	}
	return( -1 );
}

/* Frees snapshot metadata
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_snapshot_metadata_free(
     libfsapfs_snapshot_metadata_t **snapshot_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_snapshot_metadata_free";

	if( snapshot_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid snapshot metadata.",
		 function );

		return( -1 );
	}
	if( *snapshot_metadata != NULL )
	{
		if( ( *snapshot_metadata )->name != NULL )
		{
			memory_free(
			 ( *snapshot_metadata )->name );
		}
		memory_free(
		 *snapshot_metadata );

		*snapshot_metadata = NULL;
	}
	return( 1 );
}

/* Reads the snapshot metadata B-tree key data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_snapshot_metadata_read_key_data(
     libfsapfs_snapshot_metadata_t *snapshot_metadata,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_snapshot_metadata_read_key_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( snapshot_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid snapshot metadata.",
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
	if( ( data_size < sizeof( fsapfs_snapshot_metadata_btree_key_t ) )
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
		 "%s: snapshot metadata tree key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_snapshot_metadata_btree_key_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_snapshot_metadata_btree_key_t *) data )->object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the snapshot metadata B-tree value data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_snapshot_metadata_read_value_data(
     libfsapfs_snapshot_metadata_t *snapshot_metadata,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_snapshot_metadata_read_value_data";
	size_t data_offset    = 0;
	uint16_t name_size    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
#endif

	if( snapshot_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid snapshot metadata.",
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
	if( ( data_size < sizeof( fsapfs_snapshot_metadata_btree_value_t ) )
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
		 "%s: snapshot metadata tree value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_snapshot_metadata_btree_value_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_snapshot_metadata_btree_value_t *) data )->name_size,
	 name_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_snapshot_metadata_btree_value_t *) data )->extent_reference_tree_object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: extent-reference tree object identifier\t: %" PRIu32 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_snapshot_metadata_btree_value_t *) data )->volume_superblock_object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: volume superblock object identifier\t: %" PRIu32 "\n",
		 function,
		 value_64bit );

		if( libfsapfs_debug_print_posix_time_value(
		     function,
		     "creation time\t\t\t\t",
		     ( (fsapfs_snapshot_metadata_btree_value_t *) data )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_NANO_SECONDS_64BIT_SIGNED,
		     LIBFDATETIME_STRING_FORMAT_TYPE_ISO8601 | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( libfsapfs_debug_print_posix_time_value(
		     function,
		     "change time\t\t\t\t",
		     ( (fsapfs_snapshot_metadata_btree_value_t *) data )->change_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_NANO_SECONDS_64BIT_SIGNED,
		     LIBFDATETIME_STRING_FORMAT_TYPE_ISO8601 | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_snapshot_metadata_btree_value_t *) data )->extent_reference_tree_object_type,
		 value_32bit );
		libcnotify_printf(
		 "%s: extent-reference tree object type\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_snapshot_metadata_btree_value_t *) data )->flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: name size\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 name_size );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsapfs_snapshot_metadata_btree_value_t );

	if( (size_t) name_size > ( data_size - data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: name data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 (size_t) name_size,
		 0 );
	}
#endif
	snapshot_metadata->name = (uint8_t *) memory_allocate(
	                                       sizeof( uint8_t ) * name_size );

	if( snapshot_metadata->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	snapshot_metadata->name_size = name_size;

	if( memory_copy(
	     snapshot_metadata->name,
	     &( data[ data_offset ] ),
	     name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		goto on_error;
	}

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: name\t\t\t\t\t: %s\n",
		 function,
		 snapshot_metadata->name );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( snapshot_metadata->name != NULL )
	{
		memory_free(
		 snapshot_metadata->name );

		snapshot_metadata->name = NULL;
	}
	snapshot_metadata->name_size = 0;

	return( -1 );
}

