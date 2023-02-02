/*
 * Huffman tree functions
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
#include <memory.h>
#include <types.h>

#include "libfsapfs_bit_stream.h"
#include "libfsapfs_huffman_tree.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

/* Creates a Huffman tree
 * Make sure the value huffman_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_huffman_tree_initialize(
     libfsapfs_huffman_tree_t **huffman_tree,
     int number_of_symbols,
     uint8_t maximum_code_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_huffman_tree_initialize";
	size_t array_size     = 0;

	if( huffman_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Huffman tree.",
		 function );

		return( -1 );
	}
	if( *huffman_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid Huffman tree value already set.",
		 function );

		return( -1 );
	}
	if( ( number_of_symbols < 0 )
	 || ( number_of_symbols > 1024 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of symbols value out of bounds.",
		 function );

		return( -1 );
	}
	if( maximum_code_size > 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid maximum code size value out of bounds.",
		 function );

		return( -1 );
	}
	*huffman_tree = memory_allocate_structure(
	                 libfsapfs_huffman_tree_t );

	if( *huffman_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create Huffman tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *huffman_tree,
	     0,
	     sizeof( libfsapfs_huffman_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear Huffman tree.",
		 function );

		memory_free(
		 *huffman_tree );

		*huffman_tree = NULL;

		return( -1 );
	}
	array_size = sizeof( uint16_t ) * number_of_symbols;

	( *huffman_tree )->symbols = (uint16_t *) memory_allocate(
	                                           array_size );

	if( ( *huffman_tree )->symbols == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create symbols.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *huffman_tree )->symbols,
	     0,
	     array_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear symbols.",
		 function );

		goto on_error;
	}
	array_size = sizeof( int ) * ( maximum_code_size + 1 );

	( *huffman_tree )->code_size_counts = (int *) memory_allocate(
	                                               array_size );

	if( ( *huffman_tree )->code_size_counts == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create code size counts.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *huffman_tree )->code_size_counts,
	     0,
	     array_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear code size counts.",
		 function );

		goto on_error;
	}
	( *huffman_tree )->maximum_code_size = maximum_code_size;

	return( 1 );

on_error:
	if( *huffman_tree != NULL )
	{
		if( ( *huffman_tree )->code_size_counts != NULL )
		{
			memory_free(
			 ( *huffman_tree )->code_size_counts );
		}
		if( ( *huffman_tree )->symbols != NULL )
		{
			memory_free(
			 ( *huffman_tree )->symbols );
		}
		memory_free(
		 *huffman_tree );

		*huffman_tree = NULL;
	}
	return( -1 );
}

/* Frees a Huffman tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_huffman_tree_free(
     libfsapfs_huffman_tree_t **huffman_tree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_huffman_tree_free";

	if( huffman_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Huffman tree.",
		 function );

		return( -1 );
	}
	if( *huffman_tree != NULL )
	{
		if( ( *huffman_tree )->code_size_counts != NULL )
		{
			memory_free(
			 ( *huffman_tree )->code_size_counts );
		}
		if( ( *huffman_tree )->symbols != NULL )
		{
			memory_free(
			 ( *huffman_tree )->symbols );
		}
		memory_free(
		 *huffman_tree );

		*huffman_tree = NULL;
	}
	return( 1 );
}

/* Builds the Huffman tree
 * Returns 1 on success, 0 if the tree is empty or -1 on error
 */
int libfsapfs_huffman_tree_build(
     libfsapfs_huffman_tree_t *huffman_tree,
     const uint8_t *code_sizes_array,
     int number_of_code_sizes,
     libcerror_error_t **error )
{
	int *symbol_offsets   = NULL;
	static char *function = "libfsapfs_huffman_tree_build";
	size_t array_size     = 0;
	uint16_t symbol       = 0;
	uint8_t bit_index     = 0;
	uint8_t code_size     = 0;
	int code_offset       = 0;
	int left_value        = 0;

	if( huffman_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Huffman tree.",
		 function );

		return( -1 );
	}
	if( code_sizes_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid code sizes array.",
		 function );

		return( -1 );
	}
	if( ( number_of_code_sizes < 0 )
	 || ( number_of_code_sizes > (int) INT16_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of code sizes value out of bounds.",
		 function );

		return( -1 );
	}
	/* Determine the code size frequencies
	 */
	array_size = sizeof( int ) * ( huffman_tree->maximum_code_size + 1 );

	if( memory_set(
	     huffman_tree->code_size_counts,
	     0,
	     array_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear code size counts.",
		 function );

		goto on_error;
	}
	for( symbol = 0;
	     symbol < (uint16_t) number_of_code_sizes;
	     symbol++ )
	{
		code_size = code_sizes_array[ symbol ];

		if( code_size > huffman_tree->maximum_code_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid symbol: %d code size: %" PRIu8 " value out of bounds.",
			 function,
			 symbol,
			 code_size );

			goto on_error;
		}
		huffman_tree->code_size_counts[ code_size ] += 1;
	}
	/* The tree has no codes
	 */
	if( huffman_tree->code_size_counts[ 0 ] == number_of_code_sizes )
	{
		return( 0 );
	}
	/* Check if the set of code sizes is incomplete or over-subscribed
	 */
	left_value = 1;

	for( bit_index = 1;
	     bit_index <= huffman_tree->maximum_code_size;
	     bit_index++ )
	{
		left_value <<= 1;
		left_value  -= huffman_tree->code_size_counts[ bit_index ];

		if( left_value < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: code sizes are over-subscribed.",
			 function );

			goto on_error;
		}
	}
