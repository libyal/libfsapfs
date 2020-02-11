/*
 * The APFS object functions
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

#include "libfsapfs_object.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfguid.h"

#include "fsapfs_object.h"

/* Creates object
 * Make sure the value object is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_initialize(
     libfsapfs_object_t **object,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_initialize";

	if( object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object.",
		 function );

		return( -1 );
	}
	if( *object != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid object value already set.",
		 function );

		return( -1 );
	}
	*object = memory_allocate_structure(
	                         libfsapfs_object_t );

	if( *object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create object.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *object,
	     0,
	     sizeof( libfsapfs_object_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear object.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *object != NULL )
	{
		memory_free(
		 *object );

		*object = NULL;
	}
	return( -1 );
}

/* Frees object
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_free(
     libfsapfs_object_t **object,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_free";

	if( object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object.",
		 function );

		return( -1 );
	}
	if( *object != NULL )
	{
		memory_free(
		 *object );

		*object = NULL;
	}
	return( 1 );
}

/* Reads the object
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_read_file_io_handle(
     libfsapfs_object_t *object,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	fsapfs_object_t object_data;

	static char *function = "libfsapfs_object_read_file_io_handle";
	ssize_t read_count    = 0;

	if( object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading object at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek object offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &object_data,
	              sizeof( fsapfs_object_t ),
	              error );

	if( read_count != (ssize_t) sizeof( fsapfs_object_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object data.",
		 function );

		return( -1 );
	}
	if( libfsapfs_object_read_data(
	     object,
	     (uint8_t *) &object_data,
	     sizeof( fsapfs_object_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the object
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_read_data(
     libfsapfs_object_t *object,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object.",
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
	if( ( data_size < sizeof( fsapfs_object_t ) )
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
		 "%s: object data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_object_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_t *) data )->identifier,
	 object->identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_t *) data )->transaction_identifier,
	 object->transaction_identifier );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->type,
	 object->type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->subtype,
	 object->subtype );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_t *) data )->checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: identifier\t\t\t\t\t: %" PRIu64 "\n",
		 function,
		 object->identifier );

		libcnotify_printf(
		 "%s: transaction identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 object->transaction_identifier );

		libcnotify_printf(
		 "%s: type\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object->type );

		libcnotify_printf(
		 "%s: subtype\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object->subtype );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

