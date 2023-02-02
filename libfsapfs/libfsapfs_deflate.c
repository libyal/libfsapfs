/*
 * Deflate (zlib) (un)compression functions
 *
 * Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_bit_stream.h"
#include "libfsapfs_deflate.h"
#include "libfsapfs_huffman_tree.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

const uint8_t libfsapfs_deflate_code_sizes_sequence[ 19 ]  = {
	16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2,
        14, 1, 15 };

const uint16_t libfsapfs_deflate_literal_codes_base[ 29 ] = {
	3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
	35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };

const uint16_t libfsapfs_deflate_literal_codes_number_of_extra_bits[ 29 ] = {
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };

const uint16_t libfsapfs_deflate_distance_codes_base[ 30 ] = {
	1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
	257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193,
	12289, 16385, 24577 };

const uint16_t libfsapfs_deflate_distance_codes_number_of_extra_bits[ 30 ] = {
	0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
	7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };

/* Initializes the dynamic Huffman trees
 * Returns 1 on success or -1 on error
 */
int libfsapfs_deflate_build_dynamic_huffman_trees(
     libfsapfs_bit_stream_t *bit_stream,
     libfsapfs_huffman_tree_t *literals_tree,
     libfsapfs_huffman_tree_t *distances_tree,
     libcerror_error_t **error )
{
	uint8_t code_size_array[ 316 ];

	libfsapfs_huffman_tree_t *codes_tree = NULL;
	static char *function              = "libfsapfs_deflate_build_dynamic_huffman_trees";
	uint32_t code_size                 = 0;
	uint32_t code_size_index           = 0;
	uint32_t code_size_sequence        = 0;
	uint32_t number_of_code_sizes      = 0;
	uint32_t number_of_distance_codes  = 0;
	uint32_t number_of_literal_codes   = 0;
	uint32_t times_to_repeat           = 0;
	uint16_t symbol                    = 0;

	if( libfsapfs_bit_stream_get_value(
	     bit_stream,
	     14,
	     &number_of_code_sizes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from bit stream.",
		 function );

		goto on_error;
	}
	number_of_literal_codes  = number_of_code_sizes & 0x0000001fUL;
	number_of_code_sizes   >>= 5;
	number_of_distance_codes = number_of_code_sizes & 0x0000001fUL;
	number_of_code_sizes   >>= 5;

	number_of_literal_codes += 257;

	if( number_of_literal_codes > 286 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of literal codes value out of bounds.",
		 function );

		goto on_error;
	}
	number_of_distance_codes += 1;

	if( number_of_distance_codes > 30 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of distance codes value out of bounds.",
		 function );

		goto on_error;
	}
	number_of_code_sizes += 4;

	for( code_size_index = 0;
	     code_size_index < number_of_code_sizes;
	     code_size_index++ )
	{
		if( libfsapfs_bit_stream_get_value(
		     bit_stream,
		     3,
		     &code_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from bit stream.",
			 function );

			goto on_error;
		}
		code_size_sequence = libfsapfs_deflate_code_sizes_sequence[ code_size_index ];

		code_size_array[ code_size_sequence ] = (uint8_t) code_size;
	}
	while( code_size_index < 19 )
	{
		code_size_sequence = libfsapfs_deflate_code_sizes_sequence[ code_size_index++ ];

		code_size_array[ code_size_sequence ] = 0;
	}
	if( libfsapfs_huffman_tree_initialize(
	     &codes_tree,
	     19,
	     15,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to build codes tree.",
		 function );

		goto on_error;
	}
	if( libfsapfs_huffman_tree_build(
	     codes_tree,
	     code_size_array,
	     19,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to build codes tree.",
		 function );

		goto on_error;
	}
	number_of_code_sizes = number_of_literal_codes + number_of_distance_codes;

	code_size_index = 0;

	while( code_size_index < number_of_code_sizes )
	{
		if( libfsapfs_huffman_tree_get_symbol_from_bit_stream(
		     codes_tree,
		     bit_stream,
		     &symbol,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve literal value from bit stream.",
			 function );

			goto on_error;
		}
		if( symbol < 16 )
		{
			code_size_array[ code_size_index++ ] = (uint8_t) symbol;

			continue;
		}
		code_size = 0;

		if( symbol == 16 )
		{
			if( code_size_index == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid code size index value out of bounds.",
				 function );

				goto on_error;
			}
			code_size = (uint32_t) code_size_array[ code_size_index - 1 ];

			if( libfsapfs_bit_stream_get_value(
			     bit_stream,
			     2,
			     &times_to_repeat,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from bit stream.",
				 function );

				goto on_error;
			}
			times_to_repeat += 3;
		}
		else if( symbol == 17 )
		{
			if( libfsapfs_bit_stream_get_value(
			     bit_stream,
			     3,
			     &times_to_repeat,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from bit stream.",
				 function );

				goto on_error;
			}
			times_to_repeat += 3;
		}
		else if( symbol == 18 )
		{
			if( libfsapfs_bit_stream_get_value(
			     bit_stream,
			     7,
			     &times_to_repeat,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from bit stream.",
				 function );

				goto on_error;
			}
			times_to_repeat += 11;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid code value value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( code_size_index + times_to_repeat ) > number_of_code_sizes )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid times to repeat value out of bounds.",
			 function );

			goto on_error;
		}
		while( times_to_repeat > 0 )
		{
			code_size_array[ code_size_index++ ] = (uint8_t) code_size;

			times_to_repeat--;
		}
	}
	if( code_size_array[ 256 ] == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: end-of-block code value missing in literal codes array.",
		 function );

		goto on_error;
	}
	if( libfsapfs_huffman_tree_free(
	     &codes_tree,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free codes tree.",
		 function );

		goto on_error;
	}
	if( libfsapfs_huffman_tree_build(
	     literals_tree,
	     code_size_array,
	     number_of_literal_codes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to build literals tree.",
		 function );

		goto on_error;
	}
	if( libfsapfs_huffman_tree_build(
	     distances_tree,
	     &( code_size_array[ number_of_literal_codes ] ),
	     number_of_distance_codes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to build distances tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( codes_tree != NULL )
	{
		libfsapfs_huffman_tree_free(
		 &codes_tree,
		 NULL );
	}
	return( -1 );
}

/* Initializes the fixed Huffman trees
 * Returns 1 on success or -1 on error
 */
int libfsapfs_deflate_build_fixed_huffman_trees(
     libfsapfs_huffman_tree_t *literals_tree,
     libfsapfs_huffman_tree_t *distances_tree,
     libcerror_error_t **error )
{
	uint8_t code_size_array[ 318 ];

	static char *function = "libfsapfs_deflate_build_fixed_huffman_trees";
	uint16_t symbol       = 0;

	for( symbol = 0;
	     symbol < 318;
	     symbol++ )
	{
		if( symbol < 144 )
		{
			code_size_array[ symbol ] = 8;
		}
		else if( symbol < 256 )
		{
			code_size_array[ symbol ] = 9;
		}
		else if( symbol < 280 )
		{
			code_size_array[ symbol ] = 7;
		}
		else if( symbol < 288 )
		{
			code_size_array[ symbol ] = 8;
		}
		else
		{
			code_size_array[ symbol ] = 5;
		}
	}
	if( libfsapfs_huffman_tree_build(
	     literals_tree,
	     code_size_array,
	     288,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to build literals tree.",
		 function );

		return( -1 );
	}
	if( libfsapfs_huffman_tree_build(
	     distances_tree,
	     &( code_size_array[ 288 ] ),
	     30,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to build distances tree.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Decodes a Huffman compressed block
 * Returns 1 on success or -1 on error
 */
int libfsapfs_deflate_decode_huffman(
     libfsapfs_bit_stream_t *bit_stream,
     libfsapfs_huffman_tree_t *literals_tree,
     libfsapfs_huffman_tree_t *distances_tree,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     libcerror_error_t **error )
{
	static char *function         = "libfsapfs_deflate_decode_huffman";
	size_t data_offset            = 0;
	uint32_t extra_bits           = 0;
	uint16_t compression_offset   = 0;
	uint16_t compression_size     = 0;
	uint16_t number_of_extra_bits = 0;
	uint16_t symbol               = 0;

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data offset.",
		 function );

		return( -1 );
	}
	data_offset = *uncompressed_data_offset;

	do
	{
		if( libfsapfs_huffman_tree_get_symbol_from_bit_stream(
		     literals_tree,
		     bit_stream,
		     &symbol,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve literal value from bit stream.",
			 function );

			return( -1 );
		}
		if( symbol < 256 )
		{
			if( data_offset >= uncompressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: invalid uncompressed data value too small.",
				 function );

				return( -1 );
			}
			uncompressed_data[ data_offset++ ] = (uint8_t) symbol;
		}
		else if( ( symbol > 256 )
		      && ( symbol < 286 ) )
		{
			symbol -= 257;

			number_of_extra_bits = libfsapfs_deflate_literal_codes_number_of_extra_bits[ symbol ];

			if( libfsapfs_bit_stream_get_value(
			     bit_stream,
			     (uint8_t) number_of_extra_bits,
			     &extra_bits,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve literal extra value from bit stream.",
				 function );

				return( -1 );
			}
			compression_size = libfsapfs_deflate_literal_codes_base[ symbol ] + (uint16_t) extra_bits;

			if( libfsapfs_huffman_tree_get_symbol_from_bit_stream(
			     distances_tree,
			     bit_stream,
			     &symbol,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve distance value from bit stream.",
				 function );

				return( -1 );
			}
			number_of_extra_bits = libfsapfs_deflate_distance_codes_number_of_extra_bits[ symbol ];

			if( libfsapfs_bit_stream_get_value(
			     bit_stream,
			     (uint8_t) number_of_extra_bits,
			     &extra_bits,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve distance extra value from bit stream.",
				 function );

				return( -1 );
			}
			compression_offset = libfsapfs_deflate_distance_codes_base[ symbol ] + (uint16_t) extra_bits;

			if( compression_offset > data_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid compression offset value out of bounds.",
				 function );

				return( -1 );
			}
			if( ( data_offset + compression_size ) > uncompressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: invalid uncompressed data value too small.",
				 function );

				return( -1 );
			}
			while( compression_size > 0 )
			{
				uncompressed_data[ data_offset ] = uncompressed_data[ data_offset - compression_offset ];

				data_offset++;
				compression_size--;
			}
		}
		else if( symbol != 256 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: invalid code value: %" PRIu16 ".",
			 function,
			 symbol );

			return( -1 );
		}
	}
	while( symbol != 256 );

	*uncompressed_data_offset = data_offset;

	return( 1 );
}

