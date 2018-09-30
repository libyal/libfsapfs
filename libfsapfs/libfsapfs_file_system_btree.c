/*
 * The file system B-tree functions
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

#include "libfsapfs_btree_entry.h"
#include "libfsapfs_btree_node.h"
#include "libfsapfs_data_block.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_directory_record.h"
#include "libfsapfs_file_extent.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_libuna.h"

#include "fsapfs_btree.h"
#include "fsapfs_file_system.h"
#include "fsapfs_object.h"

/* Creates a file system B-tree
 * Make sure the value file_system_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_initialize(
     libfsapfs_file_system_btree_t **file_system_btree,
     libfdata_vector_t *data_block_vector,
     libfcache_cache_t *data_block_cache,
     uint64_t root_node_block_number,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_system_btree_initialize";

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( *file_system_btree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system B-tree value already set.",
		 function );

		return( -1 );
	}
	*file_system_btree = memory_allocate_structure(
	                      libfsapfs_file_system_btree_t );

	if( *file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system B-tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system_btree,
	     0,
	     sizeof( libfsapfs_file_system_btree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system B-tree.",
		 function );

		goto on_error;
	}
	( *file_system_btree )->data_block_vector      = data_block_vector;
	( *file_system_btree )->data_block_cache       = data_block_cache;
	( *file_system_btree )->root_node_block_number = root_node_block_number;

	return( 1 );

on_error:
	if( *file_system_btree != NULL )
	{
		memory_free(
		 *file_system_btree );

		*file_system_btree = NULL;
	}
	return( -1 );
}

/* Frees a file system B-tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_free(
     libfsapfs_file_system_btree_t **file_system_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_system_btree_free";

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( *file_system_btree != NULL )
	{
		/* The data_block_vector and data_block_cache are referenced and freed elsewhere
		 */
		memory_free(
		 *file_system_btree );

		*file_system_btree = NULL;
	}
	return( 1 );
}

