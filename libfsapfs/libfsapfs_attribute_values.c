/*
 * (Extended) attribute values functions
 *
 * Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsapfs_attribute_values.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_file_extent.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libuna.h"

#include "fsapfs_file_system.h"

/* Creates an attribute_values
 * Make sure the value attribute_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_attribute_values_initialize(
     libfsapfs_attribute_values_t **attribute_values,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_initialize";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( *attribute_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values value already set.",
		 function );

		return( -1 );
	}
	*attribute_values = memory_allocate_structure(
	                     libfsapfs_attribute_values_t );

	if( *attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attribute_values,
	     0,
	     sizeof( libfsapfs_attribute_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute values.",
		 function );

		memory_free(
		 attribute_values );

		return( -1 );
	}
	return( 1 );

on_error:
	if( *attribute_values != NULL )
	{
		memory_free(
		 *attribute_values );

		*attribute_values = NULL;
	}
	return( -1 );
}

/* Frees an attribute values
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_attribute_values_free(
     libfsapfs_attribute_values_t **attribute_values,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_free";
	int result            = 1;

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( *attribute_values != NULL )
	{
		if( ( *attribute_values )->value_data_file_extents != NULL )
		{
			if( libcdata_array_free(
			     &( ( *attribute_values )->value_data_file_extents ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value data file extents array.",
				 function );

				result = -1;
			}
		}
		if( ( *attribute_values )->value_data != NULL )
		{
			memory_free(
			 ( *attribute_values )->value_data );
		}
		if( ( *attribute_values )->name != NULL )
		{
			memory_free(
			 ( *attribute_values )->name );
		}
		memory_free(
		 *attribute_values );

		*attribute_values = NULL;
	}
	return( result );
}

/* Reads the attribute values key data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_attribute_values_read_key_data(
     libfsapfs_attribute_values_t *attribute_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_read_key_data";
	size_t data_offset    = 0;
	uint16_t name_size    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( attribute_values->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values - name value already set.",
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
	if( ( data_size < sizeof( fsapfs_file_system_btree_key_extended_attribute_t ) )
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
		 "%s: attribute values key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_file_system_btree_key_extended_attribute_t *) data )->name_size,
	 name_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_extended_attribute_t *) data )->file_system_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: parent identifier\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: name size\t\t\t: %" PRIu16 "\n",
		 function,
		 name_size );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsapfs_file_system_btree_key_extended_attribute_t );

	if( ( name_size == 0 )
	 || ( (size_t) name_size > ( data_size - data_offset ) ) )
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
	attribute_values->name = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * name_size );

	if( attribute_values->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	attribute_values->name_size = name_size;

	if( memory_copy(
	     attribute_values->name,
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
	return( 1 );

on_error:
	if( attribute_values->name != NULL )
	{
		memory_free(
		 attribute_values->name );

		attribute_values->name = NULL;
	}
	attribute_values->name_size = 0;

	return( -1 );
}

/* Reads the attribute values value data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_attribute_values_read_value_data(
     libfsapfs_attribute_values_t *attribute_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	const uint8_t *attribute_values_data = NULL;
	static char *function                = "libfsapfs_attribute_values_read_value_data";
	size_t data_offset                   = 0;
	uint16_t attribute_values_data_size  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                 = 0;
#endif

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( attribute_values->value_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values - value data value already set.",
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
	if( ( data_size < sizeof( fsapfs_file_system_btree_value_extended_attribute_t ) )
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
		 "%s: attribute values value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_file_system_btree_value_extended_attribute_t *) data )->flags,
	 attribute_values->flags );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_file_system_btree_value_extended_attribute_t *) data )->data_size,
	 attribute_values_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: flags\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 attribute_values->flags );
		libfsapfs_debug_print_extended_attribute_flags(
		 attribute_values->flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: data size\t\t\t: %" PRIu16 "\n",
		 function,
		 attribute_values_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsapfs_file_system_btree_value_extended_attribute_t );

	if( ( attribute_values_data_size != 0 )
	 && ( (size_t) attribute_values_data_size > ( data_size - data_offset ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid attribute values data size value out of bounds.",
		 function );

		goto on_error;
	}
	attribute_values_data = &( data[ data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute values data:\n",
		 function );
		libcnotify_print_data(
		 attribute_values_data,
		 (size_t) attribute_values_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( attribute_values->flags & 0x0001 ) != 0 )
	{
		if( (size_t) attribute_values_data_size != sizeof( fsapfs_file_system_extended_attribute_data_stream_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported attribute values data size.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_extended_attribute_data_stream_t *) attribute_values_data )->data_stream_identifier,
		 attribute_values->value_data_stream_identifier );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_extended_attribute_data_stream_t *) attribute_values_data )->used_size,
		 attribute_values->value_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data stream indentifier\t: %" PRIu64 "\n",
			 function,
			 attribute_values->value_data_stream_identifier );

			libcnotify_printf(
			 "%s: used size\t\t\t: %" PRIu64 "\n",
			 function,
			 attribute_values->value_data_size );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_extended_attribute_data_stream_t *) attribute_values_data )->allocated_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: allocated size\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_extended_attribute_data_stream_t *) attribute_values_data )->encryption_identifier,
			 value_64bit );
			libcnotify_printf(
			 "%s: encryption identifier\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_extended_attribute_data_stream_t *) attribute_values_data )->number_of_bytes_written,
			 value_64bit );
			libcnotify_printf(
			 "%s: number of bytes written\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_extended_attribute_data_stream_t *) attribute_values_data )->number_of_bytes_read,
			 value_64bit );
			libcnotify_printf(
			 "%s: number of bytes read\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( ( ( attribute_values->flags & 0x0002 ) != 0 )
	      && ( attribute_values_data_size > 0 ) )
	{
		attribute_values->value_data = (uint8_t *) memory_allocate(
		                                            sizeof( uint8_t ) * attribute_values_data_size );

		if( attribute_values->value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		attribute_values->value_data_size = (size64_t) attribute_values_data_size;

		if( memory_copy(
		     attribute_values->value_data,
		     attribute_values_data,
		     attribute_values_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( attribute_values->value_data != NULL )
	{
		memory_free(
		 attribute_values->value_data );

		attribute_values->value_data = NULL;
	}
	attribute_values->value_data_size = 0;

	return( -1 );
}

/* Compares an UTF-8 string with an attribute name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsapfs_attribute_values_compare_name_with_utf8_string(
     libfsapfs_attribute_values_t *attribute_values,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_compare_name_with_utf8_string";
	int result            = 0;

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	result = libuna_utf8_string_compare_with_utf8_stream(
	          utf8_string,
	          utf8_string_length,
	          attribute_values->name,
	          attribute_values->name_size,
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
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_attribute_values_get_utf8_name_size(
     libfsapfs_attribute_values_t *attribute_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_get_utf8_name_size";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf8_stream(
	     attribute_values->name,
	     (size_t) attribute_values->name_size,
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
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_attribute_values_get_utf8_name(
     libfsapfs_attribute_values_t *attribute_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_get_utf8_name";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     attribute_values->name,
	     (size_t) attribute_values->name_size,
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

/* Compares an UTF-16 string with an attribute name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsapfs_attribute_values_compare_name_with_utf16_string(
     libfsapfs_attribute_values_t *attribute_values,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_compare_name_with_utf16_string";
	int result            = 0;

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	result = libuna_utf16_string_compare_with_utf8_stream(
	          utf16_string,
	          utf16_string_length,
	          attribute_values->name,
	          attribute_values->name_size,
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
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_attribute_values_get_utf16_name_size(
     libfsapfs_attribute_values_t *attribute_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_get_utf16_name_size";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf8_stream(
	     attribute_values->name,
	     (size_t) attribute_values->name_size,
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
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_attribute_values_get_utf16_name(
     libfsapfs_attribute_values_t *attribute_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_get_utf16_name";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     attribute_values->name,
	     (size_t) attribute_values->name_size,
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

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_attribute_values_get_number_of_extents(
     libfsapfs_attribute_values_t *attribute_values,
     int *number_of_extents,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_get_number_of_extents";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( attribute_values->value_data_file_extents == NULL )
	{
		if( number_of_extents == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of extents.",
			 function );

			return( -1 );
		}
		*number_of_extents = 0;
	}
	else
	{
		if( libcdata_array_get_number_of_entries(
		     attribute_values->value_data_file_extents,
		     number_of_extents,
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
	}
	return( 1 );
}

/* Retrieves a specific extent
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_attribute_values_get_extent_by_index(
     libfsapfs_attribute_values_t *attribute_values,
     int extent_index,
     libfsapfs_file_extent_t **file_extent,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_attribute_values_get_extent_by_index";

	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     attribute_values->value_data_file_extents,
	     extent_index,
	     (intptr_t **) file_extent,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file extent: %d.",
		 function,
		 extent_index );

		return( -1 );
	}
	return( 1 );
}

