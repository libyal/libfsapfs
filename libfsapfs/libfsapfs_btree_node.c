/*
 * The B-tree node functions
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

#include "libfsapfs_btree_entry.h"
#include "libfsapfs_btree_footer.h"
#include "libfsapfs_btree_node_header.h"
#include "libfsapfs_btree_node.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_btree.h"
#include "fsapfs_object.h"
#include "fsapfs_object_map.h"

/* Creates a B-tree node
 * Make sure the value btree_node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_node_initialize(
     libfsapfs_btree_node_t **btree_node,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_node_initialize";

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( *btree_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree node value already set.",
		 function );

		return( -1 );
	}
	*btree_node = memory_allocate_structure(
	               libfsapfs_btree_node_t );

	if( *btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_node,
	     0,
	     sizeof( libfsapfs_btree_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree node.",
		 function );

		memory_free(
		 *btree_node );

		*btree_node = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *btree_node )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_node != NULL )
	{
		memory_free(
		 *btree_node );

		*btree_node = NULL;
	}
	return( -1 );
}

/* Frees a B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_node_free(
     libfsapfs_btree_node_t **btree_node,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_node_free";
	int result            = 1;

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( *btree_node != NULL )
	{
		if( ( *btree_node )->node_header != NULL )
		{
			if( libfsapfs_btree_node_header_free(
			     &( ( *btree_node )->node_header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free B-tree node header.",
				 function );

				result = -1;
			}
		}
		if( ( *btree_node )->footer != NULL )
		{
			if( libfsapfs_btree_footer_free(
			     &( ( *btree_node )->footer ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free B-tree footer.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( ( *btree_node )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_btree_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free entries array.",
			 function );

			result = -1;
		}
		memory_free(
		 *btree_node );

		*btree_node = NULL;
	}
	return( result );
}

/* Reads the B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_node_read_data(
     libfsapfs_btree_node_t *btree_node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry = NULL;
	const uint8_t *btree_node_entry      = NULL;
	static char *function                = "libfsapfs_btree_node_read_data";
	size_t btree_entry_data_size         = 0;
	size_t data_offset                   = 0;
	size_t minimum_data_size             = 0;
	size_t remaining_data_size           = 0;
	uint16_t entries_data_offset         = 0;
	uint16_t footer_offset               = 0;
	uint16_t key_data_offset             = 0;
	uint16_t key_data_size               = 0;
	uint16_t map_entry_index             = 0;
	uint16_t value_data_offset           = 0;
	uint16_t value_data_size             = 0;
	int entry_index                      = 0;

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( btree_node->node_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree node - node header value already set.",
		 function );

		return( -1 );
	}
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
	minimum_data_size = sizeof( fsapfs_object_t ) + sizeof( fsapfs_btree_node_header_t ) + sizeof( fsapfs_btree_footer_t );

	if( ( data_size < minimum_data_size )
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
		 "%s: B-tree node data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( libfsapfs_btree_node_read_object_data(
	     btree_node,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree node object data.",
		 function );

		goto on_error;
	}
	data_offset = sizeof( fsapfs_object_t );

	if( libfsapfs_btree_node_header_initialize(
	     &( btree_node->node_header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree node header.",
		 function );

		goto on_error;
	}
	if( libfsapfs_btree_node_header_read_data(
	     btree_node->node_header,
	     &( data[ data_offset ] ),
	     sizeof( fsapfs_btree_node_header_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree node header.",
		 function );

		goto on_error;
	}
	data_offset += sizeof( fsapfs_btree_node_header_t );

	remaining_data_size = data_size - minimum_data_size;

	if( btree_node->node_header->entries_data_offset >= remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entries offset size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= btree_node->node_header->entries_data_offset;

	if( btree_node->node_header->entries_data_size > remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entries data size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= btree_node->node_header->entries_data_size;

	if( btree_node->node_header->unused_data_offset >= remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid unused offset size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= btree_node->node_header->unused_data_offset;

/* TODO this check fails on some container
	if( btree_node->node_header->unused_data_size > remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid unused data size value out of bounds.",
		 function );

		goto on_error;
	}
*/
/* TODO sanity check other data_offset and data_size values */

	footer_offset = (uint16_t) data_size;

	if( ( btree_node->node_header->flags & 0x0001 ) != 0 )
	{
		if( libfsapfs_btree_footer_initialize(
		     &( btree_node->footer ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create B-tree footer.",
			 function );

			goto on_error;
		}
		if( libfsapfs_btree_footer_read_data(
		     btree_node->footer,
		     &( data[ data_size - sizeof( fsapfs_btree_footer_t ) ] ),
		     sizeof( fsapfs_btree_footer_t ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read B-tree footer.",
			 function );

			goto on_error;
		}
		footer_offset -= (uint16_t) sizeof( fsapfs_btree_footer_t );
	}
	if( ( btree_node->node_header->flags & 0x0004 ) == 0 )
	{
		btree_entry_data_size = sizeof( fsapfs_btree_variable_size_entry_t );
	}
	else
	{
		btree_entry_data_size = sizeof( fsapfs_btree_fixed_size_entry_t );
	}
	if( (size_t) btree_node->node_header->number_of_keys > (size_t) ( btree_node->node_header->entries_data_size / btree_entry_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of keys value out of bounds.",
		 function );

		goto on_error;
	}
	data_offset += btree_node->node_header->entries_data_offset;

	entries_data_offset = btree_node->node_header->entries_data_offset + (uint16_t) ( sizeof( fsapfs_object_t ) + sizeof( fsapfs_btree_node_header_t ) );

	for( map_entry_index = 0;
	     map_entry_index < btree_node->node_header->number_of_keys;
	     map_entry_index++ )
	{
		btree_node_entry = &( data[ data_offset ] );

		if( ( btree_node->node_header->flags & 0x0004 ) == 0 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsapfs_btree_variable_size_entry_t *) btree_node_entry )->key_data_offset,
			 key_data_offset );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsapfs_btree_variable_size_entry_t *) btree_node_entry )->key_data_size,
			 key_data_size );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsapfs_btree_variable_size_entry_t *) btree_node_entry )->value_data_offset,
			 value_data_offset );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsapfs_btree_variable_size_entry_t *) btree_node_entry )->value_data_size,
			 value_data_size );
		}
		else
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsapfs_btree_fixed_size_entry_t *) btree_node_entry )->key_data_offset,
			 key_data_offset );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsapfs_btree_fixed_size_entry_t *) btree_node_entry )->value_data_offset,
			 value_data_offset );

			switch( btree_node->object_subtype )
			{
				case 0x0000000bUL:
					key_data_size   = (uint16_t) sizeof( fsapfs_object_map_btree_key_t );
					value_data_size = (uint16_t) sizeof( fsapfs_object_map_btree_value_t );
					break;

				default:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: invalid object subtype: 0x%08" PRIx32 ".",
					 function,
					 btree_node->object_subtype );

					goto on_error;
			}
			if( ( btree_node->node_header->flags & 0x0002 ) == 0 )
			{
				value_data_size = 8;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %03" PRIu16 " key data offset\t\t: 0x%04" PRIx16 " (block offset: 0x%04" PRIzx ")\n",
			 function,
			 map_entry_index,
			 key_data_offset,
			 (size_t) key_data_offset + (size_t) entries_data_offset + (size_t) btree_node->node_header->entries_data_size );

			libcnotify_printf(
			 "%s: entry: %03" PRIu16 " key data size\t\t: %" PRIu16 "\n",
			 function,
			 map_entry_index,
			 key_data_size );

			libcnotify_printf(
			 "%s: entry: %03" PRIu16 " value data offset\t\t: 0x%04" PRIx16 " (block offset: 0x%04" PRIzx ")\n",
			 function,
			 map_entry_index,
			 value_data_offset,
			 (size_t) footer_offset - (size_t) value_data_offset);

			libcnotify_printf(
			 "%s: entry: %03" PRIu16 " value data size\t\t: %" PRIu16 "\n",
			 function,
			 map_entry_index,
			 value_data_size );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += btree_entry_data_size;

		key_data_offset += entries_data_offset + btree_node->node_header->entries_data_size;

		if( ( (size_t) key_data_offset > data_size )
		 || ( (size_t) key_data_size > ( data_size - key_data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid key data offset value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %03" PRIu16 " key data:\n",
			 function,
			 map_entry_index );
			libcnotify_print_data(
			 &( data[ key_data_offset ] ),
			 (size_t) key_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		value_data_offset = footer_offset - value_data_offset;

		if( ( (size_t) value_data_offset > data_size )
		 || ( (size_t) value_data_size > ( data_size - value_data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data offset value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %03" PRIu16 " value data:\n",
			 function,
			 map_entry_index );
			libcnotify_print_data(
			 &( data[ value_data_offset ] ),
			 (size_t) value_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		if( libfsapfs_btree_entry_initialize(
		     &btree_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create B-tree entry.",
			 function );

			goto on_error;
		}
		if( libfsapfs_btree_entry_set_key_data(
		     btree_entry,
		     &( data[ key_data_offset ] ),
		     (size_t) key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set key data in B-tree entry.",
			 function );

			goto on_error;
		}
		if( libfsapfs_btree_entry_set_value_data(
		     btree_entry,
		     &( data[ value_data_offset ] ),
		     (size_t) value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value data in B-tree entry.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     btree_node->entries_array,
		     &entry_index,
		     (intptr_t *) btree_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append B-tree entry: %" PRIu32 " to array.",
			 function,
			 map_entry_index );

			goto on_error;
		}
		btree_entry = NULL;
	}
	return( 1 );

on_error:
	if( btree_entry != NULL )
	{
		libfsapfs_btree_entry_free(
		 &btree_entry,
		 NULL );
	}
	if( btree_node->footer != NULL )
	{
		libfsapfs_btree_footer_free(
		 &( btree_node->footer ),
		 NULL );
	}
	if( btree_node->node_header != NULL )
	{
		libfsapfs_btree_node_header_free(
		 &( btree_node->node_header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the B-tree node object
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_node_read_object_data(
     libfsapfs_btree_node_t *btree_node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_node_read_object_data";
	uint32_t object_type  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
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
		 "%s: B-tree node object data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_object_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->type,
	 btree_node->object_type );

	object_type = btree_node->object_type & 0x0ffffffUL;

	if( ( object_type != 0x00000002UL )
	 && ( object_type != 0x00000003UL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 btree_node->object_type );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->subtype,
	 btree_node->object_subtype );

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
		 "%s: object identifier\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_t *) data )->transaction_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object transaction identifier\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object type\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 btree_node->object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 btree_node->object_subtype );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Determines if the node is a leaf node
 * Returns 1 if the node is a leaf node, 0 if not or -1 on error
 */
int libfsapfs_btree_node_is_leaf_node(
     libfsapfs_btree_node_t *btree_node,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_node_is_leaf_node";

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( btree_node->node_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree node - missing node header.",
		 function );

		return( -1 );
	}
	return( btree_node->node_header->flags & 0x0002 );
}

/* Retrieves the number of B-tree entries
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_node_get_number_of_entries(
     libfsapfs_btree_node_t *btree_node,
     int *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_node_get_number_of_entries";

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     btree_node->entries_array,
	     number_of_entries,
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
	return( 1 );
}

/* Retrieves a specific of B-tree entry
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_node_get_entry_by_index(
     libfsapfs_btree_node_t *btree_node,
     int entry_index,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_node_get_entry_by_index";

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     btree_node->entries_array,
	     entry_index,
	     (intptr_t **) btree_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from array.",
		 function,
		 entry_index );

		return( -1 );
	}
	return( 1 );
}

