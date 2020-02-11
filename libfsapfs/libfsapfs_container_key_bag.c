/*
 * The container key bag functions
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

#include "libfsapfs_checksum.h"
#include "libfsapfs_container_key_bag.h"
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

#include "fsapfs_key_bag.h"
#include "fsapfs_object.h"

/* Creates a container key bag
 * Make sure the value container_key_bag is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_key_bag_initialize(
     libfsapfs_container_key_bag_t **container_key_bag,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_key_bag_initialize";

	if( container_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container key bag.",
		 function );

		return( -1 );
	}
	if( *container_key_bag != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid container key bag value already set.",
		 function );

		return( -1 );
	}
	*container_key_bag = memory_allocate_structure(
	                      libfsapfs_container_key_bag_t );

	if( *container_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create container key bag.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *container_key_bag,
	     0,
	     sizeof( libfsapfs_container_key_bag_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear container key bag.",
		 function );

		memory_free(
		 *container_key_bag );

		*container_key_bag = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *container_key_bag )->entries_array ),
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
	if( *container_key_bag != NULL )
	{
		memory_free(
		 *container_key_bag );

		*container_key_bag = NULL;
	}
	return( -1 );
}

/* Frees a container key bag
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_container_key_bag_free(
     libfsapfs_container_key_bag_t **container_key_bag,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_container_key_bag_free";
	int result            = 1;

	if( container_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container key bag.",
		 function );

		return( -1 );
	}
	if( *container_key_bag != NULL )
	{
		if( libcdata_array_free(
		     &( ( *container_key_bag )->entries_array ),
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
		 *container_key_bag );

		*container_key_bag = NULL;
	}
	return( result );
}

/* Reads the container key bag
 * Returns 1 if successful, 0 if the object type does not match or -1 on error
 */
int libfsapfs_container_key_bag_read_file_io_handle(
     libfsapfs_container_key_bag_t *container_key_bag,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t data_size,
     const uint8_t *container_identifier,
     libcerror_error_t **error )
{
	libfsapfs_encryption_context_t *encryption_context = NULL;
	uint8_t *data                                      = NULL;
	uint8_t *encrypted_data                            = NULL;
	static char *function                              = "libfsapfs_container_key_bag_read_file_io_handle";
	ssize_t read_count                                 = 0;
	int result                                         = 0;

	if( container_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container key bag.",
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
		 "%s: invalid container key bag size value out of bounds.",
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
		 "%s: reading container key bag data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
	     container_identifier,
	     16,
	     container_identifier,
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
		 "%s: unencrypted container key bag data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 (size_t) data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	result = libfsapfs_container_key_bag_read_data(
	          container_key_bag,
	          data,
	          (size_t) data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read container key bag.",
		 function );

		goto on_error;
	}
	memory_free(
	 data );

	return( result );

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

/* Reads the container key bag
 * Returns 1 if successful, 0 if the object type does not match or -1 on error
 */
int libfsapfs_container_key_bag_read_data(
     libfsapfs_container_key_bag_t *container_key_bag,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfsapfs_key_bag_entry_t *bag_entry   = NULL;
	libfsapfs_key_bag_header_t *bag_header = NULL;
	static char *function                  = "libfsapfs_container_key_bag_read_data";
	size_t alignment_padding_size          = 0;
	size_t data_offset                     = 0;
	uint64_t calculated_checksum           = 0;
	uint64_t object_subtype                = 0;
	uint64_t object_type                   = 0;
	uint64_t stored_checksum               = 0;
	uint16_t bag_entry_index               = 0;
	int entry_index                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                   = 0;
#endif

	if( container_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container key bag.",
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
		 "%s: container key bag object data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_object_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 ( (fsapfs_object_t *) data )->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->type,
	 object_type );

	if( object_type != 0x6b657973UL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid object type: 0x%08" PRIx32 "\n.",
			 function,
			 object_type );
		}
