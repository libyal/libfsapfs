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
#include "libfsapfs_btree_root.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcaes.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_btree.h"
#include "fsapfs_file_system.h"
#include "fsapfs_object.h"

/* Creates a file system B-tree
 * Make sure the value file_system_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_initialize(
     libfsapfs_file_system_btree_t **file_system_btree,
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
		memory_free(
		 *file_system_btree );

		*file_system_btree = NULL;
	}
	return( 1 );
}

/* Reads the file system B-tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_read_file_io_handle(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t block_size,
     libcerror_error_t **error )
{
	uint8_t tweak_value[ 16 ];

	libcaes_tweaked_context_t *xts_context = NULL;
	uint8_t *block_data                    = NULL;
	uint8_t *data                          = NULL;
	static char *function                  = "libfsapfs_file_system_btree_read_file_io_handle";
	size_t data_offset                     = 0;
	ssize_t read_count                     = 0;
	uint64_t calculated_sector_number      = 0;

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
		 "%s: unable to create file system B-tree.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file system B-tree at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek file system B-tree offset: %" PRIi64 " (0x%08" PRIx64 ").",
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
		 "%s: unable to read file system B-tree data.",
		 function );

		goto on_error;
	}
	if( file_system_btree->is_encrypted != 0 )
	{
		if( libcaes_tweaked_context_initialize(
		     &xts_context,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to initialize XTS context.",
			 function );

			goto on_error;
		}
		if( libcaes_tweaked_context_set_keys(
		     xts_context,
		     LIBCAES_CRYPT_MODE_DECRYPT,
		     file_system_btree->volume_master_key,
		     128,
		     &( file_system_btree->volume_master_key[ 16 ] ),
		     128,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to set AES-XTS keys.",
			 function );

			goto on_error;
		}
		data = (uint8_t *) memory_allocate(
		                    sizeof( uint8_t ) * block_size );

		if( data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     tweak_value,
		     0,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to copy block number to tweak value.",
			 function );

			goto on_error;
		}
		calculated_sector_number = (uint64_t) file_offset / 512;

		for( data_offset = 0;
		     data_offset < (size_t) block_size;
		     data_offset += 512 )
		{
			byte_stream_copy_from_uint64_little_endian(
			 tweak_value,
			 calculated_sector_number );

			if( libcaes_crypt_xts(
			     xts_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     tweak_value,
			     16,
			     &( block_data[ data_offset ] ),
			     512,
			     &( data[ data_offset ] ),
			     512,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
				 "%s: unable to decrypt data.",
				 function );

				goto on_error;
			}
			calculated_sector_number += 1;
		}
		memory_free(
		 block_data );

		block_data = data;
		data       = NULL;
	}
	if( libfsapfs_file_system_btree_read_data(
	     file_system_btree,
	     block_data,
	     (size_t) block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file system B-tree data.",
		 function );

		goto on_error;
	}
	memory_free(
	 block_data );

	return( 1 );

on_error:
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	if( block_data != NULL )
	{
		memory_free(
		 block_data );
	}
	return( -1 );
}

/* Reads the file system B-tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_read_data(
     libfsapfs_file_system_btree_t *file_system_btree,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_btree_entry_t *btree_entry = NULL;
	libfsapfs_btree_root_t *btree_root   = NULL;
	static char *function                = "libfsapfs_file_system_btree_read_data";
	int btree_entry_index                = 0;
	int number_of_entries                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                 = 0;
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
	if( ( btree_root->object_type != 0x00000002UL )
	 && ( btree_root->object_type != 0x10000002UL ) )
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
	if( btree_root->object_subtype != 0x0000000eUL )
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
	if( btree_root->header->flags != 0x0003 )
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
	if( btree_root->footer->key_size != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key size value out of bounds.",
		 function );

		goto on_error;
	}
	if( btree_root->footer->value_size != 0 )
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
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (fsapfs_file_system_btree_key_common_t *) btree_entry->key_data )->file_system_identifier,
			 value_64bit );
			libcnotify_printf(
			 "%s: file system identifier\t\t: 0x%08" PRIx64 " (data type: 0x%" PRIx64 ")\n",
			 function,
			 value_64bit,
			 value_64bit >> 60 );
		}
#endif
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
	if( btree_root != NULL )
	{
		libfsapfs_btree_root_free(
		 &btree_root,
		 NULL );
	}
	return( -1 );
}

