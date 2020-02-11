/*
 * Mount file system
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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_SYS_STAT_H )
#include <sys/stat.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "fsapfstools_libcerror.h"
#include "fsapfstools_libcpath.h"
#include "fsapfstools_libfsapfs.h"
#include "fsapfstools_libuna.h"
#include "mount_file_system.h"

/* Creates a file system
 * Make sure the value file_system is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_initialize(
     mount_file_system_t **file_system,
     libcerror_error_t **error )
{
#if defined( WINAPI )
	FILETIME filetime;
	SYSTEMTIME systemtime;

#elif defined( HAVE_CLOCK_GETTIME )
	struct timespec time_structure;
#endif

	static char *function = "mount_file_system_initialize";

#if defined( WINAPI )
	DWORD error_code      = 0;
#else
	int64_t timestamp     = 0;
#endif

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system value already set.",
		 function );

		return( -1 );
	}
	*file_system = memory_allocate_structure(
	                mount_file_system_t );

	if( *file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system,
	     0,
	     sizeof( mount_file_system_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system.",
		 function );

		memory_free(
		 *file_system );

		*file_system = NULL;

		return( -1 );
	}
#if defined( WINAPI )
	if( memory_set(
	     &systemtime,
	     0,
	     sizeof( SYSTEMTIME ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear systemtime.",
		 function );

		goto on_error;
	}
	GetSystemTime(
	 &systemtime );

	if( SystemTimeToFileTime(
	     &systemtime,
	     &filetime ) == 0 )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 error_code,
		 "%s: unable to retrieve FILETIME of current time.",
		 function );

		goto on_error;
	}
	( *file_system )->mounted_timestamp = ( (uint64_t) filetime.dwHighDateTime << 32 ) | filetime.dwLowDateTime;

#elif defined( HAVE_CLOCK_GETTIME )
	if( clock_gettime(
	     CLOCK_REALTIME,
	     &time_structure ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current time structure.",
		 function );

		goto on_error;
	}
	timestamp = ( (int64_t) time_structure.tv_sec * 1000000000 ) + time_structure.tv_nsec;

	( *file_system )->mounted_timestamp = (uint64_t) timestamp;

#else
	timestamp = (int64_t) time( NULL );

	if( timestamp == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current time.",
		 function );

		goto on_error;
	}
	timestamp *= 1000000000;

	( *file_system )->mounted_timestamp = (uint64_t) timestamp;

#endif /* defined( HAVE_CLOCK_GETTIME ) */

	return( 1 );

on_error:
	if( *file_system != NULL )
	{
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( -1 );
}

/* Frees a file system
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_free(
     mount_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_free";
	int result            = 1;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
}

/* Signals the mount volume system to abort
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_signal_abort(
     mount_file_system_t *file_system,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_signal_abort";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
/* TODO implement */
	return( 1 );
}

/* Sets the volume
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_set_volume(
     mount_file_system_t *file_system,
     libfsapfs_volume_t *fsapfs_volume,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_set_volume";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	file_system->fsapfs_volume = fsapfs_volume;

	return( 1 );
}

/* Retrieves the volume
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_volume(
     mount_file_system_t *file_system,
     libfsapfs_volume_t **fsapfs_volume,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_get_volume";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( fsapfs_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	*fsapfs_volume = file_system->fsapfs_volume;

	return( 1 );
}

/* Retrieves the mounted timestamp
 * On Windows the timestamp is an unsigned 64-bit FILETIME timestamp
 * otherwise the timestamp is a signed 64-bit POSIX date and time value in number of nanoseconds
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_mounted_timestamp(
     mount_file_system_t *file_system,
     uint64_t *mounted_timestamp,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_get_mounted_timestamp";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( mounted_timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mounted timestamp.",
		 function );

		return( -1 );
	}
	*mounted_timestamp = file_system->mounted_timestamp;

	return( 1 );
}

/* Retrieves the file entry path from the path
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_file_entry_path_from_path(
     mount_file_system_t *file_system,
     const system_character_t *path,
     size_t path_length,
     system_character_t **file_entry_path,
     size_t *file_entry_path_size,
     libcerror_error_t **error )
{
	system_character_t *safe_file_entry_path     = NULL;
	static char *function                        = "mount_file_system_get_file_entry_path_from_path";
	libuna_unicode_character_t unicode_character = 0;
	system_character_t character                 = 0;
	system_character_t escape_character          = 0;
	system_character_t hex_digit                 = 0;
	system_character_t hex_value                 = 0;
	size_t file_entry_path_index                 = 0;
	size_t path_index                            = 0;
	size_t safe_file_entry_path_size             = 0;
	int result                                   = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	if( path_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path length.",
		 function );

		return( -1 );
	}
	if( path_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( file_entry_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry path.",
		 function );

		return( -1 );
	}
	if( file_entry_path_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry path size.",
		 function );

		return( -1 );
	}
	if( path[ 0 ] != (system_character_t) LIBCPATH_SEPARATOR )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path - path is not absolute.",
		 function );

		return( -1 );
	}
	*file_entry_path      = NULL;
	*file_entry_path_size = 0;

	safe_file_entry_path_size = path_length + 1;

	if( safe_file_entry_path_size > (size_t) ( SSIZE_MAX / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid file entry path size value exceeds maximum.",
		 function );

		goto on_error;
	}
	safe_file_entry_path = system_string_allocate(
	                        safe_file_entry_path_size );

	if( safe_file_entry_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file entry path.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	escape_character = (system_character_t) '^';
#else
	escape_character = (system_character_t) '\\';
#endif

	while( path_index < path_length )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_unicode_character_copy_from_utf16(
		          &unicode_character,
		          (libuna_utf16_character_t *) path,
		          path_length,
		          &path_index,
		          error );
#else
		result = libuna_unicode_character_copy_from_utf8(
		          &unicode_character,
		          (libuna_utf8_character_t *) path,
		          path_length,
		          &path_index,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from path.",
			 function );

			goto on_error;
		}
		/* On Windows replaces:
		 *   ^^ by ^
		 *   ^x5c by \
		 *   ^x## by values <= 0x1f and 0x7f
		 *
		 * On other platforms replaces:
		 *   \\ by \
		 *   \x2f by /
		 *   \x## by values <= 0x1f and 0x7f
		 *   / by \
		 */
		if( unicode_character == (libuna_unicode_character_t) escape_character )
		{
			if( ( path_index + 1 ) > path_length )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid path index value out of bounds.",
				 function );

				goto on_error;
			}
			character = path[ path_index++ ];

