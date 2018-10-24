/*
 * Extended attribute functions
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
#include "libfsapfs_extended_attribute.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfdatetime.h"
#include "libfsapfs_libuna.h"

#include "fsapfs_file_system.h"

/* Creates an extended_attribute
 * Make sure the value extended_attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_initialize(
     libfsapfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_initialize";

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute value already set.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = memory_allocate_structure(
	                               libfsapfs_internal_extended_attribute_t );

	if( internal_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extended attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_extended_attribute,
	     0,
	     sizeof( libfsapfs_internal_extended_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extended attribute.",
		 function );

		memory_free(
		 internal_extended_attribute );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBFSAPFS )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_extended_attribute->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to intialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*extended_attribute = (libfsapfs_extended_attribute_t *) internal_extended_attribute;

	return( 1 );

on_error:
	if( internal_extended_attribute != NULL )
	{
		memory_free(
		 internal_extended_attribute );
	}
	return( -1 );
}

/* Frees an extended attribute
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_free(
     libfsapfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_extended_attribute_free";

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *extended_attribute != NULL )
	{
		*extended_attribute = NULL;
	}
	return( 1 );
}

/* Frees an extended attribute
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_internal_extended_attribute_free(
     libfsapfs_internal_extended_attribute_t **internal_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_extended_attribute_free";
	int result            = 1;

	if( internal_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *internal_extended_attribute != NULL )
	{
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBFSAPFS )
		if( libcthreads_read_write_lock_free(
		     &( ( *internal_extended_attribute )->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( ( *internal_extended_attribute )->data != NULL )
		{
			memory_free(
			 ( *internal_extended_attribute )->data );
		}
		if( ( *internal_extended_attribute )->name != NULL )
		{
			memory_free(
			 ( *internal_extended_attribute )->name );
		}
		memory_free(
		 *internal_extended_attribute );

		*internal_extended_attribute = NULL;
	}
	return( result );
}

/* Reads the extended attribute key data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_read_key_data(
     libfsapfs_extended_attribute_t *extended_attribute,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_read_key_data";
	size_t data_offset                                                   = 0;
	uint16_t name_size                                                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                                                 = 0;
#endif

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

	if( internal_extended_attribute->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute - name value already set.",
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
		 "%s: extended attribute key data:\n",
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
	internal_extended_attribute->name = (uint8_t *) memory_allocate(
	                                                 sizeof( uint8_t ) * name_size );

	if( internal_extended_attribute->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	internal_extended_attribute->name_size = name_size;

	if( memory_copy(
	     internal_extended_attribute->name,
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
	if( internal_extended_attribute->name != NULL )
	{
		memory_free(
		 internal_extended_attribute->name );

		internal_extended_attribute->name = NULL;
	}
	internal_extended_attribute->name_size = 0;

	return( -1 );
}

/* Reads the extended attribute value data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_read_value_data(
     libfsapfs_extended_attribute_t *extended_attribute,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_read_value_data";
	size_t data_offset                                                   = 0;
	uint16_t extended_attribute_flags                                    = 0;
	uint16_t extended_attribute_data_size                                = 0;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

	if( internal_extended_attribute->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute - data value already set.",
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
		 "%s: extended attribute value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_file_system_btree_value_extended_attribute_t *) data )->flags,
	 extended_attribute_flags );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsapfs_file_system_btree_value_extended_attribute_t *) data )->data_size,
	 extended_attribute_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: flags\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 extended_attribute_flags );
		libfsapfs_debug_print_extended_attribute_flags(
		 extended_attribute_flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: data size\t\t\t: %" PRIu16 "\n",
		 function,
		 extended_attribute_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsapfs_file_system_btree_value_extended_attribute_t );

	if( (size_t) extended_attribute_data_size > ( data_size - data_offset ) )
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
		 "%s: extended attribute data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 (size_t) extended_attribute_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( extended_attribute_flags & 0x0001 ) != 0 )
	{
/* TODO handle */
	}
	else if( ( extended_attribute_flags & 0x0002 ) != 0 )
	{
		internal_extended_attribute->data = (uint8_t *) memory_allocate(
		                                                 sizeof( uint8_t ) * data_size );

		if( internal_extended_attribute->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		internal_extended_attribute->data_size = data_size;

		if( memory_copy(
		     internal_extended_attribute->data,
		     &( data[ data_offset ] ),
		     data_size ) == NULL )
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
	if( internal_extended_attribute->data != NULL )
	{
		memory_free(
		 internal_extended_attribute->data );

		internal_extended_attribute->data = NULL;
	}
	internal_extended_attribute->data_size = 0;

	return( -1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_identifier(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint64_t *identifier,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_identifier";

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

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
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*identifier = internal_extended_attribute->identifier;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_utf8_name_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_utf8_name_size";
	int result                                                           = 1;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libuna_utf8_string_size_from_utf8_stream(
	     internal_extended_attribute->name,
	     (size_t) internal_extended_attribute->name_size,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_utf8_name(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_utf8_name";
	int result                                                           = 1;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     internal_extended_attribute->name,
	     (size_t) internal_extended_attribute->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Compares the name with an UTF-8 encoded string
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsapfs_extended_attribute_compare_name_with_utf8_string(
     libfsapfs_extended_attribute_t *extended_attribute,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_compare_name_with_utf8_string";
	int result                                                           = 0;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

	if( internal_extended_attribute->name != NULL )
	{
		result = libuna_utf8_string_compare_with_utf8_stream(
		          utf8_string,
		          utf8_string_length,
		          internal_extended_attribute->name,
		          internal_extended_attribute->name_size,
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
int libfsapfs_extended_attribute_get_utf16_name_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_utf16_name_size";
	int result                                                           = 1;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libuna_utf16_string_size_from_utf8_stream(
	     internal_extended_attribute->name,
	     (size_t) internal_extended_attribute->name_size,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_utf16_name(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_utf16_name";
	int result                                                           = 1;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     internal_extended_attribute->name,
	     (size_t) internal_extended_attribute->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Compares the name with an UTF-16 encoded string
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsapfs_extended_attribute_compare_name_with_utf16_string(
     libfsapfs_extended_attribute_t *extended_attribute,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_compare_name_with_utf16_string";
	int result                                                           = 0;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

	if( internal_extended_attribute->name != NULL )
	{
		result = libuna_utf16_string_compare_with_utf8_stream(
		          utf16_string,
		          utf16_string_length,
		          internal_extended_attribute->name,
		          internal_extended_attribute->name_size,
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

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_data(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_data";

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) extended_attribute;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data      = internal_extended_attribute->data;
	*data_size = internal_extended_attribute->data_size;

	return( 1 );
}

