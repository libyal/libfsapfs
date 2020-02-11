/*
 * The volume key bag functions
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

#include "libfsapfs_definitions.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_key_bag_entry.h"
#include "libfsapfs_key_bag_header.h"
#include "libfsapfs_key_encrypted_key.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_volume_key_bag.h"

#include "fsapfs_key_bag.h"
#include "fsapfs_object.h"

/* Creates a volume key bag
 * Make sure the value volume_key_bag is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_key_bag_initialize(
     libfsapfs_volume_key_bag_t **volume_key_bag,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_volume_key_bag_initialize";

	if( volume_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume key bag.",
		 function );

		return( -1 );
	}
	if( *volume_key_bag != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume key bag value already set.",
		 function );

		return( -1 );
	}
	*volume_key_bag = memory_allocate_structure(
	                   libfsapfs_volume_key_bag_t );

	if( *volume_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume key bag.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_key_bag,
	     0,
	     sizeof( libfsapfs_volume_key_bag_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume key bag.",
		 function );

		memory_free(
		 *volume_key_bag );

		*volume_key_bag = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *volume_key_bag )->entries_array ),
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
	if( *volume_key_bag != NULL )
	{
		memory_free(
		 *volume_key_bag );

		*volume_key_bag = NULL;
	}
	return( -1 );
}

/* Frees a volume key bag
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_key_bag_free(
     libfsapfs_volume_key_bag_t **volume_key_bag,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_volume_key_bag_free";
	int result            = 1;

	if( volume_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume key bag.",
		 function );

		return( -1 );
	}
	if( *volume_key_bag != NULL )
	{
		if( libcdata_array_free(
		     &( ( *volume_key_bag )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_key_bag_entry_free,
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
		 *volume_key_bag );

		*volume_key_bag = NULL;
	}
	return( result );
}

/* Reads the volume key bag
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_key_bag_read_file_io_handle(
     libfsapfs_volume_key_bag_t *volume_key_bag,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t data_size,
     const uint8_t *volume_identifier,
     libcerror_error_t **error )
{
	libfsapfs_encryption_context_t *encryption_context = NULL;
	uint8_t *data                                      = NULL;
	uint8_t *encrypted_data                            = NULL;
	static char *function                              = "libfsapfs_volume_key_bag_read_file_io_handle";
	ssize_t read_count                                 = 0;

	if( volume_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume key bag.",
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
	if( io_handle->bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing bytes per sector.",
		 function );

		return( -1 );
	}
	if( data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid volume key bag size value out of bounds.",
		 function );

		return( -1 );
	}
	encrypted_data = (uint8_t *) memory_allocate(
	                              sizeof( uint8_t ) * data_size );

	if( encrypted_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create enrypted data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume key bag data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek encrypted data offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              encrypted_data,
	              (size_t) data_size,
	              error );

	if( read_count != (ssize_t) data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read encrypted data.",
		 function );

		goto on_error;
	}
	data = (uint8_t *) memory_allocate(
	                    sizeof( uint8_t ) * data_size );

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
	if( libfsapfs_encryption_context_initialize(
	     &encryption_context,
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
	if( libfsapfs_encryption_context_set_keys(
	     encryption_context,
	     volume_identifier,
	     16,
	     volume_identifier,
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
	if( libfsapfs_encryption_context_crypt(
	     encryption_context,
	     LIBCAES_CRYPT_MODE_DECRYPT,
	     encrypted_data,
	     data_size,
	     data,
	     data_size,
	     (uint64_t) ( file_offset / io_handle->bytes_per_sector ),
	     io_handle->bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
		 "%s: unable to decrypt data.",
		 function );

		goto on_error;
	}
	if( libfsapfs_encryption_context_free(
	     &encryption_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free encryption context.",
		 function );

		goto on_error;
	}
	memory_free(
	 encrypted_data );

	encrypted_data = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unencrypted volume key bag data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 (size_t) data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( libfsapfs_volume_key_bag_read_data(
	     volume_key_bag,
	     data,
	     (size_t) data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume key bag.",
		 function );

		goto on_error;
	}
	memory_free(
	 data );

	return( 1 );

on_error:
	if( encryption_context != NULL )
	{
		libfsapfs_encryption_context_free(
		 &encryption_context,
		 NULL );
	}
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	if( encrypted_data != NULL )
	{
		memory_free(
		 encrypted_data );
	}
	return( -1 );
}

/* Reads the volume key bag
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_key_bag_read_data(
     libfsapfs_volume_key_bag_t *volume_key_bag,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_key_bag_entry_t *bag_entry   = NULL;
	libfsapfs_key_bag_header_t *bag_header = NULL;
	static char *function                  = "libfsapfs_volume_key_bag_read_data";
	size_t alignment_padding_size          = 0;
	size_t data_offset                     = 0;
	uint64_t object_subtype                = 0;
	uint64_t object_type                   = 0;
	uint16_t bag_entry_index               = 0;
	int entry_index                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                   = 0;
#endif

	if( volume_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume key bag.",
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
		 "%s: volume key bag object data:\n",
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

	if( object_type != 0x72656373 )
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

	if( object_subtype != 0x00000000UL )
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
		 ( (fsapfs_object_t *) data )->transaction_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object transaction identifier\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object type\t\t\t\t: 0x%08" PRIx32 "\n",
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

	if( libfsapfs_key_bag_header_initialize(
	     &bag_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key bag header.",
		 function );

		goto on_error;
	}
	if( libfsapfs_key_bag_header_read_data(
	     bag_header,
	     &( data[ data_offset ] ),
	     data_size - data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read key bag header.",
		 function );

		goto on_error;
	}
	if( (size_t) bag_header->data_size > ( data_size - data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key bag header data size value out of bounds.",
		 function );

		goto on_error;
	}
/* TODO bounds check number of entries ? */

	data_offset += 16;

	for( bag_entry_index = 0;
	     bag_entry_index < bag_header->number_of_entries;
	     bag_entry_index++ )
	{
		if( libfsapfs_key_bag_entry_initialize(
		     &bag_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key bag entry: %" PRIu16 ".",
			 function,
			 bag_entry_index );

			goto on_error;
		}
		if( libfsapfs_key_bag_entry_read_data(
		     bag_entry,
		     &( data[ data_offset ] ),
		     data_size - data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read key bag entry: %" PRIu16 ".",
			 function,
			 bag_entry_index );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     volume_key_bag->entries_array,
		     &entry_index,
		     (intptr_t *) bag_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append bag entry: %" PRIu32 " to array.",
			 function,
			 bag_entry_index );

			goto on_error;
		}
		data_offset += bag_entry->size;

		bag_entry = NULL;

		alignment_padding_size = data_offset % 16;

		if( alignment_padding_size != 0 )
		{
			alignment_padding_size = 16 - alignment_padding_size;

			if( ( alignment_padding_size > data_size )
			 || ( data_offset > ( data_size - alignment_padding_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid data size value out of bounds.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: alignment padding data:\n",
				 function );
				libcnotify_print_data(
				 &( data[ data_offset ] ),
				 alignment_padding_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			data_offset += alignment_padding_size;
		}
	}
	if( libfsapfs_key_bag_header_free(
	     &bag_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free key bag header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( bag_entry != NULL )
	{
		libfsapfs_key_bag_entry_free(
		 &bag_entry,
		 NULL );
	}
	if( bag_header != NULL )
	{
		libfsapfs_key_bag_header_free(
		 &bag_header,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the volume key that can be unlocked
 * Returns 1 if successful, 0 if no such volume key or -1 on error
 */
int libfsapfs_volume_key_bag_get_volume_key(
     libfsapfs_volume_key_bag_t *volume_key_bag,
     const uint8_t *user_password,
     size_t user_password_length,
     const uint8_t *recovery_password,
     size_t recovery_password_length,
     uint8_t *key,
     size_t key_size,
     libcerror_error_t **error )
{
	libfsapfs_key_bag_entry_t *bag_entry             = NULL;
	libfsapfs_key_encrypted_key_t *key_encrypted_key = NULL;
	static char *function                            = "libfsapfs_volume_key_bag_get_volume_key";
	int entry_index                                  = 0;
	int number_of_entries                            = 0;
	int result                                       = 0;

	if( volume_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume key bag.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     volume_key_bag->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from array.",
		 function );

		goto on_error;
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     volume_key_bag->entries_array,
		     entry_index,
		     (intptr_t **) &bag_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from array.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( bag_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( bag_entry->type != 3 )
		{
			continue;
		}
		if( libfsapfs_key_encrypted_key_initialize(
		     &key_encrypted_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key encrypted key.",
			 function );

			goto on_error;
		}
		if( libfsapfs_key_encrypted_key_read_data(
		     key_encrypted_key,
		     bag_entry->data,
		     (size_t) bag_entry->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read key encrypted key.",
			 function );

			goto on_error;
		}
		if( user_password != NULL )
		{
			result = libfsapfs_key_encrypted_key_unlock_with_password(
			          key_encrypted_key,
			          user_password,
			          user_password_length,
			          key,
			          key_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to unlock key encrypted key with user password.",
				 function );

				goto on_error;
			}
		}
		if( ( result == 0 )
		 && ( recovery_password != NULL ) )
		{
			result = libfsapfs_key_encrypted_key_unlock_with_password(
			          key_encrypted_key,
			          recovery_password,
			          recovery_password_length,
			          key,
			          key_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to unlock key encrypted key with recovery password.",
				 function );

				goto on_error;
			}
		}
		if( libfsapfs_key_encrypted_key_free(
		     &key_encrypted_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key encrypted key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
		}
	}
	return( result );

on_error:
	if( key_encrypted_key != NULL )
	{
		libfsapfs_key_encrypted_key_free(
		 &key_encrypted_key,
		 NULL );
	}
	return( -1 );
}

