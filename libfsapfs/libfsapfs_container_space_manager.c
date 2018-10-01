/*
 * The container space manager functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_container_space_manager.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_container_space_manager.h"

/* Creates a container space manager
 * Make sure the value container_space_manager is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_space_manager_initialize(
     libfsapfs_container_space_manager_t **container_space_manager,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_space_manager_initialize";

	if( container_space_manager == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container space manager.",
		 function );

		return( -1 );
	}
	if( *container_space_manager != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container space manager value already set.",
		 function );

		return( -1 );
	}
	*container_space_manager = memory_allocate_structure(
	                           libfsapfs_container_space_manager_t );

	if( *container_space_manager == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create container space manager.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *container_space_manager,
	     0,
	     sizeof( libfsapfs_container_space_manager_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear container space manager.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *container_space_manager != NULL )
	{
		memory_free(
		 *container_space_manager );

		*container_space_manager = NULL;
	}
	return( -1 );
}

/* Frees a container space manager
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_space_manager_free(
     libfsapfs_container_space_manager_t **container_space_manager,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_space_manager_free";

	if( container_space_manager == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container space manager.",
		 function );

		return( -1 );
	}
	if( *container_space_manager != NULL )
	{
		memory_free(
		 *container_space_manager );

		*container_space_manager = NULL;
	}
	return( 1 );
}

/* Reads the container space manager
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_space_manager_read_file_io_handle(
     libfsapfs_container_space_manager_t *container_space_manager,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	fsapfs_container_space_manager_t container_space_manager_data;

	static char *function = "libfsapfs_container_space_manager_read_file_io_handle";
	ssize_t read_count    = 0;

	if( container_space_manager == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container space manager.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading container space manager at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek container space manager offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &container_space_manager_data,
	              sizeof( fsapfs_container_space_manager_t ),
	              error );

	if( read_count != (ssize_t) sizeof( fsapfs_container_space_manager_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container space manager data.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_space_manager_read_data(
	     container_space_manager,
	     (uint8_t *) &container_space_manager_data,
	     sizeof( fsapfs_container_space_manager_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container space manager data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the container space manager
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_space_manager_read_data(
     libfsapfs_container_space_manager_t *container_space_manager,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsapfs_container_space_manager_read_data";
	uint32_t object_subtype = 0;
	uint32_t object_type    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit    = 0;
	uint32_t value_32bit    = 0;
#endif

	if( container_space_manager == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container space manager.",
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
	if( ( data_size < sizeof( fsapfs_container_space_manager_t ) )
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
		 "%s: container space manager data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_container_space_manager_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_container_space_manager_t *) data )->object_type,
	 object_type );

	if( object_type != 0x80000005UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 object_type );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_container_space_manager_t *) data )->object_subtype,
	 object_subtype );

	if( object_subtype != 0x00000000UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 object_subtype );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->object_checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: object checksum\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->object_transaction_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object transaction identifier\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object type\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_subtype );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->block_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: block size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->blocks_per_chunk,
		 value_32bit );
		libcnotify_printf(
		 "%s: blocks per chunk\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->chunks_per_cib,
		 value_32bit );
		libcnotify_printf(
		 "%s: chunks per CIB\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->cibs_per_cab,
		 value_32bit );
		libcnotify_printf(
		 "%s: CIBs per CAB\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->number_of_blocks,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of blocks\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->number_of_chunks,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of chunks\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->number_of_cibs,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of CIBs\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->number_of_cabs,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of CABs\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->number_of_unused_blocks,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of unused blocks\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown1,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown2,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown3,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown4,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown5,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown6,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown7,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown8,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown9,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown10,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown11,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown11\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown12,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown12\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown13,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown13\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown14,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown14\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown15,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown15\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown16,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown16\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown17,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown17\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown18,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown18\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown19,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown19\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown20,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown20\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown21,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown21\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown22,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown22\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown23,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown23\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown24,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown24\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown25,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown25\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown26,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown26\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown27,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown27\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown28,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown28\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown29,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown29\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown30,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown30\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown31,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown31\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown32,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown32\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_container_space_manager_t *) data )->unknown33,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown33\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

