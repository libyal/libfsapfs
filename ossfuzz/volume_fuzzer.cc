/*
 * OSS-Fuzz target for libfsapfs volume type
 *
 * Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
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
	uint8_t volume_identifier[ 16 ];
	uint8_t string_value[ 64 ];

	libbfio_handle_t *file_io_handle             = NULL;
	libfsapfs_container_t *container             = NULL;
	libfsapfs_volume_t *volume                   = NULL;
	size64_t volume_size                         = 0;
	size_t string_size                           = 0;
	uint64_t compatible_features_flags           = 0;
	uint64_t incompatible_features_flags         = 0;
	uint64_t read_only_compatible_features_flags = 0;
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

		if( result != -1 )
		{
			libfsapfs_volume_get_features_flags(
			 volume,
			 &compatible_features_flags,
			 &incompatible_features_flags,
			 &read_only_compatible_features_flags,
			 NULL );

			libfsapfs_volume_get_size(
			 volume,
			 &volume_size,
			 NULL );

			libfsapfs_volume_get_identifier(
			 volume,
			 volume_identifier,
			 16,
			 NULL );

			libfsapfs_volume_get_utf8_name_size(
			 volume,
			 &string_size,
			 NULL );

			libfsapfs_volume_get_utf8_name(
			 volume,
			 string_value,
			 64,
			 NULL );

			libfsapfs_volume_is_locked(
			 volume,
			 NULL );

			libfsapfs_volume_free(
			 &volume,
			 NULL );
		}
	}
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

