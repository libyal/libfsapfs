/*
 * The object map descriptor functions
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

#if !defined( _LIBFSAPFS_OBJECT_MAP_DESCRIPTOR_H )
#define _LIBFSAPFS_OBJECT_MAP_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_object_map_descriptor libfsapfs_object_map_descriptor_t;

struct libfsapfs_object_map_descriptor
{
	/* The identifier
	 */
	uint64_t identifier;

	/* The transaction identifier
	 */
	uint64_t transaction_identifier;

	/* The flags
	 */
	uint32_t flags;

	/* The size
	 */
	uint32_t size;

	/* The physical address
	 */
	uint64_t physical_address;
};

int libfsapfs_object_map_descriptor_initialize(
     libfsapfs_object_map_descriptor_t **object_map_descriptor,
     libcerror_error_t **error );

int libfsapfs_object_map_descriptor_free(
     libfsapfs_object_map_descriptor_t **object_map_descriptor,
     libcerror_error_t **error );

int libfsapfs_object_map_descriptor_read_key_data(
     libfsapfs_object_map_descriptor_t *object_map_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_object_map_descriptor_read_value_data(
     libfsapfs_object_map_descriptor_t *object_map_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_OBJECT_MAP_DESCRIPTOR_H ) */

