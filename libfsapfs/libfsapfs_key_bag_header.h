/*
 * The key bag header functions
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

#if !defined( _LIBFSAPFS_KEY_BAG_HEADER_H )
#define _LIBFSAPFS_KEY_BAG_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_key_bag_header libfsapfs_key_bag_header_t;

struct libfsapfs_key_bag_header
{
	/* The number of entries
	 */
	uint16_t number_of_entries;

	/* The data size
	 */
	uint32_t data_size;
};

int libfsapfs_key_bag_header_initialize(
     libfsapfs_key_bag_header_t **key_bag_header,
     libcerror_error_t **error );

int libfsapfs_key_bag_header_free(
     libfsapfs_key_bag_header_t **key_bag_header,
     libcerror_error_t **error );

int libfsapfs_key_bag_header_read_data(
     libfsapfs_key_bag_header_t *key_bag_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_KEY_BAG_HEADER_H ) */

