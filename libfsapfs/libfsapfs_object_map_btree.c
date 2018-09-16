/*
 * The object map B-tree functions
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
#include "libfsapfs_btree_root.h"
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

/* Frees a object map B-tree
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

/* Reads the object map B-tree
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

/* Reads the object map B-tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_btree_read_data(
     libfsapfs_object_map_btree_t *object_map_btree,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry                     = NULL;
	libfsapfs_btree_root_t *btree_root                       = NULL;
	libfsapfs_object_map_descriptor_t *object_map_descriptor = NULL;
	static char *function                                    = "libfsapfs_object_map_btree_read_data";
	int btree_entry_index                                    = 0;
	int entry_index                                          = 0;
	int number_of_entries                                    = 0;

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
	if( libfsapfs_btree_root_initialize(
	     &btree_root,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree root.",
		 function );

		goto on_error;
	}
	if( libfsapfs_btree_root_read_data(
	     btree_root,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree root.",
		 function );

		goto on_error;
	}
	if( btree_root->object_type != 0x40000002UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 btree_root->object_type );

		goto on_error;
	}
	if( btree_root->object_subtype != 0x0000000bUL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 btree_root->object_subtype );

		goto on_error;
	}
	if( btree_root->header->flags != 0x0007 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%04" PRIx16 ".",
		 function,
		 btree_root->header->flags );

		goto on_error;
	}
	if( btree_root->footer->key_size != sizeof( fsapfs_object_map_btree_key_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key size value out of bounds.",
		 function );

		goto on_error;
	}
	if( btree_root->footer->value_size != sizeof( fsapfs_object_map_btree_value_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value size value out of bounds.",
		 function );

		goto on_error;
	}

	if( btree_root->footer->key_size != sizeof( fsapfs_object_map_btree_key_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key size value out of bounds.",
		 function );

		goto on_error;
	}
	if( btree_root->footer->value_size != sizeof( fsapfs_object_map_btree_value_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value size value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfsapfs_btree_root_get_number_of_entries(
	     btree_root,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from B-tree root.",
		 function );

		goto on_error;
	}
	for( btree_entry_index = 0;
	     btree_entry_index < number_of_entries;
	     btree_entry_index++ )
	{
		if( libfsapfs_btree_root_get_entry_by_index(
		     btree_root,
		     btree_entry_index,
		     &btree_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from B-tree root.",
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
		     btree_entry->key_data,
		     btree_entry->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read entry: %d object map key data.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		if( libfsapfs_object_map_descriptor_read_btree_value_data(
		     object_map_descriptor,
		     btree_entry->value_data,
		     btree_entry->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read entry: %d object map value data.",
			 function,
			 btree_entry_index );

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
			 "%s: unable to append object map descriptor: %d to array.",
			 function,
			 btree_entry_index );

			goto on_error;
		}
		object_map_descriptor = NULL;
	}
	if( libfsapfs_btree_root_free(
	     &btree_root,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree root.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( object_map_descriptor != NULL )
	{
		libfsapfs_object_map_descriptor_free(
		 &object_map_descriptor,
		 NULL );
	}
	if( btree_root != NULL )
	{
		libfsapfs_btree_root_free(
		 &btree_root,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the object map descriptor of a specific object identifier
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libfsapfs_object_map_btree_get_descriptor_by_object_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     uint64_t object_identifier,
     libfsapfs_object_map_descriptor_t **descriptor,
     libcerror_error_t **error )
{
	static char *function     = "libfsapfs_object_map_btree_get_descriptor_by_object_identifier";
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

