/*
 * System character string functions
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

#if !defined( _SYSTEM_STRING_H )
#define _SYSTEM_STRING_H

#include "common.h"
#include "narrow_string.h"
#include "types.h"
#include "wide_string.h"

#if defined( _cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

#if SIZEOF_WCHAR_T != 2
#error Unsupported wide system character size
#endif

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _SYSTEM_STRING_INTERMEDIATE( string ) \
	L ## string

#define _SYSTEM_STRING( string ) \
	_SYSTEM_STRING_INTERMEDIATE( string )

#define system_string_allocate( size ) \
	wide_string_allocate( size )

#define system_string_reallocate( string, size ) \
	wide_string_reallocate( string, size )

#define system_string_compare( destination, source, size ) \
	wide_string_compare( destination, source, size )

#define system_string_compare_no_case( destination, source, size ) \
	wide_string_compare_no_case( destination, source, size )

#define system_string_copy( destination, source, size ) \
	wide_string_copy( destination, source, size )

#define system_string_length( string ) \
	wide_string_length( string )

#define system_string_search_character( string, character, size ) \
	wide_string_search_character( string, character, size )

#define system_string_search_character_reverse( string, character, size ) \
	wide_string_search_character_reverse( string, character, size )

#define system_string_search_string( string, substring, size ) \
	wide_string_search_string( string, substring, size )

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define system_string_sprintf \
	wide_string_snwprintf

#else
#define system_string_sprintf( string, size, format, ... ) \
	wide_string_snwprintf( string, size, format, __VA_ARGS__ )
#endif

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define system_string_vsnprintf \
	wide_string_vsnwprintf

#else
#define system_string_vsnprintf( string, size, format, ... ) \
	wide_string_vsnwprintf( string, size, format, __VA_ARGS__ )
#endif

#else

#define _SYSTEM_STRING( string ) \
	string

#define system_string_allocate( size ) \
	narrow_string_allocate( size )

#define system_string_reallocate( string, size ) \
	narrow_string_reallocate( string, size )

#define system_string_compare( destination, source, size ) \
	narrow_string_compare( destination, source, size )

#define system_string_compare_no_case( destination, source, size ) \
	narrow_string_compare_no_case( destination, source, size )

#define system_string_copy( destination, source, size ) \
	narrow_string_copy( destination, source, size )

#define system_string_length( string ) \
	narrow_string_length( string )

#define system_string_search_character( string, character, size ) \
	narrow_string_search_character( string, character, size )

#define system_string_search_character_reverse( string, character, size ) \
	narrow_string_search_character_reverse( string, character, size )

#define system_string_search_string( string, substring, size ) \
	narrow_string_search_string( string, substring, size )

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define system_string_sprintf \
	narrow_string_snprintf

#else
#define system_string_sprintf( string, size, format, ... ) \
	narrow_string_snprintf( string, size, format, __VA_ARGS__ )
#endif

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define system_string_vsnprintf \
	narrow_string_vsnprintf

#else
#define system_string_vsnprintf( string, size, format, ... ) \
	narrow_string_vsnprintf( string, size, format, __VA_ARGS__ )
#endif

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

/* For backwards compatibility */
#define system_string_vsprintf system_string_vsnprintf

#if defined( _cplusplus )
}
#endif

#endif /* !defined( _SYSTEM_STRING_H ) */

