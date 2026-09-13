/*
 * The key encrypted key (KEK) functions
 *
 * Copyright (C) 2018-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsapfs_debug.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_key_bag_value.h"
#include "libfsapfs_key_encrypted_key.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfguid.h"
#include "libfsapfs_libhmac.h"
#include "libfsapfs_password.h"

#include "fsapfs_key_bag.h"

const uint8_t libfsapfs_key_encrypted_key_wrapped_kek_initialization_vector[ 8 ] = {
       0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6 };

/* Creates a key encrypted key
 * Make sure the value key_encrypted_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_encrypted_key_initialize(
     libfsapfs_key_encrypted_key_t **key_encrypted_key,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_encrypted_key_initialize";

	if( key_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key encrypted key.",
		 function );

		return( -1 );
	}
	if( *key_encrypted_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key encrypted key value already set.",
		 function );

		return( -1 );
	}
	*key_encrypted_key = memory_allocate_structure(
	                      libfsapfs_key_encrypted_key_t );

	if( *key_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key encrypted key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_encrypted_key,
	     0,
	     sizeof( libfsapfs_key_encrypted_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key encrypted key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_encrypted_key != NULL )
	{
		memory_free(
		 *key_encrypted_key );

		*key_encrypted_key = NULL;
	}
	return( -1 );
}

/* Frees key encrypted key
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_encrypted_key_free(
     libfsapfs_key_encrypted_key_t **key_encrypted_key,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_key_encrypted_key_free";

	if( key_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key encrypted key.",
		 function );

		return( -1 );
	}
	if( *key_encrypted_key != NULL )
	{
		memory_free(
		 *key_encrypted_key );

		*key_encrypted_key = NULL;
	}
	return( 1 );
}

/* Reads the key encrypted key
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_key_encrypted_key_read_data(
     libfsapfs_key_encrypted_key_t *key_encrypted_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	fsapfs_key_bag_kek_metadata_header_t *kek_metadata_header = NULL;
	libfsapfs_key_bag_value_t *packed_value                   = NULL;
	const uint8_t *wrapped_kek_object_data                    = NULL;
	static char *function                                     = "libfsapfs_key_encrypted_key_read_data";
	size_t data_offset                                        = 0;
	size_t object_value_data_end                              = 0;
	size_t wrapped_kek_object_header_size                     = 0;
	uint16_t kek_metadata_value_data_size                     = 0;
	uint16_t wrapped_kek_object_data_size                     = 0;
	uint16_t remaining_value_data_size                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                                      = 0;
#endif

	if( key_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key encrypted key.",
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
	if( data_size > (size_t) SSIZE_MAX )
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
		 "%s: KEK object data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfsapfs_key_bag_value_initialize(
	     &packed_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create packed value.",
		 function );

		goto on_error;
	}
	if( libfsapfs_key_bag_value_read_data(
	     packed_value,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read KEK object packed value at offset: %" PRIzd ".",
		 function,
		 data_offset );

		goto on_error;
	}
	data_offset += 2 + packed_value->extended_size;

	if( packed_value->data_size > ( data_size - data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid KEK object value data size value out of bounds.",
		 function );

		goto on_error;
	}
	object_value_data_end = data_offset + packed_value->data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object value data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 (size_t) packed_value->data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( packed_value->tag != 0x30 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported KEK object value tag: 0x02%" PRIx8 ".",
		 function,
		 packed_value->tag );

		goto on_error;
	}
	/* The attributes are bound by the object value data, not by the data,
	 * so that trailing data is not parsed as an attribute.
	 */
	while( data_offset < object_value_data_end )
	{
		if( ( object_value_data_end - data_offset ) < 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid KEK attribute value data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( data[ data_offset ] == 0 )
		 && ( data[ data_offset + 1 ] == 0 ) )
		{
			break;
		}
		if( libfsapfs_key_bag_value_read_data(
		     packed_value,
		     &( data[ data_offset ] ),
		     data_size - data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read KEK attribute packed value at offset: %" PRIzd ".",
			 function,
			 data_offset );

			goto on_error;
		}
		data_offset += 2 + packed_value->extended_size;

		if( packed_value->data_size > ( object_value_data_end - data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid KEK attribute value data size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: KEK attribute value data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) packed_value->data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		switch( packed_value->tag )
		{
			case 0x81:
				if( packed_value->data_size != 32 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported HMAC attribute value data size: %" PRIu16 ".",
					 function,
					 packed_value->data_size );

					goto on_error;
				}
				if( memory_copy(
				     key_encrypted_key->hmac,
				     &( data[ data_offset ] ),
				     32 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy HMAC.",
					 function );

					goto on_error;
				}
				break;

			case 0x82:
				if( packed_value->data_size != 8 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported KEK attribute value data size: %" PRIu16 ".",
					 function,
					 packed_value->data_size );

					goto on_error;
				}
				break;

			case 0xa3:
				if( packed_value->data_size == 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported wrapped KEK packed object attribute value data size: %" PRIu16 ".",
					 function,
					 packed_value->data_size );

					goto on_error;
				}
				wrapped_kek_object_header_size = 2 + packed_value->extended_size;

				wrapped_kek_object_data      = &( data[ data_offset - wrapped_kek_object_header_size ] );
				wrapped_kek_object_data_size = (uint16_t) ( wrapped_kek_object_header_size + packed_value->data_size );
				break;

			default:
				break;
		}
		data_offset += packed_value->data_size;
	}
	if( wrapped_kek_object_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing wrapped KEK packed object.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: wrapped KEK packed object data:\n",
		 function );
		libcnotify_print_data(
		 wrapped_kek_object_data,
		 wrapped_kek_object_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( libfsapfs_key_bag_value_read_data(
	     packed_value,
	     wrapped_kek_object_data,
	     wrapped_kek_object_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read KEK packed value at offset: %" PRIzd ".",
		 function,
		 data_offset );

		goto on_error;
	}
	data_offset = 2 + packed_value->extended_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: wrapped KEK object value data:\n",
		 function );
		libcnotify_print_data(
		 &( wrapped_kek_object_data[ data_offset ] ),
		 (size_t) packed_value->data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( packed_value->tag != 0xa3 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported wrapped KEK object value tag: 0x02%" PRIx8 ".",
		 function,
		 packed_value->tag );

		goto on_error;
	}
	if( packed_value->data_size > ( wrapped_kek_object_data_size - 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid wrapped KEK object value data size value out of bounds.",
		 function );

		goto on_error;
	}
	while( data_offset < (size_t) wrapped_kek_object_data_size )
	{
		if( ( wrapped_kek_object_data_size - data_offset ) < 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid wrapped KEK attribute value data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( wrapped_kek_object_data[ data_offset ] == 0 )
		 && ( wrapped_kek_object_data[ data_offset + 1 ] == 0 ) )
		{
			break;
		}
		if( libfsapfs_key_bag_value_read_data(
		     packed_value,
		     &( wrapped_kek_object_data[ data_offset ] ),
		     wrapped_kek_object_data_size - data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read wrapped KEK packed value at offset: %" PRIzd ".",
			 function,
			 data_offset );

			goto on_error;
		}
		data_offset += 2 + packed_value->extended_size;

		if( packed_value->data_size > ( (size_t) wrapped_kek_object_data_size - data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid wrapped KEK packed value data size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: wrapped KEK attribute value data:\n",
			 function );
			libcnotify_print_data(
			 &( wrapped_kek_object_data[ data_offset ] ),
			 (size_t) packed_value->data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		switch( packed_value->tag )
		{
			case 0x81:
				if( packed_value->data_size != 16 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported identifier wrapped KEK attribute value data size: %" PRIu16 ".",
					 function,
					 packed_value->data_size );

					goto on_error;
				}
				if( memory_copy(
				     key_encrypted_key->identifier,
				     &( wrapped_kek_object_data[ data_offset ] ),
				     16 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy identifier.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( libfsapfs_debug_print_guid_value(
					     function,
					     "identifier\t\t\t",
					     &( wrapped_kek_object_data[ data_offset ] ),
					     16,
					     LIBFGUID_ENDIAN_BIG,
					     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print UUID value.",
						 function );

						goto on_error;
					}
					libcnotify_printf(
					 "\n" );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				break;

			case 0x82:
				/* The metadata can be 8 or 22 bytes in size
				 */
				if( ( packed_value->data_size != 8 )
				 && ( packed_value->data_size != 22 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported wrapped KEK metadata attribute value data size: %" PRIu16 ".",
					 function,
					 packed_value->data_size );

					goto on_error;
				}
				kek_metadata_value_data_size = packed_value->data_size;
				kek_metadata_header          = (fsapfs_key_bag_kek_metadata_header_t *) &( wrapped_kek_object_data[ data_offset ] );

				byte_stream_copy_to_uint32_little_endian(
				 kek_metadata_header->flags,
				 key_encrypted_key->flags );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: flags\t\t\t\t: 0x%08" PRIx32 "\n",
					 function,
					 key_encrypted_key->flags );

					byte_stream_copy_to_uint16_little_endian(
					 kek_metadata_header->unknown1,
					 value_16bit );
					libcnotify_printf(
					 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
					 function,
					 value_16bit );

					if( packed_value->data_size == 8 )
					{
						byte_stream_copy_to_uint16_little_endian(
						 &( wrapped_kek_object_data[ data_offset + 6 ] ),
						 value_16bit );
						libcnotify_printf(
						 "%s: identifier\t\t\t: 0x%04" PRIx16 "\n",
						 function,
						 value_16bit );
					}
					else if( packed_value->data_size == 22 )
					{
						if( libfsapfs_debug_print_guid_value(
						     function,
						     "identifier\t\t\t",
						     &( wrapped_kek_object_data[ data_offset + 6 ] ),
						     16,
						     LIBFGUID_ENDIAN_BIG,
						     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
							 "%s: unable to print UUID value.",
							 function );

							goto on_error;
						}
					}
					libcnotify_printf(
					 "\n" );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				/* In the 22-byte variant the first 4 bytes are not the
				 * encryption method. The 40-byte wrapped KEK corresponds with
				 * AES-256.
				 */
				if( packed_value->data_size == 22 )
				{
					key_encrypted_key->flags = 0;
				}
				break;

			case 0x83:
				if( packed_value->data_size != 40 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported wrapped KEK attribute value data size: %" PRIu16 ".",
					 function,
					 packed_value->data_size );

					goto on_error;
				}
				if( memory_copy(
				     key_encrypted_key->wrapped_kek,
				     &( wrapped_kek_object_data[ data_offset ] ),
				     40 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy wrapped KEK.",
					 function );

					goto on_error;
				}
				break;

			case 0x84:
				/* A password-protected KEK entry stores a <= 8-byte PBKDF2
				 * iteration count here. A macOS 26 container key bag VEK
				 * entry, which is unwrapped with a key instead of a password,
				 * carries a 16-byte value that is not an iteration count. The
				 * 22-byte KEK metadata attribute distinguishes the two, so
				 * that a corrupt legacy entry is still rejected.
				 */
				if( ( packed_value->data_size == 0 )
				 || ( ( packed_value->data_size > 8 )
				  &&  ( kek_metadata_value_data_size != 22 ) ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported number of wrapped KEK iteration attribute value data size: %" PRIu16 ".",
					 function,
					 packed_value->data_size );

					goto on_error;
				}
				if( packed_value->data_size > 8 )
				{
					break;
				}
				key_encrypted_key->number_of_iterations = 0;

				remaining_value_data_size = packed_value->data_size;

				while( remaining_value_data_size > 0 )
				{
					key_encrypted_key->number_of_iterations <<= 8;
					key_encrypted_key->number_of_iterations  |= wrapped_kek_object_data[ data_offset++ ];

					remaining_value_data_size--;
				}
				data_offset -= packed_value->data_size;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: number of iterations\t\t: %" PRIu64 "\n",
					 function,
					 key_encrypted_key->number_of_iterations );

					libcnotify_printf(
					 "\n" );
				}
#endif
				break;

			case 0x85:
				/* A password-protected KEK entry stores a 16-byte PBKDF2 salt
				 * here. A macOS 26 container key bag VEK entry carries an
				 * unrelated 3-byte value that is not a salt. The 22-byte KEK
				 * metadata attribute distinguishes the two, so that a corrupt
				 * legacy entry is still rejected.
				 */
				if( ( packed_value->data_size != 16 )
				 && ( kek_metadata_value_data_size != 22 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported wrapped KEK salt attribute value data size: %" PRIu16 ".",
					 function,
					 packed_value->data_size );

					goto on_error;
				}
				if( packed_value->data_size != 16 )
				{
					break;
				}
				if( memory_copy(
				     key_encrypted_key->salt,
				     &( wrapped_kek_object_data[ data_offset ] ),
				     16 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy salt.",
					 function );

					goto on_error;
				}
				break;

			default:
				break;
		}
		data_offset += packed_value->data_size;
	}
	if( libfsapfs_key_bag_value_free(
	     &packed_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free packed value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( packed_value != NULL )
	{
		libfsapfs_key_bag_value_free(
		 &packed_value,
		 NULL );
	}
	return( -1 );
}

/* Unlocks the key encrypted key with a key
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfsapfs_key_encrypted_key_unlock_with_key(
     libfsapfs_key_encrypted_key_t *key_encrypted_key,
     const uint8_t *key,
     size_t key_size,
     uint8_t *unlocked_key,
     size_t unlocked_key_size,
     libcerror_error_t **error )
{
	uint8_t hash_buffer[ LIBHMAC_SHA256_HASH_SIZE ];
	uint8_t wrapped_kek[ 40 ];

	static char *function     = "libfsapfs_key_encrypted_key_unlock_with_key";
	size_t used_kek_data_size = 0;
	size_t used_key_size      = 0;
	int result                = 0;

	if( key_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key encrypted key.",
		 function );

		return( -1 );
	}
	if( key_encrypted_key->flags == 0 )
	{
		used_kek_data_size = 40;
		used_key_size      = 32;
	}
	else if( key_encrypted_key->flags == 2 )
	{
		used_kek_data_size = 24;
		used_key_size      = 16;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encryption method.",
		 function );

		return( -1 );
	}
	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_size != 256 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key size value out of bounds.",
		 function );

		return( -1 );
	}
	if( unlocked_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unlocked key.",
		 function );

		return( -1 );
	}
	if( unlocked_key_size != 256 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid unlocked key size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key:\n",
		 function );
		libcnotify_print_data(
		 key,
		 32,
		 0 );
	}
#endif
	if( libfsapfs_encryption_aes_key_unwrap(
	     key,
	     used_key_size * 8,
	     key_encrypted_key->wrapped_kek,
	     used_kek_data_size,
	     wrapped_kek,
	     used_kek_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to unwrap wrapped KEK with key.",
		 function );

		goto on_error;
	}
	if( memory_compare(
	     wrapped_kek,
	     libfsapfs_key_encrypted_key_wrapped_kek_initialization_vector,
	     8 ) == 0 )
	{
		if( memory_copy(
		     unlocked_key,
		     &( wrapped_kek[ 8 ] ),
		     used_key_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy unlocked key.",
			 function );

			goto on_error;
		}
		if( key_encrypted_key->flags == 2 )
		{
			if( memory_copy(
			     &( unlocked_key[ 16 ] ),
			     key_encrypted_key->identifier,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy identifier to unlocked key.",
				 function );

				goto on_error;
			}
			if( libhmac_sha256_calculate(
			     unlocked_key,
			     32,
			     hash_buffer,
			     LIBHMAC_SHA256_HASH_SIZE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to calculate SHA-256 of tweak key data.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     &( unlocked_key[ 16 ] ),
			     hash_buffer,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy SHA-256 hash to unlocked key.",
				 function );

				goto on_error;
			}
			memory_set(
			 hash_buffer,
			 0,
			 LIBHMAC_SHA256_HASH_SIZE );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unlocked key:\n",
			 function );
			libcnotify_print_data(
			 unlocked_key,
			 unlocked_key_size / 8,
			 0 );
		}
#endif
		result = 1;
	}
	memory_set(
	 wrapped_kek,
	 0,
	 40 );

	return( result );

on_error:
	memory_set(
	 wrapped_kek,
	 0,
	 40 );

	memory_set(
	 hash_buffer,
	 0,
	 LIBHMAC_SHA256_HASH_SIZE );

	return( -1 );
}

/* Unlocks the key encrypted key with a password
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfsapfs_key_encrypted_key_unlock_with_password(
     libfsapfs_key_encrypted_key_t *key_encrypted_key,
     const uint8_t *password,
     size_t password_length,
     uint8_t *unlocked_key,
     size_t unlocked_key_size,
     libcerror_error_t **error )
{
	uint8_t password_key[ 32 ];
	uint8_t wrapped_kek[ 40 ];

	static char *function     = "libfsapfs_key_encrypted_key_unlock_with_password";
	size_t password_key_size  = 0;
	size_t used_kek_data_size = 0;
	int result                = 0;

	if( key_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key encrypted key.",
		 function );

		return( -1 );
	}
	if( ( key_encrypted_key->flags == 0 )
	 || ( key_encrypted_key->flags == 16 ) )
	{
		password_key_size  = 32;
		used_kek_data_size = 40;
	}
	else if( key_encrypted_key->flags == 2 )
	{
		password_key_size  = 16;
		used_kek_data_size = 24;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encryption method.",
		 function );

		return( -1 );
	}
	if( unlocked_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unlocked key.",
		 function );

		return( -1 );
	}
	if( unlocked_key_size != 256 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsapfs_password_pbkdf2(
	     password,
	     password_length,
	     key_encrypted_key->salt,
	     16,
	     (uint32_t) key_encrypted_key->number_of_iterations,
	     password_key,
	     password_key_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine password key.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: password key:\n",
		 function );
		libcnotify_print_data(
		 password_key,
		 password_key_size,
		 0 );
	}
#endif
	if( libfsapfs_encryption_aes_key_unwrap(
	     password_key,
	     password_key_size * 8,
	     key_encrypted_key->wrapped_kek,
	     used_kek_data_size,
	     wrapped_kek,
	     used_kek_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to unwrap wrapped KEK with password.",
		 function );

		goto on_error;
	}
	memory_set(
	 password_key,
	 0,
	 32 );

	if( memory_compare(
	     wrapped_kek,
	     libfsapfs_key_encrypted_key_wrapped_kek_initialization_vector,
	     8 ) == 0 )
	{
		if( memory_copy(
		     unlocked_key,
		     &( wrapped_kek[ 8 ] ),
		     password_key_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy unlocked key.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unlocked key:\n",
			 function );
			libcnotify_print_data(
			 unlocked_key,
			 unlocked_key_size / 8,
			 0 );
		}
#endif
		result = 1;
	}
	memory_set(
	 wrapped_kek,
	 0,
	 40 );

	return( result );

on_error:
	memory_set(
	 wrapped_kek,
	 0,
	 40 );

	memory_set(
	 password_key,
	 0,
	 32 );

	return( -1 );
}

