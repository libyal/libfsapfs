/*
 * Extended attribute functions
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
#include "libfsapfs_libfdata.h"
#include "libfsapfs_libfdatetime.h"
#include "libfsapfs_libuna.h"

#include "fsapfs_file_system.h"

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
	internal_extended_attribute->io_handle          = io_handle;
	internal_extended_attribute->file_io_handle     = file_io_handle;
	internal_extended_attribute->encryption_context = encryption_context;
	internal_extended_attribute->file_system_btree  = file_system_btree;

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
		if( ( *internal_extended_attribute )->data_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( ( *internal_extended_attribute )->data_stream ),
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
		if( ( *internal_extended_attribute )->file_extents != NULL )
		{
			if( libcdata_array_free(
			     &( ( *internal_extended_attribute )->file_extents ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file extents array.",
				 function );

				result = -1;
			}
		}
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
	const uint8_t *extended_attribute_data                               = NULL;
	size_t data_offset                                                   = 0;
	uint16_t extended_attribute_flags                                    = 0;
	uint16_t extended_attribute_data_size                                = 0;

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
		 "%s: invalid extended attribute data size value out of bounds.",
		 function );

		goto on_error;
	}
	extended_attribute_data = &( data[ data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extended attribute data:\n",
		 function );
		libcnotify_print_data(
		 extended_attribute_data,
		 (size_t) extended_attribute_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( extended_attribute_flags & 0x0001 ) != 0 )
	{
		if( (size_t) extended_attribute_data_size != sizeof( fsapfs_file_system_extended_attribute_data_stream_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported extended attribute data size.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_extended_attribute_data_stream_t *) extended_attribute_data )->data_stream_identifier,
		 internal_extended_attribute->data_stream_identifier );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_extended_attribute_data_stream_t *) extended_attribute_data )->used_size,
		 internal_extended_attribute->data_stream_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data stream indentifier\t: %" PRIu64 "\n",
			 function,
			 internal_extended_attribute->data_stream_identifier );

			libcnotify_printf(
			 "%s: used size\t\t\t: %" PRIu64 "\n",
			 function,
			 internal_extended_attribute->data_stream_size );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_extended_attribute_data_stream_t *) extended_attribute_data )->allocated_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: allocated size\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_extended_attribute_data_stream_t *) extended_attribute_data )->encryption_identifier,
			 value_64bit );
			libcnotify_printf(
			 "%s: encryption identifier\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_extended_attribute_data_stream_t *) extended_attribute_data )->number_of_bytes_written,
			 value_64bit );
			libcnotify_printf(
			 "%s: number of bytes written\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_extended_attribute_data_stream_t *) extended_attribute_data )->number_of_bytes_read,
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
	else if( ( extended_attribute_flags & 0x0002 ) != 0 )
	{
		internal_extended_attribute->data = (uint8_t *) memory_allocate(
		                                                 sizeof( uint8_t ) * extended_attribute_data_size );

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
		internal_extended_attribute->data_stream_size = (size64_t) extended_attribute_data_size;

		if( memory_copy(
		     internal_extended_attribute->data,
		     extended_attribute_data,
		     extended_attribute_data_size ) == NULL )
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
	internal_extended_attribute->data_stream_size = 0;

	return( -1 );
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

/* Compares an UTF-8 string with an extended attribute name
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

/* Compares an UTF-16 string with an extended attribute name
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

/* Determines the file extents
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_internal_extended_attribute_get_file_extents(
     libfsapfs_internal_extended_attribute_t *internal_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_internal_extended_attribute_get_file_extents";
	int result            = 0;

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
	if( internal_extended_attribute->file_extents != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute - file extents value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_extended_attribute->file_extents ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file extents array.",
		 function );

		goto on_error;
	}
	result = libfsapfs_file_system_btree_get_file_extents(
		  internal_extended_attribute->file_system_btree,
		  internal_extended_attribute->file_io_handle,
		  internal_extended_attribute->data_stream_identifier,
		  internal_extended_attribute->file_extents,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file extents from file system B-tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_extended_attribute->file_extents != NULL )
	{
		libcdata_array_free(
		 &( internal_extended_attribute->file_extents ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
		 NULL );
	}
	return( -1 );
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
	if( internal_extended_attribute->data != NULL )
	{
		if( libfsapfs_data_stream_initialize_from_data(
		     &( internal_extended_attribute->data_stream ),
		     internal_extended_attribute->data,
		     internal_extended_attribute->data_stream_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data stream from data.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( internal_extended_attribute->file_extents == NULL )
		{
			if( libfsapfs_internal_extended_attribute_get_file_extents(
			     internal_extended_attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine file extents.",
				 function );

				goto on_error;
			}
		}
		if( libfsapfs_data_stream_initialize_from_file_extents(
		     &( internal_extended_attribute->data_stream ),
		     internal_extended_attribute->io_handle,
		     internal_extended_attribute->encryption_context,
		     internal_extended_attribute->file_extents,
		     internal_extended_attribute->data_stream_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data stream from file extents.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( internal_extended_attribute->data_stream != NULL )
	{
		libfdata_stream_free(
		 &( internal_extended_attribute->data_stream ),
		 NULL );
	}
	return( -1 );
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

			goto on_error;
		}
	}
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

		goto on_error;
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

on_error:
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
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

			goto on_error;
		}
	}
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

		goto on_error;
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

on_error:
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
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

			goto on_error;
		}
	}
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

		goto on_error;
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

on_error:
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
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

			goto on_error;
		}
	}
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

		goto on_error;
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

on_error:
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
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
	*size = internal_extended_attribute->data_stream_size;

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
	return( 1 );
}

