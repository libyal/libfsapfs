/*
 * Directory record functions
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
#include "libfsapfs_directory_record.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfdatetime.h"
#include "libfsapfs_libuna.h"
#include "libfsapfs_name.h"

#include "fsapfs_file_system.h"

/* Creates a directory_record
 * Make sure the value directory_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_initialize(
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_initialize";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( *directory_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory record value already set.",
		 function );

		return( -1 );
	}
	*directory_record = memory_allocate_structure(
	                     libfsapfs_directory_record_t );

	if( *directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_record,
	     0,
	     sizeof( libfsapfs_directory_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_record != NULL )
	{
		memory_free(
		 *directory_record );

		*directory_record = NULL;
	}
	return( -1 );
}

/* Frees a directory record
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_free(
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_free";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( *directory_record != NULL )
	{
		if( ( *directory_record )->name != NULL )
		{
			memory_free(
			 ( *directory_record )->name );
		}
		memory_free(
		 *directory_record );

		*directory_record = NULL;
	}
	return( 1 );
}

/* Clones a directory record
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_clone(
     libfsapfs_directory_record_t **destination_directory_record,
     libfsapfs_directory_record_t *source_directory_record,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_clone";

	if( destination_directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( *destination_directory_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination directory record value already set.",
		 function );

		return( -1 );
	}
	if( source_directory_record == NULL )
	{
		*destination_directory_record = source_directory_record;

		return( 1 );
	}
	*destination_directory_record = memory_allocate_structure(
	                                 libfsapfs_directory_record_t );

	if( *destination_directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination directory record.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_directory_record,
	     source_directory_record,
	     sizeof( libfsapfs_directory_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to copy source directory record to destination.",
		 function );

		( *destination_directory_record )->name = NULL;

		goto on_error;
	}
	( *destination_directory_record )->name = (uint8_t *) memory_allocate(
	                                                       sizeof( uint8_t ) * source_directory_record->name_size );

	if( ( *destination_directory_record )->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination record name.",
		 function );

		( *destination_directory_record )->name = NULL;

		goto on_error;
	}
	if( memory_copy(
	     ( *destination_directory_record )->name,
	     source_directory_record->name,
	     source_directory_record->name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source directory record name to destination.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_directory_record != NULL )
	{
		if( ( *destination_directory_record )->name != NULL )
		{
			memory_free(
			 ( *destination_directory_record )->name );
		}
		memory_free(
		 *destination_directory_record );

		*destination_directory_record = NULL;
	}
	return( -1 );
}

/* Reads the directory record key data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_read_key_data(
     libfsapfs_directory_record_t *directory_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_read_key_data";
	size_t data_offset    = 0;
	uint32_t name_hash    = 0;
	uint32_t name_size    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( directory_record->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory record - name value already set.",
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
	if( ( data_size < sizeof( fsapfs_file_system_btree_key_directory_record_t ) )
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
		 "%s: directory record key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	/* Determine if the directory record key data contains a name or a name and hash based on its size
	 */
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_file_system_btree_key_directory_record_t *) data )->name_size,
	 name_size );

	name_size &= 0x000003ffUL;

	data_offset = sizeof( fsapfs_file_system_btree_key_directory_record_t );

	if( name_size < ( data_size - data_offset ) )
	{
		if( data_size < sizeof( fsapfs_file_system_btree_key_directory_record_with_hash_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_file_system_btree_key_directory_record_with_hash_t *) data )->name_size_and_hash,
		 name_hash );

		name_size = name_hash & 0x000003ffUL;
		name_hash = ( name_hash & 0xfffffc00UL ) >> 10;

		data_offset = sizeof( fsapfs_file_system_btree_key_directory_record_with_hash_t );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_directory_record_t *) data )->file_system_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: parent identifier\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		if( name_hash == 0 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsapfs_file_system_btree_key_directory_record_t *) data )->name_size,
			 value_16bit );
			libcnotify_printf(
			 "%s: name size\t\t\t\t\t: 0x%04" PRIx16 " (size: %" PRIu32 ")\n",
			 function,
			 value_16bit,
			 name_size );
		}
		else
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (fsapfs_file_system_btree_key_directory_record_with_hash_t *) data )->name_size_and_hash,
			 value_32bit );
			libcnotify_printf(
			 "%s: name size and hash\t\t\t: 0x%04" PRIx32 " (size: %" PRIu32 ", hash: 0x%06" PRIx32 ")\n",
			 function,
			 value_32bit,
			 name_size,
			 name_hash );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

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
	directory_record->name = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * name_size );

	if( directory_record->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	directory_record->name_size = name_size;

	if( memory_copy(
	     directory_record->name,
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
	directory_record->name_hash = name_hash;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: name\t\t\t\t\t: %s\n",
		 function,
		 directory_record->name );
	}
#endif
	return( 1 );

on_error:
	if( directory_record->name != NULL )
	{
		memory_free(
		 directory_record->name );

		directory_record->name = NULL;
	}
	directory_record->name_size = 0;

	return( -1 );
}