/* Calculates the little-endian Adler-32 of a buffer
 * It uses the initial value to calculate a new Adler-32
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_deflate_calculate_adler32(
     uint32_t *checksum_value,
     const uint8_t *data,
     size_t data_size,
     uint32_t initial_value,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_deflate_calculate_adler32";
	size_t data_offset    = 0;
	uint32_t lower_word   = 0;
	uint32_t upper_word   = 0;
	uint32_t value_32bit  = 0;
	int block_index       = 0;

	if( checksum_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum value.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	lower_word = initial_value & 0xffff;
	upper_word = ( initial_value >> 16 ) & 0xffff;

	while( data_size >= 0x15b0 )
	{
		/* The modulo calculation is needed per 5552 (0x15b0) bytes
		 * 5552 / 16 = 347
		 */
		for( block_index = 0;
		     block_index < 347;
		     block_index++ )
		{
			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;
		}
		/* Optimized equivalent of:
		 * lower_word %= 0xfff1
		 */
		value_32bit = lower_word >> 16;
		lower_word &= 0x0000ffffUL;
		lower_word += ( value_32bit << 4 ) - value_32bit;

		if( lower_word > 65521 )
		{
			value_32bit = lower_word >> 16;
			lower_word &= 0x0000ffffUL;
			lower_word += ( value_32bit << 4 ) - value_32bit;
		}
		if( lower_word >= 65521 )
		{
			lower_word -= 65521;
		}
		/* Optimized equivalent of:
		 * upper_word %= 0xfff1
		 */
		value_32bit = upper_word >> 16;
		upper_word &= 0x0000ffffUL;
		upper_word += ( value_32bit << 4 ) - value_32bit;

		if( upper_word > 65521 )
		{
			value_32bit = upper_word >> 16;
			upper_word &= 0x0000ffffUL;
			upper_word += ( value_32bit << 4 ) - value_32bit;
		}
		if( upper_word >= 65521 )
		{
			upper_word -= 65521;
		}
		data_size -= 0x15b0;
	}
	if( data_size > 0 )
	{
		while( data_size > 16 )
		{
			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			data_size -= 16;
		}
		while( data_size > 0 )
		{
			lower_word += data[ data_offset++ ];
			upper_word += lower_word;

			data_size--;
		}
		/* Optimized equivalent of:
		 * lower_word %= 0xfff1
		 */
		value_32bit = lower_word >> 16;
		lower_word &= 0x0000ffffUL;
		lower_word += ( value_32bit << 4 ) - value_32bit;

		if( lower_word > 65521 )
		{
			value_32bit = lower_word >> 16;
			lower_word &= 0x0000ffffUL;
			lower_word += ( value_32bit << 4 ) - value_32bit;
		}
		if( lower_word >= 65521 )
		{
			lower_word -= 65521;
		}
		/* Optimized equivalent of:
		 * upper_word %= 0xfff1
		 */
		value_32bit = upper_word >> 16;
		upper_word &= 0x0000ffffUL;
		upper_word += ( value_32bit << 4 ) - value_32bit;

		if( upper_word > 65521 )
		{
			value_32bit = upper_word >> 16;
			upper_word &= 0x0000ffffUL;
			upper_word += ( value_32bit << 4 ) - value_32bit;
		}
		if( upper_word >= 65521 )
		{
			upper_word -= 65521;
		}
	}
	*checksum_value = ( upper_word << 16 ) | lower_word;

	return( 1 );
}

