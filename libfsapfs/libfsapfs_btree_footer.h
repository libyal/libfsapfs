/*
 * The B-tree footer functions
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

#if !defined( _LIBFSAPFS_BTREE_FOOTER_H )
#define _LIBFSAPFS_BTREE_FOOTER_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_btree_footer libfsapfs_btree_footer_t;

struct libfsapfs_btree_footer
{
	/* The node size
	 */
	uint32_t node_size;

	/* The key size
	 */
	uint32_t key_size;

	/* The value size
	 */
	uint32_t value_size;

	/* The maximum key size
	 */
	uint32_t maximum_key_size;

	/* The maximum value size
	 */
	uint32_t maximum_value_size;
};

int libfsapfs_btree_footer_initialize(
     libfsapfs_btree_footer_t **btree_footer,
     libcerror_error_t **error );

int libfsapfs_btree_footer_free(
     libfsapfs_btree_footer_t **btree_footer,
     libcerror_error_t **error );

int libfsapfs_btree_footer_read_data(
     libfsapfs_btree_footer_t *btree_footer,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_BTREE_FOOTER_H ) */