#endif
		return( 0 );
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
		libcnotify_printf(
		 "%s: object checksum\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 stored_checksum );

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
		 object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_subtype );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfsapfs_checksum_calculate_fletcher64(
	     &calculated_checksum,
	     &( data[ 8 ] ),
	     data_size - 8,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate Fletcher-64 checksum.",
		 function );

		return( -1 );
	}
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in checksum ( 0x%08" PRIx64 " != 0x%08" PRIx64 " ).\n",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
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
		     container_key_bag->entries_array,
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

/* Retrieves the volume key extent for a specific volume
 * Returns 1 if successful, 0 if no such volume or -1 on error
 */
int libfsapfs_container_key_bag_get_volume_key_bag_extent_by_identifier(
     libfsapfs_container_key_bag_t *container_key_bag,
     const uint8_t *volume_identifier,
     uint64_t *volume_key_bag_block_number,
     uint64_t *volume_key_bag_number_of_blocks,
     libcerror_error_t **error )
{
	libfsapfs_key_bag_entry_t *bag_entry = NULL;
	static char *function                = "libfsapfs_container_key_bag_get_volume_key_bag_extent_by_identifier";
	uint64_t block_number                = 0;
	uint64_t number_of_blocks            = 0;
	int entry_index                      = 0;
	int number_of_entries                = 0;

	if( container_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container key bag.",
		 function );

		return( -1 );
	}
	if( volume_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume identifier.",
		 function );

		return( -1 );
	}
	if( volume_key_bag_block_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume key bag block number.",
		 function );

		return( -1 );
	}
	if( volume_key_bag_number_of_blocks == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume key bag number of blocks.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     container_key_bag->entries_array,
	     &number_of_entries,
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
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     container_key_bag->entries_array,
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

			return( -1 );
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

			return( -1 );
		}
		if( bag_entry->type != 3 )
		{
			continue;
		}
		if( memory_compare(
		     bag_entry->identifier,
		     volume_identifier,
		     16 ) != 0 )
		{
			continue;
		}
		if( bag_entry->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid entry: %d - missing data.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( bag_entry->data_size != 16 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid entry: %d - unsupported data size.",
			 function,
			 entry_index );

			return( -1 );
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_key_bag_extent_t * ) bag_entry->data )->block_number,
		 block_number );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_key_bag_extent_t * ) bag_entry->data )->number_of_blocks,
		 number_of_blocks );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: key bag extent block number\t: %" PRIu64 "\n",
			 function,
			 block_number );

			libcnotify_printf(
			 "%s: key bag extent number of blocks\t: %" PRIu64 "\n",
			 function,
			 number_of_blocks );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		*volume_key_bag_block_number     = block_number;
		*volume_key_bag_number_of_blocks = number_of_blocks;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves the volume master key for a specific volume
 * Returns 1 if successful, 0 if no such volume or -1 on error
 */
int libfsapfs_container_key_bag_get_volume_master_key_by_identifier(
     libfsapfs_container_key_bag_t *container_key_bag,
     const uint8_t *volume_identifier,
     const uint8_t *volume_key,
     size_t volume_key_size,
     uint8_t *key,
     size_t key_size,
     libcerror_error_t **error )
{
	libfsapfs_key_bag_entry_t *bag_entry             = NULL;
	libfsapfs_key_encrypted_key_t *key_encrypted_key = NULL;
	static char *function                            = "libfsapfs_container_key_bag_get_volume_master_key_by_identifier";
	int entry_index                                  = 0;
	int number_of_entries                            = 0;
	int result                                       = 0;

	if( container_key_bag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid container key bag.",
		 function );

		return( -1 );
	}
	if( volume_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume identifier.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     container_key_bag->entries_array,
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
		     container_key_bag->entries_array,
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
		if( bag_entry->type != 2 )
		{
			continue;
		}
		if( memory_compare(
		     bag_entry->identifier,
		     volume_identifier,
		     16 ) != 0 )
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
		result = libfsapfs_key_encrypted_key_unlock_with_key(
		         key_encrypted_key,
		         volume_key,
		         volume_key_size,
		         key,
		         key_size,
		         error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to unlock key encrypted key with volume key.",
			 function );

			goto on_error;
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

