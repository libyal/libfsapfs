/*
 * Extended attribute functions
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

#include "libfsapfs_attributes.h"
#include "libfsapfs_data_stream.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_extended_attribute.h"
#include "libfsapfs_file_extent.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_libfdatetime.h"
#include "libfsapfs_types.h"

/* Creates an extended_attribute
 * Make sure the value extended_attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_initialize(
     libfsapfs_extended_attribute_t **extended_attribute,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_attribute_values_t *attribute_values,
     uint64_t transaction_identifier,
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
	internal_extended_attribute->io_handle              = io_handle;
	internal_extended_attribute->file_io_handle         = file_io_handle;
	internal_extended_attribute->encryption_context     = encryption_context;
	internal_extended_attribute->file_system_btree      = file_system_btree;
	internal_extended_attribute->attribute_values       = attribute_values;
	internal_extended_attribute->transaction_identifier = transaction_identifier;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBFSAPFS )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_extended_attribute->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
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
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_free";
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
	if( *extended_attribute != NULL )
	{
		internal_extended_attribute = (libfsapfs_internal_extended_attribute_t *) *extended_attribute;
		*extended_attribute         = NULL;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBFSAPFS )
		if( libcthreads_read_write_lock_free(
		     &( internal_extended_attribute->read_write_lock ),
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
		if( internal_extended_attribute->data_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( internal_extended_attribute->data_stream ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data stream.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_extended_attribute );
	}
	return( result );
}

/* Retrieves the data stream
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_data_stream(
     libfsapfs_extended_attribute_t *extended_attribute,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_data_stream";

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

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( internal_extended_attribute->data_stream == NULL )
	{
		if( libfsapfs_internal_extended_attribute_get_data_stream(
		     internal_extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine data stream.",
			 function );

			return( -1 );
		}
	}
	*data_stream = internal_extended_attribute->data_stream;

	return( 1 );
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
	if( libfsapfs_attribute_values_get_utf8_name_size(
	     internal_extended_attribute->attribute_values,
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
	if( libfsapfs_attribute_values_get_utf8_name(
	     internal_extended_attribute->attribute_values,
	     utf8_string,
	     utf8_string_size,
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
	if( libfsapfs_attribute_values_get_utf16_name_size(
	     internal_extended_attribute->attribute_values,
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
	if( libfsapfs_attribute_values_get_utf16_name(
	     internal_extended_attribute->attribute_values,
	     utf16_string,
	     utf16_string_size,
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

/* Determines the data stream
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_internal_extended_attribute_get_data_stream(
     libfsapfs_internal_extended_attribute_t *internal_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_internal_extended_attribute_get_data_stream";

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
	if( internal_extended_attribute->data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute - data stream value already set.",
		 function );

		return( -1 );
	}
	if( libfsapfs_attributes_get_data_stream(
	     internal_extended_attribute->attribute_values,
	     internal_extended_attribute->io_handle,
	     internal_extended_attribute->file_io_handle,
	     internal_extended_attribute->encryption_context,
	     internal_extended_attribute->file_system_btree,
	     internal_extended_attribute->transaction_identifier,
	     &( internal_extended_attribute->data_stream ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute value data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsapfs_extended_attribute_read_buffer(
         libfsapfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_read_buffer";
	ssize_t read_count                                                   = 0;

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
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_extended_attribute->data_stream == NULL )
	{
		if( libfsapfs_internal_extended_attribute_get_data_stream(
		     internal_extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine data stream.",
			 function );

			read_count = -1;
		}
	}
	if( internal_extended_attribute->data_stream != NULL )
	{
		read_count = libfdata_stream_read_buffer(
		              internal_extended_attribute->data_stream,
		              (intptr_t *) internal_extended_attribute->file_io_handle,
		              (uint8_t *) buffer,
		              buffer_size,
		              0,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer from data stream.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsapfs_extended_attribute_read_buffer_at_offset(
         libfsapfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_read_buffer_at_offset";
	ssize_t read_count                                                   = 0;

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
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_extended_attribute->data_stream == NULL )
	{
		if( libfsapfs_internal_extended_attribute_get_data_stream(
		     internal_extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine data stream.",
			 function );

			read_count = -1;
		}
	}
	if( internal_extended_attribute->data_stream != NULL )
	{
		read_count = libfdata_stream_read_buffer_at_offset(
		              internal_extended_attribute->data_stream,
		              (intptr_t *) internal_extended_attribute->file_io_handle,
		              (uint8_t *) buffer,
		              buffer_size,
		              offset,
		              0,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer at offset from data stream.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsapfs_extended_attribute_seek_offset(
         libfsapfs_extended_attribute_t *extended_attribute,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_seek_offset";

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
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_extended_attribute->data_stream == NULL )
	{
		if( libfsapfs_internal_extended_attribute_get_data_stream(
		     internal_extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine data stream.",
			 function );

			offset = -1;
		}
	}
	if( internal_extended_attribute->data_stream != NULL )
	{
		offset = libfdata_stream_seek_offset(
		          internal_extended_attribute->data_stream,
		          offset,
		          whence,
		          error );

		if( offset < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in data stream.",
			 function );

			offset = -1;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_offset(
     libfsapfs_extended_attribute_t *extended_attribute,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_offset";
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
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_extended_attribute->data_stream == NULL )
	{
		if( libfsapfs_internal_extended_attribute_get_data_stream(
		     internal_extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine data stream.",
			 function );

			result = -1;
		}
	}
	if( internal_extended_attribute->data_stream != NULL )
	{
		if( libfdata_stream_get_offset(
		     internal_extended_attribute->data_stream,
		     offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset from data stream.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the data stream object
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size64_t *size,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_size";

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

	if( internal_extended_attribute->attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal extended attribute - missing attribute values.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
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
	*size = internal_extended_attribute->attribute_values->value_data_size;

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

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_number_of_extents(
     libfsapfs_extended_attribute_t *extended_attribute,
     int *number_of_extents,
     libcerror_error_t **error )
{
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_number_of_extents";
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
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_extended_attribute->data_stream == NULL )
	{
		/* Determining the data stream will initialize the extents array
		 */
		if( libfsapfs_internal_extended_attribute_get_data_stream(
		     internal_extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine data stream.",
			 function );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libfsapfs_attribute_values_get_number_of_extents(
		     internal_extended_attribute->attribute_values,
		     number_of_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific extent
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_extended_attribute_get_extent_by_index(
     libfsapfs_extended_attribute_t *extended_attribute,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error )
{
	libfsapfs_file_extent_t *file_extent                                 = NULL;
	libfsapfs_internal_extended_attribute_t *internal_extended_attribute = NULL;
	static char *function                                                = "libfsapfs_extended_attribute_get_extent_by_index";
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

	if( internal_extended_attribute->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal extended attribute - missing IO handle.",
		 function );

		return( -1 );
	}
	if( extent_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent offset.",
		 function );

		return( -1 );
	}
	if( extent_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent size.",
		 function );

		return( -1 );
	}
	if( extent_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_extended_attribute->data_stream == NULL )
	{
		/* Determining the data stream will initialize the extents array
		 */
		if( libfsapfs_internal_extended_attribute_get_data_stream(
		     internal_extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine data stream.",
			 function );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libfsapfs_attribute_values_get_extent_by_index(
		     internal_extended_attribute->attribute_values,
		     extent_index,
		     &file_extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file extent: %d.",
			 function,
			 extent_index );

			result = -1;
		}
		else if( file_extent == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing file extent: %d.",
			 function,
			 extent_index );

			result = -1;
		}
		else
		{
			*extent_offset = file_extent->physical_block_number * internal_extended_attribute->io_handle->block_size;
			*extent_size   = file_extent->data_size;
			*extent_flags  = 0;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

