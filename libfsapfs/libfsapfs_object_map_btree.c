/*
 * The object map_btree functions
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

#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_object_map_btree.h"
#include "libfsapfs_object_map_descriptor.h"

#include "fsapfs_btree.h"
#include "fsapfs_object.h"
#include "fsapfs_object_map.h"

/* Creates a object map B-tree
 * Make sure the value object_map_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_btree_initialize(
     libfsapfs_object_map_btree_t **object_map_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_map_btree_initialize";

	if( object_map_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map B-tree.",
		 function );

		return( -1 );
	}
	if( *object_map_btree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid object map B-tree value already set.",
		 function );

		return( -1 );
	}
	*object_map_btree = memory_allocate_structure(
	                               libfsapfs_object_map_btree_t );

	if( *object_map_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create object map B-tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *object_map_btree,
	     0,
	     sizeof( libfsapfs_object_map_btree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear object map B-tree.",
		 function );

		memory_free(
		 *object_map_btree );

		*object_map_btree = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *object_map_btree )->descriptors_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptors array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *object_map_btree != NULL )
	{
		memory_free(
		 *object_map_btree );

		*object_map_btree = NULL;
	}
	return( -1 );
}

/* Frees a object map_btree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_btree_free(
     libfsapfs_object_map_btree_t **object_map_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_object_map_btree_free";
	int result            = 1;

	if( object_map_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map B-tree.",
		 function );

		return( -1 );
	}
	if( *object_map_btree != NULL )
	{
		if( libcdata_array_free(
		     &( ( *object_map_btree )->descriptors_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_object_map_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free descriptors array.",
			 function );

			result = -1;
		}
		memory_free(
		 *object_map_btree );

		*object_map_btree = NULL;
	}
	return( result );
}

/* Reads the object map_btree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_btree_read_file_io_handle(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t block_size,
     libcerror_error_t **error )
{
	uint8_t *block_data   = NULL;
	static char *function = "libfsapfs_object_map_btree_read_file_io_handle";
	ssize_t read_count    = 0;

	if( object_map_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map B-tree.",
		 function );

		return( -1 );
	}
#if ( SIZEOF_SIZE_T <= 4 )
	if( block_size > (uint32_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block size value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	block_data = (uint8_t *) memory_allocate(
	                          sizeof( uint8_t ) * block_size );

	if( block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create object map B-tree.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading object map B-tree at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek object map B-tree offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              block_data,
	              (size_t) block_size,
	              error );

	if( read_count != (ssize_t) block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object map B-tree data.",
		 function );

		goto on_error;
	}
	if( libfsapfs_object_map_btree_read_data(
	     object_map_btree,
	     block_data,
	     (size_t) block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object map B-tree data.",
		 function );

		goto on_error;
	}
	memory_free(
	 block_data );

	return( 1 );

on_error:
	if( block_data != NULL )
	{
		memory_free(
		 block_data );
	}
	return( -1 );
}

/* Reads the object map_btree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_btree_read_data(
     libfsapfs_object_map_btree_t *object_map_btree,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	fsapfs_btree_fixed_size_entry_t *btree_entry             = NULL;
	fsapfs_btree_footer_t *btree_footer                      = NULL;
	fsapfs_btree_header_t *btree_header                      = NULL;
	libfsapfs_object_map_descriptor_t *object_map_descriptor = NULL;
	static char *function                                    = "libfsapfs_object_map_btree_read_data";
	size_t data_offset                                       = 0;
	size_t remaining_data_size                               = 0;
	uint64_t map_entry_index                                 = 0;
	uint64_t number_of_entries                               = 0;
	uint32_t key_size                                        = 0;
	uint32_t maximum_key_size                                = 0;
	uint32_t maximum_value_size                              = 0;
	uint32_t object_subtype                                  = 0;
	uint32_t object_type                                     = 0;
	uint32_t value_size                                      = 0;
	uint16_t entries_data_offset                             = 0;
	uint16_t entries_data_size                               = 0;
	uint16_t flags                                           = 0;
	uint16_t footer_offset                                   = 0;
	uint16_t key_data_offset                                 = 0;
	uint16_t unused_data_offset                              = 0;
	uint16_t unused_data_size                                = 0;
	uint16_t value_data_offset                               = 0;
	int entry_index                                          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                                     = 0;
	uint32_t value_32bit                                     = 0;
	uint16_t value_16bit                                     = 0;
#endif

	if( object_map_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map B-tree.",
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
		 "%s: object map B-tree data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object map B-tree object data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_object_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->type,
	 object_type );

	if( object_type != 0x40000002UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 object_type );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->subtype,
	 object_subtype );

	if( object_subtype != 0x0000000bUL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 object_subtype );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_t *) data )->checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: object checksum\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_t *) data )->identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_t *) data )->version,
		 value_64bit );
		libcnotify_printf(
		 "%s: object version\t\t\t: %" PRIu64 "\n",
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

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsapfs_object_t );

	if( data_size < ( data_offset + sizeof( fsapfs_btree_header_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	btree_header = (fsapfs_btree_header_t *) &( data[ data_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object map B-tree header data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) btree_header,
		 sizeof( fsapfs_btree_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 btree_header->flags,
	 flags );

	byte_stream_copy_to_uint16_little_endian(
	 btree_header->entries_data_offset,
	 entries_data_offset );

	byte_stream_copy_to_uint16_little_endian(
	 btree_header->entries_data_size,
	 entries_data_size );

	byte_stream_copy_to_uint16_little_endian(
	 btree_header->unused_data_offset,
	 unused_data_offset );

	byte_stream_copy_to_uint16_little_endian(
	 btree_header->unused_data_size,
	 unused_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: flags\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 flags );

		byte_stream_copy_to_uint16_little_endian(
		 btree_header->level,
		 value_16bit );
		libcnotify_printf(
		 "%s: level\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 btree_header->number_of_keys,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of keys\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		remaining_data_size = sizeof( fsapfs_object_t ) + sizeof( fsapfs_btree_header_t );

		libcnotify_printf(
		 "%s: entries data offset\t\t: 0x%04" PRIx16 " (data offset: 0x%04" PRIzx ")\n",
		 function,
		 entries_data_offset,
		 (size_t) entries_data_offset + remaining_data_size );

		remaining_data_size += entries_data_offset;

		libcnotify_printf(
		 "%s: entries data size\t\t\t: %" PRIu16 " (data offset: 0x%04" PRIzx ")\n",
		 function,
		 entries_data_size,
		 (size_t) entries_data_size + remaining_data_size );

		remaining_data_size += entries_data_size;

		libcnotify_printf(
		 "%s: unused data offset\t\t: 0x%04" PRIx16 " (data offset: 0x%04" PRIzx ")\n",
		 function,
		 unused_data_offset,
		 (size_t) unused_data_offset + remaining_data_size );

		remaining_data_size += unused_data_offset;

		libcnotify_printf(
		 "%s: unused data size\t\t\t: %" PRIu16 " (data offset: 0x%04" PRIzx ")\n",
		 function,
		 unused_data_size,
		 (size_t) unused_data_size + remaining_data_size );

		byte_stream_copy_to_uint16_little_endian(
		 btree_header->unknown5,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 btree_header->unknown6,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 btree_header->unknown7,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 btree_header->unknown8,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( flags != 0x0007 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%04" PRIx16 ".",
		 function );

		goto on_error;
	}
	data_offset += sizeof( fsapfs_btree_header_t );

	if( data_size < ( data_offset + sizeof( fsapfs_btree_footer_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size = data_size - data_offset - sizeof( fsapfs_btree_footer_t );

	if( entries_data_offset >= remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entries offset size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= entries_data_offset;

	if( entries_data_size > remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entries data size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= entries_data_size;

	if( unused_data_offset >= remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid unused offset size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= unused_data_offset;

	if( unused_data_size > remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid unused data size value out of bounds.",
		 function );

		goto on_error;
	}
/* TODO sanity check other data_offset and data_size values */

	btree_footer = (fsapfs_btree_footer_t *) &( data[ data_size - sizeof( fsapfs_btree_footer_t ) ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object map B-tree footer data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) btree_footer,
		 sizeof( fsapfs_btree_footer_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 btree_footer->key_size,
	 key_size );

	byte_stream_copy_to_uint32_little_endian(
	 btree_footer->value_size,
	 value_size );

	byte_stream_copy_to_uint32_little_endian(
	 btree_footer->maximum_key_size,
	 maximum_key_size );

	byte_stream_copy_to_uint32_little_endian(
	 btree_footer->maximum_value_size,
	 maximum_value_size );

	byte_stream_copy_to_uint64_little_endian(
	 btree_footer->number_of_entries,
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 btree_footer->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 btree_footer->node_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: node size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: key size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 key_size );

		libcnotify_printf(
		 "%s: value size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_size );

		libcnotify_printf(
		 "%s: maximum key size\t\t\t: %" PRIu32 "\n",
		 function,
		 maximum_key_size );

		libcnotify_printf(
		 "%s: maximum value size\t\t: %" PRIu32 "\n",
		 function,
		 maximum_value_size );

		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu64 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint64_little_endian(
		 btree_footer->number_of_nodes,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of nodes\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( key_size != sizeof( fsapfs_object_map_btree_key_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key size value out of bounds.",
		 function );

		goto on_error;
	}
	if( value_size != sizeof( fsapfs_object_map_btree_value_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value size value out of bounds.",
		 function );

		goto on_error;
	}
	if( number_of_entries > (uint64_t) ( entries_data_size / sizeof( fsapfs_btree_fixed_size_entry_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
	data_offset += entries_data_offset;

	entries_data_offset += (uint16_t) ( sizeof( fsapfs_object_t ) + sizeof( fsapfs_btree_header_t ) );
	footer_offset        = (uint16_t) ( data_size - sizeof( fsapfs_btree_footer_t ) );

	for( map_entry_index = 0;
	     map_entry_index < number_of_entries;
	     map_entry_index++ )
	{
		btree_entry = (fsapfs_btree_fixed_size_entry_t *) &( data[ data_offset ] );

		byte_stream_copy_to_uint16_little_endian(
		 btree_entry->key_data_offset,
		 key_data_offset );

		byte_stream_copy_to_uint16_little_endian(
		 btree_entry->value_data_offset,
		 value_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %" PRIu64 " key data offset\t\t: 0x%04" PRIx16 " (data offset: 0x%04" PRIzx ")\n",
			 function,
			 map_entry_index,
			 key_data_offset,
			 (size_t) key_data_offset + (size_t) entries_data_offset + (size_t) entries_data_size );

			libcnotify_printf(
			 "%s: entry: %" PRIu64 " value data offset\t: 0x%04" PRIx16 " (data offset: 0x%04" PRIzx ")\n",
			 function,
			 map_entry_index,
			 value_data_offset,
			 (size_t) footer_offset - (size_t) value_data_offset);

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsapfs_btree_fixed_size_entry_t );

		key_data_offset += entries_data_offset + entries_data_size;

		if( ( (size_t) key_data_offset > data_size )
		 || ( (size_t) key_size > ( data_size - key_data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid key data offset value out of bounds.",
			 function );

			goto on_error;
		}
		value_data_offset = footer_offset - value_data_offset;

		if( ( (size_t) value_data_offset > data_size )
		 || ( (size_t) value_size > ( data_size - value_data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( libfsapfs_object_map_descriptor_initialize(
		     &object_map_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create object map descriptor.",
			 function );

			goto on_error;
		}
		if( libfsapfs_object_map_descriptor_read_btree_key_data(
		     object_map_descriptor,
		     &( data[ key_data_offset ] ),
		     sizeof( fsapfs_object_map_btree_key_t ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read entry: %" PRIu64 " object map key data.",
			 function,
			 map_entry_index );

			goto on_error;
		}
		if( libfsapfs_object_map_descriptor_read_btree_value_data(
		     object_map_descriptor,
		     &( data[ value_data_offset ] ),
		     sizeof( fsapfs_object_map_btree_value_t ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read entry: %" PRIu64 " object map value data.",
			 function,
			 map_entry_index );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     object_map_btree->descriptors_array,
		     &entry_index,
		     (intptr_t *) object_map_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append map entry: %" PRIu32 " to array.",
			 function,
			 map_entry_index );

			goto on_error;
		}
		object_map_descriptor = NULL;
	}
	return( 1 );

on_error:
	if( object_map_descriptor != NULL )
	{
		libfsapfs_object_map_descriptor_free(
		 &object_map_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the object map descriptor of a specific object identifier
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libfsapfs_object_map_get_descriptor_by_object_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     uint64_t object_identifier,
     libfsapfs_object_map_descriptor_t **descriptor,
     libcerror_error_t **error )
{
	static char *function     = "libfsapfs_object_map_get_descriptor_by_object_identifier";
	int descriptor_index      = 0;
	int number_of_descriptors = 0;

	if( object_map_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object map B-tree.",
		 function );

		return( -1 );
	}
	if( descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid descriptor.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     object_map_btree->descriptors_array,
	     &number_of_descriptors,
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
	for( descriptor_index = 0;
	     descriptor_index < number_of_descriptors;
	     descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     object_map_btree->descriptors_array,
		     descriptor_index,
		     (intptr_t **) descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from array.",
			 function,
			 descriptor_index );

			return( -1 );
		}
		if( *descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing entry: %d.",
			 function,
			 descriptor_index );

			return( -1 );
		}
		if( object_identifier == ( *descriptor )->identifier )
		{
			return( 1 );
		}
		*descriptor = NULL;
	}
	return( 0 );
}

