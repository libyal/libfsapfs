/*
 * OSS-Fuzz target for libfsapfs snapshot type
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
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 32 ];

	libbfio_handle_t *file_io_handle = NULL;
	libfsapfs_container_t *container = NULL;
	libfsapfs_volume_t *volume       = NULL;
	libfsapfs_snapshot_t *snapshot   = NULL;
	size_t utf8_string_size          = 0;
	size_t utf16_string_size         = 0;
	int number_of_snapshots          = 0;
	int number_of_volumes            = 0;
	int snapshot_index               = 0;
	int result                       = 0;

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
			if( libfsapfs_volume_get_number_of_snapshots(
			     volume,
			     &number_of_snapshots,
			     NULL ) != 1 )
			{
				goto on_error_libfsapfs_volume;
			}
			for( snapshot_index = 0;
			     ( snapshot_index < number_of_snapshots )
			   && ( snapshot_index < 16 );
			     snapshot_index++ )
			{
				result = libfsapfs_volume_get_snapshot_by_index(
				          volume,
				          snapshot_index,
				          &snapshot,
				          NULL );

				if( result == -1 )
				{
					goto on_error_libfsapfs_snapshot;
				}
				if( result != 1 )
				{
					continue;
				}
				if( libfsapfs_snapshot_get_utf8_name_size(
				     snapshot,
				     &utf8_string_size,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_snapshot;
				}
				if( utf8_string_size > 0 )
				{
					if( libfsapfs_snapshot_get_utf8_name(
					     snapshot,
					     utf8_string,
					     64,
					     NULL ) == -1 )
					{
						goto on_error_libfsapfs_snapshot;
					}
				}
				if( libfsapfs_snapshot_get_utf16_name_size(
				     snapshot,
				     &utf16_string_size,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_snapshot;
				}
				if( utf16_string_size > 0 )
				{
					if( libfsapfs_snapshot_get_utf16_name(
					     snapshot,
					     utf16_string,
					     32,
					     NULL ) == -1 )
					{
						goto on_error_libfsapfs_snapshot;
					}
				}
				libfsapfs_snapshot_free(
				 &snapshot,
				 NULL );
			}
on_error_libfsapfs_snapshot:
			libfsapfs_snapshot_free(
			 &snapshot,
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
