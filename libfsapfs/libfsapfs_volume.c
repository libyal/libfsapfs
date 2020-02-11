/*
 * Volume functions
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsapfs_container_data_handle.h"
#include "libfsapfs_container_key_bag.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_extent_reference_tree.h"
#include "libfsapfs_file_entry.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_file_system_data_handle.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_libuna.h"
#include "libfsapfs_object_map.h"
#include "libfsapfs_object_map_btree.h"
#include "libfsapfs_object_map_descriptor.h"
#include "libfsapfs_snapshot.h"
#include "libfsapfs_snapshot_metadata.h"
#include "libfsapfs_snapshot_metadata_tree.h"
#include "libfsapfs_volume.h"
#include "libfsapfs_volume_key_bag.h"
#include "libfsapfs_volume_superblock.h"

/* Creates a volume
 * Make sure the value volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_initialize(
     libfsapfs_volume_t **volume,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsapfs_container_key_bag_t *container_key_bag,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_initialize";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	internal_volume = memory_allocate_structure(
	                   libfsapfs_internal_volume_t );

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_volume,
	     0,
	     sizeof( libfsapfs_internal_volume_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.",
		 function );

		goto on_error;
	}
/* TODO clone file_io_handle? */
	internal_volume->io_handle         = io_handle;
	internal_volume->file_io_handle    = file_io_handle;
	internal_volume->container_key_bag = container_key_bag;
	internal_volume->is_locked         = 1;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_volume->read_write_lock ),
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
	*volume = (libfsapfs_volume_t *) internal_volume;

	return( 1 );

on_error:
	if( internal_volume != NULL )
	{
		memory_free(
		 internal_volume );
	}
	return( -1 );
}

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_free(
     libfsapfs_volume_t **volume,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_free";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		internal_volume = (libfsapfs_internal_volume_t *) *volume;
		*volume         = NULL;

		if( internal_volume->file_io_handle != NULL )
		{
			if( libfsapfs_internal_volume_close(
			     internal_volume,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close volume.",
				 function );

				result = -1;
			}
		}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_volume->read_write_lock ),
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
		memory_free(
		 internal_volume );
	}
	return( result );
}

/* Opens a volume for reading
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_internal_volume_open_read(
     libfsapfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsapfs_container_data_handle_t *container_data_handle     = NULL;
	libfsapfs_file_system_data_handle_t *file_system_data_handle = NULL;
	libfsapfs_object_map_t *object_map                           = NULL;
	static char *function                                        = "libfsapfs_internal_volume_open_read";
	uint64_t key_bag_block_number                                = 0;
	uint64_t key_bag_number_of_blocks                            = 0;
	int element_index                                            = 0;
	int result                                                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libfsapfs_extent_reference_tree_t *extent_reference_tree     = NULL;
#endif

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - superblock map value already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->container_data_block_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - container data block vector value already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->object_map_btree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - object map B-tree value already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->snapshot_metadata_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - snapshot metadata tree value already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->key_bag != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - key bag value already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->encryption_context != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - encryption context value already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_system_data_block_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file system data block vector value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading volume superblock:\n" );
	}
#endif
	if( libfsapfs_volume_superblock_initialize(
	     &( internal_volume->superblock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create volume superblock.",
		 function );

		goto on_error;
	}
	if( libfsapfs_volume_superblock_read_file_io_handle(
	     internal_volume->superblock,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume superblock at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( libfsapfs_container_data_handle_initialize(
	     &container_data_handle,
	     internal_volume->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create container data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_initialize(
	     &( internal_volume->container_data_block_vector ),
	     (size64_t) internal_volume->io_handle->block_size,
	     (intptr_t *) container_data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_container_data_handle_free,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsapfs_container_data_handle_read_data_block,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create container data block vector.",
		 function );

		goto on_error;
	}
	internal_volume->container_data_handle = container_data_handle;
	container_data_handle                  = NULL;

	if( libfdata_vector_append_segment(
	     internal_volume->container_data_block_vector,
	     &element_index,
	     0,
	     0,
	     internal_volume->io_handle->container_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to container data block vector.",
		 function );

		goto on_error;
	}
/* TODO refactor into function to read object map */
	if( internal_volume->superblock->object_map_block_number == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing object map block number.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading object map:\n" );
	}
