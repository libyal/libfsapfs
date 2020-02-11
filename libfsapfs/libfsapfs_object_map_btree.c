/*
 * The object map B-tree functions
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
#include "libfsapfs_definitions.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_object_map_btree.h"
#include "libfsapfs_object_map_descriptor.h"

#include "fsapfs_object.h"
#include "fsapfs_object_map.h"

/* Creates a object map B-tree
 * Make sure the value object_map_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_btree_initialize(
     libfsapfs_object_map_btree_t **object_map_btree,
     libfsapfs_io_handle_t *io_handle,
     libfdata_vector_t *data_block_vector,
     uint64_t root_node_block_number,
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
	if( libfcache_cache_initialize(
	     &( ( *object_map_btree )->data_block_cache ),
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
	     &( ( *object_map_btree )->node_cache ),
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
	( *object_map_btree )->io_handle              = io_handle;
	( *object_map_btree )->data_block_vector      = data_block_vector;
	( *object_map_btree )->root_node_block_number = root_node_block_number;

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
		/* The data_block_vector is referenced and freed elsewhere
		 */
		if( libfcache_cache_free(
		     &( ( *object_map_btree )->node_cache ),
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
		     &( ( *object_map_btree )->data_block_cache ),
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
		 *object_map_btree );

		*object_map_btree = NULL;
	}
	return( result );
}

