/*
 * The APFS compressed data definitions
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

#if !defined( _FSAPFS_COMPRESSED_DATA_H )
#define _FSAPFS_COMPRESSED_DATA_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsapfs_compressed_data_header fsapfs_compressed_data_header_t;

struct fsapfs_compressed_data_header
{
	/* The signature
	 * Consists of 4 bytes
	 * Contains "fpmc"
	 */
	uint8_t signature[ 4 ];

	/* The compression method
	 * Consists of 4 bytes
	 */
	uint8_t compression_method[ 4 ];

	/* The uncompressed data size
	 * Consists of 8 bytes
	 */
	uint8_t uncompressed_data_size[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_COMPRESSED_DATA_H ) */

