/*
 * The file system B-tree functions
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
#include "libfsapfs_btree_node.h"
#include "libfsapfs_data_block.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_directory_record.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_extended_attribute.h"
#include "libfsapfs_file_extent.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_libuna.h"
#include "libfsapfs_name_hash.h"
#include "libfsapfs_object_map_btree.h"
#include "libfsapfs_object_map_descriptor.h"

#include "fsapfs_file_system.h"
#include "fsapfs_object.h"

/* Creates a file system B-tree
 * Make sure the value file_system_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_initialize(
     libfsapfs_file_system_btree_t **file_system_btree,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfdata_vector_t *data_block_vector,
     libfsapfs_object_map_btree_t *object_map_btree,
     uint64_t root_node_block_number,
     uint8_t use_case_folding,
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

		memory_free(
		 *file_system_btree );

		*file_system_btree = NULL;

		return( -1 );
	}
	if( libfcache_cache_initialize(
	     &( ( *file_system_btree )->data_block_cache ),
	     LIBFSAPFS_MAXIMUM_CACHE_ENTRIES_DATA_BLOCKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data block cache.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *file_system_btree )->node_cache ),
	     LIBFSAPFS_MAXIMUM_CACHE_ENTRIES_BTREE_NODES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create node cache.",
		 function );

		goto on_error;
	}
	( *file_system_btree )->io_handle              = io_handle;
	( *file_system_btree )->encryption_context     = encryption_context;
	( *file_system_btree )->data_block_vector      = data_block_vector;
	( *file_system_btree )->object_map_btree       = object_map_btree;
	( *file_system_btree )->root_node_block_number = root_node_block_number;
	( *file_system_btree )->use_case_folding       = use_case_folding;

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
	int result            = 1;

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
		/* The io_handle, data_block_vector iand object_map_btree are referenced and freed elsewhere
		 */
		if( libfcache_cache_free(
		     &( ( *file_system_btree )->node_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free node cache.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *file_system_btree )->data_block_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data block cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *file_system_btree );

		*file_system_btree = NULL;
	}
	return( result );
}

