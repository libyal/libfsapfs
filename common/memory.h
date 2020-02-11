/*
 * Memory functions
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

#if !defined( _MEMORY_H )
#define _MEMORY_H

#include "common.h"

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

/* Memory allocation
 */
#if defined( HAVE_GLIB_H )
#define memory_allocate( size ) \
	g_malloc( (gsize) size )

#elif defined( WINAPI )
#define memory_allocate( size ) \
	HeapAlloc( GetProcessHeap(), 0, (SIZE_T) size )

#elif defined( HAVE_MALLOC )
#define memory_allocate( size ) \
	malloc( size )
#endif

#define memory_allocate_structure( type ) \
	(type *) memory_allocate( sizeof( type ) )

#define memory_allocate_structure_as_value( type ) \
	(intptr_t *) memory_allocate( sizeof( type ) )

/* Memory reallocation
 */
#if defined( HAVE_GLIB_H )
#define memory_reallocate( buffer, size ) \
	g_realloc( (gpointer) buffer, (gsize) size )

#elif defined( WINAPI )
/* HeapReAlloc does not allocate empty (NULL) buffers as realloc does
 */
#define memory_reallocate( buffer, size ) \
	( buffer == NULL ) ? \
	HeapAlloc( GetProcessHeap(), 0, (SIZE_T) size ) : \
	HeapReAlloc( GetProcessHeap(), 0, (LPVOID) buffer, (SIZE_T) size )

#elif defined( HAVE_REALLOC )
#define memory_reallocate( buffer, size ) \
	realloc( (void *) buffer, size )
#endif

/* Memory free
 */
#if defined( HAVE_GLIB_H )
#define memory_free( buffer ) \
	g_free( (gpointer) buffer )

#elif defined( WINAPI )
#define memory_free( buffer ) \
	( buffer == NULL ) ? TRUE : HeapFree( GetProcessHeap(), 0, (LPVOID) buffer )

#elif defined( HAVE_FREE )
#define memory_free( buffer ) \
	free( (void *) buffer )
#endif

/* Memory compare
 */
#if defined( HAVE_MEMCMP ) || defined( WINAPI )
#define memory_compare( buffer1, buffer2, size ) \
	memcmp( (const void *) buffer1, (const void *) buffer2, size )
#endif

/* Memory copy
 */
#if defined( HAVE_MEMCPY ) || defined( WINAPI )
#define memory_copy( destination, source, count ) \
	memcpy( (void *) destination, (void *) source, count )
#endif

/* Memory set
 */
#if defined( HAVE_MEMSET ) || defined( WINAPI )
#define memory_set( buffer, value, count ) \
	memset( (void *) buffer, (int) value, count )
#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MEMORY_H ) */

