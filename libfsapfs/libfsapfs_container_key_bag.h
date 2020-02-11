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

#if !defined( _LIBFSAPFS_CONTAINER_KEY_BAG_H )
#define _LIBFSAPFS_CONTAINER_KEY_BAG_H

#include <common.h>
#include <types.h>

#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_container_key_bag libfsapfs_container_key_bag_t;

struct libfsapfs_container_key_bag
{
	/* The entries array
	 */
	libcdata_array_t *entries_array;

	/* Value to indicate if the container key bag is locked
	 */
	uint8_t is_locked;
};

int libfsapfs_container_key_bag_initialize(
     libfsapfs_container_key_bag_t **container_key_bag,
     libcerror_error_t **error );

int libfsapfs_container_key_bag_free(
     libfsapfs_container_key_bag_t **container_key_bag,
     libcerror_error_t **error );

int libfsapfs_container_key_bag_read_file_io_handle(
     libfsapfs_container_key_bag_t *container_key_bag,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t data_size,
     const uint8_t *container_identifier,
     libcerror_error_t **error );

int libfsapfs_container_key_bag_read_data(
     libfsapfs_container_key_bag_t *container_key_bag,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_container_key_bag_get_volume_key_bag_extent_by_identifier(
     libfsapfs_container_key_bag_t *container_key_bag,
     const uint8_t *volume_identifier,
     uint64_t *volume_key_bag_block_number,
     uint64_t *volume_key_bag_number_of_blocks,
     libcerror_error_t **error );

int libfsapfs_container_key_bag_get_volume_master_key_by_identifier(
     libfsapfs_container_key_bag_t *container_key_bag,
     const uint8_t *volume_identifier,
     const uint8_t *volume_key,
     size_t volume_key_size,
     uint8_t *key,
     size_t key_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_CONTAINER_KEY_BAG_H ) */

