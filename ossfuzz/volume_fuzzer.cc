/*
 * OSS-Fuzz target for libfsapfs volume type
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
	uint8_t volume_identifier[ 16 ];
	uint16_t utf16_path[ 1 ]                 = { 0x002f };
	uint16_t utf16_string[ 32 ];
	uint16_t utf16_password[ 11 ]            = { 'f', 's', 'a', 'p', 'f', 's', '-', 'T', 'E', 'S', 'T' };
	uint16_t utf16_recovery_password[ 20 ]   = { 'f', 's', 'a', 'p', 'f', 's', '-', 'r', 'e', 'c', 'o', 'v',
	                                           'e', 'r', 'y', '-', 'T', 'E', 'S', 'T' };

	libbfio_handle_t *file_io_handle             = NULL;
	libfsapfs_container_t *container             = NULL;
	libfsapfs_file_entry_t *file_entry           = NULL;
	libfsapfs_volume_t *volume                   = NULL;
	size64_t volume_size                         = 0;
	size_t utf8_string_size                      = 0;
	size_t utf16_string_size                     = 0;
	uint64_t compatible_features_flags           = 0;
	uint64_t incompatible_features_flags         = 0;
	uint64_t next_file_entry_identifier          = 0;
	uint64_t read_only_compatible_features_flags = 0;
	int is_locked                                = 0;
	int number_of_snapshots                      = 0;
	int number_of_volumes                        = 0;
	int result                                   = 0;

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
			if( libfsapfs_volume_get_features_flags(
			     volume,
			     &compatible_features_flags,
			     &incompatible_features_flags,
			     &read_only_compatible_features_flags,
			     NULL ) != 1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( libfsapfs_volume_get_size(
			     volume,
			     &volume_size,
			     NULL ) != 1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( libfsapfs_volume_get_identifier(
			     volume,
			     volume_identifier,
			     16,
			     NULL ) != 1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( libfsapfs_volume_get_utf8_name_size(
			     volume,
			     &utf8_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( utf8_string_size > 0 )
			{
				if( libfsapfs_volume_get_utf8_name(
				     volume,
				     utf8_string,
				     64,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_volume;
				}
			}
			if( libfsapfs_volume_get_utf16_name_size(
			     volume,
			     &utf16_string_size,
			     NULL ) == -1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( utf16_string_size > 0 )
			{
				if( libfsapfs_volume_get_utf16_name(
				     volume,
				     utf16_string,
				     32,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_volume;
				}
			}
			is_locked = libfsapfs_volume_is_locked(
			               volume,
			               NULL );

			if( is_locked == -1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( is_locked == 1 )
			{
				if( libfsapfs_volume_set_utf8_password(
				     volume,
				     (uint8_t *) "fsapfs-TEST",
				     11,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_volume;
				}
				if( libfsapfs_volume_set_utf16_password(
				     volume,
				     utf16_password,
				     22,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_volume;
				}
				if( libfsapfs_volume_set_utf8_recovery_password(
				     volume,
				     (uint8_t *) "fsapfs-recovery-TEST",
				     20,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_volume;
				}
				if( libfsapfs_volume_set_utf16_recovery_password(
				     volume,
				     utf16_recovery_password,
				     40,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_volume;
				}
				if( libfsapfs_volume_unlock(
				     volume,
				     NULL ) == -1 )
				{
					goto on_error_libfsapfs_volume;
				}
			}
			if( libfsapfs_volume_get_next_file_entry_identifier(
			     volume,
			     &next_file_entry_identifier,
			     NULL ) != 1 )
			{
				goto on_error_libfsapfs_volume;
			}
			result = libfsapfs_volume_get_root_directory(
			          volume,
			          &file_entry,
			          NULL );

			if( result == -1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( result == 1 )
			{
				libfsapfs_file_entry_free(
				 &file_entry,
				 NULL );
			}
			result = libfsapfs_volume_get_file_entry_by_identifier(
			          volume,
			          1,
			          &file_entry,
			          NULL );

			if( result == -1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( result == 1 )
			{
				libfsapfs_file_entry_free(
				 &file_entry,
				 NULL );
			}
			result = libfsapfs_volume_get_file_entry_by_utf8_path(
			          volume,
			          (uint8_t *) "/",
			          1,
			          &file_entry,
			          NULL );

			if( result == -1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( result == 1 )
			{
				libfsapfs_file_entry_free(
				 &file_entry,
				 NULL );
			}
			result = libfsapfs_volume_get_file_entry_by_utf16_path(
			          volume,
			          utf16_path,
			          2,
			          &file_entry,
			          NULL );

			if( result == -1 )
			{
				goto on_error_libfsapfs_volume;
			}
			if( result == 1 )
			{
				libfsapfs_file_entry_free(
				 &file_entry,
				 NULL );
			}
			if( libfsapfs_volume_get_number_of_snapshots(
			     volume,
			     &number_of_snapshots,
			     NULL ) != 1 )
			{
				goto on_error_libfsapfs_volume;
			}
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