/* TODO
	if( left_value > 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: code sizes are incomplete.",
		 function );

		goto on_error;
	}
*/
	symbol_offsets = (int *) memory_allocate(
	                          array_size );

	if( symbol_offsets == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create symbol offsets.",
		 function );

		goto on_error;
	}
	/* Calculate the offsets to sort the symbols per code size
	 */
	symbol_offsets[ 0 ] = 0;
	symbol_offsets[ 1 ] = 0;

	for( bit_index = 1;
	     bit_index < huffman_tree->maximum_code_size;
	     bit_index++ )
	{
		symbol_offsets[ bit_index + 1 ] = symbol_offsets[ bit_index ] + huffman_tree->code_size_counts[ bit_index ];
	}
	/* Fill the symbols sorted by code size
	 */
	for( symbol = 0;
	     symbol < (uint16_t) number_of_code_sizes;
	     symbol++ )
	{
		code_size = code_sizes_array[ symbol ];

		if( code_size == 0 )
		{
			continue;
		}
		code_offset = symbol_offsets[ code_size ];

		if( ( code_offset < 0 )
		 || ( code_offset > number_of_code_sizes ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid symbol: %" PRIu16 " code offset: %d value out of bounds.",
			 function,
			 symbol,
			 code_offset );

			goto on_error;
		}
		symbol_offsets[ code_size ] += 1;

		huffman_tree->symbols[ code_offset ] = symbol;
	}
	memory_free(
	 symbol_offsets );

	return( 1 );

on_error:
	if( symbol_offsets != NULL )
	{
		memory_free(
		 symbol_offsets );
	}
	return( -1 );
}

/* Retrieves a symbol based on the Huffman code read from the bit-stream
 * Returns 1 on success or -1 on error
 */
int libfsapfs_huffman_tree_get_symbol_from_bit_stream(
     libfsapfs_huffman_tree_t *huffman_tree,
     libfsapfs_bit_stream_t *bit_stream,
     uint16_t *symbol,
     libcerror_error_t **error )
{
	static char *function  = "libfsapfs_huffman_tree_get_symbol_from_bit_stream";
	uint32_t value_32bit   = 0;
	uint16_t safe_symbol   = 0;
	uint8_t bit_index      = 0;
	int code_size_count    = 0;
	int first_huffman_code = 0;
	int first_index        = 0;
	int huffman_code       = 0;
	int result             = 0;

	if( huffman_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Huffman tree.",
		 function );

		return( -1 );
	}
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
	if( symbol == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid symbol.",
		 function );

		return( -1 );
	}
/* TODO get maximum_code_size of bits into local bit buffer */

	for( bit_index = 1;
	     bit_index <= huffman_tree->maximum_code_size;
	     bit_index++ )
	{
		if( libfsapfs_bit_stream_get_value(
		     bit_stream,
		     1,
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
		huffman_code <<= 1;
		huffman_code  |= (int) value_32bit;

		code_size_count = huffman_tree->code_size_counts[ bit_index ];

		if( ( huffman_code - code_size_count ) < first_huffman_code )
		{
			safe_symbol = huffman_tree->symbols[ first_index + ( huffman_code - first_huffman_code ) ];

			result = 1;

			break;
		}
		first_huffman_code  += code_size_count;
		first_huffman_code <<= 1;
		first_index         += code_size_count;
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid Huffman code: 0x%08" PRIx32 ".",
		 function,
		 huffman_code );

		return( -1 );
	}
	*symbol = safe_symbol;

	return( 1 );
}