/* Retrieves the sub node block number from a B-tree entry
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_entry_t *entry,
     uint64_t *sub_node_block_number,
     libcerror_error_t **error )
{
	libfsapfs_object_map_descriptor_t *object_map_descriptor = NULL;
	static char *function                                    = "libfsapfs_file_system_btree_get_sub_node_block_number_from_entry";
	uint64_t sub_node_object_identifier                      = 0;
	int result                                               = 0;

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
	if( entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree entry.",
		 function );

		return( -1 );
	}
	if( entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree entry - missing value data.",
		 function );

		return( -1 );
	}
	if( entry->value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid B-tree entry - unsupported value data size.",
		 function );

		return( -1 );
	}
	if( sub_node_block_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node block number.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 entry->value_data,
	 sub_node_object_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: sub node object identifier: %" PRIu64 "\n",
		 function,
		 sub_node_object_identifier );
	}
#endif
	result = libfsapfs_object_map_btree_get_descriptor_by_object_identifier(
	          file_system_btree->object_map_btree,
	          file_io_handle,
	          sub_node_object_identifier,
	          &object_map_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve object map descriptor for sub node object identifier: %" PRIu64 ".",
		 function,
		 sub_node_object_identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( object_map_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid object map descriptor.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: sub node block number: %" PRIu64 "\n",
			 function,
			 object_map_descriptor->physical_address );
		}
#endif
		*sub_node_block_number = object_map_descriptor->physical_address;

		if( libfsapfs_object_map_descriptor_free(
		     &object_map_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free object map descriptor.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( object_map_descriptor != NULL )
	{
		libfsapfs_object_map_descriptor_free(
		 &object_map_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the file system B-tree root node
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_get_root_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t root_node_block_number,
     libfsapfs_btree_node_t **root_node,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	libfsapfs_btree_node_t *node         = NULL;
	libfsapfs_data_block_t *data_block   = NULL;
	static char *function                = "libfsapfs_file_system_btree_get_root_node";
	int result                           = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp     = 0;
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
	if( file_system_btree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system B-tree entry - missing IO handle.",
		 function );

		return( -1 );
	}
	if( root_node_block_number > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid root node block number value out of bounds.",
		 function );

		return( -1 );
	}
	if( root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_start_timing(
		     file_system_btree->io_handle->profiler,
		     &profiler_start_timestamp,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to start timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	result = libfcache_cache_get_value_by_identifier(
	          file_system_btree->node_cache,
	          0,
	          (off64_t) root_node_block_number,
	          0,
	          &cache_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from cache.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libfdata_vector_get_element_value_by_index(
		     file_system_btree->data_block_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) file_system_btree->data_block_cache,
		     (int) root_node_block_number,
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
			 root_node_block_number );

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
			 root_node_block_number );

			goto on_error;
		}
		if( libfsapfs_btree_node_initialize(
		     &node,
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
		     node,
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
		if( ( node->object_type != 0x00000002UL )
		 && ( node->object_type != 0x10000002UL ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid object type: 0x%08" PRIx32 ".",
			 function,
			 node->object_type );

			goto on_error;
		}
		if( node->object_subtype != 0x0000000eUL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid object subtype: 0x%08" PRIx32 ".",
			 function,
			 node->object_subtype );

			goto on_error;
		}
		if( ( ( node->node_header->flags & 0x0001 ) == 0 )
		 || ( ( node->node_header->flags & 0x0004 ) != 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported flags: 0x%04" PRIx16 ".",
			 function,
			 ( *root_node )->node_header->flags );

			goto on_error;
		}
		if( node->footer->node_size != 4096 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid node size value out of bounds.",
			 function );

			goto on_error;
		}
		if( node->footer->key_size != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid key size value out of bounds.",
			 function );

			goto on_error;
		}
		if( node->footer->value_size != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value size value out of bounds.",
			 function );

			goto on_error;
		}
		if( libfcache_cache_set_value_by_identifier(
		     file_system_btree->node_cache,
		     0,
		     (off64_t) root_node_block_number,
		     0,
		     (intptr_t *) node,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_btree_node_free,
		     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value in cache.",
			 function );

			goto on_error;
		}
		node = NULL;

		if( libfcache_cache_get_value_by_identifier(
		     file_system_btree->node_cache,
		     0,
		     (off64_t) root_node_block_number,
		     0,
		     &cache_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from cache.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_stop_timing(
		     file_system_btree->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     root_node_block_number * file_system_btree->io_handle->block_size,
		     file_system_btree->io_handle->block_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to stop timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	if( libfcache_cache_value_get_value(
	     cache_value,
	     (intptr_t **) root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( node != NULL )
	{
		libfsapfs_btree_node_free(
		 &node,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a file system B-tree sub node
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_get_sub_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t sub_node_block_number,
     libfsapfs_btree_node_t **sub_node,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	libfsapfs_btree_node_t *node         = NULL;
	libfsapfs_data_block_t *data_block   = NULL;
	static char *function                = "libfsapfs_file_system_btree_get_sub_node";
	int result                           = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp     = 0;
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
	if( file_system_btree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system B-tree entry - missing IO handle.",
		 function );

		return( -1 );
	}
	if( sub_node_block_number > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sub node block number value out of bounds.",
		 function );

		return( -1 );
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_start_timing(
		     file_system_btree->io_handle->profiler,
		     &profiler_start_timestamp,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to start timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	result = libfcache_cache_get_value_by_identifier(
	          file_system_btree->node_cache,
	          0,
	          (off64_t) sub_node_block_number,
	          0,
	          &cache_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from cache.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libfdata_vector_get_element_value_by_index(
		     file_system_btree->data_block_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) file_system_btree->data_block_cache,
		     (int) sub_node_block_number,
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
			 sub_node_block_number );

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
			 sub_node_block_number );

			goto on_error;
		}
		if( libfsapfs_btree_node_initialize(
		     &node,
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
		     node,
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
		if( ( node->object_type != 0x00000003UL )
		 && ( node->object_type != 0x10000003UL ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid object type: 0x%08" PRIx32 ".",
			 function,
			 node->object_type );

			goto on_error;
		}
		if( node->object_subtype != 0x0000000eUL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid object subtype: 0x%08" PRIx32 ".",
			 function,
			 node->object_subtype );

			goto on_error;
		}
		if( ( ( node->node_header->flags & 0x0001 ) != 0 )
		 || ( ( node->node_header->flags & 0x0004 ) != 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported flags: 0x%04" PRIx16 ".",
			 function,
			 node->node_header->flags );

			goto on_error;
		}
		if( libfcache_cache_set_value_by_identifier(
		     file_system_btree->node_cache,
		     0,
		     (off64_t) sub_node_block_number,
		     0,
		     (intptr_t *) node,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_btree_node_free,
		     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value in cache.",
			 function );

			goto on_error;
		}
		node = NULL;

		if( libfcache_cache_get_value_by_identifier(
		     file_system_btree->node_cache,
		     0,
		     (off64_t) sub_node_block_number,
		     0,
		     &cache_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from cache.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_stop_timing(
		     file_system_btree->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     sub_node_block_number * file_system_btree->io_handle->block_size,
		     file_system_btree->io_handle->block_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to stop timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	if( libfcache_cache_value_get_value(
	     cache_value,
	     (intptr_t **) sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( node != NULL )
	{
		libfsapfs_btree_node_free(
		 &node,
		 NULL );
	}
	return( -1 );
}

/* Retrieves an entry for a specific identifier from the file system B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_entry_from_node_by_identifier(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     uint8_t data_type,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry          = NULL;
	libfsapfs_btree_entry_t *previous_entry = NULL;
	static char *function                   = "libfsapfs_file_system_btree_get_entry_from_node_by_identifier";
	uint64_t file_system_identifier         = 0;
	uint64_t lookup_identifier              = 0;
	uint8_t file_system_data_type           = 0;
	int btree_entry_index                   = 0;
	int is_leaf_node                        = 0;
	int number_of_entries                   = 0;

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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving B-tree entry identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
		 function,
		 identifier,
		 data_type,
		 libfsapfs_debug_print_file_system_data_type(
		  data_type ) );
	}
#endif
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
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

		return( -1 );
	}
	lookup_identifier = ( (uint64_t) data_type << 60 ) | identifier;

	for( btree_entry_index = 0;
	     btree_entry_index < number_of_entries;
	     btree_entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     btree_entry_index,
		     &entry,
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
		if( entry == NULL )
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
		if( entry->key_data == NULL )
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
		 entry->key_data,
		 file_system_identifier );

		file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 btree_entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( is_leaf_node != 0 )
		{
			if( file_system_identifier == lookup_identifier )
			{
				*btree_entry = entry;

				return( 1 );
			}
		}
		else
		{
			file_system_identifier &= 0x0fffffffffffffffUL;

			if( file_system_identifier >= identifier )
			{
				if( ( previous_entry == NULL )
				 || ( ( file_system_identifier == identifier )
				  &&  ( file_system_data_type <= data_type ) ) )
				{
					previous_entry = entry;
				}
				*btree_entry = previous_entry;

				return( 1 );
			}
			previous_entry = entry;
		}
	}
	if( is_leaf_node == 0 )
	{
		*btree_entry = previous_entry;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves an entry for a specific identifier from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_entry_by_identifier(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     uint8_t data_type,
     libfsapfs_btree_node_t **btree_node,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry = NULL;
	libfsapfs_btree_node_t *node   = NULL;
	static char *function          = "libfsapfs_file_system_btree_get_entry_by_identifier";
	uint64_t sub_node_block_number = 0;
	int is_leaf_node               = 0;
	int recursion_depth            = 0;
	int result                     = 0;

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
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     file_system_btree->root_node_block_number,
	     &node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		return( -1 );
	}
	do
	{
		if( ( recursion_depth < 0 )
		 || ( recursion_depth > LIBFSAPFS_MAXIMUM_BTREE_NODE_RECURSION_DEPTH ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid recursion depth value out of bounds.",
			 function );

			return( -1 );
		}
		is_leaf_node = libfsapfs_btree_node_is_leaf_node(
		                node,
		                error );

		if( is_leaf_node == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if B-tree node is a leaf node.",
			 function );

			return( -1 );
		}
		result = libfsapfs_file_system_btree_get_entry_from_node_by_identifier(
		          file_system_btree,
		          node,
		          identifier,
		          data_type,
		          &entry,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry from B-tree node.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			break;
		}
		if( is_leaf_node != 0 )
		{
			*btree_node  = node;
			*btree_entry = entry;

			return( 1 );
		}
		if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
		     file_system_btree,
		     file_io_handle,
		     entry,
		     &sub_node_block_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub node block number.",
			 function );

			return( -1 );
		}
		node = NULL;

		if( libfsapfs_file_system_btree_get_sub_node(
		     file_system_btree,
		     file_io_handle,
		     sub_node_block_number,
		     &node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
			 function,
			 sub_node_block_number );

			return( -1 );
		}
		recursion_depth++;
	}
	while( is_leaf_node == 0 );

	return( 0 );
}

/* Retrieves a directory record for an UTF-8 encoded name from the file system B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf8_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t name_hash,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry                      = NULL;
	libfsapfs_directory_record_t *safe_directory_record = NULL;
	static char *function                               = "libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf8_name";
	uint64_t file_system_identifier                     = 0;
	uint64_t lookup_identifier                          = 0;
	int compare_result                                  = 0;
	int entry_index                                     = 0;
	int is_leaf_node                                    = 0;
	int number_of_entries                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t file_system_data_type                       = 0;
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving directory record: %" PRIu64 "\n",
		 function,
		 parent_identifier );
	}
#endif
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a leaf node.",
		 function );

		goto on_error;
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

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     entry_index,
		     &entry,
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
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) entry->key_data )->file_system_identifier,
		 file_system_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( file_system_identifier == lookup_identifier )
		{
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
			     entry->key_data,
			     (size_t) entry->key_data_size,
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
			compare_result = libfsapfs_directory_record_compare_name_with_utf8_string(
			                  safe_directory_record,
			                  utf8_string,
			                  utf8_string_length,
			                  name_hash,
			                  file_system_btree->use_case_folding,
			                  error );

			if( compare_result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-8 string with name of directory record.",
				 function );

				goto on_error;
			}
			else if( compare_result == LIBUNA_COMPARE_EQUAL )
			{
				if( libfsapfs_directory_record_read_value_data(
				     safe_directory_record,
				     entry->value_data,
				     (size_t) entry->value_data_size,
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

/* Retrieves a directory record for an UTF-8 encoded name from the file system B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf8_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t name_hash,
     libfsapfs_directory_record_t **directory_record,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry                      = NULL;
	libfsapfs_btree_entry_t *previous_entry             = NULL;
	libfsapfs_btree_node_t *sub_node                    = NULL;
	libfsapfs_directory_record_t *safe_directory_record = NULL;
	static char *function                               = "libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf8_name";
	uint64_t file_system_identifier                     = 0;
	uint64_t sub_node_block_number                      = 0;
	uint8_t file_system_data_type                       = 0;
	int compare_result                                  = 0;
	int entry_index                                     = 0;
	int is_leaf_node                                    = 0;
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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
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
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSAPFS_MAXIMUM_BTREE_NODE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving directory record: %" PRIu64 "\n",
		 function,
		 parent_identifier );
	}
#endif
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a branch node.",
		 function );

		goto on_error;
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
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     entry_index,
		     &entry,
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
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) entry->key_data )->file_system_identifier,
		 file_system_identifier );

		file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		file_system_identifier &= 0x0fffffffffffffffUL;

		if( ( file_system_identifier > parent_identifier )
		 || ( ( file_system_identifier == parent_identifier )
		  &&  ( file_system_data_type > LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD ) ) )
		{
			break;
		}
		if( ( file_system_identifier == parent_identifier )
		 && ( file_system_data_type == LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD ) )
		{
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
			     entry->key_data,
			     (size_t) entry->key_data_size,
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
			/* The directory records are sorted by case-sensitive name
			 */
			compare_result = libfsapfs_directory_record_compare_name_with_utf8_string(
			                  safe_directory_record,
			                  utf8_string,
			                  utf8_string_length,
			                  name_hash,
			                  0,
			                  error );

			if( compare_result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-8 string with name of directory record.",
				 function );

				goto on_error;
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
			if( compare_result == LIBUNA_COMPARE_LESS )
			{
				break;
			}
		}
		previous_entry = entry;
	}
	if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
	     file_system_btree,
	     file_io_handle,
	     previous_entry,
	     &sub_node_block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine sub node block number.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_system_btree_get_sub_node(
	     file_system_btree,
	     file_io_handle,
	     sub_node_block_number,
	     &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
		 function,
		 sub_node_block_number );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                sub_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree sub node is a leaf node.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf8_name(
			  file_system_btree,
			  sub_node,
			  parent_identifier,
			  utf8_string,
			  utf8_string_length,
			  name_hash,
			  directory_record,
			  error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf8_name(
			  file_system_btree,
			  file_io_handle,
			  sub_node,
			  parent_identifier,
			  utf8_string,
			  utf8_string_length,
			  name_hash,
			  directory_record,
			  recursion_depth + 1,
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
	sub_node = NULL;

	return( result );

on_error:
	if( *directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 directory_record,
		 NULL );
	}
	if( safe_directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &safe_directory_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory record for an UTF-16 encoded name from the file system B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf16_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t name_hash,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry                      = NULL;
	libfsapfs_directory_record_t *safe_directory_record = NULL;
	static char *function                               = "libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf16_name";
	uint64_t file_system_identifier                     = 0;
	uint64_t lookup_identifier                          = 0;
	int compare_result                                  = 0;
	int entry_index                                     = 0;
	int is_leaf_node                                    = 0;
	int number_of_entries                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t file_system_data_type                       = 0;
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving directory record: %" PRIu64 "\n",
		 function,
		 parent_identifier );
	}
