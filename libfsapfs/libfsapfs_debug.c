/*
 * Debug functions
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
#include <memory.h>
#include <types.h>

#include "libfsapfs_debug.h"
#include "libfsapfs_definitions.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfdatetime.h"
#include "libfsapfs_libfguid.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the directory entry flags
 */
void libfsapfs_debug_print_directory_entry_flags(
      uint16_t directory_entry_flags )
{
	switch( directory_entry_flags & 0x000f )
	{
		case 0x0000:
			libcnotify_printf(
			 "\t(DT_UNKNOWN)\n" );
			break;

		case 0x0001:
			libcnotify_printf(
			 "\t(DT_FIFO)\n" );
			break;

		case 0x0002:
			libcnotify_printf(
			 "\t(DT_CHR)\n" );
			break;

		case 0x0004:
			libcnotify_printf(
			 "\t(DT_DIR)\n" );
			break;

		case 0x0006:
			libcnotify_printf(
			 "\t(DT_BLK)\n" );
			break;

		case 0x0008:
			libcnotify_printf(
			 "\t(DT_REG)\n" );
			break;

		case 0x000a:
			libcnotify_printf(
			 "\t(DT_LNK)\n" );
			break;

		case 0x000c:
			libcnotify_printf(
			 "\t(DT_SOCK)\n" );
			break;

		case 0x000e:
			libcnotify_printf(
			 "\t(DT_WHT)\n" );
			break;

		default:
			libcnotify_printf(
			 "\tUnknown: 0x%04" PRIx16 "\n",
			 directory_entry_flags & 0x000f );
			break;
	}
	if( ( directory_entry_flags & 0x0010 ) != 0 )
	{
		libcnotify_printf(
		 "\t(RESERVED_10)\n" );
	}
}

/* Prints the inode flags
 */
void libfsapfs_debug_print_inode_flags(
      uint64_t inode_flags )
{
	if( ( inode_flags & 0x00000001 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_IS_APFS_PRIVATE)\n" );
	}
	if( ( inode_flags & 0x00000002 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_MAINTAIN_DIR_STATS)\n" );
	}
	if( ( inode_flags & 0x00000004 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_DIR_STATS_ORIGIN)\n" );
	}
	if( ( inode_flags & 0x00000008 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_PROT_CLASS_EXPLICIT)\n" );
	}
	if( ( inode_flags & 0x00000010 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_WAS_CLONED)\n" );
	}
	if( ( inode_flags & 0x00000020 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_FLAG_UNUSED)\n" );
	}
	if( ( inode_flags & 0x00000040 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_HAS_SECURITY_EA)\n" );
	}
	if( ( inode_flags & 0x00000080 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_BEING_TRUNCATED)\n" );
	}
	if( ( inode_flags & 0x00000100 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_HAS_FINDER_INFO)\n" );
	}
	if( ( inode_flags & 0x00000200 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_IS_SPARSE)\n" );
	}
	if( ( inode_flags & 0x00000400 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_WAS_EVER_CLONED)\n" );
	}
	if( ( inode_flags & 0x00000800 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_ACTIVE_FILE_TRIMMED)\n" );
	}
	if( ( inode_flags & 0x00001000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_PINNED_TO_MAIN)\n" );
	}
	if( ( inode_flags & 0x00002000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_PINNED_TO_TIER2)\n" );
	}
	if( ( inode_flags & 0x00004000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_HAS_RSRC_FORK)\n" );
	}
	if( ( inode_flags & 0x00008000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_NO_RSRC_FORK)\n" );
	}
	if( ( inode_flags & 0x00010000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(INODE_ALLOCATION_SPILLEDOVER)\n" );
	}
}

/* Prints the file system data type
 */
const char *libfsapfs_debug_print_file_system_data_type(
             uint8_t file_system_data_type )
{
	switch( file_system_data_type )
	{
		case 0:
			return( "(APFS_TYPE_ANY)" );

		case 1:
			return( "(APFS_TYPE_SNAP_METADATA)" );

		case 2:
			return( "(APFS_TYPE_EXTENT)" );

		case 3:
			return( "(APFS_TYPE_INODE)" );

		case 4:
			return( "(APFS_TYPE_XATTR)" );

		case 5:
			return( "(APFS_TYPE_SIBLING_LINK)" );

		case 6:
			return( "(APFS_TYPE_DSTREAM_ID)" );

		case 7:
			return( "(APFS_TYPE_CRYPTO_STATE)" );

		case 8:
			return( "(APFS_TYPE_FILE_EXTENT)" );

		case 9:
			return( "(APFS_TYPE_DIR_REC)" );

		case 10:
			return( "(APFS_TYPE_DIR_STATS)" );

		case 11:
			return( "(APFS_TYPE_SNAP_NAME)" );

		case 12:
			return( "(APFS_TYPE_SIBLING_MAP)" );
	}
	return( "Unknown" );
}

/* Prints the inode extended field type
 */
const char *libfsapfs_debug_print_inode_extended_field_type(
             uint8_t extended_field_type )
{
	switch( extended_field_type )
	{
		case 1:
			return( "(INO_EXT_TYPE_SNAP_XID)" );

		case 2:
			return( "(INO_EXT_TYPE_DELTA_TREE_OID)" );

		case 3:
			return( "(INO_EXT_TYPE_DOCUMENT_ID)" );

		case 4:
			return( "(INO_EXT_TYPE_NAME)" );

		case 5:
			return( "(INO_EXT_TYPE_PREV_FSIZE)" );

		case 6:
			return( "(INO_EXT_TYPE_RESERVED_6)" );

		case 7:
			return( "(INO_EXT_TYPE_FINDER_INFO)" );

		case 8:
			return( "(INO_EXT_TYPE_DSTREAM)" );

		case 9:
			return( "(INO_EXT_TYPE_RESERVED_9)" );

		case 10:
			return( "(INO_EXT_TYPE_DIR_STATS_KEY)" );

		case 11:
			return( "(INO_EXT_TYPE_FS_UUID)" );

		case 12:
			return( "(INO_EXT_TYPE_RESERVED_12)" );

		case 13:
			return( "(INO_EXT_TYPE_SPARSE_BYTES)" );

		case 14:
			return( "(INO_EXT_TYPE_RDEV)" );
	}
	return( "Unknown" );
}

/* Prints a POSIX value
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_debug_print_posix_time_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint8_t value_type,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char date_time_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	static char *function                 = "libfsapfs_debug_print_posix_time_value";

	if( libfdatetime_posix_time_initialize(
	     &posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create POSIX time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_from_byte_stream(
	     posix_time,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to POSIX time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_to_utf8_string(
	     posix_time,
	     (uint8_t *) date_time_string,
	     32,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy POSIX time to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s UTC\n",
	 function_name,
	 value_name,
	 date_time_string );

	if( libfdatetime_posix_time_free(
	     &posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free POSIX time.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	return( -1 );
}

/* Prints a GUID/UUID value
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_debug_print_guid_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
        system_character_t guid_string[ 48 ];

        libfguid_identifier_t *guid = NULL;
	static char *function       = "libfsapfs_debug_print_guid_value";

	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_to_utf8_string(
	     guid,
	     (uint8_t *) guid_string,
	     48,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 guid_string );

	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

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
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

