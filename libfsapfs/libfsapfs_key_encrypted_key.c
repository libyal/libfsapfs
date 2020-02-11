/*
 * The key encrypted key (KEK) functions
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

#include "libfsapfs_debug.h"
#include "libfsapfs_encryption_context.h"
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
	fsapfs_key_bag_kek_metadata_t *kek_metadata = NULL;
	const uint8_t *wrapped_kek_object_data      = NULL;
	static char *function                       = "libfsapfs_key_encrypted_key_read_data";
	size_t data_offset                          = 0;
	uint16_t value_data_size                    = 0;
	uint16_t wrapped_kek_object_data_size       = 0;
	uint8_t byte_value                          = 0;
	uint8_t value_tag                           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                        = 0;
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
	if( ( data_size < 2 )
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
	value_tag  = data[ data_offset++ ];
	byte_value = data[ data_offset++ ];

	if( ( ( byte_value & 0x80 ) != 0 )
	 && ( byte_value != 0x81 )
	 && ( byte_value != 0x82 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported object extended value data size: 0x02%" PRIx8 ".",
		 function,
		 byte_value );

		return( -1 );
	}
	if( ( byte_value & 0x80 ) == 0 )
	{
		value_data_size = (uint16_t) byte_value;
	}
	else if( byte_value == 0x81 )
	{
		value_data_size = (uint16_t) data[ data_offset++ ];
	}
	else if( byte_value == 0x82 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 &( data[ data_offset ] ),
		 value_data_size );

		data_offset += 2;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: KEK packed object data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object value tag\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 value_tag );

		libcnotify_printf(
		 "%s: object value data size\t\t: %" PRIu16 "\n",
		 function,
		 value_data_size );

		libcnotify_printf(
		 "%s: object value data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 (size_t) value_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( value_tag != 0x30 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported object value tag: 0x02%" PRIx8 ".",
		 function,
		 value_tag );

		return( -1 );
	}
	if( value_data_size > ( data_size - 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid object value data size value out of bounds.",
		 function );

		return( -1 );
	}
	while( data_offset < data_size )
	{
		value_tag  = data[ data_offset++ ];
		byte_value = data[ data_offset++ ];

		if( ( ( byte_value & 0x80 ) != 0 )
		 && ( byte_value != 0x81 )
		 && ( byte_value != 0x82 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported attribute extended value data size: 0x02%" PRIx8 ".",
			 function,
			 byte_value );

			return( -1 );
		}
		if( ( byte_value & 0x80 ) == 0 )
		{
			value_data_size = (uint16_t) byte_value;
		}
		else if( byte_value == 0x81 )
		{
			value_data_size = (uint16_t) data[ data_offset++ ];
		}
		else if( byte_value == 0x82 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( data[ data_offset ] ),
			 value_data_size );

			data_offset += 2;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: attribute value tag\t\t: 0x%02" PRIx8 "\n",
			 function,
			 value_tag );

			libcnotify_printf(
			 "%s: attribute value data size\t: %" PRIu16 "\n",
			 function,
			 value_data_size );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( ( value_tag == 0 )
		 && ( value_data_size == 0 ) )
		{
			break;
		}
		if( ( data_offset >= data_size )
		 || ( value_data_size > ( data_size - data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data size value out of bounds.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: attribute value data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 (size_t) value_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		switch( value_tag )
		{
			case 0x81:
				if( value_data_size != 32 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported HMAC attribute value data size: %" PRIu16 ".",
					 function,
					 value_data_size );

					return( -1 );
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

					return( -1 );
				}
				break;

			case 0x82:
				if( value_data_size != 8 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported attribute value data size: %" PRIu16 ".",
					 function,
					 value_data_size );

					return( -1 );
				}
				break;

			case 0xa3:
				if( value_data_size == 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported wrapped KEK packed object attribute value data size: %" PRIu16 ".",
					 function,
					 value_data_size );

					return( -1 );
				}
				wrapped_kek_object_data      = &( data[ data_offset - 2 ] );
				wrapped_kek_object_data_size = value_data_size + 2;

				break;

			default:
				break;
		}
		data_offset += value_data_size;
	}
	if( wrapped_kek_object_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing wrapped KEK packed object.",
		 function );

		return( -1 );
	}
	data_offset = 0;

	value_tag  = wrapped_kek_object_data[ data_offset++ ];
	byte_value = wrapped_kek_object_data[ data_offset++ ];

	if( ( ( byte_value & 0x80 ) != 0 )
	 && ( byte_value != 0x82 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported object extended value data size: 0x02%" PRIx8 ".",
		 function,
		 byte_value );

		return( -1 );
	}
	if( ( byte_value & 0x80 ) == 0 )
	{
		value_data_size = (uint16_t) byte_value;
	}
	else if( byte_value == 0x82 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 &( wrapped_kek_object_data[ data_offset ] ),
		 value_data_size );

		data_offset += 2;
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
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: object value tag\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 value_tag );

		libcnotify_printf(
		 "%s: object value data size\t\t: %" PRIu16 "\n",
		 function,
		 value_data_size );

		libcnotify_printf(
		 "%s: object value data:\n",
		 function );
		libcnotify_print_data(
		 &( wrapped_kek_object_data[ data_offset ] ),
		 (size_t) value_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( value_tag != 0xa3 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported object value tag: 0x02%" PRIx8 ".",
		 function,
		 value_tag );

		return( -1 );
	}
	if( value_data_size > ( wrapped_kek_object_data_size - 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid object value data size value out of bounds.",
		 function );

		return( -1 );
	}
	while( data_offset < wrapped_kek_object_data_size )
	{
		value_tag  = wrapped_kek_object_data[ data_offset++ ];
		byte_value = wrapped_kek_object_data[ data_offset++ ];

		if( ( ( byte_value & 0x80 ) != 0 )
		 && ( byte_value != 0x82 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported attribute extended value data size: 0x02%" PRIx8 ".",
			 function,
			 byte_value );

			return( -1 );
		}
		if( ( byte_value & 0x80 ) == 0 )
		{
			value_data_size = (uint16_t) byte_value;
		}
		else if( byte_value == 0x82 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( wrapped_kek_object_data[ data_offset ] ),
			 value_data_size );

			data_offset += 2;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: attribute value tag\t\t: 0x%02" PRIx8 "\n",
			 function,
			 value_tag );

			libcnotify_printf(
			 "%s: attribute value data size\t: %" PRIu16 "\n",
			 function,
			 value_data_size );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( ( value_tag == 0 )
		 && ( value_data_size == 0 ) )
		{
			break;
		}
		if( ( data_offset >= wrapped_kek_object_data_size )
		 || ( value_data_size > ( wrapped_kek_object_data_size - data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data size value out of bounds.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{

			libcnotify_printf(
			 "%s: attribute value data:\n",
			 function );
			libcnotify_print_data(
			 &( wrapped_kek_object_data[ data_offset ] ),
			 (size_t) value_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		switch( value_tag )
		{
			case 0x81:
				if( value_data_size != 16 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported identifier attribute value data size: %" PRIu16 ".",
					 function,
					 value_data_size );

					return( -1 );
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

					return( -1 );
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

						return( -1 );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				break;

			case 0x82:
				if( value_data_size != 8 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported KEK metadata attribute value data size: %" PRIu16 ".",
					 function,
					 value_data_size );

					return( -1 );
				}
				kek_metadata = (fsapfs_key_bag_kek_metadata_t *) &( wrapped_kek_object_data[ data_offset ] );

				byte_stream_copy_to_uint32_little_endian(
				 kek_metadata->encryption_method,
				 key_encrypted_key->encryption_method );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: encryption method\t\t: %" PRIu32 "\n",
					 function,
					 key_encrypted_key->encryption_method );

					byte_stream_copy_to_uint16_little_endian(
					 kek_metadata->unknown1,
					 value_16bit );
					libcnotify_printf(
					 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
					 function,
					 value_16bit );

					libcnotify_printf(
					 "%s: unknown2\t\t\t\t: 0x%02" PRIx8 "\n",
					 function,
					 kek_metadata->unknown2 );

					libcnotify_printf(
					 "%s: unknown3\t\t\t\t: 0x%02" PRIx8 "\n",
					 function,
					 kek_metadata->unknown3 );

					libcnotify_printf(
					 "\n" );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				break;

			case 0x83:
				if( value_data_size != 40 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported wrapped KEK attribute value data size: %" PRIu16 ".",
					 function,
					 value_data_size );

					return( -1 );
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

					return( -1 );
				}
				break;

			case 0x84:
				if( ( value_data_size == 0 )
				 || ( value_data_size > 8 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported number of iteration attribute value data size: %" PRIu16 ".",
					 function,
					 value_data_size );

					return( -1 );
				}
				key_encrypted_key->number_of_iterations = 0;

				while( value_data_size > 0 )
				{
					key_encrypted_key->number_of_iterations <<= 8;
					key_encrypted_key->number_of_iterations  |= wrapped_kek_object_data[ data_offset++ ];

					value_data_size--;
				}

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: number of iterations\t\t: %" PRIu32 "\n",
					 function,
					 key_encrypted_key->number_of_iterations );

					libcnotify_printf(
					 "\n" );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				break;

			case 0x85:
				if( value_data_size != 16 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported salt attribute value data size: %" PRIu16 ".",
					 function,
					 value_data_size );

					return( -1 );
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

					return( -1 );
				}
				break;

			default:
				break;
		}
		data_offset += value_data_size;
	}
	return( 1 );
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
	if( key_encrypted_key->encryption_method == 0 )
	{
		used_kek_data_size = 40;
		used_key_size      = 32;
	}
	else if( key_encrypted_key->encryption_method == 2 )
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
		if( key_encrypted_key->encryption_method == 2 )
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
	if( ( key_encrypted_key->encryption_method == 0 )
	 || ( key_encrypted_key->encryption_method == 16 ) )
	{
		password_key_size  = 32;
		used_kek_data_size = 40;
	}
	else if( key_encrypted_key->encryption_method == 2 )
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
	     key_encrypted_key->number_of_iterations,
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