#if defined( WINAPI )
			if( ( character != escape_character )
			 && ( character != (system_character_t) 'X' )
			 && ( character != (system_character_t) 'x' ) )
#else
			if( ( character != escape_character )
			 && ( character != (system_character_t) 'x' ) )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported path - invalid character: %" PRIc_SYSTEM " after escape character.",
				 function,
				 character );

				goto on_error;
			}
			if( character == escape_character )
			{
				if( ( file_entry_path_index + 1 ) > safe_file_entry_path_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid file entry path index value out of bounds.",
					 function );

					goto on_error;
				}
				safe_file_entry_path[ file_entry_path_index++ ] = escape_character;
			}
			else
			{
				if( ( path_index + 2 ) > path_length )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid path index value out of bounds.",
					 function );

					goto on_error;
				}
				hex_digit = path[ path_index++ ];

				if( ( hex_digit >= (system_character_t) '0' )
				 && ( hex_digit <= (system_character_t) '9' ) )
				{
					hex_value = hex_digit - (system_character_t) '0';
				}
#if defined( WINAPI )
				else if( ( hex_digit >= (system_character_t) 'A' )
				      && ( hex_digit <= (system_character_t) 'F' ) )
				{
					hex_value = hex_digit - (system_character_t) 'A' + 10;
				}
#endif
				else if( ( hex_digit >= (system_character_t) 'a' )
				      && ( hex_digit <= (system_character_t) 'f' ) )
				{
					hex_value = hex_digit - (system_character_t) 'a' + 10;
				}
				else
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported path - invalid hexadecimal character: %" PRIc_SYSTEM " after escape character.",
					 function,
					 hex_digit );

					goto on_error;
				}
				hex_value <<= 4;

				hex_digit = path[ path_index++ ];

				if( ( hex_digit >= (system_character_t) '0' )
				 && ( hex_digit <= (system_character_t) '9' ) )
				{
					hex_value |= hex_digit - (system_character_t) '0';
				}
#if defined( WINAPI )
				else if( ( hex_digit >= (system_character_t) 'A' )
				      && ( hex_digit <= (system_character_t) 'F' ) )
				{
					hex_value = hex_digit - (system_character_t) 'A' + 10;
				}
#endif
				else if( ( hex_digit >= (system_character_t) 'a' )
				      && ( hex_digit <= (system_character_t) 'f' ) )
				{
					hex_value |= hex_digit - (system_character_t) 'a' + 10;
				}
				else
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported path - invalid hexadecimal character: %" PRIc_SYSTEM " after escape character.",
					 function,
					 hex_digit );

					goto on_error;
				}
#if defined( WINAPI )
				if( ( hex_value == 0 )
				 || ( ( hex_value > 0x1f )
				  &&  ( hex_value != 0x5c )
				  &&  ( hex_value != 0x7f ) ) )
#else
				if( ( hex_value == 0 )
				 || ( ( hex_value > 0x1f )
				  &&  ( hex_value != 0x2f )
				  &&  ( hex_value != 0x7f ) ) )
