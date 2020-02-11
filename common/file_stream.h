/*
 * FILE stream functions
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

#if !defined( _FILE_STREAM_H )
#define _FILE_STREAM_H

#include "common.h"

#if defined( HAVE_GLIB_H )
#include <glib.h>
#include <glib/gstdio.h>
#endif

#include <stdio.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define FILE_STREAM_OPEN_APPEND			"a"
#define FILE_STREAM_OPEN_READ			"r"
#define FILE_STREAM_OPEN_WRITE			"w"

#if defined( WINAPI )
#define FILE_STREAM_BINARY_OPEN_APPEND		"ab"
#define FILE_STREAM_BINARY_OPEN_READ		"rb"
#define FILE_STREAM_BINARY_OPEN_WRITE		"wb"

#else
#define FILE_STREAM_BINARY_OPEN_APPEND		"a"
#define FILE_STREAM_BINARY_OPEN_READ		"r"
#define FILE_STREAM_BINARY_OPEN_WRITE		"w"

#endif

/* narrow character FILE stream open
 */
#if defined( HAVE_GLIB_H )
#define file_stream_open( filename, mode ) \
	g_fopen( filename, mode )

#elif defined( HAVE_FOPEN ) || defined( WINAPI )
#define file_stream_open( filename, mode ) \
	fopen( filename, mode )
#endif

/* wide character FILE stream open
 */
#if defined( WINAPI )
#define file_stream_open_wide( filename, mode ) \
	_wfopen( filename, mode )
#endif

/* FILE stream close
 */
#if defined( HAVE_FCLOSE ) || defined( WINAPI )
#define file_stream_close( stream ) \
	fclose( stream )
#endif

/* FILE stream read
 */
#if defined( HAVE_FREAD ) || defined( WINAPI )
#define file_stream_read( stream, data, size ) \
	fread( data, 1, size, stream )
#endif

/* FILE stream write
 */
#if defined( HAVE_FWRITE ) || defined( WINAPI )
#define file_stream_write( stream, data, size ) \
	fwrite( data, 1, size, stream )
#endif

/* FILE stream seek
 */
#if defined( WINAPI )
#define file_stream_seek_offset( stream, offset, whence ) \
	fseek( stream, offset, whence )

#elif defined( HAVE_FSEEKO )
#define file_stream_seek_offset( stream, offset, whence ) \
	fseeko( stream, offset, whence )

#elif defined( HAVE_FSEEKO64 )
#define file_stream_seek_offset( stream, offset, whence ) \
	fseeko64( stream, offset, whence )
#endif

/* End of FILE stream
 */
#if defined( HAVE_FEOF ) || defined( WINAPI )
#define file_stream_at_end( stream ) \
        feof( stream )
#endif

/* Get narrow character string from FILE stream
 */
#if defined( HAVE_FGETS ) || defined( WINAPI )
#define file_stream_get_string( stream, string, size ) \
        fgets( string, size, stream )
#endif

/* Get wide characters string from FILE stream
 */
#if defined( HAVE_FGETWS ) || defined( WINAPI )
#define file_stream_get_string_wide( stream, string, size ) \
        fgetws( string, size, stream )
#endif

/* Variable arguments formatted print to stream function
 */
#if defined( HAVE_GLIB_H )
#define file_stream_vfprintf( stream, format, ... ) \
	g_vfprintf( stream, format, __VA_ARGS__ )

/* Borland BCC previous to version 5.6.0 cannot handle the macro form: MACRO( ... )
 */
#elif defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define file_stream_vfprintf \
	vfprintf

#elif defined( HAVE_VFPRINTF ) || defined( WINAPI )
#define file_stream_vfprintf( stream, format, ... ) \
	vfprintf( stream, format, __VA_ARGS__ )
#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FILE_STREAM_H ) */