/* Reads the directory record value data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_read_value_data(
     libfsapfs_directory_record_t *directory_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function              = "libfsapfs_directory_record_read_value_data";
	const uint8_t *value_data          = NULL;
	size_t data_offset                 = 0;
	size_t trailing_data_size          = 0;
	size_t value_data_offset           = 0;
	size_t value_data_size             = 0;
	uint16_t extended_field_index      = 0;
	uint16_t number_of_extended_fields = 0;
	uint8_t extended_field_flags       = 0;
	uint8_t extended_field_type        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit               = 0;
#endif

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
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
	if( ( data_size < sizeof( fsapfs_file_system_btree_value_directory_record_t ) )
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
		 "%s: directory record value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_file_system_btree_value_directory_record_t *) data )->file_system_identifier,
	 directory_record->identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_file_system_btree_value_directory_record_t *) data )->added_time,
	 directory_record->added_time );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu64 "\n",
		 function,
		 directory_record->identifier );

		if( libfsapfs_debug_print_posix_time_value(
		     function,
		     "added time\t\t\t\t",
		     ( (fsapfs_file_system_btree_value_directory_record_t *) data )->added_time,
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

			return( -1 );
		}
		byte_stream_copy_to_uint16_little_endian(
		 ( (fsapfs_file_system_btree_value_directory_record_t *) data )->directory_entry_flags,
		 value_16bit );
		libcnotify_printf(
		 "%s: directory entry flags\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
		libfsapfs_debug_print_directory_entry_flags(
		 value_16bit );
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( data_size > sizeof( fsapfs_file_system_btree_value_directory_record_t ) )
	{
		data_offset = sizeof( fsapfs_file_system_btree_value_directory_record_t );

		if( data_offset > ( data_size - 4 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint16_little_endian(
		 &( data[ data_offset ] ),
		 number_of_extended_fields );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: number of extended fields\t\t: %" PRIu16 "\n",
			 function,
			 number_of_extended_fields );

			byte_stream_copy_to_uint16_little_endian(
			 &( data[ data_offset + 2 ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset      += 4;
		value_data_offset = data_offset + ( number_of_extended_fields * 4 );

		for( extended_field_index = 0;
		     extended_field_index < number_of_extended_fields;
		     extended_field_index++ )
		{
			if( data_offset > ( data_size - 4 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid data size value out of bounds.",
				 function );

				return( -1 );
			}
			extended_field_type  = data[ data_offset ];
			extended_field_flags = data[ data_offset + 1 ];

			byte_stream_copy_to_uint16_little_endian(
			 &( data[ data_offset + 2 ] ),
			 value_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: extended field: %" PRIu16 " type\t\t: %" PRIu8 " %s\n",
				 function,
				 extended_field_index,
				 extended_field_type,
				 libfsapfs_debug_print_directory_record_extended_field_type(
				  extended_field_type ) );

				libcnotify_printf(
				 "%s: extended field: %" PRIu16 " flags\t\t: 0x%04" PRIx16 "\n",
				 function,
				 extended_field_index,
				 extended_field_flags );
				libfsapfs_debug_print_extended_field_flags(
				 extended_field_flags );
				libcnotify_printf(
				 "\n" );

				libcnotify_printf(
				 "%s: extended field: %" PRIu16 " value data size\t: %" PRIu16 "\n",
				 function,
				 extended_field_index,
				 value_data_size );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			data_offset += 4;

			if( value_data_offset > data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid data size value out of bounds.",
				 function );

				return( -1 );
			}
			if( value_data_size > ( data_size - value_data_offset ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid value data size value out of bounds.",
				 function );

				return( -1 );
			}
			value_data = &( data[ value_data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: extended field: %" PRIu16 " value data:\n",
				 function,
				 extended_field_index );
				libcnotify_print_data(
				 value_data,
				 value_data_size,
				 0 );
			}
#endif
			switch( extended_field_type )
			{
				case 1:
					break;

				default:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported extended field type: %" PRIu8 ".",
					 function,
					 extended_field_type );

					return( -1 );
			}
			value_data_offset += value_data_size;

			trailing_data_size = value_data_size % 8;

			if( trailing_data_size > 0 )
			{
				trailing_data_size = 8 - trailing_data_size;

				if( value_data_offset > ( data_size - trailing_data_size ) )
				{
					trailing_data_size = data_size - value_data_offset;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: extended field: %" PRIu16 " trailing data:\n",
					 function,
					 extended_field_index );
					libcnotify_print_data(
					 &( data[ value_data_offset ] ),
					 trailing_data_size,
					 0 );
				}
#endif
				value_data_offset += trailing_data_size;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	return( 1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_get_identifier(
     libfsapfs_directory_record_t *directory_record,
     uint64_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_get_identifier";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = directory_record->identifier;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_get_utf8_name_size(
     libfsapfs_directory_record_t *directory_record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_get_utf8_name_size";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf8_stream(
	     directory_record->name,
	     (size_t) directory_record->name_size,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_get_utf8_name(
     libfsapfs_directory_record_t *directory_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_get_utf8_name";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     directory_record->name,
	     (size_t) directory_record->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares an UTF-8 string with a directory record name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsapfs_directory_record_compare_name_with_utf8_string(
     libfsapfs_directory_record_t *directory_record,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t name_hash,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_compare_name_with_utf8_string";
	int result            = LIBUNA_COMPARE_EQUAL;

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( ( directory_record->name_hash != 0 )
	 && ( name_hash != 0 ) )
	{
		if( name_hash < directory_record->name_hash )
		{
			return( LIBUNA_COMPARE_LESS );
		}
		else if( name_hash > directory_record->name_hash )
		{
			return( LIBUNA_COMPARE_GREATER );
		}
	}
	if( directory_record->name != NULL )
	{
		result = libfsapfs_name_compare_with_utf8_string(
		          directory_record->name,
		          directory_record->name_size,
		          utf8_string,
		          utf8_string_length,
		          use_case_folding,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with name.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_get_utf16_name_size(
     libfsapfs_directory_record_t *directory_record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_get_utf16_name_size";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf8_stream(
	     directory_record->name,
	     (size_t) directory_record->name_size,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_get_utf16_name(
     libfsapfs_directory_record_t *directory_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_get_utf16_name";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     directory_record->name,
	     (size_t) directory_record->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares an UTF-16 string with a directory record name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsapfs_directory_record_compare_name_with_utf16_string(
     libfsapfs_directory_record_t *directory_record,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t name_hash,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_compare_name_with_utf16_string";
	int result            = LIBUNA_COMPARE_EQUAL;

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( ( directory_record->name_hash != 0 )
	 && ( name_hash != 0 ) )
	{
		if( name_hash < directory_record->name_hash )
		{
			return( LIBUNA_COMPARE_LESS );
		}
		else if( name_hash > directory_record->name_hash )
		{
			return( LIBUNA_COMPARE_GREATER );
		}
	}
	if( directory_record->name != NULL )
	{
		result = libfsapfs_name_compare_with_utf16_string(
		          directory_record->name,
		          directory_record->name_size,
		          utf16_string,
		          utf16_string_length,
		          use_case_folding,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with name.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the added time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_directory_record_get_added_time(
     libfsapfs_directory_record_t *directory_record,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_directory_record_get_added_time";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = (int64_t) directory_record->added_time;

	return( 1 );
}