/* Reads the compressed data header
 * Returns 1 on success or -1 on error
 */
int libfsapfs_deflate_read_data_header(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *compressed_data_offset,
     libcerror_error_t **error )
{
	static char *function                 = "libfsapfs_deflate_read_data_header";
	size_t safe_offset                    = 0;
	uint32_t compression_window_size      = 0;
	uint32_t preset_dictionary_identifier = 0;
	uint8_t flags                         = 0;
	uint8_t compression_data              = 0;
	uint8_t compression_information       = 0;
	uint8_t compression_method            = 0;
	uint8_t compression_window_bits       = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data offset.",
		 function );

		return( -1 );
	}
	safe_offset = *compressed_data_offset;

	if( ( compressed_data_size < 2 )
	 || ( safe_offset > ( compressed_data_size - 2 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid compressed data value too small.",
		 function );

		return( -1 );
	}
	compression_data = compressed_data[ safe_offset++ ];
	flags            = compressed_data[ safe_offset++ ];

	compression_method      = compression_data & 0x0f;
	compression_information = compression_data >> 4;

/* TODO validate check bits */
	if( ( flags & 0x20 ) != 0 )
	{
		if( ( compressed_data_size < 6 )
		 || ( safe_offset > ( compressed_data_size - 6 ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid compressed data value too small.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint32_big_endian(
		 &( compressed_data[ 2 ] ),
		 preset_dictionary_identifier );

		safe_offset += 4;
	}
	if( compression_method != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method: %" PRIu8 ".",
		 function,
		 compression_method );

		return( -1 );
	}
	compression_window_bits = (uint8_t) compression_information + 8;
	compression_window_size = (uint32_t) 1UL << compression_window_bits;

	if( compression_window_size > 32768 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression window size: %" PRIu32 ".",
		 function,
		 compression_window_size );

		return( -1 );
	}
	*compressed_data_offset += safe_offset;

	return( 1 );
}

/* Reads the header of a block of compressed data
 * Returns 1 on success or -1 on error
 */
int libfsapfs_deflate_read_block_header(
     libfsapfs_bit_stream_t *bit_stream,
     uint8_t *block_type,
     uint8_t *last_block_flag,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_deflate_read_block_header";
	uint32_t value_32bit  = 0;

	if( block_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block type.",
		 function );

		return( -1 );
	}
	if( last_block_flag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid last block flag.",
		 function );

		return( -1 );
	}
	if( libfsapfs_bit_stream_get_value(
	     bit_stream,
	     3,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from bit stream.",
		 function );

		return( -1 );
	}
	*last_block_flag = (uint8_t) ( value_32bit & 0x00000001UL );
	value_32bit    >>= 1;
	*block_type      = (uint8_t) value_32bit;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: block header last block flag\t\t\t: %" PRIu8 "\n",
		 function,
		 *last_block_flag );

		libcnotify_printf(
		 "%s: block header block type\t\t\t: %" PRIu8 " (",
		 function,
		 *block_type );

		switch( *block_type )
		{
			case LIBFSAPFS_DEFLATE_BLOCK_TYPE_UNCOMPRESSED:
				libcnotify_printf(
				 "Uncompressed" );
				break;

			case LIBFSAPFS_DEFLATE_BLOCK_TYPE_HUFFMAN_FIXED:
				libcnotify_printf(
				 "Fixed Huffman" );
				break;

			case LIBFSAPFS_DEFLATE_BLOCK_TYPE_HUFFMAN_DYNAMIC:
				libcnotify_printf(
				 "Dynamic Huffman" );
				break;

			case LIBFSAPFS_DEFLATE_BLOCK_TYPE_RESERVED:
			default:
				libcnotify_printf(
				 "Reserved" );
				break;
		}
		libcnotify_printf(
		 ")\n" );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads a block of compressed data
 * Returns 1 on success or -1 on error
 */
