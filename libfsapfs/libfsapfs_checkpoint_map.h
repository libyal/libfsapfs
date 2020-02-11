/*
 * The checkpoint map functions
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

#if !defined( _LIBFSAPFS_CHECKPOINT_MAP_H )
#define _LIBFSAPFS_CHECKPOINT_MAP_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_checkpoint_map libfsapfs_checkpoint_map_t;

struct libfsapfs_checkpoint_map
{
	/* The entries array
	 */
	libcdata_array_t *entries_array;
};

int libfsapfs_checkpoint_map_initialize(
     libfsapfs_checkpoint_map_t **checkpoint_map,
     libcerror_error_t **error );

int libfsapfs_checkpoint_map_free(
     libfsapfs_checkpoint_map_t **checkpoint_map,
     libcerror_error_t **error );

int libfsapfs_checkpoint_map_read_file_io_handle(
     libfsapfs_checkpoint_map_t *checkpoint_map,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsapfs_checkpoint_map_read_data(
     libfsapfs_checkpoint_map_t *checkpoint_map,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_checkpoint_map_get_physical_address_by_object_identifier(
     libfsapfs_checkpoint_map_t *checkpoint_map,
     uint64_t object_identifier,
     uint64_t *physical_address,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_CHECKPOINT_MAP_H ) */