#endif
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid escaped character value out of bounds.",
					 function );

					goto on_error;
				}
				if( ( file_entry_path_index + 1 ) > safe_file_entry_path_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid file entry path index value out of bounds.",
					 function );

					goto on_error;
				}
				safe_file_entry_path[ file_entry_path_index++ ] = hex_value;
			}
		}
#if !defined( WINAPI )
		else if( unicode_character == (system_character_t) '\\' )
		{
			if( ( file_entry_path_index + 1 ) > safe_file_entry_path_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid file entry path index value out of bounds.",
				 function );

				goto on_error;
			}
			safe_file_entry_path[ file_entry_path_index++ ] = (system_character_t) '\\';
		}
#endif
		else
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_unicode_character_copy_to_utf16(
			          unicode_character,
			          (libuna_utf16_character_t *) safe_file_entry_path,
			          safe_file_entry_path_size,
			          &file_entry_path_index,
			          error );
#else
			result = libuna_unicode_character_copy_to_utf8(
			          unicode_character,
			          (libuna_utf8_character_t *) safe_file_entry_path,
			          safe_file_entry_path_size,
			          &file_entry_path_index,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy Unicode character to file entry path.",
				 function );

				goto on_error;
			}
		}
	}
	if( file_entry_path_index >= safe_file_entry_path_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file entry path index value out of bounds.",
		 function );

		goto on_error;
	}
	safe_file_entry_path[ file_entry_path_index ] = 0;

	*file_entry_path      = safe_file_entry_path;
	*file_entry_path_size = safe_file_entry_path_size;

	return( 1 );

on_error:
	if( safe_file_entry_path != NULL )
	{
		memory_free(
		 safe_file_entry_path );
	}
	return( -1 );
}
/* Retrieves the file entry of a specific path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int mount_file_system_get_file_entry_by_path(
     mount_file_system_t *file_system,
     const system_character_t *path,
     size_t path_length,
     libfsapfs_file_entry_t **fsapfs_file_entry,
     libcerror_error_t **error )
{
	system_character_t *file_entry_path = NULL;
	static char *function               = "mount_file_system_get_file_entry_by_path";
	size_t file_entry_path_length       = 0;
	size_t file_entry_path_size         = 0;
	int result                          = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( mount_file_system_get_file_entry_path_from_path(
	     file_system,
	     path,
	     path_length,
	     &file_entry_path,
	     &file_entry_path_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry path from path.",
		 function );

		goto on_error;
	}
	if( file_entry_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing file entry path.",
		 function );

		goto on_error;
	}
	/* Need to determine length here since size is based on the worst case
	 */
	file_entry_path_length = system_string_length(
	                          file_entry_path );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_volume_get_file_entry_by_utf16_path(
	          file_system->fsapfs_volume,
	          (uint16_t *) file_entry_path,
	          file_entry_path_length,
	          fsapfs_file_entry,
	          error );
#else
	result = libfsapfs_volume_get_file_entry_by_utf8_path(
	          file_system->fsapfs_volume,
	          (uint8_t *) file_entry_path,
	          file_entry_path_length,
	          fsapfs_file_entry,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry ",
		 function );

		goto on_error;
	}
	memory_free(
	 file_entry_path );

	return( result );

on_error:
	if( file_entry_path != NULL )
	{
		memory_free(
		 file_entry_path );
	}
	return( -1 );
}

/* Retrieves a filename from the name
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_filename_from_name(
     mount_file_system_t *file_system,
     const system_character_t *name,
     size_t name_length,
     system_character_t **filename,
     size_t *filename_size,
     libcerror_error_t **error )
{
	system_character_t *safe_filename            = NULL;
	static char *function                        = "mount_file_system_get_filename_from_name";
	libuna_unicode_character_t unicode_character = 0;
	system_character_t escape_character          = 0;
	system_character_t hex_digit                 = 0;
	size_t filename_index                        = 0;
	size_t name_index                            = 0;
	size_t safe_filename_size                    = 0;
	int result                                   = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename size.",
		 function );

		return( -1 );
	}
	*filename      = NULL;
	*filename_size = 0;

	safe_filename_size = ( name_length * 4 ) + 1;

	if( safe_filename_size > (size_t) ( SSIZE_MAX / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename size value exceeds maximum.",
		 function );

		goto on_error;
	}
	safe_filename = system_string_allocate(
	                 safe_filename_size );

	if( safe_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create filename.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	escape_character = (system_character_t) '^';
#else
	escape_character = (system_character_t) '\\';
#endif

	while( name_index < name_length )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_unicode_character_copy_from_utf16(
		          &unicode_character,
		          (libuna_utf16_character_t *) name,
		          name_length,
		          &name_index,
		          error );
#else
		result = libuna_unicode_character_copy_from_utf8(
		          &unicode_character,
		          (libuna_utf8_character_t *) name,
		          name_length,
		          &name_index,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from name.",
			 function );

			goto on_error;
		}
		if( unicode_character == 0 )
		{
			break;
		}
		/* On Windows replaces:
		 *   values <= 0x1f and 0x7f by ^x##
		 *   \ by ^x5c
		 *   ^ by ^^
		 *
		 * On other platforms replaces:
		 *   values <= 0x1f and 0x7f by \x##
		 *   / by \x2f
		 *   \ by \\
		 */
