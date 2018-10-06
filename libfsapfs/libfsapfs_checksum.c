/*
 * Checksum functions
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

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

/* Table of CRC-32 values of 8-bit values
 */
uint32_t libfsapfs_checksum_crc32_table[ 256 ];

/* Value to indicate the CRC-32 table been computed
 */
int libfsapfs_checksum_crc32_table_computed = 0;

/* Initializes the internal CRC-32 table
 * The table speeds up the CRC-32 calculation
 */
void libfsapfs_checksum_initialize_crc32_table(
      uint32_t polynomial )
{
	uint32_t checksum    = 0;
	uint32_t table_index = 0;
	uint8_t bit_iterator = 0;

	for( table_index = 0;
	     table_index < 256;
	     table_index++ )
	{
		checksum = (uint32_t) table_index;

		for( bit_iterator = 0;
		     bit_iterator < 8;
		     bit_iterator++ )
		{
			if( checksum & 1 )
			{
				checksum = polynomial ^ ( checksum >> 1 );
			}
			else
			{
				checksum = checksum >> 1;
			}
		}
		libfsapfs_checksum_crc32_table[ table_index ] = checksum;
	}
	libfsapfs_checksum_crc32_table_computed = 1;
}

/* Calculates the weak CRC-32 checksum of a buffer
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_checksum_calculate_weak_crc32(
     uint32_t *checksum,
     const uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_checkcum_calculate_weak_crc32";
	size_t buffer_offset  = 0;
	uint32_t table_index  = 0;

	if( checksum == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
        if( libfsapfs_checksum_crc32_table_computed == 0 )
	{
		libfsapfs_checksum_initialize_crc32_table(
		 0x82f63b78UL );
	}
	*checksum = initial_value;

        for( buffer_offset = 0;
	     buffer_offset < size;
	     buffer_offset++ )
	{
		table_index = ( *checksum ^ buffer[ buffer_offset ] ) & 0x000000ffUL;

		*checksum = libfsapfs_checksum_crc32_table[ table_index ] ^ ( *checksum >> 8 );
        }
	return( 1 );
}

