/*
 * Info handle
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "digest_hash.h"
#include "fsapfstools_libbfio.h"
#include "fsapfstools_libcerror.h"
#include "fsapfstools_libclocale.h"
#include "fsapfstools_libcnotify.h"
#include "fsapfstools_libcpath.h"
#include "fsapfstools_libfdatetime.h"
#include "fsapfstools_libfguid.h"
#include "fsapfstools_libfsapfs.h"
#include "fsapfstools_libhmac.h"
#include "fsapfstools_libuna.h"
#include "info_handle.h"

#if !defined( LIBFSAPFS_HAVE_BFIO )

extern \
int libfsapfs_container_open_file_io_handle(
     libfsapfs_container_t *container,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsapfs_error_t **error );

#endif /* !defined( LIBFSAPFS_HAVE_BFIO ) */

#define DIGEST_HASH_STRING_SIZE_MD5	33
#define INFO_HANDLE_NOTIFY_STREAM	stdout

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int info_handle_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "info_handle_system_string_copy_from_64_bit_in_decimal";
	size_t string_index                = 0;
	system_character_t character_value = 0;
	uint8_t maximum_string_index       = 20;
	int8_t sign                        = 1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	*value_64bit = 0;

	if( string[ string_index ] == (system_character_t) '-' )
	{
		string_index++;
		maximum_string_index++;

		sign = -1;
	}
	else if( string[ string_index ] == (system_character_t) '+' )
	{
		string_index++;
		maximum_string_index++;
	}
	while( string_index < string_size )
	{
		if( string[ string_index ] == 0 )
		{
			break;
		}
		if( string_index > (size_t) maximum_string_index )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
			 "%s: string too large.",
			 function );

			return( -1 );
		}
		*value_64bit *= 10;

		if( ( string[ string_index ] >= (system_character_t) '0' )
		 && ( string[ string_index ] <= (system_character_t) '9' ) )
		{
			character_value = (system_character_t) ( string[ string_index ] - (system_character_t) '0' );
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: %" PRIc_SYSTEM " at index: %d.",
			 function,
			 string[ string_index ],
			 string_index );

			return( -1 );
		}
		*value_64bit += character_value;

		string_index++;
	}
	if( sign == -1 )
	{
		*value_64bit *= (uint64_t) -1;
	}
	return( 1 );
}

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     uint8_t calculate_md5,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		memory_free(
		 *info_handle );

		*info_handle = NULL;

		return( -1 );
	}
	if( libbfio_file_range_initialize(
	     &( ( *info_handle )->input_file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file IO handle.",
		 function );

		goto on_error;
	}
	if( libfsapfs_container_initialize(
	     &( ( *info_handle )->input_container ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input container.",
		 function );

		goto on_error;
	}
	( *info_handle )->calculate_md5 = calculate_md5;
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_file_io_handle != NULL )
		{
			libbfio_handle_free(
			 &( ( *info_handle )->input_file_io_handle ),
			 NULL );
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( libfsapfs_container_free(
		     &( ( *info_handle )->input_container ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input container.",
			 function );

			result = -1;
		}
		if( libbfio_handle_free(
		     &( ( *info_handle )->input_file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file IO handle.",
			 function );

			result = -1;
		}
		if( ( *info_handle )->bodyfile_stream != NULL )
		{
			if( file_stream_close(
			     ( *info_handle )->bodyfile_stream ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close bodyfile stream.",
				 function );

				result = -1;
			}
			( *info_handle )->bodyfile_stream = NULL;
		}
		if( ( *info_handle )->recovery_password != NULL )
		{
			if( memory_set(
			     ( *info_handle )->recovery_password,
			     0,
			     ( *info_handle )->recovery_password_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear recovery password.",
				 function );

				result = -1;
			}
			memory_free(
			 ( *info_handle )->recovery_password );

			( *info_handle )->recovery_password      = NULL;
			( *info_handle )->recovery_password_size = 0;
		}
		if( ( *info_handle )->user_password != NULL )
		{
			if( memory_set(
			     ( *info_handle )->user_password,
			     0,
			     ( *info_handle )->user_password_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear user password.",
				 function );

				result = -1;
			}
			memory_free(
			 ( *info_handle )->user_password );

			( *info_handle )->user_password      = NULL;
			( *info_handle )->user_password_size = 0;
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	info_handle->abort = 1;

	if( info_handle->input_container != NULL )
	{
		if( libfsapfs_container_signal_abort(
		     info_handle->input_container,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input container to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the bodyfile
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_bodyfile(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_bodyfile";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - bodyfile stream value already set.",
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
	info_handle->bodyfile_stream = file_stream_open(
	                                filename,
	                                "wb" );

	if( info_handle->bodyfile_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open bodyfile stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the file system index
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_file_system_index(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_file_system_index";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( info_handle_system_string_copy_from_64_bit_in_decimal(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	if( ( value_64bit == 0 )
	 || ( value_64bit > 100 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file system index value out of bounds.",
		 function );

		return( -1 );
	}
	info_handle->file_system_index = (int) value_64bit;

	return( 1 );
}

/* Sets the password
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_password";
	size_t string_length  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->user_password != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - user password value already set.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	info_handle->user_password_size = string_length + 1;

	info_handle->user_password = system_string_allocate(
	                              info_handle->user_password_size );

	if( info_handle->user_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create user password.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     info_handle->user_password,
	     string,
	     info_handle->user_password_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy user password.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( info_handle->user_password != NULL )
	{
		memory_free(
		 info_handle->user_password );

		info_handle->user_password = NULL;
	}
	info_handle->user_password_size = 0;

	return( -1 );
}

/* Sets the recovery password
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_recovery_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_recovery_password";
	size_t string_length  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->recovery_password != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - recovery password value already set.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	info_handle->recovery_password_size = string_length + 1;

	info_handle->recovery_password = system_string_allocate(
	                                  info_handle->recovery_password_size );

	if( info_handle->recovery_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create recovery password.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     info_handle->recovery_password,
	     string,
	     info_handle->recovery_password_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy recovery password.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( info_handle->recovery_password != NULL )
	{
		memory_free(
		 info_handle->recovery_password );

		info_handle->recovery_password = NULL;
	}
	info_handle->recovery_password_size = 0;

	return( -1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_volume_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( info_handle_system_string_copy_from_64_bit_in_decimal(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	info_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_open_input";
	size_t filename_length = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	filename_length = system_string_length(
	                   filename );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open set file name.",
		 function );

		return( -1 );
	}
	if( libbfio_file_range_set(
	     info_handle->input_file_io_handle,
	     info_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open set volume offset.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_open_file_io_handle(
	     info_handle->input_container,
	     info_handle->input_file_io_handle,
	     LIBFSAPFS_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input container.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_close(
	     info_handle->input_container,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input container.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Retrieves a specific volume from the container
 * Returns 1 if successful or -1 on error
 */
int info_handle_get_volume_by_index(
     info_handle_t *info_handle,
     int volume_index,
     libfsapfs_volume_t **volume,
     libcerror_error_t **error )
{
	static char *function = "info_handle_get_volume_by_index";
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume value already set.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_get_volume_by_index(
	     info_handle->input_container,
	     volume_index,
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume: %d.",
		 function,
		 volume_index );

		goto on_error;
	}
	if( info_handle->user_password != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libfsapfs_volume_set_utf16_password(
		     *volume,
		     (uint16_t *) info_handle->user_password,
		     info_handle->user_password_size - 1,
		     error ) != 1 )
#else
		if( libfsapfs_volume_set_utf8_password(
		     *volume,
		     (uint8_t *) info_handle->user_password,
		     info_handle->user_password_size - 1,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set password.",
			 function );

			goto on_error;
		}
	}
	if( info_handle->recovery_password != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libfsapfs_volume_set_utf16_recovery_password(
		     *volume,
		     (uint16_t *) info_handle->recovery_password,
		     info_handle->recovery_password_size - 1,
		     error ) != 1 )
#else
		if( libfsapfs_volume_set_utf8_recovery_password(
		     *volume,
		     (uint8_t *) info_handle->recovery_password,
		     info_handle->recovery_password_size - 1,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovery password.",
			 function );

			goto on_error;
		}
	}
	result = libfsapfs_volume_is_locked(
	          *volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if volume is locked.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( info_handle->container_is_locked == 0 )
		{
			if( libfsapfs_volume_unlock(
			     *volume,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to unlock volume.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( *volume != NULL )
	{
		libfsapfs_volume_free(
		 volume,
		 NULL );
	}
	return( -1 );
}

/* Calculates the MD5 of the contents of a file entry
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_entry_calculate_md5(
     info_handle_t *info_handle,
     libfsapfs_file_entry_t *file_entry,
     char *md5_string,
     size_t md5_string_size,
     libcerror_error_t **error )
{
	uint8_t md5_hash[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t read_buffer[ 4096 ];

	libhmac_md5_context_t *md5_context = NULL;
	static char *function              = "info_handle_file_entry_calculate_md5";
	size64_t data_size                 = 0;
	size_t read_size                   = 0;
	ssize_t read_count                 = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsapfs_file_entry_get_size(
	     file_entry,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_seek_offset(
	     file_entry,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset: 0 in file entry.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_initialize(
	     &md5_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize MD5 context.",
		 function );

		goto on_error;
	}
	while( data_size > 0 )
	{
		read_size = 4096;

		if( (size64_t) read_size > data_size )
		{
			read_size = (size_t) data_size;
		}
		read_count = libfsapfs_file_entry_read_buffer(
		              file_entry,
		              read_buffer,
		              read_size,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from file entry.",
			 function );

			goto on_error;
		}
		data_size -= read_size;

		if( libhmac_md5_update(
		     md5_context,
		     read_buffer,
		     read_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update MD5 hash.",
			 function );

			goto on_error;
		}
	}
	if( libhmac_md5_finalize(
	     md5_context,
	     md5_hash,
	     LIBHMAC_MD5_HASH_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize MD5 hash.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_free(
	     &md5_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MD5 context.",
		 function );

		goto on_error;
	}
	if( digest_hash_copy_to_string(
	     md5_hash,
	     LIBHMAC_MD5_HASH_SIZE,
	     md5_string,
	     md5_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set MD5 hash string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( md5_context != NULL )
	{
		libhmac_md5_free(
		 &md5_context,
		 NULL );
	}
	return( -1 );
}

/* Prints a file entry or data stream name
 * Returns 1 if successful or -1 on error
 */
int info_handle_name_value_fprint(
     info_handle_t *info_handle,
     const system_character_t *value_string,
     size_t value_string_length,
     libcerror_error_t **error )
{
	system_character_t *escaped_value_string     = NULL;
	static char *function                        = "info_handle_name_value_fprint";
	libuna_unicode_character_t unicode_character = 0;
	size_t escaped_value_string_index            = 0;
	size_t escaped_value_string_size             = 0;
	size_t value_string_index                    = 0;
	int print_count                              = 0;
	int result                                   = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string.",
		 function );

		return( -1 );
	}
	/* To ensure normalization in the escaped string is handled correctly
	 * it stored in a temporary variable. Note that there is a worst-case of
	 * a 1 to 4 ratio for each escaped character.
	 */
	if( value_string_length > (size_t) ( ( SSIZE_MAX - 1 ) / ( sizeof( system_character_t ) * 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	escaped_value_string_size = ( value_string_length * 4 ) + 1;

	escaped_value_string = (system_character_t *) memory_allocate(
	                                               sizeof( system_character_t ) * escaped_value_string_size );

	if( escaped_value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create escaped value string.",
		 function );

		goto on_error;
	}
	while( value_string_index < value_string_length )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_unicode_character_copy_from_utf16(
		          &unicode_character,
		          (libuna_utf16_character_t *) value_string,
		          value_string_length,
		          &value_string_index,
		          error );
#else
		result = libuna_unicode_character_copy_from_utf8(
		          &unicode_character,
		          (libuna_utf8_character_t *) value_string,
		          value_string_length,
		          &value_string_index,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from value string.",
			 function );

			goto on_error;
		}
		/* Replace:
		 *   Control characters ([U+0-U+1f, U+7f-U+9f]) by \x##
		 */
		if( ( unicode_character <= 0x1f )
		 || ( ( unicode_character >= 0x7f )
		  &&  ( unicode_character <= 0x9f ) ) )
		{
			print_count = system_string_sprintf(
			               &( escaped_value_string[ escaped_value_string_index ] ),
			               escaped_value_string_size - escaped_value_string_index,
			               "\\x%02" PRIx32 "",
			               unicode_character );

			if( print_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy escaped Unicode character to escaped value string.",
				 function );

				goto on_error;
			}
			escaped_value_string_index += print_count;
		}
		else
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_unicode_character_copy_to_utf16(
			          unicode_character,
			          (libuna_utf16_character_t *) escaped_value_string,
			          escaped_value_string_size,
			          &escaped_value_string_index,
			          error );
#else
			result = libuna_unicode_character_copy_to_utf8(
			          unicode_character,
			          (libuna_utf8_character_t *) escaped_value_string,
			          escaped_value_string_size,
			          &escaped_value_string_index,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy Unicode character to escaped value string.",
				 function );

				goto on_error;
			}
		}
	}
	escaped_value_string[ escaped_value_string_index ] = 0;

	if( info_handle->bodyfile_stream != NULL )
	{
		fprintf(
		 info_handle->bodyfile_stream,
		 "%" PRIs_SYSTEM "",
		 escaped_value_string );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 escaped_value_string );
	}
	memory_free(
	 escaped_value_string );

	return( 1 );

on_error:
	if( escaped_value_string != NULL )
	{
		memory_free(
		 escaped_value_string );
	}
	return( -1 );
}

/* Prints a nano seconds POSIX time value
 * Returns 1 if successful or -1 on error
 */
int info_handle_posix_time_in_nano_seconds_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     int64_t value_64bit,
     libcerror_error_t **error )
{
	system_character_t date_time_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	static char *function                 = "info_handle_posix_time_in_nano_seconds_value_fprint";
	int result                            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "%s: Not set (0)\n",
		 value_name );
	}
	else
	{
		if( libfdatetime_posix_time_initialize(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create POSIX time.",
			 function );

			goto on_error;
		}
		if( libfdatetime_posix_time_copy_from_64bit(
		     posix_time,
		     (uint64_t) value_64bit,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_NANO_SECONDS_64BIT_SIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time from 64-bit.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_posix_time_copy_to_utf16_string(
			  posix_time,
			  (uint16_t *) date_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_ISO8601 | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_posix_time_copy_to_utf8_string(
			  posix_time,
			  (uint8_t *) date_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_ISO8601 | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%s: %" PRIs_SYSTEM "Z\n",
		 value_name,
		 date_time_string );

		if( libfdatetime_posix_time_free(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free POSIX time.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	return( -1 );
}

/* Prints an UUID value
 * Returns 1 if successful or -1 on error
 */
int info_handle_uuid_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     const uint8_t *uuid_data,
     libcerror_error_t **error )
{
	system_character_t uuid_string[ 48 ];

	libfguid_identifier_t *uuid = NULL;
	static char *function       = "info_handle_uuid_value_fprint";
	int result                  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfguid_identifier_initialize(
	     &uuid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create UUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     uuid,
	     uuid_data,
	     16,
	     LIBFGUID_ENDIAN_BIG,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to UUID.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfguid_identifier_copy_to_utf16_string(
		  uuid,
		  (uint16_t *) uuid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#else
	result = libfguid_identifier_copy_to_utf8_string(
		  uuid,
		  (uint8_t *) uuid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UUID to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "%s: %" PRIs_SYSTEM "\n",
	 value_name,
	 uuid_string );

	if( libfguid_identifier_free(
	     &uuid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free UUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( uuid != NULL )
	{
		libfguid_identifier_free(
		 &uuid,
		 NULL );
	}
	return( -1 );
}

/* Prints the compatible features flags to the notify stream
 */
void info_handle_compatible_features_flags_fprint(
      uint64_t compatible_features_flags,
      FILE *notify_stream )
{
	if( ( compatible_features_flags & 0x0000000000000001 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(NX_FEATURE_DEFRAG)\n" );
	}
	if( ( compatible_features_flags & 0x0000000000000002 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(NX_FEATURE_LCFD)\n" );
	}
	fprintf(
	 notify_stream,
	 "\n" );
}

/* Prints the incompatible features flags to the notify stream
 */
void info_handle_incompatible_features_flags_fprint(
      uint64_t incompatible_features_flags,
      FILE *notify_stream )
{
	if( ( incompatible_features_flags & 0x0000000000000001 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(NX_INCOMPAT_VERSION1)\n" );
	}
	if( ( incompatible_features_flags & 0x0000000000000002 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(NX_INCOMPAT_VERSION2)\n" );
	}

	if( ( incompatible_features_flags & 0x0000000000000100 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(NX_INCOMPAT_FUSION)\n" );
	}
	fprintf(
	 notify_stream,
	 "\n" );
}

/* Prints the read-only compatible features flags to the notify stream
 */
void info_handle_read_only_compatible_features_flags_fprint(
      uint64_t read_only_compatible_features_flags,
      FILE *notify_stream )
{
	/* Currently there are no container read-only compatible feature flags defined */

	fprintf(
	 notify_stream,
	 "\n" );
}

/* Prints a file entry value
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_file_entry_value_fprint(
     info_handle_t *info_handle,
     libfsapfs_file_entry_t *file_entry,
     const system_character_t *path,
     size_t path_length,
     libcerror_error_t **error )
{
	system_character_t *file_entry_name = NULL;
	static char *function               = "info_handle_file_entry_value_fprint";
	size_t file_entry_name_length       = 0;
	size_t file_entry_name_size         = 0;
	int result                          = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_file_entry_get_utf16_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#else
	result = libfsapfs_file_entry_get_utf8_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( file_entry_name_size > 0 ) )
	{
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
		          file_entry,
		          (uint16_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#else
		result = libfsapfs_file_entry_get_utf8_name(
		          file_entry,
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
			 "%s: unable to retrieve file entry name string.",
			 function );

			goto on_error;
		}
		file_entry_name_length = file_entry_name_size - 1;
	}
	if( info_handle_file_entry_value_with_name_fprint(
	     info_handle,
	     file_entry,
	     path,
	     path_length,
	     file_entry_name,
	     file_entry_name_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print file entry.",
		 function );

		goto on_error;
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );

		file_entry_name = NULL;
	}
	return( 1 );

on_error:
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

/* Prints a file entry value with name
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_file_entry_value_with_name_fprint(
     info_handle_t *info_handle,
     libfsapfs_file_entry_t *file_entry,
     const system_character_t *path,
     size_t path_length,
     const system_character_t *file_entry_name,
     size_t file_entry_name_length,
     libcerror_error_t **error )
{
	char md5_string[ DIGEST_HASH_STRING_SIZE_MD5 ]    = {
		'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
		'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
		0 };

	char file_mode_string[ 11 ]                        = { '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 0 };

	libfsapfs_extended_attribute_t *extended_attribute = NULL;
	system_character_t *extended_attribute_name        = NULL;
	system_character_t *symbolic_link_target           = NULL;
	static char *function                              = "info_handle_file_entry_value_with_name_fprint";
	size64_t size                                      = 0;
	size_t extended_attribute_name_size                = 0;
	size_t symbolic_link_target_size                   = 0;
	uint64_t file_entry_identifier                     = 0;
	int64_t access_time                                = 0;
	int64_t added_time                                 = 0;
	int64_t creation_time                              = 0;
	int64_t inode_change_time                          = 0;
	int64_t modification_time                          = 0;
	uint32_t group_identifier                          = 0;
	uint32_t owner_identifier                          = 0;
	uint16_t file_mode                                 = 0;
	int extended_attribute_index                       = 0;
	int number_of_extended_attributes                  = 0;
	int result                                         = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsapfs_file_entry_get_identifier(
	     file_entry,
	     &file_entry_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_get_creation_time(
	     file_entry,
	     &creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_get_modification_time(
	     file_entry,
	     &modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_get_inode_change_time(
	     file_entry,
	     &inode_change_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode change time.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_get_access_time(
	     file_entry,
	     &access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_get_owner_identifier(
	     file_entry,
	     &owner_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve owner identifier.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_get_group_identifier(
	     file_entry,
	     &group_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve group identifier.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_get_file_mode(
	     file_entry,
	     &file_mode,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file mode.",
		 function );

		goto on_error;
	}
/* TODO move into function */
	if( ( file_mode & 0x0001 ) != 0 )
	{
		file_mode_string[ 9 ] = 'x';
	}
	if( ( file_mode & 0x0002 ) != 0 )
	{
		file_mode_string[ 8 ] = 'w';
	}
	if( ( file_mode & 0x0004 ) != 0 )
	{
		file_mode_string[ 7 ] = 'r';
	}
	if( ( file_mode & 0x0008 ) != 0 )
	{
		file_mode_string[ 6 ] = 'x';
	}
	if( ( file_mode & 0x0010 ) != 0 )
	{
		file_mode_string[ 5 ] = 'w';
	}
	if( ( file_mode & 0x0020 ) != 0 )
	{
		file_mode_string[ 4 ] = 'r';
	}
	if( ( file_mode & 0x0040 ) != 0 )
	{
		file_mode_string[ 3 ] = 'x';
	}
	if( ( file_mode & 0x0080 ) != 0 )
	{
		file_mode_string[ 2 ] = 'w';
	}
	if( ( file_mode & 0x0100 ) != 0 )
	{
		file_mode_string[ 1 ] = 'r';
	}
	switch( file_mode & 0xf000 )
	{
		case 0x1000:
			file_mode_string[ 0 ] = 'p';
			break;

		case 0x2000:
			file_mode_string[ 0 ] = 'c';
			break;

		case 0x4000:
			file_mode_string[ 0 ] = 'd';
			break;

		case 0x6000:
			file_mode_string[ 0 ] = 'b';
			break;

		case 0xa000:
			file_mode_string[ 0 ] = 'l';
			break;

		case 0xc000:
			file_mode_string[ 0 ] = 's';
			break;

		default:
			break;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_file_entry_get_utf16_symbolic_link_target_size(
	          file_entry,
	          &symbolic_link_target_size,
	          error );
#else
	result = libfsapfs_file_entry_get_utf8_symbolic_link_target_size(
	          file_entry,
	          &symbolic_link_target_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve symbolic link target string size.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		symbolic_link_target = system_string_allocate(
		                        symbolic_link_target_size );

		if( symbolic_link_target == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create symbolic link target string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsapfs_file_entry_get_utf16_symbolic_link_target(
		          file_entry,
		          (uint16_t *) symbolic_link_target,
		          symbolic_link_target_size,
		          error );
#else
		result = libfsapfs_file_entry_get_utf8_symbolic_link_target(
		          file_entry,
		          (uint8_t *) symbolic_link_target,
		          symbolic_link_target_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve symbolic link target string.",
			 function );

			goto on_error;
		}
	}
	if( libfsapfs_file_entry_get_size(
	     file_entry,
	     &size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		goto on_error;
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		if( info_handle->calculate_md5 == 0 )
		{
			md5_string[ 1 ] = 0;
		}
		else if( ( file_mode & 0xf000 ) == 0x8000 )
		{
			if( info_handle_file_entry_calculate_md5(
			     info_handle,
			     file_entry,
			     md5_string,
			     DIGEST_HASH_STRING_SIZE_MD5,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retreive MD5 string.",
				 function );

				goto on_error;
			}
		}
		/* Colums in a Sleuthkit 3.x and later bodyfile
		 * MD5|name|inode|mode_as_string|UID|GID|size|atime|mtime|ctime|crtime
		 */
		fprintf(
		 info_handle->bodyfile_stream,
		 "%s|",
		 md5_string );

		if( path != NULL )
		{
			if( info_handle_name_value_fprint(
			     info_handle,
			     path,
			     path_length,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print path string.",
				 function );

				goto on_error;
			}
		}
		if( ( file_entry_name != NULL )
		 && ( file_entry_identifier != 2 ) )
		{
			if( info_handle_name_value_fprint(
			     info_handle,
			     file_entry_name,
			     file_entry_name_length,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file entry name string.",
				 function );

				goto on_error;
			}
		}
		if( symbolic_link_target != NULL )
		{
			fprintf(
			 info_handle->bodyfile_stream,
			 " -> %" PRIs_SYSTEM "",
			 symbolic_link_target );
		}
		fprintf(
		 info_handle->bodyfile_stream,
		 "|%" PRIu64 "|%s|%" PRIu32 "|%" PRIu32 "|%" PRIu64 "|%.9f|%.9f|%.9f|%.9f\n",
		 file_entry_identifier,
		 file_mode_string,
		 owner_identifier,
		 group_identifier,
		 size,
		 (double) access_time / 1000000000,
		 (double) modification_time / 1000000000,
		 (double) inode_change_time / 1000000000,
		 (double) creation_time / 1000000000 );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tIdentifier\t\t: %" PRIu64 "\n",
		 file_entry_identifier );

		if( file_entry_name != NULL )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tName\t\t\t: " );

			if( path != NULL )
			{
				if( info_handle_name_value_fprint(
				     info_handle,
				     path,
				     path_length,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print path string.",
					 function );

					goto on_error;
				}
			}
			if( info_handle_name_value_fprint(
			     info_handle,
			     file_entry_name,
			     file_entry_name_length,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file entry name string.",
				 function );

				goto on_error;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\n" );
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tSize\t\t\t: %" PRIu64 "\n",
		 size );

		if( info_handle_posix_time_in_nano_seconds_value_fprint(
		     info_handle,
		     "\tCreation time\t\t",
		     creation_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( info_handle_posix_time_in_nano_seconds_value_fprint(
		     info_handle,
		     "\tModification time\t",
		     modification_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( info_handle_posix_time_in_nano_seconds_value_fprint(
		     info_handle,
		     "\tInode change time\t",
		     inode_change_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( info_handle_posix_time_in_nano_seconds_value_fprint(
		     info_handle,
		     "\tAccess time\t\t",
		     access_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		result = libfsapfs_file_entry_get_added_time(
		          file_entry,
		          &added_time,
		          error );

		if( result == - 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve added time.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( info_handle_posix_time_in_nano_seconds_value_fprint(
			     info_handle,
			     "\tAdded time\t\t",
			     added_time,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print POSIX time value.",
				 function );

				goto on_error;
			}
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tOwner identifier\t: %" PRIu32 "\n",
		 owner_identifier );

		fprintf(
		 info_handle->notify_stream,
		 "\tGroup identifier\t: %" PRIu32 "\n",
		 group_identifier );

		fprintf(
		 info_handle->notify_stream,
		 "\tFile mode\t\t: %s (%07" PRIo16 ")\n",
		 file_mode_string,
		 file_mode );

		if( symbolic_link_target != NULL )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tSymbolic link target\t: %" PRIs_SYSTEM "\n",
			 symbolic_link_target );
		}
		if( libfsapfs_file_entry_get_number_of_extended_attributes(
		     file_entry,
		     &number_of_extended_attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of extended attributes.",
			 function );

			goto on_error;
		}
		if( number_of_extended_attributes > 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tExtended attributes:\n" );

			for( extended_attribute_index = 0;
			     extended_attribute_index < number_of_extended_attributes;
			     extended_attribute_index++ )
			{
				if( libfsapfs_file_entry_get_extended_attribute_by_index(
				     file_entry,
				     extended_attribute_index,
				     &extended_attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve extended attribute: %d.",
					 function,
					 extended_attribute_index );

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libfsapfs_extended_attribute_get_utf16_name_size(
				          extended_attribute,
				          &extended_attribute_name_size,
				          error );
#else
				result = libfsapfs_extended_attribute_get_utf8_name_size(
				          extended_attribute,
				          &extended_attribute_name_size,
				          error );
#endif
				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve extended attribute name string size.",
					 function );

					goto on_error;
				}
				fprintf(
				 info_handle->notify_stream,
				 "\t\tAttribute: %d\t: ",
				 extended_attribute_index + 1 );

				if( ( result == 1 )
				 && ( extended_attribute_name_size > 0 ) )
				{
					extended_attribute_name = system_string_allocate(
					                           extended_attribute_name_size );

					if( extended_attribute_name == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create extended attribute name string.",
						 function );

						goto on_error;
					}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libfsapfs_extended_attribute_get_utf16_name(
					          extended_attribute,
					          (uint16_t *) extended_attribute_name,
					          extended_attribute_name_size,
					          error );
#else
					result = libfsapfs_extended_attribute_get_utf8_name(
					          extended_attribute,
					          (uint8_t *) extended_attribute_name,
					          extended_attribute_name_size,
					          error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve extended attribute name string.",
						 function );

						goto on_error;
					}
					fprintf(
					 info_handle->notify_stream,
					 "%" PRIs_SYSTEM "",
					 extended_attribute_name );

					memory_free(
					 extended_attribute_name );

					extended_attribute_name = NULL;
				}
				fprintf(
				 info_handle->notify_stream,
				 "\n" );

				if( libfsapfs_extended_attribute_free(
				     &extended_attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free extended attribute: %d.",
					 function,
					 extended_attribute_index );

					goto on_error;
				}
			}
		}
	}
	if( symbolic_link_target != NULL )
	{
		memory_free(
		 symbolic_link_target );

		symbolic_link_target = NULL;
	}
	return( 1 );

on_error:
	if( extended_attribute_name != NULL )
	{
		memory_free(
		 extended_attribute_name );
	}
	if( extended_attribute != NULL )
	{
		libfsapfs_extended_attribute_free(
		 &extended_attribute,
		 NULL );
	}
	if( symbolic_link_target != NULL )
	{
		memory_free(
		 symbolic_link_target );
	}
	return( -1 );
}

/* Prints file entry information as part of the file system hierarchy
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_system_hierarchy_fprint_file_entry(
     info_handle_t *info_handle,
     libfsapfs_file_entry_t *file_entry,
     const system_character_t *path,
     libcerror_error_t **error )
{
	libfsapfs_file_entry_t *sub_file_entry = NULL;
	system_character_t *file_entry_name    = NULL;
	system_character_t *sub_path           = NULL;
	static char *function                  = "info_handle_file_system_hierarchy_fprint_file_entry";
	size_t file_entry_name_length          = 0;
	size_t file_entry_name_size            = 0;
	size_t path_length                     = 0;
	size_t sub_path_size                   = 0;
	uint64_t file_entry_identifier         = 0;
	int number_of_sub_file_entries         = 0;
	int result                             = 0;
	int sub_file_entry_index               = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
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
	if( libfsapfs_file_entry_get_identifier(
	     file_entry,
	     &file_entry_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		goto on_error;
	}
	path_length = system_string_length(
	               path );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_file_entry_get_utf16_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#else
	result = libfsapfs_file_entry_get_utf8_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( file_entry_name_size > 0 ) )
	{
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
		          file_entry,
		          (uint16_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#else
		result = libfsapfs_file_entry_get_utf8_name(
		          file_entry,
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
			 "%s: unable to retrieve file entry name string.",
			 function );

			goto on_error;
		}
		file_entry_name_length = file_entry_name_size - 1;
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		if( info_handle_file_entry_value_with_name_fprint(
		     info_handle,
		     file_entry,
		     path,
		     path_length,
		     file_entry_name,
		     file_entry_name_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print file entry.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( info_handle_name_value_fprint(
		     info_handle,
		     path,
		     path_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print path string.",
			 function );

			goto on_error;
		}
		if( ( file_entry_name != NULL )
		 && ( file_entry_identifier != 2 ) )
		{
			if( info_handle_name_value_fprint(
			     info_handle,
			     file_entry_name,
			     file_entry_name_length,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file entry name string.",
				 function );

				goto on_error;
			}
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	if( number_of_sub_file_entries > 0 )
	{
		sub_path_size = path_length + 1;

		if( ( file_entry_name != NULL )
		 && ( file_entry_identifier != 2 ) )
		{
			sub_path_size += file_entry_name_size;
		}
		sub_path = system_string_allocate(
		            sub_path_size );

		if( sub_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create sub path.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     sub_path,
		     path,
		     path_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy path to sub path.",
			 function );

			goto on_error;
		}
		if( ( file_entry_name != NULL )
		 && ( file_entry_identifier != 2 ) )
		{
			if( system_string_copy(
			     &( sub_path[ path_length ] ),
			     file_entry_name,
			     file_entry_name_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy file entry name to sub path.",
				 function );

				goto on_error;
			}
			sub_path[ sub_path_size - 2 ] = (system_character_t) LIBFSAPFS_SEPARATOR;
		}
		sub_path[ sub_path_size - 1 ] = (system_character_t) 0;

		for( sub_file_entry_index = 0;
		     sub_file_entry_index < number_of_sub_file_entries;
		     sub_file_entry_index++ )
		{
			if( libfsapfs_file_entry_get_sub_file_entry_by_index(
			     file_entry,
			     sub_file_entry_index,
			     &sub_file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( info_handle_file_system_hierarchy_fprint_file_entry(
			     info_handle,
			     sub_file_entry,
			     sub_path,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file entry: %d information.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( libfsapfs_file_entry_free(
			     &sub_file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
		}
	}
	if( sub_path != NULL )
	{
		memory_free(
		 sub_path );

		sub_path = NULL;
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );

		file_entry_name = NULL;
	}
	return( 1 );

on_error:
	if( sub_file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	if( sub_path != NULL )
	{
		memory_free(
		 sub_path );
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

/* Prints the file system hierarchy information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_system_hierarchy_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	system_character_t uuid_string[ 48 ];
	uint8_t uuid_data[ 16 ];

	libfguid_identifier_t *uuid        = NULL;
	libfsapfs_file_entry_t *file_entry = NULL;
	libfsapfs_volume_t *volume         = NULL;
	static char *function              = "info_handle_file_system_hierarchy_fprint";
	int number_of_volumes              = 0;
	int result                         = 0;
	int volume_index                   = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Apple File System (APFS) information:\n\n" );

		fprintf(
		 info_handle->notify_stream,
		 "File system hierarchy:\n" );
	}
	if( libfsapfs_container_get_number_of_volumes(
	     info_handle->input_container,
	     &number_of_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of volumes.",
		 function );

		goto on_error;
	}
	if( ( info_handle->file_system_index < 0 )
	 || ( info_handle->file_system_index > number_of_volumes ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file system index value out of bounds.",
		 function );

		goto on_error;
	}
	for( volume_index = 0;
	     volume_index < number_of_volumes;
	     volume_index++ )
	{
		if( ( info_handle->file_system_index != 0 )
		 && ( info_handle->file_system_index != ( volume_index + 1 ) ) )
		{
			continue;
		}
		if( info_handle_get_volume_by_index(
		     info_handle,
		     volume_index,
		     &volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
		if( libfsapfs_volume_get_identifier(
		     volume,
		     uuid_data,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume identifier.",
			 function );

			goto on_error;
		}
		if( libfguid_identifier_initialize(
		     &uuid,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create UUID.",
			 function );

			goto on_error;
		}
		if( libfguid_identifier_copy_from_byte_stream(
		     uuid,
		     uuid_data,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to UUID.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  uuid,
			  (uint16_t *) &( uuid_string[ 1 ] ),
			  48 - 1,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE | LIBFGUID_STRING_FORMAT_FLAG_USE_SURROUNDING_BRACES,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  uuid,
			  (uint8_t *) &( uuid_string[ 1 ] ),
			  48 - 1,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE | LIBFGUID_STRING_FORMAT_FLAG_USE_SURROUNDING_BRACES,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UUID to string.",
			 function );

			goto on_error;
		}
		uuid_string[ 0 ]  = (system_character_t) LIBFSAPFS_SEPARATOR;
		uuid_string[ 39 ] = (system_character_t) LIBFSAPFS_SEPARATOR;
		uuid_string[ 40 ] = (system_character_t) 0;

		if( libfguid_identifier_free(
		     &uuid,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free UUID.",
			 function );

			goto on_error;
		}
		if( libfsapfs_volume_get_root_directory(
		     volume,
		     &file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root directory file entry from volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
		if( info_handle_file_system_hierarchy_fprint_file_entry(
		     info_handle,
		     file_entry,
		     uuid_string,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print root directory file entry information.",
			 function );

			goto on_error;
		}
		if( libfsapfs_file_entry_free(
		     &file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			goto on_error;
		}
		if( libfsapfs_volume_free(
		     &volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume.",
			 function );

			goto on_error;
		}
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	if( uuid != NULL )
	{
		libfguid_identifier_free(
		 &uuid,
		 NULL );
	}
	if( volume != NULL )
	{
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	return( -1 );
}

/* Prints the file entry information for a specific identifier
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_file_entry_fprint_by_identifier(
     info_handle_t *info_handle,
     uint64_t file_system_identifier,
     libcerror_error_t **error )
{
	libfsapfs_file_entry_t *file_entry = NULL;
	libfsapfs_volume_t *volume         = NULL;
	static char *function              = "info_handle_file_entry_fprint_by_identifier";
	int number_of_volumes              = 0;
	int result                         = 0;
	int volume_index                   = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_get_number_of_volumes(
	     info_handle->input_container,
	     &number_of_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of volumes.",
		 function );

		goto on_error;
	}
	volume_index = info_handle->file_system_index;

	if( ( volume_index == 0 )
	 && ( number_of_volumes == 1 ) )
	{
		volume_index = 1;
	}
	if( ( volume_index <= 0 )
	 || ( volume_index > number_of_volumes ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file system index value out of bounds.",
		 function );

		goto on_error;
	}
	volume_index -= 1;

	if( info_handle_get_volume_by_index(
	     info_handle,
	     volume_index,
	     &volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume: %d.",
		 function,
		 volume_index );

		goto on_error;
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Apple File System (APFS) information:\n\n" );
	}
	result = libfsapfs_volume_get_file_entry_by_identifier(
	          volume,
	          file_system_identifier,
	          &file_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to retrieve file entry: %" PRIu64 ".",
		 function,
		 file_system_identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( info_handle->bodyfile_stream == NULL )
		{
			fprintf(
			 info_handle->notify_stream,
			 "File entry:\n" );
		}
		if( info_handle_file_entry_value_fprint(
		     info_handle,
		     file_entry,
		     NULL,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print file entry.",
			 function );

			goto on_error;
		}
		if( libfsapfs_file_entry_free(
		     &file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			goto on_error;
		}
		if( info_handle->bodyfile_stream == NULL )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\n" );
		}
	}
	if( libfsapfs_volume_free(
	     &volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	if( volume != NULL )
	{
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	return( -1 );
}

/* Prints the file entry information for a specific path
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_entry_fprint_by_path(
     info_handle_t *info_handle,
     const system_character_t *path,
     libcerror_error_t **error )
{
	libfsapfs_file_entry_t *file_entry = NULL;
	libfsapfs_volume_t *volume         = NULL;
	static char *function              = "info_handle_file_entry_fprint_by_path";
	size_t path_length                 = 0;
	int number_of_volumes              = 0;
	int result                         = 0;
	int volume_index                   = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_get_number_of_volumes(
	     info_handle->input_container,
	     &number_of_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of volumes.",
		 function );

		goto on_error;
	}
	volume_index = info_handle->file_system_index;

	if( ( volume_index == 0 )
	 && ( number_of_volumes == 1 ) )
	{
		volume_index = 1;
	}
	if( ( volume_index <= 0 )
	 || ( volume_index > number_of_volumes ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file system index value out of bounds.",
		 function );

		goto on_error;
	}
	volume_index -= 1;

	if( info_handle_get_volume_by_index(
	     info_handle,
	     volume_index,
	     &volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume: %d.",
		 function,
		 0 );

		goto on_error;
	}
	path_length = system_string_length(
	               path );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_volume_get_file_entry_by_utf16_path(
	          volume,
	          (uint16_t *) path,
	          path_length,
	          &file_entry,
	          error );
#else
	result = libfsapfs_volume_get_file_entry_by_utf8_path(
	          volume,
	          (uint8_t *) path,
	          path_length,
	          &file_entry,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: file entry not found.",
		 function );

		goto on_error;
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Apple File System (APFS) information:\n\n" );

		fprintf(
		 info_handle->notify_stream,
		 "File entry:\n" );

		fprintf(
		 info_handle->notify_stream,
		 "\tPath\t\t\t: " );

		if( info_handle_name_value_fprint(
		     info_handle,
		     path,
		     path_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print path string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	if( info_handle_file_entry_value_fprint(
	     info_handle,
	     file_entry,
	     NULL,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print file entry.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	if( libfsapfs_volume_free(
	     &volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		goto on_error;
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	if( volume != NULL )
	{
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	return( -1 );
}

/* Prints the file entries information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_entries_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libfsapfs_file_entry_t *file_entry  = NULL;
	libfsapfs_volume_t *volume          = NULL;
	static char *function               = "info_handle_file_entries_fprint";
	uint64_t file_system_identifier     = 0;
	uint64_t next_file_entry_identifier = 0;
	int number_of_volumes               = 0;
	int result                          = 0;
	int volume_index                    = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfsapfs_container_get_number_of_volumes(
	     info_handle->input_container,
	     &number_of_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of volumes.",
		 function );

		goto on_error;
	}
	volume_index = info_handle->file_system_index;

	if( ( volume_index == 0 )
	 && ( number_of_volumes == 1 ) )
	{
		volume_index = 1;
	}
	if( ( volume_index <= 0 )
	 || ( volume_index > number_of_volumes ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file system index value out of bounds.",
		 function );

		goto on_error;
	}
	volume_index -= 1;

	if( info_handle_get_volume_by_index(
	     info_handle,
	     volume_index,
	     &volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume: %d.",
		 function,
		 0 );

		goto on_error;
	}
	if( libfsapfs_volume_get_next_file_entry_identifier(
	     volume,
	     &next_file_entry_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve next file entry identifier.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "Apple File System (APFS) information:\n\n" );

	for( file_system_identifier = 0;
	     file_system_identifier < next_file_entry_identifier;
	     file_system_identifier++ )
	{
		result = libfsapfs_volume_get_file_entry_by_identifier(
		          volume,
		          file_system_identifier,
		          &file_entry,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to retrieve file entry: %" PRIu64 ".",
			 function,
			 file_system_identifier );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( info_handle->bodyfile_stream == NULL )
			{
				fprintf(
				 info_handle->notify_stream,
				 "File entry:\n" );
			}
			if( info_handle_file_entry_value_fprint(
			     info_handle,
			     file_entry,
			     NULL,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file entry: %" PRIu64 " information.",
				 function,
				 file_system_identifier );

				goto on_error;
			}
			if( libfsapfs_file_entry_free(
			     &file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file entry.",
				 function );

				goto on_error;
			}
			if( info_handle->bodyfile_stream == NULL )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\n" );
			}
		}
	}
	if( libfsapfs_volume_free(
	     &volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	if( volume != NULL )
	{
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	return( -1 );
}

/* Prints the volume information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_fprint(
     info_handle_t *info_handle,
     int volume_index,
     libfsapfs_volume_t *volume,
     libcerror_error_t **error )
{
	uint8_t uuid_data[ 16 ];

	libfsapfs_snapshot_t *snapshot               = NULL;
	system_character_t *snapshot_name            = NULL;
	system_character_t *volume_name              = NULL;
	static char *function                        = "info_handle_volume_fprint";
	size_t snapshot_name_size                    = 0;
	size_t volume_name_size                      = 0;
	uint64_t compatible_features_flags           = 0;
	uint64_t incompatible_features_flags         = 0;
	uint64_t read_only_compatible_features_flags = 0;
	int number_of_snapshots                      = 0;
	int result                                   = 0;
	int snapshot_index                           = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\nVolume: %d information:\n",
	 volume_index + 1 );

	if( libfsapfs_volume_get_identifier(
	     volume,
	     uuid_data,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume identifier.",
		 function );

		goto on_error;
	}
	if( info_handle_uuid_value_fprint(
	     info_handle,
	     "\tIdentifier\t\t\t",
	     uuid_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print UUID value.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsapfs_volume_get_utf16_name_size(
	          volume,
	          &volume_name_size,
	          error );
#else
	result = libfsapfs_volume_get_utf8_name_size(
	          volume,
	          &volume_name_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume name size.",
		 function );

		goto on_error;
	}
	if( volume_name_size > 0 )
	{
		volume_name = system_string_allocate(
		               volume_name_size );

		if( volume_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create volume name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsapfs_volume_get_utf16_name(
		          volume,
		          (uint16_t *) volume_name,
		          volume_name_size,
		          error );
#else
		result = libfsapfs_volume_get_utf8_name(
		          volume,
		          (uint8_t *) volume_name,
		          volume_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume name.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 volume_name );

		memory_free(
		 volume_name );

		volume_name = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfsapfs_volume_get_features_flags(
	     volume,
	     &compatible_features_flags,
	     &incompatible_features_flags,
	     &read_only_compatible_features_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve feature flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tCompatible features\t\t: 0x%08" PRIx64 "\n",
	 compatible_features_flags );

	info_handle_compatible_features_flags_fprint(
	 compatible_features_flags,
	 info_handle->notify_stream );

	fprintf(
	 info_handle->notify_stream,
	 "\tIncompatible features\t\t: 0x%08" PRIx64 "\n",
	 incompatible_features_flags );

	info_handle_incompatible_features_flags_fprint(
	 incompatible_features_flags,
	 info_handle->notify_stream );

	fprintf(
	 info_handle->notify_stream,
	 "\tRead-only compatible features\t: 0x%08" PRIx64 "\n",
	 read_only_compatible_features_flags );

	info_handle_read_only_compatible_features_flags_fprint(
	 read_only_compatible_features_flags,
	 info_handle->notify_stream );

/* TODO print additional volume information such as size */

	result = libfsapfs_volume_is_locked(
	          volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if volume is locked.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tIs locked\n" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfsapfs_volume_get_number_of_snapshots(
	     volume,
	     &number_of_snapshots,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of snapshots.",
		 function );

		goto on_error;
	}
	if( number_of_snapshots > 0 )
	{
		for( snapshot_index = 0;
		     snapshot_index < number_of_snapshots;
		     snapshot_index++ )
		{
			if( libfsapfs_volume_get_snapshot_by_index(
			     volume,
			     snapshot_index,
			     &snapshot,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve snapshot: %d.",
				 function,
				 snapshot_index );

				goto on_error;
			}
/* TODO move into separate print function */
			fprintf(
			 info_handle->notify_stream,
			 "Volume: %d snapshot: %d information:\n",
			 volume_index + 1,
			 snapshot_index + 1 );

			fprintf(
			 info_handle->notify_stream,
			 "\tName\t\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsapfs_snapshot_get_utf16_name_size(
				  snapshot,
				  &snapshot_name_size,
				  error );
#else
			result = libfsapfs_snapshot_get_utf8_name_size(
				  snapshot,
				  &snapshot_name_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve snapshot name size.",
				 function );

				goto on_error;
			}
			if( snapshot_name_size > 0 )
			{
				snapshot_name = system_string_allocate(
				                 snapshot_name_size );

				if( snapshot_name == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create snapshot name string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libfsapfs_snapshot_get_utf16_name(
					  snapshot,
					  (uint16_t *) snapshot_name,
					  snapshot_name_size,
					  error );
#else
				result = libfsapfs_snapshot_get_utf8_name(
					  snapshot,
					  (uint8_t *) snapshot_name,
					  snapshot_name_size,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve snapshot name.",
					 function );

					goto on_error;
				}
				fprintf(
				 info_handle->notify_stream,
				 "%" PRIs_SYSTEM "",
				 snapshot_name );

				memory_free(
				 snapshot_name );

				snapshot_name = NULL;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\n" );

			if( libfsapfs_snapshot_free(
			     &snapshot,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free snapshot.",
				 function );

				goto on_error;
			}
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( snapshot != NULL )
	{
		libfsapfs_snapshot_free(
		 &snapshot,
		 NULL );
	}
	if( volume_name != NULL )
	{
		memory_free(
		 volume_name );
	}
	return( -1 );
}

/* Prints the container information
 * Returns 1 if successful or -1 on error
 */
int info_handle_container_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	uint8_t uuid_data[ 16 ];

	libfsapfs_volume_t *volume = NULL;
	static char *function      = "info_handle_container_fprint";
	int number_of_volumes      = 0;
	int result                 = 0;
	int volume_index           = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Apple File System (APFS) information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "Container information:\n" );

	if( libfsapfs_container_get_identifier(
	     info_handle->input_container,
	     uuid_data,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve container identifier.",
		 function );

		goto on_error;
	}
	if( info_handle_uuid_value_fprint(
	     info_handle,
	     "\tIdentifier\t\t\t",
	     uuid_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print UUID value.",
		 function );

		goto on_error;
	}
/* TODO print additional container information such as size */

	if( libfsapfs_container_get_number_of_volumes(
	     info_handle->input_container,
	     &number_of_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of volumes.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of volumes\t\t: %d\n",
	 number_of_volumes );

	result = libfsapfs_container_is_locked(
	          info_handle->input_container,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if container is locked.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tIs locked (requires T2)\n" );
	}
	info_handle->container_is_locked = (uint8_t) result;

	for( volume_index = 0;
	     volume_index < number_of_volumes;
	     volume_index++ )
	{
		if( info_handle_get_volume_by_index(
		     info_handle,
		     volume_index,
		     &volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
		if( info_handle_volume_fprint(
		     info_handle,
		     volume_index,
		     volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
		if( libfsapfs_volume_free(
		     &volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( volume != NULL )
	{
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	return( -1 );
}

