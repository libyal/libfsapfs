/*
 * The container data handle functions
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

#if !defined( _LIBFSAPFS_CONTAINER_DATA_HANDLE_H )
#define _LIBFSAPFS_CONTAINER_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfcache.h"
#include "libfsapfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_container_data_handle libfsapfs_container_data_handle_t;

struct libfsapfs_container_data_handle
{
	/* The IO handle
	 */
	libfsapfs_io_handle_t *io_handle;
};

int libfsapfs_container_data_handle_initialize(
     libfsapfs_container_data_handle_t **container_data_handle,
     libfsapfs_io_handle_t *io_handle,
     libcerror_error_t **error );

int libfsapfs_container_data_handle_free(
     libfsapfs_container_data_handle_t **container_data_handle,
     libcerror_error_t **error );

int libfsapfs_container_data_handle_read_data_block(
     libfsapfs_container_data_handle_t *container_data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_CONTAINER_DATA_HANDLE_H ) */