#endif
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a leaf node.",
		 function );

		goto on_error;
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

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     entry_index,
		     &entry,
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
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) entry->key_data )->file_system_identifier,
		 file_system_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( file_system_identifier == lookup_identifier )
		{
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
			     entry->key_data,
			     (size_t) entry->key_data_size,
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
			compare_result = libfsapfs_directory_record_compare_name_with_utf16_string(
			                  safe_directory_record,
			                  utf16_string,
			                  utf16_string_length,
			                  name_hash,
			                  file_system_btree->use_case_folding,
			                  error );

			if( compare_result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-16 string with name of directory record.",
				 function );

				goto on_error;
			}
			else if( compare_result == LIBUNA_COMPARE_EQUAL )
			{
				if( libfsapfs_directory_record_read_value_data(
				     safe_directory_record,
				     entry->value_data,
				     (size_t) entry->value_data_size,
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

/* Retrieves a directory record for an UTF-16 encoded name from the file system B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf16_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t name_hash,
     libfsapfs_directory_record_t **directory_record,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry                      = NULL;
	libfsapfs_btree_entry_t *previous_entry             = NULL;
	libfsapfs_btree_node_t *sub_node                    = NULL;
	libfsapfs_directory_record_t *safe_directory_record = NULL;
	static char *function                               = "libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf16_name";
	uint64_t file_system_identifier                     = 0;
	uint64_t sub_node_block_number                      = 0;
	uint8_t file_system_data_type                       = 0;
	int compare_result                                  = 0;
	int entry_index                                     = 0;
	int is_leaf_node                                    = 0;
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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
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
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSAPFS_MAXIMUM_BTREE_NODE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving directory record: %" PRIu64 "\n",
		 function,
		 parent_identifier );
	}
#endif
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a branch node.",
		 function );

		goto on_error;
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
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     entry_index,
		     &entry,
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
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) entry->key_data )->file_system_identifier,
		 file_system_identifier );

		file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		file_system_identifier &= 0x0fffffffffffffffUL;

		if( ( file_system_identifier > parent_identifier )
		 || ( ( file_system_identifier == parent_identifier )
		  &&  ( file_system_data_type > LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD ) ) )
		{
			break;
		}
		if( ( file_system_identifier == parent_identifier )
		 && ( file_system_data_type == LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD ) )
		{
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
			     entry->key_data,
			     (size_t) entry->key_data_size,
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
			/* The directory records are sorted by case-sensitive name
			 */
			compare_result = libfsapfs_directory_record_compare_name_with_utf16_string(
			                  safe_directory_record,
			                  utf16_string,
			                  utf16_string_length,
			                  name_hash,
			                  0,
			                  error );

			if( compare_result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-16 string with name of directory record.",
				 function );

				goto on_error;
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
			if( compare_result == LIBUNA_COMPARE_LESS )
			{
				break;
			}
		}
		previous_entry = entry;
	}
	if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
	     file_system_btree,
	     file_io_handle,
	     previous_entry,
	     &sub_node_block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine sub node block number.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_system_btree_get_sub_node(
	     file_system_btree,
	     file_io_handle,
	     sub_node_block_number,
	     &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
		 function,
		 sub_node_block_number );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                sub_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree sub node is a leaf node.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf16_name(
			  file_system_btree,
			  sub_node,
			  parent_identifier,
			  utf16_string,
			  utf16_string_length,
			  name_hash,
			  directory_record,
			  error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf16_name(
			  file_system_btree,
			  file_io_handle,
			  sub_node,
			  parent_identifier,
			  utf16_string,
			  utf16_string_length,
			  name_hash,
			  directory_record,
			  recursion_depth + 1,
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
	sub_node = NULL;

	return( result );

on_error:
	if( *directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 directory_record,
		 NULL );
	}
	if( safe_directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &safe_directory_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves directory entries for a specific parent identifier from the file system B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_entries_from_leaf_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry           = NULL;
	libfsapfs_directory_record_t *directory_record = NULL;
	static char *function                          = "libfsapfs_file_system_btree_get_directory_entries_from_leaf_node";
	uint64_t file_system_identifier                = 0;
	uint64_t lookup_identifier                     = 0;
	int btree_entry_index                          = 0;
	int entry_index                                = 0;
	int found_directory_entry                      = 0;
	int is_leaf_node                               = 0;
	int number_of_entries                          = 0;

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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a leaf node.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving directory entries of: %" PRIu64 "\n",
		 function,
		 parent_identifier );
	}
#endif
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
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 btree_entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( ( file_system_identifier & 0x0fffffffffffffffUL ) > parent_identifier )
		{
			break;
		}
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

		found_directory_entry = 1;
	}
	return( found_directory_entry );

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