/* Retrieves the object map B-tree root node
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_btree_get_root_node(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t root_node_block_number,
     libfsapfs_btree_node_t **root_node,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	libfsapfs_btree_node_t *node         = NULL;
	libfsapfs_data_block_t *data_block   = NULL;
	static char *function                = "libfsapfs_object_map_btree_get_root_node";
	int result                           = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp     = 0;
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
	if( object_map_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_start_timing(
		     object_map_btree->io_handle->profiler,
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
	          object_map_btree->node_cache,
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
		     object_map_btree->data_block_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) object_map_btree->data_block_cache,
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
		if( node->object_type != 0x40000002UL )
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
		if( node->object_subtype != 0x0000000bUL )
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
		 || ( ( node->node_header->flags & 0x0004 ) == 0 ) )
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
		if( node->footer->key_size != sizeof( fsapfs_object_map_btree_key_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid key size value out of bounds.",
			 function );

			goto on_error;
		}
		if( node->footer->value_size != sizeof( fsapfs_object_map_btree_value_t ) )
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
		     object_map_btree->node_cache,
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
		     object_map_btree->node_cache,
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
	if( object_map_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_stop_timing(
		     object_map_btree->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     root_node_block_number * object_map_btree->io_handle->block_size,
		     object_map_btree->io_handle->block_size,
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

/* Retrieves a object map B-tree sub node
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_object_map_btree_get_sub_node(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t sub_node_block_number,
     libfsapfs_btree_node_t **sub_node,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	libfsapfs_btree_node_t *node         = NULL;
	libfsapfs_data_block_t *data_block   = NULL;
	static char *function                = "libfsapfs_object_map_btree_get_sub_node";
	int result                           = 0;

#if defined( HAVE_PROFILER )
	int64_t profiler_start_timestamp     = 0;
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
	if( object_map_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_start_timing(
		     object_map_btree->io_handle->profiler,
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
	          object_map_btree->node_cache,
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
		     object_map_btree->data_block_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) object_map_btree->data_block_cache,
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
		if( node->object_type != 0x40000003UL )
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
		if( node->object_subtype != 0x0000000bUL )
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
		 || ( ( node->node_header->flags & 0x0004 ) == 0 ) )
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
		     object_map_btree->node_cache,
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
		     object_map_btree->node_cache,
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
	if( object_map_btree->io_handle->profiler != NULL )
	{
		if( libfsapfs_profiler_stop_timing(
		     object_map_btree->io_handle->profiler,
		     profiler_start_timestamp,
		     function,
		     sub_node_block_number * object_map_btree->io_handle->block_size,
		     object_map_btree->io_handle->block_size,
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

/* Retrieves an entry for a specific identifier from the object map B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_object_map_btree_get_entry_from_node_by_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     libfsapfs_btree_node_t *node,
     uint64_t object_identifier,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry          = NULL;
	libfsapfs_btree_entry_t *previous_entry = NULL;
	static char *function                   = "libfsapfs_object_map_btree_get_entry_from_node_by_identifier";
	uint64_t object_map_identifier          = 0;
	int btree_entry_index                   = 0;
	int is_leaf_node                        = 0;
	int number_of_entries                   = 0;

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
		 "%s: retrieving B-tree entry identifier: %" PRIu64 ".\n",
		 function,
		 object_identifier );
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
		 object_map_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree entry: %d, identifier: %" PRIu64 "\n",
			 function,
			 btree_entry_index,
			 object_map_identifier );
		}
#endif
		if( object_map_identifier > object_identifier )
		{
			break;
		}
		if( is_leaf_node != 0 )
		{
			if( object_map_identifier == object_identifier )
			{
				*btree_entry = entry;

				return( 1 );
			}
		}
		else
		{
			if( object_map_identifier >= object_identifier )
			{
				if( ( previous_entry == NULL )
				 || ( object_map_identifier == object_identifier ) )
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

/* Retrieves an entry for a specific identifier from the object map B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_object_map_btree_get_entry_by_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
     libfsapfs_btree_node_t **btree_node,
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry = NULL;
	libfsapfs_btree_node_t *node   = NULL;
	static char *function          = "libfsapfs_object_map_btree_get_entry_by_identifier";
	uint64_t sub_node_block_number = 0;
	int is_leaf_node               = 0;
	int recursion_depth            = 0;
	int result                     = 0;

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
	if( libfsapfs_object_map_btree_get_root_node(
	     object_map_btree,
	     file_io_handle,
	     object_map_btree->root_node_block_number,
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
		result = libfsapfs_object_map_btree_get_entry_from_node_by_identifier(
		          object_map_btree,
		          node,
		          object_identifier,
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
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
		byte_stream_copy_to_uint64_little_endian(
		 entry->value_data,
		 sub_node_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree sub node block number: %" PRIu64 "\n",
			 function,
			 sub_node_block_number );
		}
#endif
		node = NULL;

		if( libfsapfs_object_map_btree_get_sub_node(
		     object_map_btree,
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

/* Retrieves the object map descriptor of a specific object identifier
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libfsapfs_object_map_btree_get_descriptor_by_object_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     uint64_t object_identifier,
     libfsapfs_object_map_descriptor_t **descriptor,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *entry = NULL;
	libfsapfs_btree_node_t *node   = NULL;
	static char *function          = "libfsapfs_object_map_btree_get_descriptor_by_object_identifier";
	int result                     = 0;

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
	if( *descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid descriptor value already set.",
		 function );

		return( -1 );
	}
	result = libfsapfs_object_map_btree_get_entry_by_identifier(
	          object_map_btree,
	          file_io_handle,
	          object_identifier,
	          &node,
	          &entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry from B-tree.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree node.",
			 function );

			goto on_error;
		}
		if( entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid B-tree entry.",
			 function );

			goto on_error;
		}
		if( libfsapfs_object_map_descriptor_initialize(
		     descriptor,
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
		if( libfsapfs_object_map_descriptor_read_key_data(
		     *descriptor,
		     entry->key_data,
		     (size_t) entry->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read object map descriptor key data.",
			 function );

			goto on_error;
		}
		if( libfsapfs_object_map_descriptor_read_value_data(
		     *descriptor,
		     entry->value_data,
		     (size_t) entry->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read object map descriptor value data.",
			 function );

			goto on_error;
		}
		node = NULL;
	}
	return( result );

on_error:
	if( *descriptor != NULL )
	{
		libfsapfs_object_map_descriptor_free(
		 descriptor,
		 NULL );
	}
	return( -1 );
}

