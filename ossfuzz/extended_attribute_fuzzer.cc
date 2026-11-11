/*
 * OSS-Fuzz target for libfsapfs extended_attribute type
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libfsapfs.h"

#if !defined( LIBFSAPFS_HAVE_BFIO )

/* Opens a container using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_open_file_io_handle(
      libfsapfs_container_t *container,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libfsapfs_error_t **error );

#endif /* !defined( LIBFSAPFS_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	uint8_t buffer[ 512 ];
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 32 ];

	libbfio_handle_t *file_io_handle                   = NULL;
	libfsapfs_container_t *container                   = NULL;
	libfsapfs_extended_attribute_t *extended_attribute = NULL;
	libfsapfs_file_entry_t *file_entry                 = NULL;
	libfsapfs_file_entry_t *root_directory             = NULL;
	libfsapfs_file_entry_t *sub_file_entry             = NULL;
	libfsapfs_volume_t *volume                         = NULL;
	ssize_t read_count                                 = 0;
	size64_t extent_size                               = 0;
	size64_t stream_size                               = 0;
	size_t utf8_string_size                            = 0;
	size_t utf16_string_size                           = 0;
	off64_t extent_offset                              = 0;
	off64_t stream_offset                              = 0;
	uint64_t extended_attribute_identifier             = 0;
	uint32_t extent_flags                              = 0;
	int number_of_extended_attributes                  = 0;
	int number_of_extents                              = 0;
	int number_of_sub_file_entries                     = 0;
	int number_of_volumes                              = 0;
	int extended_attribute_index                       = 0;
	int extent_index                                   = 0;
	int read_iterator                                  = 0;
	int result                                         = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfsapfs_container_initialize(
	     &container,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfsapfs_container_open_file_io_handle(
	     container,
	     file_io_handle,
	     LIBFSAPFS_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libfsapfs_container;
	}
	if( libfsapfs_container_get_number_of_volumes(
	     container,
	     &number_of_volumes,
	     NULL ) != 1 )
	{
		goto on_error_libfsapfs_container;
	}
	if( number_of_volumes > 0 )
	{
		result = libfsapfs_container_get_volume_by_index(
		          container,
		          0,
		          &volume,
		          NULL );

		if( result == -1 )
		{
			goto on_error_libfsapfs_container;
		}
		if( result == 1 )
		{
			result = libfsapfs_volume_get_root_directory(
			          volume,
			          &root_directory,
			          NULL );

			if( result == -1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( result == 1 )
			{
				if( libfsapfs_file_entry_get_number_of_sub_file_entries(
				     root_directory,
				     &number_of_sub_file_entries,
				     NULL ) != 1 )
				{
					goto on_error_libfsapfs_root_directory;
				}
				if( number_of_sub_file_entries > 0 )
				{
					if( libfsapfs_file_entry_get_sub_file_entry_by_index(
					     root_directory,
					     0,
					     &sub_file_entry,
					     NULL ) != 1 )
					{
						goto on_error_libfsapfs_root_directory;
					}
					file_entry = sub_file_entry;
				}
				else
				{
					file_entry = root_directory;
				}
				if( libfsapfs_file_entry_get_number_of_extended_attributes(
				     file_entry,
				     &number_of_extended_attributes,
				     NULL ) != 1 )
				{
					goto on_error_libfsapfs_file_entry;
				}
				for( extended_attribute_index = 0;
				     ( extended_attribute_index < number_of_extended_attributes )
				   && ( extended_attribute_index < 16 );
				     extended_attribute_index++ )
				{
					if( libfsapfs_file_entry_get_extended_attribute_by_index(
					     file_entry,
					     extended_attribute_index,
					     &extended_attribute,
					     NULL ) != 1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					if( libfsapfs_extended_attribute_get_identifier(
					     extended_attribute,
					     &extended_attribute_identifier,
					     NULL ) != 1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					if( libfsapfs_extended_attribute_get_utf8_name_size(
					     extended_attribute,
					     &utf8_string_size,
					     NULL ) == -1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					if( utf8_string_size > 0 )
					{
						if( libfsapfs_extended_attribute_get_utf8_name(
						     extended_attribute,
						     utf8_string,
						     64,
						     NULL ) == -1 )
						{
							goto on_error_libfsapfs_extended_attribute;
						}
					}
					if( libfsapfs_extended_attribute_get_utf16_name_size(
					     extended_attribute,
					     &utf16_string_size,
					     NULL ) == -1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					if( utf16_string_size > 0 )
					{
						if( libfsapfs_extended_attribute_get_utf16_name(
						     extended_attribute,
						     utf16_string,
						     32,
						     NULL ) == -1 )
						{
							goto on_error_libfsapfs_extended_attribute;
						}
					}
					if( libfsapfs_extended_attribute_get_size(
					     extended_attribute,
					     &stream_size,
					     NULL ) != 1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					if( libfsapfs_extended_attribute_get_offset(
					     extended_attribute,
					     &stream_offset,
					     NULL ) != 1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					if( libfsapfs_extended_attribute_seek_offset(
					     extended_attribute,
					     0,
					     0,
					     NULL ) == -1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					read_count = libfsapfs_extended_attribute_read_buffer(
					           extended_attribute,
					           buffer,
					           497,
					           NULL );

					if( read_count == -1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					for( read_iterator = 0;
					     read_iterator < 128;
					     read_iterator++ )
					{
						if( stream_offset >= stream_size )
						{
							break;
						}
						read_count = libfsapfs_extended_attribute_read_buffer_at_offset(
						           extended_attribute,
						           buffer,
						           497,
						           stream_offset,
						           NULL );

						if( read_count == -1 )
						{
							goto on_error_libfsapfs_extended_attribute;
						}
						stream_offset += 497;
					}
					if( libfsapfs_extended_attribute_get_number_of_extents(
					     extended_attribute,
					     &number_of_extents,
					     NULL ) != 1 )
					{
						goto on_error_libfsapfs_extended_attribute;
					}
					for( extent_index = 0;
					     ( extent_index < number_of_extents )
					   && ( extent_index < 16 );
					     extent_index++ )
					{
						if( libfsapfs_extended_attribute_get_extent_by_index(
						     extended_attribute,
						     extent_index,
						     &extent_offset,
						     &extent_size,
						     &extent_flags,
						     NULL ) != 1 )
						{
							goto on_error_libfsapfs_extended_attribute;
						}
					}
					libfsapfs_extended_attribute_free(
					 &extended_attribute,
					 NULL );
				}
on_error_libfsapfs_extended_attribute:
				libfsapfs_extended_attribute_free(
				 &extended_attribute,
				 NULL );
on_error_libfsapfs_file_entry:
				if( sub_file_entry != NULL )
				{
					libfsapfs_file_entry_free(
					 &sub_file_entry,
					 NULL );
				}
			}
on_error_libfsapfs_root_directory:
			libfsapfs_file_entry_free(
			 &root_directory,
			 NULL );
		}
on_error_libfsapfs_volume:
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	libfsapfs_container_close(
	 container,
	 NULL );

on_error_libfsapfs_container:
	libfsapfs_container_free(
	 &container,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
