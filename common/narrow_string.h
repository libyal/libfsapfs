/*
 * Narrow character string functions
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

#if !defined( _NARROW_STRING_H )
#define _NARROW_STRING_H

#include "common.h"
#include "memory.h"
#include "types.h"

#if defined( HAVE_GLIB_H )
#include <glib.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* String allocation
 */
#define narrow_string_allocate( size ) \
	(char *) memory_allocate( sizeof( char ) * ( size ) )

/* String reallocation
 */
#define narrow_string_reallocate( string, size ) \
	(char *) memory_reallocate( string, ( sizeof( char ) * ( size ) ) )

/* String length
 */
#if defined( HAVE_STRLEN ) || defined( WINAPI )
#define narrow_string_length( string ) \
	strlen( string )
#endif

/* String compare
 */
#if defined( HAVE_MEMCMP ) || defined( WINAPI )
#define narrow_string_compare( string1, string2, size ) \
	memcmp( (void *) string1, (void *) string2, size )

#elif defined( HAVE_STRNCMP )
#define narrow_string_compare( string1, string2, size ) \
	strncmp( string1, string2, size )
#endif

/* Caseless string compare
 */
#if defined( HAVE_GLIB_H )
#define narrow_string_compare_no_case( string1, string2, size ) \
	g_ascii_strncasecmp( string1, string2, size )

#elif defined( _MSC_VER )
#define narrow_string_compare_no_case( string1, string2, size ) \
	_strnicmp( string1, string2, size )

#elif ( defined( WINAPI ) && !defined( __CYGWIN__ ) ) || defined( HAVE_STRNICMP )
#define narrow_string_compare_no_case( string1, string2, size ) \
	strnicmp( string1, string2, size )

#elif defined( HAVE_STRNCASECMP )
#define narrow_string_compare_no_case( string1, string2, size ) \
	strncasecmp( string1, string2, size )

#elif defined( HAVE_STRCASECMP )
#define narrow_string_compare_no_case( string1, string2, size ) \
	strcasecmp( string1, string2 )
#endif

/* String copy
 */
#if defined( HAVE_MEMCPY ) || defined( WINAPI )
#define narrow_string_copy( destination, source, size ) \
	(char *) memcpy( (void *) destination, (void *) source, size )

#elif defined( HAVE_STRNCPY )
#define narrow_string_copy( destination, source, size ) \
	strncpy( destination, source, size )
#endif

/* String character search
 */
#if defined( HAVE_MEMCHR ) || defined( WINAPI )
#define narrow_string_search_character( string, character, size ) \
	(char *) memchr( (void *) string, (int) character, size )

#elif defined( HAVE_STRCHR )
#define narrow_string_search_character( string, character, size ) \
	strchr( string, (int) character )
#endif

/* String reverse character search
 */
#if defined( HAVE_MEMRCHR ) && ( HAVE_DECL_MEMRCHR == 1 )
#define narrow_string_search_character_reverse( string, character, size ) \
	(char *) memrchr( (void *) string, (int) character, size )

#elif defined( HAVE_STRRCHR ) || defined( WINAPI )
#define narrow_string_search_character_reverse( string, character, size ) \
	strrchr( string, (int) character )
#endif

/* String sub-string search
 */
#if defined( HAVE_STRSTR ) || defined( WINAPI )
#define narrow_string_search_string( string, substring, size ) \
	strstr( string, substring )
#endif

/* String formatted print (snprintf)
 */
#if defined( HAVE_GLIB_H )
#define narrow_string_snprintf( target, size, ... ) \
	g_snprintf( target, size, __VA_ARGS__ )

#elif defined( _MSC_VER )
#define narrow_string_snprintf( target, size, ... ) \
	sprintf_s( target, size, __VA_ARGS__ )

#elif defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define narrow_string_snprintf \
	snprintf

#elif defined( HAVE_SNPRINTF ) || defined( WINAPI )
#define narrow_string_snprintf( target, size, ... ) \
	snprintf( target, size, __VA_ARGS__ )
#endif

/* String input conversion (sscanf)
 */
#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define narrow_string_sscanf \
	sscanf

#elif defined( HAVE_SSCANF ) || defined( WINAPI )
#define narrow_string_sscanf( string, format, ... ) \
	sscanf( string, format, __VA_ARGS__ )
#endif

/* Variable arguments formatted print to string function (vsnprintf)
 */
#if defined( HAVE_GLIB_H )
#define narrow_string_vsnprintf( string, size, format, ... ) \
	g_vsnprintf( string, size, format, __VA_ARGS__ )

#elif defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define narrow_string_vsnprintf \
	vsnprintf

#elif defined( HAVE_VSNPRINTF ) || defined( WINAPI )
#define narrow_string_vsnprintf( string, size, format, ... ) \
	vsnprintf( string, size, format, __VA_ARGS__ )
#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _NARROW_STRING_H ) */