/* Retrieves directory entries for a specific parent identifier from the file system B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_entries_from_branch_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t parent_identifier,
     libcdata_array_t *directory_entries,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry          = NULL;
	libfsapfs_btree_entry_t *previous_entry = NULL;
	libfsapfs_btree_node_t *sub_node        = NULL;
	static char *function                   = "libfsapfs_file_system_btree_get_directory_entries_from_branch_node";
	uint64_t file_system_identifier         = 0;
	uint64_t sub_node_block_number          = 0;
	uint8_t file_system_data_type           = 0;
	int entry_index                         = 0;
	int found_directory_entry               = 0;
	int is_leaf_node                        = 0;
	int number_of_entries                   = 0;
	int result                              = 0;

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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSAPFS_MAXIMUM_BTREE_NODE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a branch node.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving directory entries of: %" PRIu64 "\n",
		 function,
		 parent_identifier );
	}
#endif
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
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     entry_index,
		     &entry,
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
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) entry->key_data )->file_system_identifier,
		 file_system_identifier );

		file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		file_system_identifier &= 0x0fffffffffffffffUL;

		if( ( file_system_identifier > parent_identifier )
		 || ( ( file_system_identifier == parent_identifier )
		  &&  ( file_system_data_type > LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD ) ) )
		{
			break;
		}
		if( ( file_system_identifier == parent_identifier )
		 && ( file_system_data_type == LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_DIRECTORY_RECORD )
		 && ( previous_entry != NULL ) )
		{
			if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
			     file_system_btree,
			     file_io_handle,
			     previous_entry,
			     &sub_node_block_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine sub node block number.",
				 function );

				goto on_error;
			}
			if( libfsapfs_file_system_btree_get_sub_node(
			     file_system_btree,
			     file_io_handle,
			     sub_node_block_number,
			     &sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
				 function,
				 sub_node_block_number );

				goto on_error;
			}
			is_leaf_node = libfsapfs_btree_node_is_leaf_node(
			                sub_node,
			                error );

			if( is_leaf_node == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if B-tree sub node is a leaf node.",
				 function );

				goto on_error;
			}
			if( is_leaf_node != 0 )
			{
				result = libfsapfs_file_system_btree_get_directory_entries_from_leaf_node(
				          file_system_btree,
				          sub_node,
				          parent_identifier,
				          directory_entries,
				          error );
			}
			else
			{
				result = libfsapfs_file_system_btree_get_directory_entries_from_branch_node(
				          file_system_btree,
				          file_io_handle,
				          sub_node,
				          parent_identifier,
				          directory_entries,
				          recursion_depth + 1,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entries: %" PRIu64 " from file system B-tree sub node.",
				 function,
				 parent_identifier );

				goto on_error;
			}
			else if( result != 0 )
			{
				found_directory_entry = 1;
			}
			sub_node = NULL;
		}
		previous_entry = entry;
	}
	if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
	     file_system_btree,
	     file_io_handle,
	     previous_entry,
	     &sub_node_block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine sub node block number.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_system_btree_get_sub_node(
	     file_system_btree,
	     file_io_handle,
	     sub_node_block_number,
	     &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
		 function,
		 sub_node_block_number );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                sub_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree sub node is a leaf node.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_directory_entries_from_leaf_node(
		          file_system_btree,
		          sub_node,
		          parent_identifier,
		          directory_entries,
		          error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_directory_entries_from_branch_node(
		          file_system_btree,
		          file_io_handle,
		          sub_node,
		          parent_identifier,
		          directory_entries,
		          recursion_depth + 1,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entries: %" PRIu64 " from file system B-tree sub node.",
		 function,
		 parent_identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		found_directory_entry = 1;
	}
	return( found_directory_entry );

on_error:
	libcdata_array_empty(
	 directory_entries,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_directory_record_free,
	 NULL );

	return( -1 );
}

/* Retrieves directory entries for a specific parent identifier from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_directory_entries(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
	libfsapfs_btree_node_t *root_node = NULL;
	static char *function             = "libfsapfs_file_system_btree_get_directory_entries";
	int is_leaf_node                  = 0;
	int result                        = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp  = 0;
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
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_start_timing(
		     file_system_btree->io_handle->profiler,
		     &profiler_start_timestamp,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to start timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving directory entries of: %" PRIu64 "\n",
		 function,
		 parent_identifier );
	}
#endif
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     file_system_btree->root_node_block_number,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	if( root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree root node.",
		 function );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                root_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node is a leaf node.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_directory_entries_from_leaf_node(
		          file_system_btree,
		          root_node,
		          parent_identifier,
		          directory_entries,
		          error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_directory_entries_from_branch_node(
		          file_system_btree,
		          file_io_handle,
		          root_node,
		          parent_identifier,
		          directory_entries,
		          0,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entries: %" PRIu64 " from file system B-tree root node.",
		 function,
		 parent_identifier );

		goto on_error;
	}
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_stop_timing(
		     file_system_btree->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to stop timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	return( result );

on_error:
	libcdata_array_empty(
	 directory_entries,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_directory_record_free,
	 NULL );

	return( -1 );
}

/* Retrieves extended attributes for a specific identifier from the file system B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_extended_attributes_from_leaf_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *extended_attributes,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry               = NULL;
	libfsapfs_extended_attribute_t *extended_attribute = NULL;
	static char *function                              = "libfsapfs_file_system_btree_get_extended_attributes_from_leaf_node";
	uint64_t file_system_identifier                    = 0;
	uint64_t lookup_identifier                         = 0;
	int btree_entry_index                              = 0;
	int entry_index                                    = 0;
	int found_extended_attribute                       = 0;
	int is_leaf_node                                   = 0;
	int number_of_entries                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t file_system_data_type                      = 0;
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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a leaf node.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving extended attributes of: %" PRIu64 "\n",
		 function,
		 identifier );
	}
#endif
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
	lookup_identifier = ( (uint64_t) LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_EXTENDED_ATTRIBUTE << 60 ) | identifier;

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
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 btree_entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( ( file_system_identifier & 0x0fffffffffffffffUL ) > identifier )
		{
			break;
		}
		if( file_system_identifier != lookup_identifier )
		{
			continue;
		}
		if( libfsapfs_extended_attribute_initialize(
		     &extended_attribute,
		     file_system_btree->io_handle,
		     file_io_handle,
		     file_system_btree->encryption_context,
		     file_system_btree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extended attribute.",
			 function );

			goto on_error;
		}
		if( libfsapfs_extended_attribute_read_key_data(
		     extended_attribute,
		     btree_entry->key_data,
		     (size_t) btree_entry->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read extended attribute key data.",
			 function );

			goto on_error;
		}
		if( libfsapfs_extended_attribute_read_value_data(
		     extended_attribute,
		     btree_entry->value_data,
		     (size_t) btree_entry->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read extended attribute value data.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     extended_attributes,
		     &entry_index,
		     (intptr_t *) extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extended attribute to array.",
			 function );

			goto on_error;
		}
		extended_attribute = NULL;

		found_extended_attribute = 1;
	}
	return( found_extended_attribute );

on_error:
	if( extended_attribute != NULL )
	{
		libfsapfs_internal_extended_attribute_free(
		 (libfsapfs_internal_extended_attribute_t **) &extended_attribute,
		 NULL );
	}
	libcdata_array_empty(
	 extended_attributes,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_internal_extended_attribute_free,
	 NULL );

	return( -1 );
}

/* Retrieves extended attributes for a specific identifier from the file system B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_extended_attributes_from_branch_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *extended_attributes,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry          = NULL;
	libfsapfs_btree_entry_t *previous_entry = NULL;
	libfsapfs_btree_node_t *sub_node        = NULL;
	static char *function                   = "libfsapfs_file_system_btree_get_extended_attributes_from_branch_node";
	uint64_t file_system_identifier         = 0;
	uint64_t sub_node_block_number          = 0;
	uint8_t file_system_data_type           = 0;
	int entry_index                         = 0;
	int found_extended_attribute            = 0;
	int is_leaf_node                        = 0;
	int number_of_entries                   = 0;
	int result                              = 0;

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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSAPFS_MAXIMUM_BTREE_NODE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a branch node.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving extended attributes of: %" PRIu64 "\n",
		 function,
		 identifier );
	}
#endif
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
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     entry_index,
		     &entry,
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
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) entry->key_data )->file_system_identifier,
		 file_system_identifier );

		file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		file_system_identifier &= 0x0fffffffffffffffUL;

		if( ( file_system_identifier > identifier )
		 || ( ( file_system_identifier == identifier )
		  &&  ( file_system_data_type > LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_EXTENDED_ATTRIBUTE ) ) )
		{
			break;
		}
		if( ( file_system_identifier == identifier )
		 && ( file_system_data_type == LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_EXTENDED_ATTRIBUTE )
		 && ( previous_entry != NULL ) )
		{
			if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
			     file_system_btree,
			     file_io_handle,
			     previous_entry,
			     &sub_node_block_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine sub node block number.",
				 function );

				goto on_error;
			}
			if( libfsapfs_file_system_btree_get_sub_node(
			     file_system_btree,
			     file_io_handle,
			     sub_node_block_number,
			     &sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
				 function,
				 sub_node_block_number );

				goto on_error;
			}
			is_leaf_node = libfsapfs_btree_node_is_leaf_node(
			                sub_node,
			                error );

			if( is_leaf_node == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if B-tree sub node is a leaf node.",
				 function );

				goto on_error;
			}
			if( is_leaf_node != 0 )
			{
				result = libfsapfs_file_system_btree_get_extended_attributes_from_leaf_node(
				          file_system_btree,
				          file_io_handle,
				          sub_node,
				          identifier,
				          extended_attributes,
				          error );
			}
			else
			{
				result = libfsapfs_file_system_btree_get_extended_attributes_from_branch_node(
				          file_system_btree,
				          file_io_handle,
				          sub_node,
				          identifier,
				          extended_attributes,
				          recursion_depth + 1,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve extended attributes: %" PRIu64 " from file system B-tree sub node.",
				 function,
				 identifier );

				goto on_error;
			}
			else if( result != 0 )
			{
				found_extended_attribute = 1;
			}
			sub_node = NULL;
		}
		previous_entry = entry;
	}
	if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
	     file_system_btree,
	     file_io_handle,
	     previous_entry,
	     &sub_node_block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine sub node block number.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_system_btree_get_sub_node(
	     file_system_btree,
	     file_io_handle,
	     sub_node_block_number,
	     &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
		 function,
		 sub_node_block_number );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                sub_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree sub node is a leaf node.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_extended_attributes_from_leaf_node(
		          file_system_btree,
		          file_io_handle,
		          sub_node,
		          identifier,
		          extended_attributes,
		          error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_extended_attributes_from_branch_node(
		          file_system_btree,
		          file_io_handle,
		          sub_node,
		          identifier,
		          extended_attributes,
		          recursion_depth + 1,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extended attributes: %" PRIu64 " from file system B-tree sub node.",
		 function,
		 identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		found_extended_attribute = 1;
	}
	return( found_extended_attribute );

on_error:
	libcdata_array_empty(
	 extended_attributes,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_internal_extended_attribute_free,
	 NULL );

	return( -1 );
}

/* Retrieves extended attributes for a specific identifier from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_extended_attributes(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libcdata_array_t *extended_attributes,
     libcerror_error_t **error )
{
	libfsapfs_btree_node_t *root_node = NULL;
	static char *function             = "libfsapfs_file_system_btree_get_extended_attributes";
	int is_leaf_node                  = 0;
	int result                        = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp  = 0;
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
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_start_timing(
		     file_system_btree->io_handle->profiler,
		     &profiler_start_timestamp,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to start timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving extended attributes of: %" PRIu64 "\n",
		 function,
		 identifier );
	}
#endif
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     file_system_btree->root_node_block_number,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	if( root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree root node.",
		 function );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                root_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node is a leaf node.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_extended_attributes_from_leaf_node(
		          file_system_btree,
		          file_io_handle,
		          root_node,
		          identifier,
		          extended_attributes,
		          error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_extended_attributes_from_branch_node(
		          file_system_btree,
		          file_io_handle,
		          root_node,
		          identifier,
		          extended_attributes,
		          0,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extended attributes: %" PRIu64 " from file system B-tree root node.",
		 function,
		 identifier );

		goto on_error;
	}
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_stop_timing(
		     file_system_btree->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to stop timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	return( result );

on_error:
	libcdata_array_empty(
	 extended_attributes,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_internal_extended_attribute_free,
	 NULL );

	return( -1 );
}

/* Retrieves file extents for a specific identifier from the file system B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_file_extents_from_leaf_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *file_extents,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry = NULL;
	libfsapfs_file_extent_t *file_extent = NULL;
	static char *function                = "libfsapfs_file_system_btree_get_file_extents_from_leaf_node";
	uint64_t file_system_identifier      = 0;
	uint64_t lookup_identifier           = 0;
	int btree_entry_index                = 0;
	int entry_index                      = 0;
	int found_file_extent                = 0;
	int is_leaf_node                     = 0;
	int number_of_entries                = 0;

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
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a leaf node.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving file extents of: %" PRIu64 "\n",
		 function,
		 identifier );
	}
#endif
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
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 btree_entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		if( ( file_system_identifier & 0x0fffffffffffffffUL ) > identifier )
		{
			break;
		}
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

		found_file_extent = 1;
	}
	return( found_file_extent );

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

/* Retrieves file extents for a specific identifier from the file system B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_file_extents_from_branch_node(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     libfsapfs_btree_node_t *node,
     uint64_t identifier,
     libcdata_array_t *file_extents,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry          = NULL;
	libfsapfs_btree_entry_t *previous_entry = NULL;
	libfsapfs_btree_node_t *sub_node        = NULL;
	static char *function                   = "libfsapfs_file_system_btree_get_file_extents_from_branch_node";
	uint64_t file_extent_logical_address    = 0;
	uint64_t file_system_identifier         = 0;
	uint64_t sub_node_block_number          = 0;
	uint8_t file_system_data_type           = 0;
	int entry_index                         = 0;
	int found_file_extent                   = 0;
	int is_leaf_node                        = 0;
	int number_of_entries                   = 0;
	int result                              = 0;

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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSAPFS_MAXIMUM_BTREE_NODE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a branch node.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving file extents of: %" PRIu64 "\n",
		 function,
		 identifier );
	}
#endif
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
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfsapfs_btree_node_get_entry_by_index(
		     node,
		     entry_index,
		     &entry,
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
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry: %d - missing key data.",
			 function,
			 entry_index );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_file_system_btree_key_common_t *) entry->key_data )->file_system_identifier,
		 file_system_identifier );

		file_system_data_type = (uint8_t) ( file_system_identifier >> 60 );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 ", data type: 0x%" PRIx8 " %s\n",
			 function,
			 entry_index,
			 file_system_identifier & 0x0fffffffffffffffUL,
			 file_system_data_type,
			 libfsapfs_debug_print_file_system_data_type(
			  file_system_data_type ) );
		}
#endif
		file_system_identifier &= 0x0fffffffffffffffUL;

		if( ( file_system_identifier > identifier )
		 || ( ( file_system_identifier == identifier )
		  &&  ( file_system_data_type > LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_FILE_EXTENT ) ) )
		{
			break;
		}
		if( ( file_system_identifier == identifier )
		 && ( file_system_data_type == LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_FILE_EXTENT ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_btree_key_file_extent_t *) entry->key_data )->logical_address,
			 file_extent_logical_address );
		}
		else
		{
			file_extent_logical_address = 0;
		}
		if( ( file_system_identifier == identifier )
		 && ( file_system_data_type == LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_FILE_EXTENT )
		 && ( file_extent_logical_address > 0 )
		 && ( previous_entry != NULL ) )
		{
			if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
			     file_system_btree,
			     file_io_handle,
			     previous_entry,
			     &sub_node_block_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine sub node block number.",
				 function );

				goto on_error;
			}
			if( libfsapfs_file_system_btree_get_sub_node(
			     file_system_btree,
			     file_io_handle,
			     sub_node_block_number,
			     &sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
				 function,
				 sub_node_block_number );

				goto on_error;
			}
			is_leaf_node = libfsapfs_btree_node_is_leaf_node(
			                sub_node,
			                error );

			if( is_leaf_node == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if B-tree sub node is a leaf node.",
				 function );

				goto on_error;
			}
			if( is_leaf_node != 0 )
			{
				result = libfsapfs_file_system_btree_get_file_extents_from_leaf_node(
				          file_system_btree,
				          sub_node,
				          identifier,
				          file_extents,
				          error );
			}
			else
			{
				result = libfsapfs_file_system_btree_get_file_extents_from_branch_node(
				          file_system_btree,
				          file_io_handle,
				          sub_node,
				          identifier,
				          file_extents,
				          recursion_depth + 1,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file extents: %" PRIu64 " from file system B-tree sub node.",
				 function,
				 identifier );

				goto on_error;
			}
			else if( result != 0 )
			{
				found_file_extent = 1;
			}
			sub_node = NULL;
		}
		previous_entry = entry;
	}
	/* Fall-through for the last B-Tree entry
	 */
	if( libfsapfs_file_system_btree_get_sub_node_block_number_from_entry(
	     file_system_btree,
	     file_io_handle,
	     previous_entry,
	     &sub_node_block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine sub node block number.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_system_btree_get_sub_node(
	     file_system_btree,
	     file_io_handle,
	     sub_node_block_number,
	     &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree sub node from block: %" PRIu64 ".",
		 function,
		 sub_node_block_number );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                sub_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree sub node is a leaf node.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_file_extents_from_leaf_node(
		          file_system_btree,
		          sub_node,
		          identifier,
		          file_extents,
		          error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_file_extents_from_branch_node(
		          file_system_btree,
		          file_io_handle,
		          sub_node,
		          identifier,
		          file_extents,
		          recursion_depth + 1,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file extents: %" PRIu64 " from file system B-tree sub node.",
		 function,
		 identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		found_file_extent = 1;
	}
	return( found_file_extent );

on_error:
	libcdata_array_empty(
	 file_extents,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
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
	int is_leaf_node                  = 0;
	int result                        = 0;

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving file extents of: %" PRIu64 "\n",
		 function,
		 identifier );
	}
#endif
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     file_system_btree->root_node_block_number,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                root_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node is a leaf node.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_file_extents_from_leaf_node(
		          file_system_btree,
		          root_node,
		          identifier,
		          file_extents,
		          error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_file_extents_from_branch_node(
		          file_system_btree,
		          file_io_handle,
		          root_node,
		          identifier,
		          file_extents,
		          0,
		          error );
	}
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
	return( result );

on_error:
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
	libfsapfs_btree_node_t *btree_node   = NULL;
	static char *function                = "libfsapfs_file_system_btree_get_inode_by_identifier";
	int result                           = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp     = 0;
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
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_start_timing(
		     file_system_btree->io_handle->profiler,
		     &profiler_start_timestamp,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to start timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving inode of: %" PRIu64 "\n",
		 function,
		 identifier );
	}
#endif
	result = libfsapfs_file_system_btree_get_entry_by_identifier(
	          file_system_btree,
	          file_io_handle,
	          identifier,
	          LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_INODE,
	          &btree_node,
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
		if( btree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree node.",
			 function );

			goto on_error;
		}
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
		btree_node = NULL;
	}