int libfsapfs_deflate_read_block(
     libfsapfs_bit_stream_t *bit_stream,
     uint8_t block_type,
     libfsapfs_huffman_tree_t *fixed_huffman_literals_tree,
     libfsapfs_huffman_tree_t *fixed_huffman_distances_tree,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     libcerror_error_t **error )
{
	libfsapfs_huffman_tree_t *dynamic_huffman_distances_tree = NULL;
	libfsapfs_huffman_tree_t *dynamic_huffman_literals_tree  = NULL;
	static char *function                                  = "libfsapfs_deflate_read_block";
	size_t safe_uncompressed_data_offset                   = 0;
	uint32_t block_size                                    = 0;
	uint32_t block_size_copy                               = 0;
	uint32_t value_32bit                                   = 0;
	uint8_t skip_bits                                      = 0;

	if( bit_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bit stream.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	safe_uncompressed_data_offset = *uncompressed_data_offset;

	switch( block_type )
	{
		case LIBFSAPFS_DEFLATE_BLOCK_TYPE_UNCOMPRESSED:
			/* Ignore the bits in the buffer upto the next byte
			 */
			skip_bits = bit_stream->bit_buffer_size & 0x07;

			if( skip_bits > 0 )
			{
				if( libfsapfs_bit_stream_get_value(
				     bit_stream,
				     skip_bits,
				     &value_32bit,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve value from bit stream.",
					 function );

					goto on_error;
				}
			}
			if( libfsapfs_bit_stream_get_value(
			     bit_stream,
			     32,
			     &block_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from bit stream.",
				 function );

				goto on_error;
			}
			block_size_copy = ( block_size >> 16 ) ^ 0x0000ffffUL;
			block_size     &= 0x0000ffffUL;

			if( block_size != block_size_copy )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_INPUT,
				 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
				 "%s: mismatch in block size ( %" PRIu32 " != %" PRIu32 " ).",
				 function,
				 block_size,
				 block_size_copy );

				goto on_error;
			}
			if( block_size == 0 )
			{
				break;
			}
			if( (size_t) block_size > ( bit_stream->byte_stream_size - bit_stream->byte_stream_offset ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: invalid compressed data value too small.",
				 function );

				goto on_error;
			}
			if( (size_t) block_size > ( uncompressed_data_size - safe_uncompressed_data_offset ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: invalid uncompressed data value too small.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     &( uncompressed_data[ safe_uncompressed_data_offset ] ),
			     &( bit_stream->byte_stream[ bit_stream->byte_stream_offset ] ),
			     (size_t) block_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to initialize lz buffer.",
				 function );

				goto on_error;
			}
			bit_stream->byte_stream_offset += block_size;
			safe_uncompressed_data_offset  += block_size;

			/* Flush the bit stream buffer
			 */
			bit_stream->bit_buffer      = 0;
			bit_stream->bit_buffer_size = 0;

			break;

		case LIBFSAPFS_DEFLATE_BLOCK_TYPE_HUFFMAN_FIXED:
			if( libfsapfs_deflate_decode_huffman(
			     bit_stream,
			     fixed_huffman_literals_tree,
			     fixed_huffman_distances_tree,
			     uncompressed_data,
			     uncompressed_data_size,
			     &safe_uncompressed_data_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to decode fixed Huffman encoded bit stream.",
				 function );

				goto on_error;
			}
			break;

		case LIBFSAPFS_DEFLATE_BLOCK_TYPE_HUFFMAN_DYNAMIC:
			if( libfsapfs_huffman_tree_initialize(
			     &dynamic_huffman_literals_tree,
			     288,
			     15,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to build dynamic literals Huffman tree.",
				 function );

				goto on_error;
			}
			if( libfsapfs_huffman_tree_initialize(
			     &dynamic_huffman_distances_tree,
			     30,
			     15,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to build dynamic distances Huffman tree.",
				 function );

				goto on_error;
			}
			if( libfsapfs_deflate_build_dynamic_huffman_trees(
			     bit_stream,
			     dynamic_huffman_literals_tree,
			     dynamic_huffman_distances_tree,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to build dynamic Huffman trees.",
				 function );

				goto on_error;
			}
			if( libfsapfs_deflate_decode_huffman(
			     bit_stream,
			     dynamic_huffman_literals_tree,
			     dynamic_huffman_distances_tree,
			     uncompressed_data,
			     uncompressed_data_size,
			     &safe_uncompressed_data_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to decode dynamic Huffman encoded bit stream.",
				 function );

				goto on_error;
			}
			if( libfsapfs_huffman_tree_free(
			     &dynamic_huffman_distances_tree,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free dynamic distances Huffman tree.",
				 function );

				goto on_error;
			}
			if( libfsapfs_huffman_tree_free(
			     &dynamic_huffman_literals_tree,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free dynamic literals Huffman tree.",
				 function );

				goto on_error;
			}
			break;

		case LIBFSAPFS_DEFLATE_BLOCK_TYPE_RESERVED:
		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported block type.",
			 function );

			goto on_error;
	}
	*uncompressed_data_offset = safe_uncompressed_data_offset;

	return( 1 );

