/*
 * The APFS container space manager definition
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

#if !defined( _FSAPFS_CONTAINER_SPACE_MANAGER_H )
#define _FSAPFS_CONTAINER_SPACE_MANAGER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_container_space_manager fsapfs_container_space_manager_t;

struct fsapfs_container_space_manager
{
	/* The object checksum
	 * Consists of 8 bytes
	 */
	uint8_t object_checksum[ 8 ];

	/* The object identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_identifier[ 8 ];

	/* The object transaction identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_transaction_identifier[ 8 ];

	/* The object type
	 * Consists of 4 bytes
	 */
	uint8_t object_type[ 4 ];

	/* The object subtype
	 * Consists of 4 bytes
	 */
	uint8_t object_subtype[ 4 ];

	/* The block size
	 * Consists of 4 bytes
	 */
	uint8_t block_size[ 4 ];

	/* The number of blocks per chunk
	 * Consists of 4 bytes
	 */
	uint8_t blocks_per_chunk[ 4 ];

	/* The number of chunks per CIB
	 * Consists of 4 bytes
	 */
	uint8_t chunks_per_cib[ 4 ];

	/* The number of CIBs per CAB
	 * Consists of 4 bytes
	 */
	uint8_t cibs_per_cab[ 4 ];

	/* The number of blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_blocks[ 8 ];

	/* The number of chunks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_chunks[ 8 ];

	/* The number of CIBs
	 * Consists of 4 bytes
	 */
	uint8_t number_of_cibs[ 4 ];

	/* The number of CABs
	 * Consists of 4 bytes
	 */
	uint8_t number_of_cabs[ 4 ];

	/* The number of unsused blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_unused_blocks[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown3[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown4[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown6[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown7[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown8[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown9[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown10[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown11[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown12[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown13[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown14[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown15[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown16[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown17[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown18[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown19[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown20[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown21[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown22[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown23[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown24[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown25[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown26[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown27[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown28[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown29[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown30[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown31[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown32[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown33[ 8 ];

/* TODO add values starting with bitmap_next_array_free */
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_CONTAINER_SPACE_MANAGER_H ) */