/* Retrieves the file system B-tree root node
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_get_root_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t **root_node,
     libcerror_error_t **error )
{
	libfsapfs_data_block_t *data_block = NULL;
	static char *function              = "libfsapfs_file_system_btree_get_root_node";

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     file_system_btree->data_block_vector,
	     (intptr_t *) file_io_handle,
	     file_system_btree->data_block_cache,
	     (int) file_system_btree->root_node_block_number,
	     (intptr_t **) &data_block,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data block: %" PRIu64 ".",
		 function,
		 file_system_btree->root_node_block_number );

		goto on_error;
	}
	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block: %" PRIu64 ".",
		 function,
		 file_system_btree->root_node_block_number );

		goto on_error;
	}
	if( libfsapfs_btree_node_initialize(
	     root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree node.",
		 function );

		goto on_error;
	}
	if( libfsapfs_btree_node_read_data(
	     *root_node,
	     data_block->data,
	     data_block->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree node.",
		 function );

		goto on_error;
	}
	if( ( ( *root_node )->object_type != 0x00000002UL )
	 && ( ( *root_node )->object_type != 0x10000002UL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 ( *root_node )->object_type );

		goto on_error;
	}
	if( ( *root_node )->object_subtype != 0x0000000eUL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 ( *root_node )->object_subtype );

		goto on_error;
	}
	if( ( *root_node )->header->flags != 0x0003 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%04" PRIx16 ".",
		 function,
		 ( *root_node )->header->flags );

		goto on_error;
	}
	if( ( *root_node )->footer->key_size != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( *root_node )->footer->value_size != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value size value out of bounds.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *root_node != NULL )
	{
		libfsapfs_btree_node_free(
		 root_node,
		 NULL );
	}
	return( -1 );
}

/* Retrieves an entry for a specific identifier from the file system B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_entry_from_node_by_identifier(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     uint8_t data_type,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *safe_btree_entry = NULL;
	static char *function                     = "libfsapfs_file_system_btree_get_entry_from_node_by_identifier";
	uint64_t file_system_identifier           = 0;
	uint64_t lookup_identifier                = 0;
	int btree_entry_index                     = 0;
	int number_of_entries                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t file_system_data_type             = 0;
#endif

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( btree_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree entry.",
		 function );

		return( -1 );
	}
	lookup_identifier = ( (uint64_t) data_type << 60 ) | identifier;

	if( libfsapfs_btree_node_get_number_of_entries(
	     node,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from B-tree node.",
		 function );

		return( -1 );
	}
/* TODO implement B-tree sub node support */

	for( btree_entry_index = 0;
	     btree_entry_index < number_of_entries;
	     btree_entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     btree_entry_index,
		     &safe_btree_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from B-tree node.",
			 function );

			return( -1 );
		}
		if( safe_btree_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 btree_entry_index );

			return( -1 );
		}
		if( safe_btree_entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 btree_entry_index );

			return( -1 );
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) safe_btree_entry->key_data )->file_system_identifier,
		 file_system_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx64 " %s\n",
			 function,
			 btree_entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( file_system_identifier == lookup_identifier )
		{
			*btree_entry = safe_btree_entry;

			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves a directory record for an UTF-8 encoded name from the file system B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_record_from_node_by_utf8_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry                = NULL;
	libfsapfs_directory_record_t *safe_directory_record = NULL;
	static char *function                               = "libfsapfs_file_system_btree_get_directory_record_from_node_by_utf8_name";
	uint64_t file_system_identifier                     = 0;
	uint64_t lookup_identifier                          = 0;
	int btree_entry_index                               = 0;
	int number_of_entries                               = 0;
	int result                                          = 0;

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( libfsapfs_btree_node_get_number_of_entries(
	     node,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from B-tree node.",
		 function );

		goto on_error;
	}
	lookup_identifier = ( (uint64_t) LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD << 60 ) | parent_identifier;

/* TODO implement B-tree sub node support */

	for( btree_entry_index = 0;
	     btree_entry_index < number_of_entries;
	     btree_entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     btree_entry_index,
		     &btree_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from B-tree node.",
			 function );

			goto on_error;
		}
		if( btree_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		if( btree_entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) btree_entry->key_data )->file_system_identifier,
		 file_system_identifier );

		if( file_system_identifier != lookup_identifier )
		{
			continue;
		}
		if( libfsapfs_directory_record_initialize(
		     &safe_directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory record.",
			 function );

			goto on_error;
		}
		if( libfsapfs_directory_record_read_key_data(
		     safe_directory_record,
		     btree_entry->key_data,
		     (size_t) btree_entry->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory record key data.",
			 function );

			goto on_error;
		}
		result = libfsapfs_directory_record_compare_name_with_utf8_string(
		          safe_directory_record,
		          utf8_string,
		          utf8_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with name of directory record.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libfsapfs_directory_record_read_value_data(
			     safe_directory_record,
			     btree_entry->value_data,
			     (size_t) btree_entry->value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory record value data.",
				 function );

				goto on_error;
			}
			*directory_record = safe_directory_record;

			return( 1 );
		}
		if( libfsapfs_directory_record_free(
		     &safe_directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory record.",
			 function );

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( safe_directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &safe_directory_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory record for an UTF-16 encoded name from the file system B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_record_from_node_by_utf16_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry                = NULL;
	libfsapfs_directory_record_t *safe_directory_record = NULL;
	static char *function                               = "libfsapfs_file_system_btree_get_directory_record_from_node_by_utf16_name";
	uint64_t file_system_identifier                     = 0;
	uint64_t lookup_identifier                          = 0;
	int btree_entry_index                               = 0;
	int number_of_entries                               = 0;
	int result                                          = 0;

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( libfsapfs_btree_node_get_number_of_entries(
	     node,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from B-tree node.",
		 function );

		goto on_error;
	}
	lookup_identifier = ( (uint64_t) LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD << 60 ) | parent_identifier;

/* TODO implement B-tree sub node support */

	for( btree_entry_index = 0;
	     btree_entry_index < number_of_entries;
	     btree_entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     btree_entry_index,
		     &btree_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from B-tree node.",
			 function );

			goto on_error;
		}
		if( btree_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		if( btree_entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) btree_entry->key_data )->file_system_identifier,
		 file_system_identifier );

		if( file_system_identifier != lookup_identifier )
		{
			continue;
		}
		if( libfsapfs_directory_record_initialize(
		     &safe_directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory record.",
			 function );

			goto on_error;
		}
		if( libfsapfs_directory_record_read_key_data(
		     safe_directory_record,
		     btree_entry->key_data,
		     (size_t) btree_entry->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory record key data.",
			 function );

			goto on_error;
		}
		result = libfsapfs_directory_record_compare_name_with_utf16_string(
		          safe_directory_record,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with name of directory record.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libfsapfs_directory_record_read_value_data(
			     safe_directory_record,
			     btree_entry->value_data,
			     (size_t) btree_entry->value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory record value data.",
				 function );

				goto on_error;
			}
			*directory_record = safe_directory_record;

			return( 1 );
		}
		if( libfsapfs_directory_record_free(
		     &safe_directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory record.",
			 function );

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( safe_directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &safe_directory_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves directory entries for a specific identifier from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_entries(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
	libfsapfs_btree_node_t *root_node = NULL;
	static char *function             = "libfsapfs_file_system_btree_get_directory_entries";
	int result                        = 0;

	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     &root_node,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	result = libfsapfs_file_system_btree_get_directory_entries_from_node(
	          file_system_btree,
	          file_io_handle,
	          root_node,
	          identifier,
	          directory_entries,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entries: %" PRIu64 " from file system B-tree root node.",
		 function,
		 identifier );

		goto on_error;
	}
	if( libfsapfs_btree_node_free(
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree root node.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( root_node != NULL )
	{
		libfsapfs_btree_node_free(
		 &root_node,
		 NULL );
	}
	libcdata_array_empty(
	 directory_entries,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_directory_record_free,
	 NULL );

	return( -1 );
}

/* Retrieves directory entries for a specific identifier from the file system B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_entries_from_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry           = NULL;
	libfsapfs_directory_record_t *directory_record = NULL;
	static char *function                          = "libfsapfs_file_system_btree_get_directory_entries_from_node";
	uint64_t file_system_identifier                = 0;
	uint64_t lookup_identifier                     = 0;
	int btree_entry_index                          = 0;
	int entry_index                                = 0;
	int number_of_entries                          = 0;
	int result                                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t file_system_data_type                  = 0;
#endif

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( libfsapfs_btree_node_get_number_of_entries(
	     node,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from B-tree node.",
		 function );

		goto on_error;
	}
	lookup_identifier = ( (uint64_t) LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD << 60 ) | parent_identifier;

/* TODO implement B-tree sub node support */

	for( btree_entry_index = 0;
	     btree_entry_index < number_of_entries;
	     btree_entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     btree_entry_index,
		     &btree_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from B-tree node.",
			 function );

			goto on_error;
		}
		if( btree_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		if( btree_entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) btree_entry->key_data )->file_system_identifier,
		 file_system_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx64 " %s\n",
			 function,
			 btree_entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( file_system_identifier != lookup_identifier )
		{
			continue;
		}
		if( libfsapfs_directory_record_initialize(
		     &directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory record.",
			 function );

			goto on_error;
		}
		if( libfsapfs_directory_record_read_key_data(
		     directory_record,
		     btree_entry->key_data,
		     (size_t) btree_entry->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory record key data.",
			 function );

			goto on_error;
		}
		if( libfsapfs_directory_record_read_value_data(
		     directory_record,
		     btree_entry->value_data,
		     (size_t) btree_entry->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory record value data.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     directory_entries,
		     &entry_index,
		     (intptr_t *) directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append directory record to array.",
			 function );

			goto on_error;
		}
		directory_record = NULL;

		result = 1;
	}
	return( result );

on_error:
	if( directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &directory_record,
		 NULL );
	}
	libcdata_array_empty(
	 directory_entries,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_directory_record_free,
	 NULL );

	return( -1 );
}

