/*
 * The object map B-tree functions
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

#if !defined( _LIBFSAPFS_OBJECT_MAP_BTREE_H )
#define _LIBFSAPFS_OBJECT_MAP_BTREE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_object_map_descriptor.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_object_map_btree libfsapfs_object_map_btree_t;

struct libfsapfs_object_map_btree
{
	/* The descriptors array
	 */
	libcdata_array_t *descriptors_array;
};

int libfsapfs_object_map_btree_initialize(
     libfsapfs_object_map_btree_t **object_map_btree,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_free(
     libfsapfs_object_map_btree_t **object_map_btree,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_read_file_io_handle(
     libfsapfs_object_map_btree_t *object_map_btree,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t block_size,
     libcerror_error_t **error );

int libfsapfs_object_map_btree_read_data(
     libfsapfs_object_map_btree_t *object_map_btree,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_object_map_get_descriptor_by_object_identifier(
     libfsapfs_object_map_btree_t *object_map_btree,
     uint64_t object_identifier,
     libfsapfs_object_map_descriptor_t **descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_OBJECT_MAP_BTREE_H ) */

