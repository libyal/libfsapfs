/*
 * Wide character string functions
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

#if !defined( _WIDE_STRING_H )
#define _WIDE_STRING_H

#include "common.h"
#include "memory.h"
#include "types.h"

#if defined( HAVE_WCHAR_H ) || defined( WINAPI )
#include <wchar.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _WIDE_STRING_INTERMEDIATE( string ) \
	L ## string

#define _WIDE_STRING( string ) \
	_WIDE_STRING_INTERMEDIATE( string )

/* String allocation
 */
#define wide_string_allocate( size ) \
	(wchar_t *) memory_allocate( sizeof( wchar_t ) * ( size ) )

/* String reallocation
 */
#define wide_string_reallocate( string, size ) \
	(wchar_t *) memory_reallocate( string, ( sizeof( wchar_t ) * ( size ) ) )

/* String length
 */
#if defined( HAVE_WCSLEN ) || defined( WINAPI )
#define wide_string_length( string ) \
	wcslen( string )
#endif

/* String compare
 */
#if defined( HAVE_WMEMCMP )
#define wide_string_compare( string1, string2, size ) \
	wmemcmp( (void *) string1, (void *) string2, size )

#elif defined( HAVE_WCSNCMP ) || defined( WINAPI )
#define wide_string_compare( string1, string2, size ) \
	wcsncmp( string1, string2, size )
#endif

/* Caseless string compare
 */
#if defined( _MSC_VER ) || ( defined( __BORLANDC__ ) && ( __BORLANDC__ >= 0x0551 ) )
#define wide_string_compare_no_case( string1, string2, size ) \
	_wcsnicmp( string1, string2, size )

#elif ( defined( WINAPI ) && !defined( __CYGWIN__ ) ) || defined( HAVE_WCSNICMP )
#define wide_string_compare_no_case( string1, string2, size ) \
	wcsnicmp( string1, string2, size )

#elif defined( HAVE_WCSNCASECMP )
#define wide_string_compare_no_case( string1, string2, size ) \
	wcsncasecmp( string1, string2, size )

#elif defined( HAVE_WCSCASECMP )
#define wide_string_compare_no_case( string1, string2, size ) \
	wcscasecmp( string1, string2 )
#endif

/* String copy
 */
#if defined( HAVE_WMEMCPY )
#define wide_string_copy( destination, source, size ) \
	(wchar_t *) wmemcpy( (void *) destination, (void *) source, size )

#elif defined( HAVE_WCSNCPY ) || defined( WINAPI )
#define wide_string_copy( destination, source, size ) \
	wcsncpy( destination, source, size )
#endif

/* String character search
 */
#if defined( HAVE_WMEMCHR )
#define wide_string_search_character( string, character, size ) \
	(wchar_t *) wmemchr( (void *) string, (wchar_t) character, size )

#elif defined( HAVE_WCSCHR ) || defined( WINAPI )
#define wide_string_search_character( string, character, size ) \
	wcschr( string, (wchar_t) character )

#endif

/* String reverse character search
 */
#if defined( HAVE_WMEMRCHR )
#define wide_string_search_character_reverse( string, character, size ) \
	(wchar_t *) wmemrchr( (void *) string, (wchar_t) character, size )

#elif defined( HAVE_WCSRCHR ) || defined( WINAPI )
#define wide_string_search_character_reverse( string, character, size ) \
	wcsrchr( string, (wchar_t) character )
#endif

/* String sub-string search
 */
#if defined( HAVE_WCSSTR ) || defined( WINAPI )
#define wide_string_search_string( string, substring, size ) \
	wcsstr( string, substring )

#endif

/* String formatted print (snwprintf)
 */
#if defined( _MSC_VER )
#define wide_string_snwprintf( target, size, ... ) \
	swprintf_s( target, size, __VA_ARGS__ )

#elif defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define wide_string_snwprintf \
	snwprintf

#elif defined( WINAPI )
#define wide_string_snwprintf( target, size, ... ) \
	snwprintf( target, size, __VA_ARGS__ )

#elif defined( HAVE_SWPRINTF )
#define wide_string_snwprintf( target, size, ... ) \
	swprintf( target, size, __VA_ARGS__ )
#endif

/* Variable arguments formatted print to string function (vsnwprintf)
 */
#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define wide_string_vsnwprintf \
	_vsnwprintf

#elif defined( WINAPI )
#define wide_string_vsnwprintf( string, size, format, ... ) \
	_vsnwprintf( string, size, format, __VA_ARGS__ )

#elif defined( HAVE_VSWPRINTF )
#define wide_string_vsnwprintf( string, size, format, ... ) \
	vswprintf( string, size, format, __VA_ARGS__ )
#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _WIDE_STRING_H ) */