on_error:
	if( dynamic_huffman_distances_tree != NULL )
	{
		libfsapfs_huffman_tree_free(
		 &dynamic_huffman_distances_tree,
		 NULL );
	}
	if( dynamic_huffman_literals_tree != NULL )
	{
		libfsapfs_huffman_tree_free(
		 &dynamic_huffman_literals_tree,
		 NULL );
	}
	return( -1 );
}

/* Decompresses data using deflate compression
 * Returns 1 on success or -1 on error
 */
int libfsapfs_deflate_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	libfsapfs_bit_stream_t *bit_stream                     = NULL;
	libfsapfs_huffman_tree_t *fixed_huffman_distances_tree = NULL;
	libfsapfs_huffman_tree_t *fixed_huffman_literals_tree  = NULL;
	static char *function                                = "libfsapfs_deflate_decompress";
	size_t compressed_data_offset                        = 0;
	size_t safe_uncompressed_data_size                   = 0;
	size_t uncompressed_data_offset                      = 0;
	uint8_t block_type                                   = 0;
	uint8_t last_block_flag                              = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	safe_uncompressed_data_size = *uncompressed_data_size;

	if( safe_uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_offset >= compressed_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid compressed data value too small.",
		 function );

		return( -1 );
	}
	if( libfsapfs_bit_stream_initialize(
	     &bit_stream,
	     compressed_data,
	     compressed_data_size,
	     compressed_data_offset,
	     LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create bit stream.",
		 function );

		goto on_error;
	}
	while( bit_stream->byte_stream_offset < bit_stream->byte_stream_size )
	{
		if( libfsapfs_deflate_read_block_header(
		     bit_stream,
		     &block_type,
		     &last_block_flag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read compressed data block header.",
			 function );

			goto on_error;
		}
		if( block_type == LIBFSAPFS_DEFLATE_BLOCK_TYPE_HUFFMAN_FIXED )
		{
			if( ( fixed_huffman_literals_tree == NULL )
			 && ( fixed_huffman_distances_tree == NULL ) )
			{
				if( libfsapfs_huffman_tree_initialize(
				     &fixed_huffman_literals_tree,
				     288,
				     15,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to build fixed literals Huffman tree.",
					 function );

					goto on_error;
				}
				if( libfsapfs_huffman_tree_initialize(
				     &fixed_huffman_distances_tree,
				     30,
				     15,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to build fixed distances Huffman tree.",
					 function );

					goto on_error;
				}
				if( libfsapfs_deflate_build_fixed_huffman_trees(
				     fixed_huffman_literals_tree,
				     fixed_huffman_distances_tree,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to build fixed Huffman trees.",
					 function );

					goto on_error;
				}
			}
		}
		if( libfsapfs_deflate_read_block(
		     bit_stream,
		     block_type,
		     fixed_huffman_literals_tree,
		     fixed_huffman_distances_tree,
		     uncompressed_data,
		     safe_uncompressed_data_size,
		     &uncompressed_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read block of compressed data.",
			 function );

			goto on_error;
		}
		if( last_block_flag != 0 )
		{
			break;
		}
	}
	if( fixed_huffman_distances_tree != NULL )
	{
		if( libfsapfs_huffman_tree_free(
		     &fixed_huffman_distances_tree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free fixed distances Huffman tree.",
			 function );

			goto on_error;
		}
	}
	if( fixed_huffman_literals_tree != NULL )
	{
		if( libfsapfs_huffman_tree_free(
		     &fixed_huffman_literals_tree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free fixed literals Huffman tree.",
			 function );

			goto on_error;
		}
	}
	if( libfsapfs_bit_stream_free(
	     &bit_stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free bit stream.",
		 function );

		goto on_error;
	}
	*uncompressed_data_size = uncompressed_data_offset;

	return( 1 );

