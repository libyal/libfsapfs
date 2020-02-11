/*
 * The key bag entry functions
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

#if !defined( _LIBFSAPFS_KEY_BAG_ENTRY_H )
#define _LIBFSAPFS_KEY_BAG_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_key_bag_entry libfsapfs_key_bag_entry_t;

struct libfsapfs_key_bag_entry
{
	/* The size
	 */
	size_t size;

	/* The identifier
	 */
	uint8_t identifier[ 16 ];

	/* The entry type
	 */
	uint16_t type;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	uint16_t data_size;
};

int libfsapfs_key_bag_entry_initialize(
     libfsapfs_key_bag_entry_t **key_bag_entry,
     libcerror_error_t **error );

int libfsapfs_key_bag_entry_free(
     libfsapfs_key_bag_entry_t **key_bag_entry,
     libcerror_error_t **error );

int libfsapfs_key_bag_entry_read_data(
     libfsapfs_key_bag_entry_t *key_bag_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_KEY_BAG_ENTRY_H ) */

