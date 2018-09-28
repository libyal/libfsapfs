/*
 * Inode functions
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
#include <memory.h>
#include <types.h>

#include "libfsapfs_debug.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfdatetime.h"
#include "libfsapfs_libuna.h"

#include "fsapfs_file_system.h"

/* Creates a inode
 * Make sure the value inode is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_inode_initialize(
     libfsapfs_inode_t **inode,
     uint64_t identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_inode_initialize";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( *inode != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid inode value already set.",
		 function );

		return( -1 );
	}
	*inode = memory_allocate_structure(
	          libfsapfs_inode_t );

	if( *inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *inode,
	     0,
	     sizeof( libfsapfs_inode_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inode.",
		 function );

		goto on_error;
	}
	( *inode )->identifier = identifier;

	return( 1 );

on_error:
	if( *inode != NULL )
	{
		memory_free(
		 *inode );

		*inode = NULL;
	}
	return( -1 );
}

/* Frees a inode
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_inode_free(
     libfsapfs_inode_t **inode,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_inode_free";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( *inode != NULL )
	{
		if( ( *inode )->name != NULL )
		{
			memory_free(
			 ( *inode )->name );
		}
		memory_free(
		 *inode );

		*inode = NULL;
	}
	return( 1 );
}

/* Reads the inode
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_inode_read_data(
     libfsapfs_inode_t *inode,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function              = "libfsapfs_inode_read_data";
	size_t data_offset                 = 0;
	size_t name_size                   = 0;
	uint16_t extended_field_index      = 0;
	uint16_t number_of_extended_fields = 0;
	uint8_t extended_field_type        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit               = 0;
	uint32_t value_32bit               = 0;
	uint16_t value_16bit               = 0;
#endif

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( inode->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid inode - name value already set.",
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
	if( ( data_size < sizeof( fsapfs_file_system_btree_value_inode_t ) )
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
		 "%s: inode data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_file_system_btree_value_inode_t *) data )->parent_identifier,
	 inode->parent_identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_file_system_btree_value_inode_t *) data )->data_stream_identifier,
	 inode->data_stream_identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_file_system_btree_value_inode_t *) data )->modification_time,
	 inode->modification_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_file_system_btree_value_inode_t *) data )->creation_time,
	 inode->creation_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_file_system_btree_value_inode_t *) data )->change_time,
	 inode->change_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_file_system_btree_value_inode_t *) data )->access_time,
	 inode->access_time );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: parent identifier\t\t\t\t: %" PRIu64 "\n",
		 function,
		 inode->parent_identifier );

		libcnotify_printf(
		 "%s: data stream identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 inode->data_stream_identifier );

		if( libfsapfs_debug_print_posix_time_value(
		     function,
		     "modification time\t\t\t\t",
		     ( (fsapfs_file_system_btree_value_inode_t *) data )->modification_time,
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
		     "creation time\t\t\t\t",
		     ( (fsapfs_file_system_btree_value_inode_t *) data )->creation_time,
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
		     "change time\t\t\t\t\t",
		     ( (fsapfs_file_system_btree_value_inode_t *) data )->change_time,
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
		     "access time\t\t\t\t\t",
		     ( (fsapfs_file_system_btree_value_inode_t *) data )->access_time,
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
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->inode_flags,
		 value_64bit );
		libcnotify_printf(
		 "%s: inode flags\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );
		libfsapfs_debug_print_inode_flags(
		 value_64bit );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->number_of_children,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of children\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->bsd_flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: BSD flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->owner_identifier,
		 value_32bit );
		libcnotify_printf(
		 "%s: owner identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->group_identifier,
		 value_32bit );
		libcnotify_printf(
		 "%s: group identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->file_mode,
		 value_16bit );
		libcnotify_printf(
		 "%s: file mode\t\t\t\t\t: %" PRIo16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->unknown3,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_value_inode_t *) data )->unknown4,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( data_size > sizeof( fsapfs_file_system_btree_value_inode_t ) )
	{
		data_offset = sizeof( fsapfs_file_system_btree_value_inode_t );

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
			 "%s: number of extended fields\t\t\t: %" PRIu16 "\n",
			 function,
			 number_of_extended_fields );

			byte_stream_copy_to_uint16_little_endian(
			 &( data[ data_offset + 2 ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown5\t\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += 4;

/* TODO add support for multi extended fields */
		if( number_of_extended_fields > 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported number of extended fields.",
			 function );

			return( -1 );
		}
		for( extended_field_index = 0;
		     extended_field_index < number_of_extended_fields;
		     extended_field_index++ )
		{
			if( data_offset > ( data_size - 2 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid data size value out of bounds.",
				 function );

				return( -1 );
			}
			extended_field_type = data[ data_offset ];

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "\n" );

				libcnotify_printf(
				 "%s: extended field: %" PRIu16 " type\t\t\t: %" PRIu8 "\n",
				 function,
				 extended_field_index,
				 extended_field_type );

				libcnotify_printf(
				 "%s: extended field: %" PRIu16 " flags\t\t\t: 0x%04" PRIx16 "\n",
				 function,
				 extended_field_index,
				 data[ data_offset + 1 ] );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			data_offset += 4;

/* TODO check bounds of data */
			switch( extended_field_type )
			{
				case 1:
				case 2:
				case 5:
				case 13:
					data_offset += 8;
					break;

				case 3:
				case 7:
				case 14:
					data_offset += 4;
					break;

				case 4:
					for( name_size = data_offset;
					     name_size < data_size;
					     name_size++ )
					{
						if( data[ name_size ] == 0 )
						{
							name_size++;

							break;
						}
					}
					name_size -= data_offset;

					inode->name = (uint8_t *) memory_allocate(
					                           sizeof( uint8_t ) * name_size );

					if( inode->name == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create name.",
						 function );

						goto on_error;
					}
					inode->name_size = name_size;

					if( memory_copy(
					     inode->name,
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
					data_offset += name_size;

					break;

				case 11:
					data_offset += 16;
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
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	return( 1 );

on_error:
	if( inode->name != NULL )
	{
		memory_free(
		 inode->name );

		inode->name = NULL;
	}
	inode->name_size = 0;

	return( -1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_inode_get_identifier(
     libfsapfs_inode_t *inode,
     uint64_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_inode_get_identifier";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
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
	*identifier = inode->identifier;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_inode_get_utf8_name_size(
     libfsapfs_inode_t *inode,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_inode_get_utf8_name_size";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf8_stream(
	     inode->name,
	     (size_t) inode->name_size,
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
int libfsapfs_inode_get_utf8_name(
     libfsapfs_inode_t *inode,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_inode_get_utf8_name";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     inode->name,
	     (size_t) inode->name_size,
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

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_inode_get_utf16_name_size(
     libfsapfs_inode_t *inode,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_inode_get_utf16_name_size";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf8_stream(
	     inode->name,
	     (size_t) inode->name_size,
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
int libfsapfs_inode_get_utf16_name(
     libfsapfs_inode_t *inode,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_inode_get_utf16_name";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     inode->name,
	     (size_t) inode->name_size,
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