/* Retrieves file extents for a specific identifier from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_file_extents(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libcdata_array_t *file_extents,
     libcerror_error_t **error )
{
	libfsapfs_btree_node_t *root_node = NULL;
	static char *function             = "libfsapfs_file_system_btree_get_file_extents";
	int result                        = 0;

	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     &root_node,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	result = libfsapfs_file_system_btree_get_file_extents_from_node(
	          file_system_btree,
	          file_io_handle,
	          root_node,
	          identifier,
	          file_extents,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file extents: %" PRIu64 " from file system B-tree root node.",
		 function,
		 identifier );

		goto on_error;
	}
	if( libfsapfs_btree_node_free(
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree root node.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( root_node != NULL )
	{
		libfsapfs_btree_node_free(
		 &root_node,
		 NULL );
	}
	libcdata_array_empty(
	 file_extents,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
	 NULL );

	return( -1 );
}

/* Retrieves file extents for a specific identifier from the file system B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_file_extents_from_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *file_extents,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry = NULL;
	libfsapfs_file_extent_t *file_extent = NULL;
	static char *function                = "libfsapfs_file_system_btree_get_file_extents_from_node";
	uint64_t file_system_identifier      = 0;
	uint64_t lookup_identifier           = 0;
	uint8_t data_type                    = 0;
	int btree_entry_index                = 0;
	int entry_index                      = 0;
	int number_of_entries                = 0;
	int result                           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t file_system_data_type        = 0;
#endif

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( libfsapfs_btree_node_get_number_of_entries(
	     node,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from B-tree node.",
		 function );

		goto on_error;
	}
	lookup_identifier = ( (uint64_t) LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_FILE_EXTENT << 60 ) | identifier;

/* TODO implement B-tree sub node support */

	for( btree_entry_index = 0;
	     btree_entry_index < number_of_entries;
	     btree_entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     btree_entry_index,
		     &btree_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from B-tree node.",
			 function );

			goto on_error;
		}
		if( btree_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		if( btree_entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) btree_entry->key_data )->file_system_identifier,
		 file_system_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx64 " %s\n",
			 function,
			 btree_entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( file_system_identifier != lookup_identifier )
		{
			continue;
		}
		if( libfsapfs_file_extent_initialize(
		     &file_extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file extent.",
			 function );

			goto on_error;
		}
		if( libfsapfs_file_extent_read_key_data(
		     file_extent,
		     btree_entry->key_data,
		     (size_t) btree_entry->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read file extent key data.",
			 function );

			goto on_error;
		}
		if( libfsapfs_file_extent_read_value_data(
		     file_extent,
		     btree_entry->value_data,
		     (size_t) btree_entry->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read file extent value data.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     file_extents,
		     &entry_index,
		     (intptr_t *) file_extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append file extent to array.",
			 function );

			goto on_error;
		}
		file_extent = NULL;

		result = 1;
	}
	return( result );