#if defined( WINAPI )
		if( ( unicode_character <= 0x1f )
		 || ( unicode_character == 0x5c )
		 || ( unicode_character == 0x7f ) )
#else
		if( ( unicode_character <= 0x1f )
		 || ( unicode_character == 0x2f )
		 || ( unicode_character == 0x7f ) )
#endif
		{
			if( ( filename_index + 4 ) > safe_filename_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid filename index value out of bounds.",
				 function );

				goto on_error;
			}
			safe_filename[ filename_index++ ] = escape_character;
			safe_filename[ filename_index++ ] = (system_character_t) 'x';

			hex_digit = unicode_character >> 4;

			if( hex_digit <= 0x09 )
			{
				safe_filename[ filename_index++ ] = (system_character_t) '0' + hex_digit;
			}
			else
			{
				safe_filename[ filename_index++ ] = (system_character_t) 'a' + hex_digit - 10;
			}
			hex_digit = unicode_character & 0x0f;

			if( hex_digit <= 0x09 )
			{
				safe_filename[ filename_index++ ] = (system_character_t) '0' + hex_digit;
			}
			else
			{
				safe_filename[ filename_index++ ] = (system_character_t) 'a' + hex_digit - 10;
			}
		}
		else if( unicode_character == (libuna_unicode_character_t) escape_character )
		{
			if( ( filename_index + 2 ) > safe_filename_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid filename index value out of bounds.",
				 function );

				goto on_error;
			}
			safe_filename[ filename_index++ ] = escape_character;
			safe_filename[ filename_index++ ] = escape_character;
		}
		else
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_unicode_character_copy_to_utf16(
			          unicode_character,
			          (libuna_utf16_character_t *) safe_filename,
			          safe_filename_size,
			          &filename_index,
			          error );
#else
			result = libuna_unicode_character_copy_to_utf8(
			          unicode_character,
			          (libuna_utf8_character_t *) safe_filename,
			          safe_filename_size,
			          &filename_index,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy Unicode character to filename.",
				 function );

				goto on_error;
			}
		}
	}
	if( filename_index >= safe_filename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename index value out of bounds.",
		 function );

		goto on_error;
	}
	safe_filename[ filename_index ] = 0;

	*filename      = safe_filename;
	*filename_size = safe_filename_size;

	return( 1 );

on_error:
	if( safe_filename != NULL )
	{
		memory_free(
		 safe_filename );
	}
	return( -1 );
}

/* Retrieves the filename from an file entry
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_filename_from_file_entry(
     mount_file_system_t *file_system,
     libfsapfs_file_entry_t *fsapfs_file_entry,
     system_character_t **filename,
     size_t *filename_size,
     libcerror_error_t **error )
{
	system_character_t *file_entry_name = NULL;
	static char *function               = "mount_file_system_get_filename_from_file_entry";
	size_t file_entry_name_size         = 0;
	int result                          = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_file_entry_get_utf16_name_size(
	          fsapfs_file_entry,
	          &file_entry_name_size,
	          error );
#else
	result = libfsapfs_file_entry_get_utf8_name_size(
	          fsapfs_file_entry,
	          &file_entry_name_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name size.",
		 function );

		goto on_error;
	}
	if( ( file_entry_name_size == 0 )
	 || ( file_entry_name_size > SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file entry name size value out of bounds.",
		 function );

		goto on_error;
	}
	file_entry_name = system_string_allocate(
	                   file_entry_name_size );

	if( file_entry_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file entry name string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_file_entry_get_utf16_name(
	          fsapfs_file_entry,
	          (uint16_t *) file_entry_name,
	          file_entry_name_size,
	          error );
#else
	result = libfsapfs_file_entry_get_utf8_name(
	          fsapfs_file_entry,
	          (uint8_t *) file_entry_name,
	          file_entry_name_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name.",
		 function );

		goto on_error;
	}
	if( mount_file_system_get_filename_from_name(
	     file_system,
	     file_entry_name,
	     file_entry_name_size - 1,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve a filename from the file entry name.",
		 function );

		goto on_error;
	}
	memory_free(
	 file_entry_name );

	return( 1 );

on_error:
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

