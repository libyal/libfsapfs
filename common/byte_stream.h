/*
 * Byte stream functions
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

#if !defined( _BYTE_STREAM_H )
#define _BYTE_STREAM_H

#include "common.h"
#include "types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define _BYTE_STREAM_HOST_IS_ENDIAN_BIG		( *((uint32_t *) "\x01\x02\x03\x04" ) == 0x01020304 )
#define _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE	( *((uint32_t *) "\x01\x02\x03\x04" ) == 0x04030201 )
#define _BYTE_STREAM_HOST_IS_ENDIAN_MIDDLE	( *((uint32_t *) "\x01\x02\x03\x04" ) == 0x02010403 )

#define _BYTE_STREAM_ENDIAN_BIG			(uint8_t) 'b'
#define _BYTE_STREAM_ENDIAN_LITTLE		(uint8_t) 'l'
#define _BYTE_STREAM_ENDIAN_MIDDLE		(uint8_t) 'm'

typedef union byte_stream_float32
{
	float floating_point;
	uint32_t integer;

} byte_stream_float32_t;

typedef union byte_stream_float64
{
	double floating_point;
	uint64_t integer;

} byte_stream_float64_t;

#define byte_stream_copy_to_uint16_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ];

#define byte_stream_copy_to_uint16_little_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_to_uint24_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ];

#define byte_stream_copy_to_uint24_little_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_to_uint32_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ];

#define byte_stream_copy_to_uint32_little_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_to_uint48_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 4 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 5 ];

#define byte_stream_copy_to_uint48_little_endian( byte_stream, value ) \
	( value )  |= ( byte_stream )[ 5 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 4 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_to_uint64_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 4 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 5 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 6 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 7 ];

#define byte_stream_copy_to_uint64_little_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 7 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 6 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 5 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 4 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_from_uint16_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint16_little_endian( byte_stream, value ) \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint24_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint24_little_endian( byte_stream, value ) \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint32_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint32_little_endian( byte_stream, value ) \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint48_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 40 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 32 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 4 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 5 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint48_little_endian( byte_stream, value ) \
	( byte_stream )[ 5 ] = (uint8_t) ( ( ( value ) >> 40 ) & 0x0ff ); \
	( byte_stream )[ 4 ] = (uint8_t) ( ( ( value ) >> 32 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint64_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 56 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 48 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 40 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 32 ) & 0x0ff ); \
	( byte_stream )[ 4 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 5 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 6 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 7 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint64_little_endian( byte_stream, value ) \
	( byte_stream )[ 7 ] = (uint8_t) ( ( ( value ) >> 56 ) & 0x0ff ); \
	( byte_stream )[ 6 ] = (uint8_t) ( ( ( value ) >> 48 ) & 0x0ff ); \
	( byte_stream )[ 5 ] = (uint8_t) ( ( ( value ) >> 40 ) & 0x0ff ); \
	( byte_stream )[ 4 ] = (uint8_t) ( ( ( value ) >> 32 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_bit_rotate_left_8bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) << ( number_of_bits ) ) | ( ( byte_stream ) >> ( 8 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right_8bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) >> ( number_of_bits ) ) | ( ( byte_stream ) << ( 8 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_left_16bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) << ( number_of_bits ) ) | ( ( byte_stream ) >> ( 16 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right_16bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) >> ( number_of_bits ) ) | ( ( byte_stream ) << ( 16 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_left_32bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) << ( number_of_bits ) ) | ( ( byte_stream ) >> ( 32 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right_32bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) >> ( number_of_bits ) ) | ( ( byte_stream ) << ( 32 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_left_64bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) << ( number_of_bits ) ) | ( ( byte_stream ) >> ( 64 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right_64bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) >> ( number_of_bits ) ) | ( ( byte_stream ) << ( 64 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_left( value, number_of_bits ) \
	( ( ( value ) << ( number_of_bits ) ) | ( ( value ) >> ( ( sizeof( value ) << 3 ) - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right( value, number_of_bits ) \
	( ( ( value ) >> ( number_of_bits ) ) | ( ( value ) << ( ( sizeof( value ) << 3 ) - ( number_of_bits ) ) ) )

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _BYTE_STREAM_H ) */

