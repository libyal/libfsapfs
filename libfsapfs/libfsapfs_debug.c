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

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the MFT attribute data flags
 */
void libfsapfs_debug_print_mft_attribute_data_flags(
      uint16_t mft_attribute_data_flags )
{
	if( ( mft_attribute_data_flags & 0x0001 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs compressed\n" );
	}

	if( ( mft_attribute_data_flags & 0x4000 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs encrypted\n" );
	}
	if( ( mft_attribute_data_flags & 0x8000 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs sparse\n" );
	}
}

/* Prints the file attribute flags
 */
void libfsapfs_debug_print_file_attribute_flags(
      uint32_t file_attribute_flags )
{
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	{
		libcnotify_printf(
		 "\tIs read-only (FILE_ATTRIBUTE_READ_ONLY)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_HIDDEN ) != 0 )
	{
		libcnotify_printf(
		 "\tIs hidden (FILE_ATTRIBUTE_HIDDEN)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 )
	{
		libcnotify_printf(
		 "\tIs system (FILE_ATTRIBUTE_SYSTEM)\n" );
	}

	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	{
		libcnotify_printf(
		 "\tIs directory (FILE_ATTRIBUTE_DIRECTORY)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_ARCHIVE ) != 0 )
	{
		libcnotify_printf(
		 "\tShould be archived (FILE_ATTRIBUTE_ARCHIVE)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	{
		libcnotify_printf(
		 "\tIs device (FILE_ATTRIBUTE_DEVICE)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_NORMAL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs normal (FILE_ATTRIBUTE_NORMAL)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_TEMPORARY ) != 0 )
	{
		libcnotify_printf(
		 "\tIs temporary (FILE_ATTRIBUTE_TEMPORARY)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_SPARSE_FILE ) != 0 )
	{
		libcnotify_printf(
		 "\tIs a sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_REPARSE_POINT ) != 0 )
	{
		libcnotify_printf(
		 "\tIs a reparse point or symbolic link (FILE_ATTRIBUTE_FLAG_REPARSE_POINT)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_COMPRESSED ) != 0 )
	{
		libcnotify_printf(
		 "\tIs compressed (FILE_ATTRIBUTE_COMPRESSED)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_OFFLINE ) != 0 )
	{
		libcnotify_printf(
		 "\tIs offline (FILE_ATTRIBUTE_OFFLINE)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED ) != 0 )
	{
		libcnotify_printf(
		 "\tContent should not be indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n" );
	}
	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_ENCRYPTED ) != 0 )
	{
		libcnotify_printf(
		 "\tIs encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n" );
	}

	if( ( file_attribute_flags & LIBFSAPFS_FILE_ATTRIBUTE_FLAG_VIRTUAL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs virtual (FILE_ATTRIBUTE_VIRTUAL)\n" );
	}

	if( ( file_attribute_flags & 0x10000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs directory (0x10000000)\n" );
	}
	if( ( file_attribute_flags & 0x20000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs index view (0x20000000)\n" );
	}
}

/* Prints the index node flags
 */
void libfsapfs_debug_print_index_node_flags(
      uint32_t index_node_flags )
{
	if( ( index_node_flags & 0x00000001UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas index allocation\n" );
	}
}

/* Prints the index value flags
 */
void libfsapfs_debug_print_index_value_flags(
      uint32_t index_value_flags )
{
	if( ( index_value_flags & LIBFSAPFS_INDEX_VALUE_FLAG_HAS_SUB_NODE ) != 0 )
	{
		libcnotify_printf(
		 "\tHas sub node\n" );
	}
	if( ( index_value_flags & LIBFSAPFS_INDEX_VALUE_FLAG_IS_LAST ) != 0 )
	{
		libcnotify_printf(
		 "\tIs last\n" );
	}
}

/* Prints the attribute type
 */
const char *libfsapfs_debug_print_attribute_type(
             uint32_t attribute_type )
{
	switch( attribute_type )
	{
		case 0x00000000UL:
			return( "" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			return( "$STANDARD_INFORMATION" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST:
			return( "$ATTRIBUTE_LIST" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_FILE_NAME:
			return( "$FILE_NAME" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			return( "$OBJECT_ID" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
			return( "$SECURITY_DESCRIPTOR" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			return( "$VOLUME_NAME" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			return( "$VOLUME_INFORMATION" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_DATA:
			return( "$DATA" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			return( "$INDEX_ROOT" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			return( "$INDEX_ALLOCATION" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_BITMAP:
			return( "$BITMAP" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			return( "$REPARSE_POINT" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_EXTENDED_INFORMATION:
			return( "$EA_INFORMATION" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_EXTENDED:
			return( "$EA" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_PROPERTY_SET:
			return( "$PROPERTY_SET" );

		case LIBFSAPFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
			return( "$LOGGED_UTILITY_STREAM" );
	}
	return( "_UNKNOWN_" );
}

/* Prints the file name attribute namespace
 */
const char *libfsapfs_debug_print_file_name_attribute_namespace(
             uint8_t name_namespace )
{
	switch( name_namespace )
	{
		case LIBFSAPFS_FILE_NAME_NAMESPACE_POSIX:
			return( "POSIX" );

		case LIBFSAPFS_FILE_NAME_NAMESPACE_WINDOWS:
			return( "Windows" );

		case LIBFSAPFS_FILE_NAME_NAMESPACE_DOS:
			return( "DOS" );

		case LIBFSAPFS_FILE_NAME_NAMESPACE_DOS_WINDOWS:
			return( "DOS and Windows" );
	}
	return( "_UNKNOWN_" );
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

#endif