on_error:
	if( fixed_huffman_distances_tree != NULL )
	{
		libfsapfs_huffman_tree_free(
		 &fixed_huffman_distances_tree,
		 NULL );
	}
	if( fixed_huffman_literals_tree != NULL )
	{
		libfsapfs_huffman_tree_free(
		 &fixed_huffman_literals_tree,
		 NULL );
	}
	if( bit_stream != NULL )
	{
		libfsapfs_bit_stream_free(
		 &bit_stream,
		 NULL );
	}
	return( -1 );
}

/* Decompresses data using zlib compression
 * Returns 1 on success or -1 on error
 */
int libfsapfs_deflate_decompress_zlib(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	libfsapfs_bit_stream_t *bit_stream                     = NULL;
	libfsapfs_huffman_tree_t *fixed_huffman_distances_tree = NULL;
	libfsapfs_huffman_tree_t *fixed_huffman_literals_tree  = NULL;
	static char *function                                = "libfsapfs_deflate_decompress_zlib";
	size_t compressed_data_offset                        = 0;
	size_t safe_uncompressed_data_size                   = 0;
	size_t uncompressed_data_offset                      = 0;
	uint32_t calculated_checksum                         = 0;
	uint32_t stored_checksum                             = 0;
	uint8_t block_type                                   = 0;
	uint8_t last_block_flag                              = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	safe_uncompressed_data_size = *uncompressed_data_size;

	if( safe_uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfsapfs_deflate_read_data_header(
	     compressed_data,
	     compressed_data_size,
	     &compressed_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data header.",
		 function );

		goto on_error;
	}
	if( compressed_data_offset >= compressed_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid compressed data value too small.",
		 function );

		goto on_error;
	}
	if( libfsapfs_bit_stream_initialize(
	     &bit_stream,
	     compressed_data,
	     compressed_data_size,
	     compressed_data_offset,
	     LIBFSAPFS_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create bit stream.",
		 function );

		goto on_error;
	}
	while( bit_stream->byte_stream_offset < bit_stream->byte_stream_size )
	{
		if( libfsapfs_deflate_read_block_header(
		     bit_stream,
		     &block_type,
		     &last_block_flag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read compressed data block header.",
			 function );

			goto on_error;
		}
		if( block_type == LIBFSAPFS_DEFLATE_BLOCK_TYPE_HUFFMAN_FIXED )
		{
			if( ( fixed_huffman_literals_tree == NULL )
			 && ( fixed_huffman_distances_tree == NULL ) )
			{
				if( libfsapfs_huffman_tree_initialize(
				     &fixed_huffman_literals_tree,
				     288,
				     15,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to build fixed literals Huffman tree.",
					 function );

					goto on_error;
				}
				if( libfsapfs_huffman_tree_initialize(
				     &fixed_huffman_distances_tree,
				     30,
				     15,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to build fixed distances Huffman tree.",
					 function );

					goto on_error;
				}
				if( libfsapfs_deflate_build_fixed_huffman_trees(
				     fixed_huffman_literals_tree,
				     fixed_huffman_distances_tree,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to build fixed Huffman trees.",
					 function );

					goto on_error;
				}
			}
		}
		if( libfsapfs_deflate_read_block(
		     bit_stream,
		     block_type,
		     fixed_huffman_literals_tree,
		     fixed_huffman_distances_tree,
		     uncompressed_data,
		     safe_uncompressed_data_size,
		     &uncompressed_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read block of compressed data.",
			 function );

			goto on_error;
		}
		if( last_block_flag != 0 )
		{
			break;
		}
	}
	if( ( bit_stream->byte_stream_size - bit_stream->byte_stream_offset ) >= 4 )
	{
		while( bit_stream->bit_buffer_size >= 8 )
		{
			bit_stream->byte_stream_offset -= 1;
			bit_stream->bit_buffer_size    -= 8;
		}
		byte_stream_copy_to_uint32_big_endian(
		 &( bit_stream->byte_stream[ bit_stream->byte_stream_offset ] ),
		 stored_checksum );

		if( libfsapfs_deflate_calculate_adler32(
		     &calculated_checksum,
		     uncompressed_data,
		     uncompressed_data_offset,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate checksum.",
			 function );

			goto on_error;
		}
		if( stored_checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
			 function,
			 stored_checksum,
			 calculated_checksum );

			goto on_error;
		}
	}
	if( fixed_huffman_distances_tree != NULL )
	{
		if( libfsapfs_huffman_tree_free(
		     &fixed_huffman_distances_tree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free fixed distances Huffman tree.",
			 function );

			goto on_error;
		}
	}
	if( fixed_huffman_literals_tree != NULL )
	{
		if( libfsapfs_huffman_tree_free(
		     &fixed_huffman_literals_tree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free fixed literals Huffman tree.",
			 function );

			goto on_error;
		}
	}
	if( libfsapfs_bit_stream_free(
	     &bit_stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free bit stream.",
		 function );

		goto on_error;
	}
	*uncompressed_data_size = uncompressed_data_offset;

	return( 1 );

on_error:
	if( fixed_huffman_distances_tree != NULL )
	{
		libfsapfs_huffman_tree_free(
		 &fixed_huffman_distances_tree,
		 NULL );
	}
	if( fixed_huffman_literals_tree != NULL )
	{
		libfsapfs_huffman_tree_free(
		 &fixed_huffman_literals_tree,
		 NULL );
	}
	if( bit_stream != NULL )
	{
		libfsapfs_bit_stream_free(
		 &bit_stream,
		 NULL );
	}
	return( -1 );
}