#if defined( HAVE_PROFILER )
	if( file_system_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_stop_timing(
		     file_system_btree->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to stop timing.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_PROFILER ) */

	return( result );

on_error:
	if( *inode != NULL )
	{
		libfsapfs_inode_free(
		 inode,
		 NULL );
	}
	return( -1 );
}

/* Retrieves an inode for an UTF-8 encoded name from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_inode_by_utf8_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_inode_t **inode,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry = NULL;
	libfsapfs_btree_node_t *btree_node   = NULL;
	libfsapfs_btree_node_t *root_node    = NULL;
	static char *function                = "libfsapfs_file_system_btree_get_inode_by_utf8_name";
	uint64_t lookup_identifier           = 0;
	uint32_t name_hash                   = 0;
	int is_leaf_node                     = 0;
	int result                           = 0;

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
	if( *directory_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory record value already set.",
		 function );

		return( -1 );
	}
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     file_system_btree->root_node_block_number,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                root_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node is a leaf node.",
		 function );

		goto on_error;
	}
	if( libfsapfs_name_hash_calculate_from_utf8_string(
	     &name_hash,
	     utf8_string,
	     utf8_string_length,
	     file_system_btree->use_case_folding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine name hash.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf8_name(
			  file_system_btree,
			  root_node,
			  parent_identifier,
			  utf8_string,
			  utf8_string_length,
			  name_hash,
			  directory_record,
			  error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf8_name(
			  file_system_btree,
			  file_io_handle,
			  root_node,
			  parent_identifier,
			  utf8_string,
			  utf8_string_length,
			  name_hash,
			  directory_record,
			  0,
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
	else if( result != 0 )
	{
		if( libfsapfs_directory_record_get_identifier(
		     *directory_record,
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
	}
	root_node = NULL;

	if( result != 0 )
	{
		result = libfsapfs_file_system_btree_get_entry_by_identifier(
		          file_system_btree,
		          file_io_handle,
		          lookup_identifier,
		          LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_INODE,
		          &btree_node,
		          &btree_entry,
		          error );

		if( result != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry from B-tree node.",
			 function );

			goto on_error;
		}
		if( btree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree node.",
			 function );

			goto on_error;
		}
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
		btree_node = NULL;
	}
	return( result );

on_error:
	if( *directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 directory_record,
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
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry                = NULL;
	libfsapfs_btree_node_t *btree_node                  = NULL;
	libfsapfs_btree_node_t *root_node                   = NULL;
	libfsapfs_directory_record_t *safe_directory_record = NULL;
	const uint8_t *utf8_string_segment                  = NULL;
	static char *function                               = "libfsapfs_file_system_btree_get_inode_by_utf8_path";
	libuna_unicode_character_t unicode_character        = 0;
	size_t utf8_string_index                            = 0;
	size_t utf8_string_segment_length                   = 0;
	uint64_t lookup_identifier                          = 0;
	uint32_t name_hash                                  = 0;
	int is_leaf_node                                    = 0;
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
	if( *directory_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory record value already set.",
		 function );

		return( -1 );
	}
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     file_system_btree->root_node_block_number,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                root_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node is a leaf node.",
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
			if( libfsapfs_name_hash_calculate_from_utf8_string(
			     &name_hash,
			     utf8_string_segment,
			     utf8_string_segment_length,
			     file_system_btree->use_case_folding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine name hash.",
				 function );

				goto on_error;
			}
			if( safe_directory_record != NULL )
			{
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
			if( is_leaf_node != 0 )
			{
				result = libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf8_name(
					  file_system_btree,
					  root_node,
					  lookup_identifier,
					  utf8_string_segment,
					  utf8_string_segment_length,
				          name_hash,
					  &safe_directory_record,
					  error );
			}
			else
			{
				result = libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf8_name(
					  file_system_btree,
					  file_io_handle,
					  root_node,
					  lookup_identifier,
					  utf8_string_segment,
					  utf8_string_segment_length,
				          name_hash,
					  &safe_directory_record,
					  0,
					  error );
			}
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
		     safe_directory_record,
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
	}
	root_node = NULL;

	if( result != 0 )
	{
		result = libfsapfs_file_system_btree_get_entry_by_identifier(
		          file_system_btree,
		          file_io_handle,
		          lookup_identifier,
		          LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_INODE,
		          &btree_node,
		          &btree_entry,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry from B-tree node.",
			 function );

			goto on_error;
		}
		if( btree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree node.",
			 function );

			goto on_error;
		}
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
		btree_node = NULL;

		*directory_record = safe_directory_record;
	}
	return( result );

on_error:
	if( safe_directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &safe_directory_record,
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

/* Retrieves an inode for an UTF-16 encoded name from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_btree_get_inode_by_utf16_name(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_inode_t **inode,
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry = NULL;
	libfsapfs_btree_node_t *btree_node   = NULL;
	libfsapfs_btree_node_t *root_node    = NULL;
	static char *function                = "libfsapfs_file_system_btree_get_inode_by_utf16_name";
	uint64_t lookup_identifier           = 0;
	uint32_t name_hash                   = 0;
	int is_leaf_node                     = 0;
	int result                           = 0;

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
	if( *directory_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory record value already set.",
		 function );

		return( -1 );
	}
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     file_system_btree->root_node_block_number,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                root_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node is a leaf node.",
		 function );

		goto on_error;
	}
	if( libfsapfs_name_hash_calculate_from_utf16_string(
	     &name_hash,
	     utf16_string,
	     utf16_string_length,
	     file_system_btree->use_case_folding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine name hash.",
		 function );

		goto on_error;
	}
	if( is_leaf_node != 0 )
	{
		result = libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf16_name(
			  file_system_btree,
			  root_node,
			  parent_identifier,
			  utf16_string,
			  utf16_string_length,
			  name_hash,
			  directory_record,
			  error );
	}
	else
	{
		result = libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf16_name(
			  file_system_btree,
			  file_io_handle,
			  root_node,
			  parent_identifier,
			  utf16_string,
			  utf16_string_length,
			  name_hash,
			  directory_record,
			  0,
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
	else if( result != 0 )
	{
		if( libfsapfs_directory_record_get_identifier(
		     *directory_record,
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
	}
	root_node = NULL;

	if( result != 0 )
	{
		result = libfsapfs_file_system_btree_get_entry_by_identifier(
		          file_system_btree,
		          file_io_handle,
		          lookup_identifier,
		          LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_INODE,
		          &btree_node,
		          &btree_entry,
		          error );

		if( result != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry from B-tree node.",
			 function );

			goto on_error;
		}
		if( btree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree node.",
			 function );

			goto on_error;
		}
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
		btree_node = NULL;
	}
	return( result );

on_error:
	if( *directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 directory_record,
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
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry                = NULL;
	libfsapfs_btree_node_t *btree_node                  = NULL;
	libfsapfs_btree_node_t *root_node                   = NULL;
	libfsapfs_directory_record_t *safe_directory_record = NULL;
	const uint16_t *utf16_string_segment                = NULL;
	static char *function                               = "libfsapfs_file_system_btree_get_inode_by_utf16_path";
	libuna_unicode_character_t unicode_character        = 0;
	size_t utf16_string_index                           = 0;
	size_t utf16_string_segment_length                  = 0;
	uint64_t lookup_identifier                          = 0;
	uint32_t name_hash                                  = 0;
	int is_leaf_node                                    = 0;
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
	if( *directory_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory record value already set.",
		 function );

		return( -1 );
	}
	if( libfsapfs_file_system_btree_get_root_node(
	     file_system_btree,
	     file_io_handle,
	     file_system_btree->root_node_block_number,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	is_leaf_node = libfsapfs_btree_node_is_leaf_node(
	                root_node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node is a leaf node.",
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
			if( libfsapfs_name_hash_calculate_from_utf16_string(
			     &name_hash,
			     utf16_string_segment,
			     utf16_string_segment_length,
			     file_system_btree->use_case_folding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine name hash.",
				 function );

				goto on_error;
			}
			if( safe_directory_record != NULL )
			{
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
			if( is_leaf_node != 0 )
			{
				result = libfsapfs_file_system_btree_get_directory_record_from_leaf_node_by_utf16_name(
					  file_system_btree,
					  root_node,
					  lookup_identifier,
					  utf16_string_segment,
					  utf16_string_segment_length,
				          name_hash,
					  &safe_directory_record,
					  error );
			}
			else
			{
				result = libfsapfs_file_system_btree_get_directory_record_from_branch_node_by_utf16_name(
					  file_system_btree,
					  file_io_handle,
					  root_node,
					  lookup_identifier,
					  utf16_string_segment,
					  utf16_string_segment_length,
				          name_hash,
					  &safe_directory_record,
					  0,
					  error );
			}
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
		     safe_directory_record,
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
	}
	root_node = NULL;

	if( result != 0 )
	{
		result = libfsapfs_file_system_btree_get_entry_by_identifier(
		          file_system_btree,
		          file_io_handle,
		          lookup_identifier,
		          LIBFSAPFS_FILE_SYSTEM_DATA_TYPE_INODE,
		          &btree_node,
		          &btree_entry,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry from B-tree node.",
			 function );

			goto on_error;
		}
		if( btree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree node.",
			 function );

			goto on_error;
		}
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
		btree_node = NULL;

		*directory_record = safe_directory_record;
	}
	return( result );

on_error:
	if( safe_directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &safe_directory_record,
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