on_error:
	if( file_extent != NULL )
	{
		libfsapfs_file_extent_free(
		 &file_extent,
		 NULL );
	}
	libcdata_array_empty(
	 file_extents,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
	 NULL );

	return( -1 );
}

/* Retrieves an inode for a specific identifier from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_inode_by_identifier(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libfsapfs_inode_t **inode,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry = NULL;
	libfsapfs_btree_node_t *root_node    = NULL;
	static char *function                = "libfsapfs_file_system_btree_get_inode_by_identifier";
	int result                           = 0;

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
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     &root_node,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	result = libfsapfs_file_system_btree_get_entry_from_node_by_identifier(
	          file_system_btree,
	          file_io_handle,
	          root_node,
	          identifier,
	          LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_INODE,
	          &btree_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry from B-tree node.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( btree_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry.",
			 function );

			goto on_error;
		}
		if( libfsapfs_inode_initialize(
		     inode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create inode.",
			 function );

			goto on_error;
		}
		if( libfsapfs_inode_read_key_data(
		     *inode,
		     btree_entry->key_data,
		     (size_t) btree_entry->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read inode key data.",
			 function );

			goto on_error;
		}
		if( libfsapfs_inode_read_value_data(
		     *inode,
		     btree_entry->value_data,
		     (size_t) btree_entry->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read inode value data.",
			 function );

			goto on_error;
		}
	}
	if( libfsapfs_btree_node_free(
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree root node.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( root_node != NULL )
	{
		libfsapfs_btree_node_free(
		 &root_node,
		 NULL );
	}
	if( *inode != NULL )
	{
		libfsapfs_inode_free(
		 inode,
		 NULL );
	}
	return( -1 );
}

/* Retrieves an inode for an UTF-8 encoded path from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_inode_by_utf8_path(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_inode_t **inode,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry           = NULL;
	libfsapfs_btree_node_t *root_node              = NULL;
	libfsapfs_directory_record_t *directory_record = NULL;
	const uint8_t *utf8_string_segment             = NULL;
	static char *function                          = "libfsapfs_file_system_btree_get_inode_by_utf8_path";
	libuna_unicode_character_t unicode_character   = 0;
	size_t utf8_string_index                       = 0;
	size_t utf8_string_segment_length              = 0;
	uint64_t lookup_identifier                     = 0;
	int result                                     = 0;

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
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
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     &root_node,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	lookup_identifier = parent_identifier;

	if( utf8_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf8_string[ utf8_string_index ] == (uint8_t) LIBFSAPFS_SEPARATOR )
		{
			utf8_string_index++;
		}
	}
	if( ( utf8_string_length == 0 )
	 || ( utf8_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf8_string_index < utf8_string_length )
	{
		utf8_string_segment        = &( utf8_string[ utf8_string_index ] );
		utf8_string_segment_length = utf8_string_index;

		while( utf8_string_index < utf8_string_length )
		{
			if( libuna_unicode_character_copy_from_utf8(
			     &unicode_character,
			     utf8_string,
			     utf8_string_length,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSAPFS_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf8_string_segment_length += 1;

				break;
			}
		}
		utf8_string_segment_length = utf8_string_index - utf8_string_segment_length;

		if( utf8_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			result = libfsapfs_file_system_btree_get_directory_record_from_node_by_utf8_name(
				  file_system_btree,
				  file_io_handle,
				  root_node,
				  lookup_identifier,
				  utf8_string_segment,
				  utf8_string_segment_length,
				  &directory_record,
				  error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( libfsapfs_directory_record_get_identifier(
		     directory_record,
		     &lookup_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry identifier.",
			 function );

			goto on_error;
		}
		if( libfsapfs_directory_record_free(
		     &directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory record.",
			 function );

			goto on_error;
		}
	}
	if( result != 0 )
	{
		result = libfsapfs_file_system_btree_get_entry_from_node_by_identifier(
		          file_system_btree,
		          file_io_handle,
		          root_node,
		          lookup_identifier,
		          LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_INODE,
		          &btree_entry,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry from B-tree node.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( btree_entry == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid B-tree entry.",
				 function );

				goto on_error;
			}
			if( libfsapfs_inode_initialize(
			     inode,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create inode.",
				 function );

				goto on_error;
			}
			if( libfsapfs_inode_read_key_data(
			     *inode,
			     btree_entry->key_data,
			     (size_t) btree_entry->key_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read inode key data.",
				 function );

				goto on_error;
			}
			if( libfsapfs_inode_read_value_data(
			     *inode,
			     btree_entry->value_data,
			     (size_t) btree_entry->value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read inode value data.",
				 function );

				goto on_error;
			}
		}
	}
	if( libfsapfs_btree_node_free(
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree root node.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( root_node != NULL )
	{
		libfsapfs_btree_node_free(
		 &root_node,
		 NULL );
	}
	if( *inode != NULL )
	{
		libfsapfs_inode_free(
		 inode,
		 NULL );
	}
	return( -1 );
}

/* Retrieves an inode for an UTF-16 encoded path from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_inode_by_utf16_path(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_inode_t **inode,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry           = NULL;
	libfsapfs_btree_node_t *root_node              = NULL;
	libfsapfs_directory_record_t *directory_record = NULL;
	const uint16_t *utf16_string_segment           = NULL;
	static char *function                          = "libfsapfs_file_system_btree_get_inode_by_utf16_path";
	libuna_unicode_character_t unicode_character   = 0;
	size_t utf16_string_index                      = 0;
	size_t utf16_string_segment_length             = 0;
	uint64_t lookup_identifier                     = 0;
	int result                                     = 0;

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
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
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     &root_node,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	lookup_identifier = parent_identifier;

	if( utf16_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf16_string[ utf16_string_index ] == (uint16_t) LIBFSAPFS_SEPARATOR )
		{
			utf16_string_index++;
		}
	}
	if( ( utf16_string_length == 0 )
	 || ( utf16_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf16_string_index < utf16_string_length )
	{
		utf16_string_segment        = &( utf16_string[ utf16_string_index ] );
		utf16_string_segment_length = utf16_string_index;

		while( utf16_string_index < utf16_string_length )
		{
			if( libuna_unicode_character_copy_from_utf16(
			     &unicode_character,
			     utf16_string,
			     utf16_string_length,
			     &utf16_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSAPFS_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf16_string_segment_length += 1;

				break;
			}
		}
		utf16_string_segment_length = utf16_string_index - utf16_string_segment_length;

		if( utf16_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			result = libfsapfs_file_system_btree_get_directory_record_from_node_by_utf16_name(
				  file_system_btree,
				  file_io_handle,
				  root_node,
				  lookup_identifier,
				  utf16_string_segment,
				  utf16_string_segment_length,
				  &directory_record,
				  error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( libfsapfs_directory_record_get_identifier(
		     directory_record,
		     &lookup_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry identifier.",
			 function );

			goto on_error;
		}
		if( libfsapfs_directory_record_free(
		     &directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory record.",
			 function );

			goto on_error;
		}
	}
	if( result != 0 )
	{
		result = libfsapfs_file_system_btree_get_entry_from_node_by_identifier(
		          file_system_btree,
		          file_io_handle,
		          root_node,
		          lookup_identifier,
		          LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_INODE,
		          &btree_entry,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry from B-tree node.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( btree_entry == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid B-tree entry.",
				 function );

				goto on_error;
			}
			if( libfsapfs_inode_initialize(
			     inode,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create inode.",
				 function );

				goto on_error;
			}
			if( libfsapfs_inode_read_key_data(
			     *inode,
			     btree_entry->key_data,
			     (size_t) btree_entry->key_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read inode key data.",
				 function );

				goto on_error;
			}
			if( libfsapfs_inode_read_value_data(
			     *inode,
			     btree_entry->value_data,
			     (size_t) btree_entry->value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read inode value data.",
				 function );

				goto on_error;
			}
		}
	}
	if( libfsapfs_btree_node_free(
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree root node.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( root_node != NULL )
	{
		libfsapfs_btree_node_free(
		 &root_node,
		 NULL );
	}
	if( *inode != NULL )
	{
		libfsapfs_inode_free(
		 inode,
		 NULL );
	}
	return( -1 );
}

