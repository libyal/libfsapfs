/*
 * The APFS B-tree definition
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

#if !defined( _FSAPFS_BTREE_H )
#define _FSAPFS_BTREE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_btree_node_header fsapfs_btree_node_header_t;

struct fsapfs_btree_node_header
{
	/* The node flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* The level
	 * Consists of 2 bytes
	 */
	uint8_t level[ 2 ];

	/* The number of keys
	 * Consists of 4 bytes
	 */
	uint8_t number_of_keys[ 4 ];

	/* The entries data offset
	 * Consists of 2 bytes
	 */
	uint8_t entries_data_offset[ 2 ];

	/* The entries data size
	 * Consists of 2 bytes
	 */
	uint8_t entries_data_size[ 2 ];

	/* The unused data offset
	 * Consists of 2 bytes
	 */
	uint8_t unused_data_offset[ 2 ];

	/* The unused data size
	 * Consists of 2 bytes
	 */
	uint8_t unused_data_size[ 2 ];

	/* The key free list offset
	 * Consists of 2 bytes
	 */
	uint8_t key_free_list_offset[ 2 ];

	/* The value free list size
	 * Consists of 2 bytes
	 */
	uint8_t key_free_list_size[ 2 ];

	/* The value free list offset
	 * Consists of 2 bytes
	 */
	uint8_t value_free_list_offset[ 2 ];

	/* The value free list size
	 * Consists of 2 bytes
	 */
	uint8_t value_free_list_size[ 2 ];
};

typedef struct fsapfs_btree_fixed_size_entry fsapfs_btree_fixed_size_entry_t;

struct fsapfs_btree_fixed_size_entry
{
	/* The key data offset
	 * Consists of 2 bytes
	 */
	uint8_t key_data_offset[ 2 ];

	/* The value data offset
	 * Consists of 2 bytes
	 */
	uint8_t value_data_offset[ 2 ];
};

typedef struct fsapfs_btree_variable_size_entry fsapfs_btree_variable_size_entry_t;

struct fsapfs_btree_variable_size_entry
{
	/* The key data offset
	 * Consists of 2 bytes
	 */
	uint8_t key_data_offset[ 2 ];

	/* The key data size
	 * Consists of 2 bytes
	 */
	uint8_t key_data_size[ 2 ];

	/* The value data offset
	 * Consists of 2 bytes
	 */
	uint8_t value_data_offset[ 2 ];

	/* The value data size
	 * Consists of 2 bytes
	 */
	uint8_t value_data_size[ 2 ];
};

typedef struct fsapfs_btree_footer fsapfs_btree_footer_t;

struct fsapfs_btree_footer
{
	/* The flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* The node size
	 * Consists of 4 bytes
	 */
	uint8_t node_size[ 4 ];

	/* The key size
	 * Consists of 4 bytes
	 */
	uint8_t key_size[ 4 ];

	/* The value size
	 * Consists of 4 bytes
	 */
	uint8_t value_size[ 4 ];

	/* The maximum key size
	 * Consists of 4 bytes
	 */
	uint8_t maximum_key_size[ 4 ];

	/* The maximum value size
	 * Consists of 4 bytes
	 */
	uint8_t maximum_value_size[ 4 ];

	/* The total number of keys
	 * Consists of 8 bytes
	 */
	uint8_t total_number_of_keys[ 8 ];

	/* The total number of nodes
	 * Consists of 8 bytes
	 */
	uint8_t total_number_of_nodes[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_BTREE_H ) */