#endif
	file_offset = internal_volume->superblock->object_map_block_number * internal_volume->io_handle->block_size;

	if( libfsapfs_object_map_initialize(
	     &object_map,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create object map.",
		 function );

		goto on_error;
	}
	if( libfsapfs_object_map_read_file_io_handle(
	     object_map,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object map at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( object_map->btree_block_number == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing object map B-tree block number.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading object map B-tree:\n" );
	}
#endif
	if( libfsapfs_object_map_btree_initialize(
	     &( internal_volume->object_map_btree ),
	     internal_volume->io_handle,
	     internal_volume->container_data_block_vector,
	     object_map->btree_block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create object map B-tree.",
		 function );

		goto on_error;
	}
	if( libfsapfs_object_map_free(
	     &object_map,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free object map.",
		 function );

		goto on_error;
	}
	internal_volume->is_locked = 0;

	if( ( internal_volume->container_key_bag != NULL )
	 && ( ( internal_volume->superblock->volume_flags & 0x00000001UL ) == 0 ) )
	{
		internal_volume->is_locked = internal_volume->container_key_bag->is_locked;

		result = libfsapfs_container_key_bag_get_volume_key_bag_extent_by_identifier(
		          internal_volume->container_key_bag,
		          internal_volume->superblock->volume_identifier,
		          &key_bag_block_number,
		          &key_bag_number_of_blocks,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume key bag extent.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( ( key_bag_block_number == 0 )
			 || ( key_bag_number_of_blocks == 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid volume key bag extent.",
				 function );

				goto on_error;
			}
			if( libfsapfs_volume_key_bag_initialize(
			     &( internal_volume->key_bag ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create volume key bag.",
				 function );

				goto on_error;
			}
			file_offset = key_bag_block_number * internal_volume->io_handle->block_size;

			if( libfsapfs_volume_key_bag_read_file_io_handle(
			     internal_volume->key_bag,
			     internal_volume->io_handle,
			     file_io_handle,
			     file_offset,
			     (size64_t) key_bag_number_of_blocks * internal_volume->io_handle->block_size,
			     internal_volume->superblock->volume_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read volume key bag at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 file_offset,
				 file_offset );

				goto on_error;
			}
			if( libfsapfs_encryption_context_initialize(
			     &( internal_volume->encryption_context ),
			     LIBFSAPFS_ENCRYPTION_METHOD_AES_128_XTS,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize encryption context.",
				 function );

				goto on_error;
			}
			internal_volume->is_locked = 1;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( internal_volume->superblock->extent_reference_tree_block_number != 0 )
	{
		if( libfsapfs_extent_reference_tree_initialize(
		     &extent_reference_tree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent reference tree.",
			 function );

			goto on_error;
		}
		file_offset = internal_volume->superblock->extent_reference_tree_block_number * internal_volume->io_handle->block_size;

		if( libfsapfs_extent_reference_tree_read_file_io_handle(
		     extent_reference_tree,
		     file_io_handle,
		     file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read extent reference tree at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 file_offset,
			 file_offset );

			goto on_error;
		}
		if( libfsapfs_extent_reference_tree_free(
		     &extent_reference_tree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extent reference tree.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libcdata_array_initialize(
	     &( internal_volume->snapshots ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create snapshots array.",
		 function );

		goto on_error;
	}
	if( internal_volume->superblock->snapshot_metadata_tree_block_number != 0 )
	{
		if( libfsapfs_snapshot_metadata_tree_initialize(
		     &( internal_volume->snapshot_metadata_tree ),
		     internal_volume->io_handle,
		     internal_volume->container_data_block_vector,
		     internal_volume->object_map_btree,
		     internal_volume->superblock->snapshot_metadata_tree_block_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create snapshot metadata tree.",
			 function );

			goto on_error;
		}
		if( libfsapfs_snapshot_metadata_tree_get_snapshots(
		     internal_volume->snapshot_metadata_tree,
		     internal_volume->file_io_handle,
		     internal_volume->snapshots,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve snapshots.",
			 function );

			goto on_error;
		}
	}
	if( internal_volume->superblock->file_system_root_object_identifier == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file system root object identifier - value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_system_data_handle_initialize(
	     &file_system_data_handle,
	     internal_volume->io_handle,
	     internal_volume->encryption_context,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file system data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_initialize(
	     &( internal_volume->file_system_data_block_vector ),
	     (size64_t) internal_volume->io_handle->block_size,
	     (intptr_t *) file_system_data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_system_data_handle_free,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsapfs_file_system_data_handle_read_data_block,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file system data block vector.",
		 function );

		goto on_error;
	}
	internal_volume->file_system_data_handle = file_system_data_handle;
	file_system_data_handle                  = NULL;

	if( libfdata_vector_append_segment(
	     internal_volume->file_system_data_block_vector,
	     &element_index,
	     0,
	     0,
	     internal_volume->io_handle->container_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to file system data block vector.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_volume->file_system_data_block_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_volume->file_system_data_block_vector ),
		 NULL );
	}
	if( file_system_data_handle != NULL )
	{
		libfsapfs_file_system_data_handle_free(
		 &file_system_data_handle,
		 NULL );
	}
	if( internal_volume->encryption_context != NULL )
	{
		libfsapfs_encryption_context_free(
		 &( internal_volume->encryption_context ),
		 NULL );
	}
	if( internal_volume->key_bag != NULL )
	{
		libfsapfs_volume_key_bag_free(
		 &( internal_volume->key_bag ),
		 NULL );
	}
	if( internal_volume->snapshot_metadata_tree != NULL )
	{
		libcdata_array_free(
		 &( internal_volume->snapshots ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_snapshot_metadata_free,
		 NULL );
	}
	if( internal_volume->snapshot_metadata_tree != NULL )
	{
		libfsapfs_snapshot_metadata_tree_free(
		 &( internal_volume->snapshot_metadata_tree ),
		 NULL );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( extent_reference_tree != NULL )
	{
		libfsapfs_extent_reference_tree_free(
		 &extent_reference_tree,
		 NULL );
	}
#endif
	if( internal_volume->object_map_btree != NULL )
	{
		libfsapfs_object_map_btree_free(
		 &( internal_volume->object_map_btree ),
		 NULL );
	}
	if( object_map != NULL )
	{
		libfsapfs_object_map_free(
		 &object_map,
		 NULL );
	}
	if( internal_volume->container_data_block_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_volume->container_data_block_vector ),
		 NULL );
	}
	if( container_data_handle != NULL )
	{
		libfsapfs_container_data_handle_free(
		 &container_data_handle,
		 NULL );
	}
	if( internal_volume->superblock != NULL )
	{
		libfsapfs_volume_superblock_free(
		 &( internal_volume->superblock ),
		 NULL );
	}
	return( -1 );
}

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
int libfsapfs_internal_volume_close(
     libfsapfs_internal_volume_t *internal_volume,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_internal_volume_close";
	int result            = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_volume->file_io_handle = NULL;
	internal_volume->is_locked      = 1;

	if( internal_volume->user_password != NULL )
	{
		if( memory_set(
		     internal_volume->user_password,
		     0,
		     internal_volume->user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume->user_password );

		internal_volume->user_password      = NULL;
		internal_volume->user_password_size = 0;
	}
	if( internal_volume->recovery_password != NULL )
	{
		if( memory_set(
		     internal_volume->recovery_password,
		     0,
		     internal_volume->recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume->recovery_password );

		internal_volume->recovery_password      = NULL;
		internal_volume->recovery_password_size = 0;
	}
	if( internal_volume->superblock != NULL )
	{
		if( libfsapfs_volume_superblock_free(
		     &( internal_volume->superblock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free superblock.",
			 function );

			result = -1;
		}
	}
	if( libfdata_vector_free(
	     &( internal_volume->container_data_block_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free container data block vector.",
		 function );

		result = -1;
	}
	if( internal_volume->object_map_btree != NULL )
	{
		if( libfsapfs_object_map_btree_free(
		     &( internal_volume->object_map_btree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free object map B-tree.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->snapshot_metadata_tree != NULL )
	{
		if( libfsapfs_snapshot_metadata_tree_free(
		     &( internal_volume->snapshot_metadata_tree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free snapshot metadata tree.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->snapshots != NULL )
	{
		if( libcdata_array_free(
		     &( internal_volume->snapshots ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_snapshot_metadata_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free snapshots array.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->key_bag != NULL )
	{
		if( libfsapfs_volume_key_bag_free(
		     &( internal_volume->key_bag ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key bag.",
			 function );

			result = -1;
		}
	}
	if( internal_volume->encryption_context != NULL )
	{
		if( libfsapfs_encryption_context_free(
		     &( internal_volume->encryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encryption context.",
			 function );

			result = -1;
		}
	}
	if( libfdata_vector_free(
	     &( internal_volume->file_system_data_block_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file system data block vector.",
		 function );

		result = -1;
	}
	if( internal_volume->file_system_btree != NULL )
	{
		if( libfsapfs_file_system_btree_free(
		     &( internal_volume->file_system_btree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file system B-tree.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Unlocks an encrypted volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
int libfsapfs_internal_volume_unlock(
     libfsapfs_internal_volume_t *internal_volume,
     libcerror_error_t **error )
{
	uint8_t volume_key[ 32 ];
	uint8_t volume_master_key[ 32 ];

	static char *function = "libfsapfs_internal_volume_unlock";
	int result            = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing superblock.",
		 function );

		return( -1 );
	}
	if( internal_volume->key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing key bag.",
		 function );

		return( -1 );
	}
	result = libfsapfs_volume_key_bag_get_volume_key(
	          internal_volume->key_bag,
	          internal_volume->user_password,
	          internal_volume->user_password_size - 1,
	          internal_volume->recovery_password,
	          internal_volume->recovery_password_size - 1,
	          volume_key,
	          256,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume key using password.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsapfs_container_key_bag_get_volume_master_key_by_identifier(
		     internal_volume->container_key_bag,
		     internal_volume->superblock->volume_identifier,
		     volume_key,
		     256,
		     volume_master_key,
		     256,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume master key.",
			 function );

			goto on_error;
		}
		memory_set(
		 volume_key,
		 0,
		 32 );

		if( libfsapfs_encryption_context_set_keys(
		     internal_volume->encryption_context,
		     volume_master_key,
		     16,
		     &( volume_master_key[ 16 ] ),
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set keys in encryption context.",
			 function );

			goto on_error;
		}
		memory_set(
		 volume_master_key,
		 0,
		 32 );
	}
	if( result != 0 )
	{
		internal_volume->is_locked = 0;
	}
	return( result );

on_error:
	memory_set(
	 volume_master_key,
	 0,
	 32 );

	memory_set(
	 volume_key,
	 0,
	 32 );

	return( -1 );
}

/* Unlocks the volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
int libfsapfs_volume_unlock(
     libfsapfs_volume_t *volume,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_unlock";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->is_locked != 0 )
	{
		result = libfsapfs_internal_volume_unlock(
		          internal_volume,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to unlock volume.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_get_size(
     libfsapfs_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

	if( internal_volume->superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing superblock.",
		 function );

		return( -1 );
	}
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
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
/* TODO implement */
	*size = (size64_t) 0;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
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
	return( -1 );
}

/* Retrieves the identifier
 * The identifier is an UUID stored in big-endian and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_get_identifier(
     libfsapfs_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_identifier";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

	if( internal_volume->superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing superblock.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
	if( libfsapfs_volume_superblock_get_volume_identifier(
	     internal_volume->superblock,
	     uuid_data,
	     uuid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume identifier.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
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

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_get_utf8_name_size(
     libfsapfs_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_utf8_name_size";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
	if( libfsapfs_volume_superblock_get_utf8_volume_name_size(
	     internal_volume->superblock,
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
	     internal_volume->read_write_lock,
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
int libfsapfs_volume_get_utf8_name(
     libfsapfs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_utf8_name";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
	if( libfsapfs_volume_superblock_get_utf8_volume_name(
	     internal_volume->superblock,
	     utf8_string,
	     utf8_string_size,
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
	     internal_volume->read_write_lock,
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

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_get_utf16_name_size(
     libfsapfs_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_utf16_name_size";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
	if( libfsapfs_volume_superblock_get_utf16_volume_name_size(
	     internal_volume->superblock,
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
	     internal_volume->read_write_lock,
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
int libfsapfs_volume_get_utf16_name(
     libfsapfs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_utf16_name";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
	if( libfsapfs_volume_superblock_get_utf16_volume_name(
	     internal_volume->superblock,
	     utf16_string,
	     utf16_string_size,
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
	     internal_volume->read_write_lock,
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

/* Determines if the volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
int libfsapfs_volume_is_locked(
     libfsapfs_volume_t *volume,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_is_locked";
	uint8_t is_locked                            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
	is_locked = internal_volume->is_locked;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
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
	return( is_locked );
}

/* Sets an UTF-8 formatted password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libfsapfs_volume_set_utf8_password(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_set_utf8_password";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->user_password != NULL )
	{
		if( memory_set(
		     internal_volume->user_password,
		     0,
		     internal_volume->user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			goto on_error;
		}
		memory_free(
		 internal_volume->user_password );

		internal_volume->user_password      = NULL;
		internal_volume->user_password_size = 0;
	}
	internal_volume->user_password_size = 1 + narrow_string_length(
	                                           (char *) utf8_string );

	internal_volume->user_password = (uint8_t *) memory_allocate(
	                                              sizeof( uint8_t ) * internal_volume->user_password_size );

	if( internal_volume->user_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create user password.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_volume->user_password,
	     utf8_string,
	     utf8_string_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy user password.",
		 function );

		goto on_error;
	}
	internal_volume->user_password[ internal_volume->user_password_size - 1 ] = 0;

	internal_volume->user_password_is_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: user password: %s\n",
		 function,
		 internal_volume->user_password );
	}
#endif
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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

on_error:
	if( internal_volume->user_password != NULL )
	{
		memory_set(
		 internal_volume->user_password,
		 0,
		 internal_volume->user_password_size );
		memory_free(
		 internal_volume->user_password );

		internal_volume->user_password = NULL;
	}
	internal_volume->user_password_size = 0;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets an UTF-16 formatted password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libfsapfs_volume_set_utf16_password(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_set_utf16_password";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->user_password != NULL )
	{
		if( memory_set(
		     internal_volume->user_password,
		     0,
		     internal_volume->user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			goto on_error;
		}
		memory_free(
		 internal_volume->user_password );

		internal_volume->user_password      = NULL;
		internal_volume->user_password_size = 0;
	}
	if( libuna_utf8_string_size_from_utf16(
	     utf16_string,
	     utf16_string_length,
	     &( internal_volume->user_password_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set password size.",
		 function );

		goto on_error;
	}
	internal_volume->user_password_size += 1;

	internal_volume->user_password = (uint8_t *) memory_allocate(
	                                              sizeof( uint8_t ) * internal_volume->user_password_size );

	if( internal_volume->user_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create user password.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16(
	     internal_volume->user_password,
	     internal_volume->user_password_size,
	     utf16_string,
	     utf16_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy user password.",
		 function );

		goto on_error;
	}
	internal_volume->user_password[ internal_volume->user_password_size - 1 ] = 0;

	internal_volume->user_password_is_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: user password: %s\n",
		 function,
		 internal_volume->user_password );
	}
#endif
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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

on_error:
	if( internal_volume->user_password != NULL )
	{
		memory_set(
		 internal_volume->user_password,
		 0,
		 internal_volume->user_password_size );
		memory_free(
		 internal_volume->user_password );

		internal_volume->user_password = NULL;
	}
	internal_volume->user_password_size = 0;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libfsapfs_volume_set_utf8_recovery_password(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_set_utf8_recovery_password";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->recovery_password != NULL )
	{
		if( memory_set(
		     internal_volume->recovery_password,
		     0,
		     internal_volume->recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			goto on_error;
		}
		memory_free(
		 internal_volume->recovery_password );

		internal_volume->recovery_password      = NULL;
		internal_volume->recovery_password_size = 0;
	}
	internal_volume->recovery_password_size = 1 + narrow_string_length(
	                                               (char *) utf8_string );

	internal_volume->recovery_password = (uint8_t *) memory_allocate(
	                                                  sizeof( uint8_t ) * internal_volume->recovery_password_size );

	if( internal_volume->recovery_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create recovery password.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_volume->recovery_password,
	     utf8_string,
	     utf8_string_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy recovery password.",
		 function );

		goto on_error;
	}
	internal_volume->recovery_password[ internal_volume->recovery_password_size - 1 ] = 0;

	internal_volume->recovery_password_is_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: recovery password: %s\n",
		 function,
		 internal_volume->recovery_password );
	}
#endif
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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

on_error:
	if( internal_volume->recovery_password != NULL )
	{
		memory_set(
		 internal_volume->recovery_password,
		 0,
		 internal_volume->recovery_password_size );
		memory_free(
		 internal_volume->recovery_password );

		internal_volume->recovery_password = NULL;
	}
	internal_volume->recovery_password_size = 0;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libfsapfs_volume_set_utf16_recovery_password(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_set_utf16_recovery_password";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->recovery_password != NULL )
	{
		if( memory_set(
		     internal_volume->recovery_password,
		     0,
		     internal_volume->recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			goto on_error;
		}
		memory_free(
		 internal_volume->recovery_password );

		internal_volume->recovery_password      = NULL;
		internal_volume->recovery_password_size = 0;
	}
	if( libuna_utf8_string_size_from_utf16(
	     utf16_string,
	     utf16_string_length,
	     &( internal_volume->recovery_password_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set password size.",
		 function );

		goto on_error;
	}
	internal_volume->recovery_password_size += 1;

	internal_volume->recovery_password = (uint8_t *) memory_allocate(
	                                                  sizeof( uint8_t ) * internal_volume->recovery_password_size );

	if( internal_volume->recovery_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create recovery password.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16(
	     internal_volume->recovery_password,
	     internal_volume->recovery_password_size,
	     utf16_string,
	     utf16_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy recovery password.",
		 function );

		goto on_error;
	}
	internal_volume->recovery_password[ internal_volume->recovery_password_size - 1 ] = 0;

	internal_volume->recovery_password_is_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: recovery password: %s\n",
		 function,
		 internal_volume->recovery_password );
	}
#endif
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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

on_error:
	if( internal_volume->recovery_password != NULL )
	{
		memory_set(
		 internal_volume->recovery_password,
		 0,
		 internal_volume->recovery_password_size );
		memory_free(
		 internal_volume->recovery_password );

		internal_volume->recovery_password = NULL;
	}
	internal_volume->recovery_password_size = 0;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the next file entry identifier
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_get_next_file_entry_identifier(
     libfsapfs_volume_t *volume,
     uint64_t *identifier,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_next_file_entry_identifier";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

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
	     internal_volume->read_write_lock,
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
	*identifier = internal_volume->superblock->next_file_system_object_identifier;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
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

/* Determines the file system B-tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_internal_volume_get_file_system_btree(
     libfsapfs_internal_volume_t *internal_volume,
     libcerror_error_t **error )
{
	libfsapfs_object_map_descriptor_t *object_map_descriptor = NULL;
	static char *function                                    = "libfsapfs_internal_volume_get_file_system_btree";
	uint8_t use_case_folding                                 = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing superblock.",
		 function );

		return( -1 );
	}
	if( internal_volume->is_locked != 0 )
	{
		if( libfsapfs_internal_volume_unlock(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to unlock volume.",
			 function );

			goto on_error;
		}
	}
	if( libfsapfs_object_map_btree_get_descriptor_by_object_identifier(
	     internal_volume->object_map_btree,
	     internal_volume->file_io_handle,
	     internal_volume->superblock->file_system_root_object_identifier,
	     &object_map_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve object map descriptor for file system root object identifier: %" PRIu64 ".",
		 function,
		 internal_volume->superblock->file_system_root_object_identifier );

		goto on_error;
	}
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
	if( ( internal_volume->superblock->incompatibility_features_flags & 0x00000000000000001 ) != 0 )
	{
		use_case_folding = 1;
	}
	if( libfsapfs_file_system_btree_initialize(
	     &( internal_volume->file_system_btree ),
	     internal_volume->io_handle,
	     internal_volume->encryption_context,
	     internal_volume->file_system_data_block_vector,
	     internal_volume->object_map_btree,
	     object_map_descriptor->physical_address,
	     use_case_folding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file system B-tree.",
		 function );

		goto on_error;
	}
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
	return( 1 );

on_error:
	if( object_map_descriptor != NULL )
	{
		libfsapfs_object_map_descriptor_free(
		 &object_map_descriptor,
		 NULL );
	}
	if( internal_volume->file_system_btree != NULL )
	{
		libfsapfs_file_system_btree_free(
		 &( internal_volume->file_system_btree ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific file entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_volume_get_file_entry_by_identifier(
     libfsapfs_volume_t *volume,
     uint64_t identifier,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsapfs_inode_t *inode                     = NULL;
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_file_entry_by_identifier";
	int result                                   = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->file_system_btree == NULL )
	{
		if( libfsapfs_internal_volume_get_file_system_btree(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine file system B-tree.",
			 function );

			goto on_error;
		}
	}
	result = libfsapfs_file_system_btree_get_inode_by_identifier(
	          internal_volume->file_system_btree,
	          internal_volume->file_io_handle,
	          identifier,
	          &inode,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode: %" PRIu64 " from file system B-tree.",
		 function,
		 identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsapfs_file_entry_initialize(
		     file_entry,
		     internal_volume->io_handle,
		     internal_volume->file_io_handle,
		     internal_volume->encryption_context,
		     internal_volume->file_system_btree,
		     inode,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the root directory file entry
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_get_root_directory(
     libfsapfs_volume_t *volume,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsapfs_inode_t *inode                     = NULL;
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_root_directory";
	int result                                   = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->file_system_btree == NULL )
	{
		if( libfsapfs_internal_volume_get_file_system_btree(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine file system B-tree.",
			 function );

			goto on_error;
		}
	}
	result = libfsapfs_file_system_btree_get_inode_by_identifier(
	          internal_volume->file_system_btree,
	          internal_volume->file_io_handle,
	          2,
	          &inode,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root directory inode from file system B-tree.",
		 function );

		goto on_error;
	}
/* TODO return 0 if no root directory inode */

	if( libfsapfs_file_entry_initialize(
	     file_entry,
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     internal_volume->encryption_context,
	     internal_volume->file_system_btree,
	     inode,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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

on_error:
	if( inode != NULL )
	{
		libfsapfs_inode_free(
		 &inode,
		 NULL );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the file entry for an UTF-8 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsapfs_volume_get_file_entry_by_utf8_path(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsapfs_directory_record_t *directory_record = NULL;
	libfsapfs_inode_t *inode                       = NULL;
	libfsapfs_internal_volume_t *internal_volume   = NULL;
	static char *function                          = "libfsapfs_volume_get_file_entry_by_utf8_path";
	int result                                     = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->file_system_btree == NULL )
	{
		if( libfsapfs_internal_volume_get_file_system_btree(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine file system B-tree.",
			 function );

			goto on_error;
		}
	}
	result = libfsapfs_file_system_btree_get_inode_by_utf8_path(
	          internal_volume->file_system_btree,
	          internal_volume->file_io_handle,
	          2,
	          utf8_string,
	          utf8_string_length,
	          &inode,
	          &directory_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode from file system B-tree.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsapfs_file_entry_initialize(
		     file_entry,
		     internal_volume->io_handle,
		     internal_volume->file_io_handle,
		     internal_volume->encryption_context,
		     internal_volume->file_system_btree,
		     inode,
		     directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			goto on_error;
		}
		inode            = NULL;
		directory_record = NULL;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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
	if( *file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 file_entry,
		 NULL );
	}
	if( directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &directory_record,
		 NULL );
	}
	if( inode != NULL )
	{
		libfsapfs_inode_free(
		 &inode,
		 NULL );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the file entry for an UTF-16 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsapfs_volume_get_file_entry_by_utf16_path(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsapfs_directory_record_t *directory_record = NULL;
	libfsapfs_inode_t *inode                       = NULL;
	libfsapfs_internal_volume_t *internal_volume   = NULL;
	static char *function                          = "libfsapfs_volume_get_file_entry_by_utf16_path";
	int result                                     = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
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
	if( internal_volume->file_system_btree == NULL )
	{
		if( libfsapfs_internal_volume_get_file_system_btree(
		     internal_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine file system B-tree.",
			 function );

			goto on_error;
		}
	}
	result = libfsapfs_file_system_btree_get_inode_by_utf16_path(
	          internal_volume->file_system_btree,
	          internal_volume->file_io_handle,
	          2,
	          utf16_string,
	          utf16_string_length,
	          &inode,
	          &directory_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode from file system B-tree.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsapfs_file_entry_initialize(
		     file_entry,
		     internal_volume->io_handle,
		     internal_volume->file_io_handle,
		     internal_volume->encryption_context,
		     internal_volume->file_system_btree,
		     inode,
		     directory_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			goto on_error;
		}
		inode            = NULL;
		directory_record = NULL;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
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
	if( *file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 file_entry,
		 NULL );
	}
	if( directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &directory_record,
		 NULL );
	}
	if( inode != NULL )
	{
		libfsapfs_inode_free(
		 &inode,
		 NULL );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the number of snapshots
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_get_number_of_snapshots(
     libfsapfs_volume_t *volume,
     int *number_of_snapshots,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsapfs_volume_get_number_of_snapshots";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
	if( libcdata_array_get_number_of_entries(
	     internal_volume->snapshots,
	     number_of_snapshots,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from snapshots array.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
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

/* Retrieves a specific of snapshot
 * The snapshot reference must be freed after use with libfsapfs_snapshot_free
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_get_snapshot_by_index(
     libfsapfs_volume_t *volume,
     int snapshot_index,
     libfsapfs_snapshot_t **snapshot,
     libcerror_error_t **error )
{
	libfsapfs_internal_volume_t *internal_volume     = NULL;
	libfsapfs_snapshot_metadata_t *snapshot_metadata = NULL;
	static char *function                            = "libfsapfs_volume_get_snapshot_by_index";
	off64_t file_offset                              = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsapfs_internal_volume_t *) volume;

	if( snapshot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid snapshot.",
		 function );

		return( -1 );
	}
	if( *snapshot != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid snapshot value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
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
	if( libcdata_array_get_entry_by_index(
	     internal_volume->snapshots,
	     snapshot_index,
	     (intptr_t **) &snapshot_metadata,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve snapshot metadata: %d.",
		 function,
		 snapshot_index );

		return( -1 );
	}
	if( snapshot_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid snapshot metadata: %d.",
		 function,
		 snapshot_index );

		goto on_error;
	}
	if( libfsapfs_snapshot_initialize(
	     snapshot,
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     snapshot_metadata,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create snapshot.",
		 function );

		goto on_error;
	}
	file_offset = (off64_t) ( snapshot_metadata->volume_superblock_block_number * internal_volume->io_handle->block_size );

	if( libfsapfs_internal_snapshot_open_read(
	     (libfsapfs_internal_snapshot_t *) *snapshot,
	     internal_volume->file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open snapshot: %d.",
		 function,
		 snapshot_index );

		goto on_error;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
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

on_error:
	if( *snapshot != NULL )
	{
		libfsapfs_snapshot_free(
		 snapshot,
		 NULL );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

