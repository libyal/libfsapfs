/*
 * The APFS container superblock functions
 *
 * Copyright (C) 2010-2018, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsapfs_container_superblock.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_container_superblock.h"

/* Creates container superblock
 * Make sure the value container_superblock is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_superblock_initialize(
     libfsapfs_container_superblock_t **container_superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_superblock_initialize";

	if( container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container superblock.",
		 function );

		return( -1 );
	}
	if( *container_superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container superblock value already set.",
		 function );

		return( -1 );
	}
	*container_superblock = memory_allocate_structure(
	                         libfsapfs_container_superblock_t );

	if( *container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create container superblock.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *container_superblock,
	     0,
	     sizeof( libfsapfs_container_superblock_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear container superblock.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *container_superblock != NULL )
	{
		memory_free(
		 *container_superblock );

		*container_superblock = NULL;
	}
	return( -1 );
}

/* Frees container superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_superblock_free(
     libfsapfs_container_superblock_t **container_superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_superblock_free";

	if( container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container superblock.",
		 function );

		return( -1 );
	}
	if( *container_superblock != NULL )
	{
		memory_free(
		 *container_superblock );

		*container_superblock = NULL;
	}
	return( 1 );
}

/* Reads the container superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_superblock_read_file_io_handle(
     libfsapfs_container_superblock_t *container_superblock,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	fsapfs_container_superblock_t container_superblock_data;

	static char *function = "libfsapfs_container_superblock_read_file_io_handle";
	ssize_t read_count    = 0;

	if( container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container superblock.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading container superblock at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek container superblock offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &container_superblock_data,
	              sizeof( fsapfs_container_superblock_t ),
	              error );

	if( read_count != (ssize_t) sizeof( fsapfs_container_superblock_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container superblock data.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_superblock_read_data(
	     container_superblock,
	     (uint8_t *) &container_superblock_data,
	     sizeof( fsapfs_container_superblock_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container superblock data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the container superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_superblock_read_data(
     libfsapfs_container_superblock_t *container_superblock,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_superblock_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
#endif

	if( container_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container superblock.",
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
	if( ( data_size < sizeof( fsapfs_container_superblock_t ) )
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
		 "%s: container superblock data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_container_superblock_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (fsapfs_container_superblock_t *) data )->signature,
	     fsapfs_volume_file_system_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->object_checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: object checksum\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->object_unknown1,
		 value_64bit );
		libcnotify_printf(
		 "%s: object unknown1\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->object_type,
		 value_32bit );
		libcnotify_printf(
		 "%s: object type\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_superblock_t *) data )->object_subtype,
		 value_32bit );
		libcnotify_printf(
		 "%s: object subtype\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: signature\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsapfs_container_superblock_t *) data )->signature[ 0 ],
		 ( (fsapfs_container_superblock_t *) data )->signature[ 1 ],
		 ( (fsapfs_container_superblock_t *) data )->signature[ 2 ],
		 ( (fsapfs_container_superblock_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

