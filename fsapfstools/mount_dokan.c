/*
 * Mount tool dokan functions
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
#include <types.h>
#include <wide_string.h>

#include "fsapfstools_libcerror.h"
#include "fsapfstools_libcnotify.h"
#include "fsapfstools_libfsapfs.h"
#include "fsapfstools_unused.h"
#include "mount_dokan.h"
#include "mount_file_entry.h"
#include "mount_handle.h"

extern mount_handle_t *fsapfsmount_mount_handle;

#if defined( HAVE_LIBDOKAN )

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
#define MOUNT_DOKAN_ERROR_BAD_ARGUMENTS -ERROR_BAD_ARGUMENTS
#define MOUNT_DOKAN_ERROR_FILE_NOT_FOUND -ERROR_FILE_NOT_FOUND
#define MOUNT_DOKAN_ERROR_GENERIC_FAILURE -ERROR_GEN_FAILURE
#define MOUNT_DOKAN_ERROR_READ_FAULT -ERROR_READ_FAULT

#else
#define MOUNT_DOKAN_ERROR_BAD_ARGUMENTS STATUS_UNSUCCESSFUL
#define MOUNT_DOKAN_ERROR_FILE_NOT_FOUND STATUS_OBJECT_NAME_NOT_FOUND
#define MOUNT_DOKAN_ERROR_GENERIC_FAILURE STATUS_UNSUCCESSFUL
#define MOUNT_DOKAN_ERROR_READ_FAULT STATUS_UNEXPECTED_IO_ERROR

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

/* Sets the values in a file information structure
 * The time values contain an unsigned 64-bit FILETIME timestamp
 * Returns 1 if successful or -1 on error
 */
int mount_dokan_set_file_information(
     BY_HANDLE_FILE_INFORMATION *file_information,
     size64_t size,
     uint16_t file_mode,
     uint64_t creation_time,
     uint64_t access_time,
     uint64_t modification_time,
     libcerror_error_t **error )
{
	static char *function = "mount_dokan_set_file_information";

	if( file_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		return( -1 );
	}
	if( size > 0 )
	{
		file_information->nFileSizeHigh = (DWORD) ( size >> 32 );
		file_information->nFileSizeLow  = (DWORD) ( size & 0xffffffffUL );
	}
	if( ( file_mode & 0x4000 ) != 0 )
	{
		file_information->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
	}
	else
	{
		file_information->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	}
	file_information->ftCreationTime.dwLowDateTime  = (uint32_t) ( creation_time & 0x00000000ffffffffULL );
	file_information->ftCreationTime.dwHighDateTime = creation_time >> 32;

	file_information->ftLastAccessTime.dwLowDateTime  = (uint32_t) ( access_time & 0x00000000ffffffffULL );
	file_information->ftLastAccessTime.dwHighDateTime = access_time >> 32;

	file_information->ftLastWriteTime.dwLowDateTime  = (uint32_t) ( modification_time & 0x00000000ffffffffULL );
	file_information->ftLastWriteTime.dwHighDateTime = modification_time >> 32;

	return( 1 );
}

/* Sets the values in a find data structure
 * The time values contain an unsigned 64-bit FILETIME timestamp
 * Returns 1 if successful or -1 on error
 */
int mount_dokan_set_find_data(
     WIN32_FIND_DATAW *find_data,
     size64_t size,
     uint16_t file_mode,
     uint64_t creation_time,
     uint64_t access_time,
     uint64_t modification_time,
     libcerror_error_t **error )
{
	static char *function = "mount_dokan_set_find_data";

	if( find_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid find data.",
		 function );

		return( -1 );
	}
	if( size > 0 )
	{
		find_data->nFileSizeHigh = (DWORD) ( size >> 32 );
		find_data->nFileSizeLow  = (DWORD) ( size & 0xffffffffUL );
	}
	if( ( file_mode & 0x4000 ) != 0 )
	{
		find_data->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
	}
	else
	{
		find_data->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	}
	find_data->ftCreationTime.dwLowDateTime  = (uint32_t) ( creation_time & 0x00000000ffffffffULL );
	find_data->ftCreationTime.dwHighDateTime = creation_time >> 32;

	find_data->ftLastAccessTime.dwLowDateTime  = (uint32_t) ( access_time & 0x00000000ffffffffULL );
	find_data->ftLastAccessTime.dwHighDateTime = access_time >> 32;

	find_data->ftLastWriteTime.dwLowDateTime  = (uint32_t) ( modification_time & 0x00000000ffffffffULL );
	find_data->ftLastWriteTime.dwHighDateTime = modification_time >> 32;

	return( 1 );
}

/* Fills a directory entry
 * Returns 1 if successful or -1 on error
 */
int mount_dokan_filldir(
     PFillFindData fill_find_data,
     DOKAN_FILE_INFO *file_info,
     wchar_t *name,
     size_t name_size,
     WIN32_FIND_DATAW *find_data,
     mount_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	static char *function      = "mount_dokan_filldir";
	size64_t file_size         = 0;
	uint64_t access_time       = 0;
	uint64_t creation_time     = 0;
	uint64_t modification_time = 0;
	uint16_t file_mode         = 0;

	if( fill_find_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fill find data.",
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
	if( name_size > (size_t) MAX_PATH )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name size value out of bounds.",
		 function );

		return( -1 );
	}
	if( file_entry != NULL )
	{
		if( mount_file_entry_get_size(
		     file_entry,
		     &file_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry size.",
			 function );

			return( -1 );
		}
		if( mount_file_entry_get_file_mode(
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

			return( -1 );
		}
		if( mount_file_entry_get_creation_time(
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

			return( -1 );
		}
		if( mount_file_entry_get_access_time(
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

			return( -1 );
		}
		if( mount_file_entry_get_modification_time(
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

			return( -1 );
		}
	}
	if( memory_set(
	     find_data,
	     0,
	     sizeof( WIN32_FIND_DATAW ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear find data.",
		 function );

		return( -1 );
	}
	if( wide_string_copy(
	     find_data->cFileName,
	     name,
	     name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy filename.",
		 function );

		return( -1 );
	}
	if( name_size <= (size_t) 14 )
	{
		if( wide_string_copy(
		     find_data->cAlternateFileName,
		     name,
		     name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy alternate filename.",
			 function );

			return( -1 );
		}
	}
	if( mount_dokan_set_find_data(
	     find_data,
	     file_size,
	     file_mode,
	     creation_time,
	     access_time,
	     modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		return( -1 );
	}
	if( fill_find_data(
	     find_data,
	     file_info ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )

/* Opens a file or directory
 * Returns 0 if successful or an error code otherwise
 */
int __stdcall mount_dokan_CreateFile(
               const wchar_t *path,
               DWORD desired_access,
               DWORD share_mode FSAPFSTOOLS_ATTRIBUTE_UNUSED,
               DWORD creation_disposition,
               DWORD attribute_flags FSAPFSTOOLS_ATTRIBUTE_UNUSED,
               DOKAN_FILE_INFO *file_info )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_CreateFile";
	int result               = 0;

	FSAPFSTOOLS_UNREFERENCED_PARAMETER( share_mode )
	FSAPFSTOOLS_UNREFERENCED_PARAMETER( attribute_flags )

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %ls\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( ( desired_access & GENERIC_WRITE ) != 0 )
	{
		return( -ERROR_WRITE_PROTECT );
	}
	/* Ignore the share_mode
	 */
	if( creation_disposition == CREATE_NEW )
	{
		return( -ERROR_FILE_EXISTS );
	}
	else if( creation_disposition == CREATE_ALWAYS )
	{
		return( -ERROR_ALREADY_EXISTS );
	}
	else if( creation_disposition == OPEN_ALWAYS )
	{
		return( -ERROR_FILE_NOT_FOUND );
	}
	else if( creation_disposition == TRUNCATE_EXISTING )
	{
		return( -ERROR_FILE_NOT_FOUND );
	}
	else if( creation_disposition != OPEN_EXISTING )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid creation disposition.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info->Context != (ULONG64) NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file information - context already set.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( mount_handle_get_file_entry_by_path(
	     fsapfsmount_mount_handle,
	     path,
	     (mount_file_entry_t **) &( file_info->Context ),
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry for path: %ls.",
		 function,
		 path );

		result = -ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

#else

/* Opens a file or directory
 * Returns 0 if successful or an error code otherwise
 */
NTSTATUS __stdcall mount_dokan_ZwCreateFile(
                    const wchar_t *path,
                    DOKAN_IO_SECURITY_CONTEXT *security_context FSAPFSTOOLS_ATTRIBUTE_UNUSED,
                    ACCESS_MASK desired_access,
                    ULONG file_attributes FSAPFSTOOLS_ATTRIBUTE_UNUSED,
                    ULONG share_access FSAPFSTOOLS_ATTRIBUTE_UNUSED,
                    ULONG creation_disposition,
                    ULONG creation_options FSAPFSTOOLS_ATTRIBUTE_UNUSED,
                    DOKAN_FILE_INFO *file_info )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_ZwCreateFile";
	int result               = 0;

	FSAPFSTOOLS_UNREFERENCED_PARAMETER( security_context )
	FSAPFSTOOLS_UNREFERENCED_PARAMETER( file_attributes )
	FSAPFSTOOLS_UNREFERENCED_PARAMETER( share_access )
	FSAPFSTOOLS_UNREFERENCED_PARAMETER( creation_options )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = STATUS_UNSUCCESSFUL;

		goto on_error;
	}
	if( ( desired_access & GENERIC_WRITE ) != 0 )
	{
		return( STATUS_MEDIA_WRITE_PROTECTED );
	}
	/* Ignore the share_mode
	 */
	if( creation_disposition == FILE_CREATE )
	{
		return( STATUS_OBJECT_NAME_COLLISION );
	}
	else if( ( creation_disposition != FILE_OPEN )
	      && ( creation_disposition != FILE_OPEN_IF ) )
	{
		return( STATUS_ACCESS_DENIED );
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		result = STATUS_UNSUCCESSFUL;

		goto on_error;
	}
	if( file_info->Context != (ULONG64) NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file information - context already set.",
		 function );

		result = STATUS_UNSUCCESSFUL;

		goto on_error;
	}
	if( mount_handle_get_file_entry_by_path(
	     fsapfsmount_mount_handle,
	     path,
	     (mount_file_entry_t **) &( file_info->Context ),
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry for path: %ls.",
		 function,
		 path );

		result = STATUS_OBJECT_NAME_NOT_FOUND;

		goto on_error;
	}
	return( STATUS_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )

/* Opens a directory
 * Returns 0 if successful or an error code otherwise
 */
int __stdcall mount_dokan_OpenDirectory(
               const wchar_t *path,
               DOKAN_FILE_INFO *file_info )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_OpenDirectory";
	int result               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %ls\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info->Context != (ULONG64) NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file information - context already set.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( mount_handle_get_file_entry_by_path(
	     fsapfsmount_mount_handle,
	     path,
	     (mount_file_entry_t **) &( file_info->Context ),
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry for path: %ls.",
		 function,
		 path );

		result = MOUNT_DOKAN_ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

/* Closes a file or direcotry
 * Returns 0 if successful or an error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_CloseFile(
               const wchar_t *path,
               DOKAN_FILE_INFO *file_info )
#else
NTSTATUS __stdcall mount_dokan_CloseFile(
                    const wchar_t *path,
                    DOKAN_FILE_INFO *file_info )
#endif
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_CloseFile";
	int result               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %ls\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info->Context != (ULONG64) NULL )
	{
		if( mount_file_entry_free(
		     (mount_file_entry_t **) &( file_info->Context ),
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a buffer of data at the specified offset
 * Returns 0 if successful or an error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_ReadFile(
               const wchar_t *path,
               void *buffer,
               DWORD number_of_bytes_to_read,
               DWORD *number_of_bytes_read,
               LONGLONG offset,
               DOKAN_FILE_INFO *file_info )
#else
NTSTATUS __stdcall mount_dokan_ReadFile(
                    const wchar_t *path,
                    void *buffer,
                    DWORD number_of_bytes_to_read,
                    DWORD *number_of_bytes_read,
                    LONGLONG offset,
                    DOKAN_FILE_INFO *file_info )
#endif
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_ReadFile";
	ssize_t read_count       = 0;
	int result               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %ls\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( number_of_bytes_to_read > (DWORD) INT32_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of bytes to read value exceeds maximum.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( number_of_bytes_read == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of bytes read.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info->Context == (ULONG64) NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file information - missing context.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	read_count = mount_file_entry_read_buffer_at_offset(
		      (mount_file_entry_t *) file_info->Context,
		      buffer,
		      (size_t) number_of_bytes_to_read,
		      (off64_t) offset,
		      &error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from mount handle.",
		 function );

		result = MOUNT_DOKAN_ERROR_READ_FAULT;

		goto on_error;
	}
	if( read_count > (size_t) INT32_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid read count value exceeds maximum.",
		 function );

		result = MOUNT_DOKAN_ERROR_READ_FAULT;

		goto on_error;
	}
	/* Dokan does not require the read function to return ERROR_HANDLE_EOF
	 */
	*number_of_bytes_read = (DWORD) read_count;

	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a directory
 * Returns 0 if successful or an error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_FindFiles(
               const wchar_t *path,
               PFillFindData fill_find_data,
               DOKAN_FILE_INFO *file_info )
#else
NTSTATUS __stdcall mount_dokan_FindFiles(
                    const wchar_t *path,
                    PFillFindData fill_find_data,
                    DOKAN_FILE_INFO *file_info )
#endif
{
	WIN32_FIND_DATAW find_data;

	libcerror_error_t *error              = NULL;
	mount_file_entry_t *file_entry        = NULL;
	mount_file_entry_t *parent_file_entry = NULL;
	mount_file_entry_t *sub_file_entry    = NULL;
	wchar_t *name                         = NULL;
	static char *function                 = "mount_dokan_FindFiles";
	size_t name_size                      = 0;
	int number_of_sub_file_entries        = 0;
	int result                            = 0;
	int sub_file_entry_index              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %ls\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( mount_handle_get_file_entry_by_path(
	     fsapfsmount_mount_handle,
	     path,
	     &file_entry,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry for path: %ls.",
		 function,
		 path );

		result = MOUNT_DOKAN_ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	if( mount_dokan_filldir(
	     fill_find_data,
	     file_info,
	     L".",
	     2,
	     &find_data,
	     file_entry,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

		goto on_error;
	}
	result = mount_file_entry_get_parent_file_entry(
	          file_entry,
	          &parent_file_entry,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent file entry.",
		 function );

		result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

		goto on_error;
	}
	if( mount_dokan_filldir(
	     fill_find_data,
	     file_info,
	     L"..",
	     3,
	     &find_data,
	     parent_file_entry,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

		goto on_error;
	}
	if( mount_file_entry_free(
	     &parent_file_entry,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free parent file entry.",
		 function );

		result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

		goto on_error;
	}
	if( mount_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

		goto on_error;
	}
	for( sub_file_entry_index = 0;
	     sub_file_entry_index < number_of_sub_file_entries;
	     sub_file_entry_index++ )
	{
		if( mount_file_entry_get_sub_file_entry_by_index(
		     file_entry,
		     sub_file_entry_index,
		     &sub_file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		if( mount_file_entry_get_name_size(
		     sub_file_entry,
		     &name_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub file entry: %d name size.",
			 function,
			 sub_file_entry_index );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		name = wide_string_allocate(
		        name_size );

		if( name == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create sub file entry: %d name.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		if( mount_file_entry_get_name(
		     sub_file_entry,
		     name,
		     name_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub file entry: %d name.",
			 function,
			 sub_file_entry_index );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		if( mount_dokan_filldir(
		     fill_find_data,
		     file_info,
		     name,
		     name_size,
		     &find_data,
		     sub_file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set find data for sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		memory_free(
		 name );

		name = NULL;

		if( mount_file_entry_free(
		     &sub_file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
	}
	if( mount_file_entry_free(
	     &file_entry,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	if( sub_file_entry != NULL )
	{
		mount_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	if( parent_file_entry != NULL )
	{
		mount_file_entry_free(
		 &parent_file_entry,
		 NULL );
	}
	if( file_entry != NULL )
	{
		mount_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( result );
}

/* Retrieves the file information
 * Returns 0 if successful or an error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_GetFileInformation(
               const wchar_t *path,
               BY_HANDLE_FILE_INFORMATION *file_information,
               DOKAN_FILE_INFO *file_info FSAPFSTOOLS_ATTRIBUTE_UNUSED )
#else
NTSTATUS __stdcall mount_dokan_GetFileInformation(
                    const wchar_t *path,
                    BY_HANDLE_FILE_INFORMATION *file_information,
                    DOKAN_FILE_INFO *file_info FSAPFSTOOLS_ATTRIBUTE_UNUSED )
#endif
{
	libcerror_error_t *error       = NULL;
	mount_file_entry_t *file_entry = NULL;
	static char *function          = "mount_dokan_GetFileInformation";
	size64_t file_size             = 0;
	uint64_t access_time           = 0;
	uint64_t creation_time         = 0;
	uint64_t modification_time     = 0;
	uint16_t file_mode             = 0;
	int result                     = 0;

	FSAPFSTOOLS_UNREFERENCED_PARAMETER( file_info )

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %ls\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = MOUNT_DOKAN_ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( mount_handle_get_file_entry_by_path(
	     fsapfsmount_mount_handle,
	     path,
	     &file_entry,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry for path: %ls.",
		 function,
		 path );

		result = MOUNT_DOKAN_ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	if( file_entry != NULL )
	{
		if( mount_file_entry_get_size(
		     file_entry,
		     &file_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry size.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		if( mount_file_entry_get_file_mode(
		     file_entry,
		     &file_mode,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file mode.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		if( mount_file_entry_get_creation_time(
		     file_entry,
		     &creation_time,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve creation time.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		if( mount_file_entry_get_access_time(
		     file_entry,
		     &access_time,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve access time.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
		if( mount_file_entry_get_modification_time(
		     file_entry,
		     &modification_time,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve modification time.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
	}
	if( mount_dokan_set_file_information(
	     file_information,
	     file_size,
	     file_mode,
	     creation_time,
	     access_time,
	     modification_time,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file information.",
		 function );

		result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

		goto on_error;
	}
	if( mount_file_entry_free(
	     &file_entry,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( file_entry != NULL )
	{
		mount_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( result );
}

/* Retrieves the volume information
 * Returns 0 if successful or an error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_GetVolumeInformation(
               wchar_t *volume_name,
               DWORD volume_name_size,
               DWORD *volume_serial_number,
               DWORD *maximum_filename_length,
               DWORD *file_system_flags,
               wchar_t *file_system_name,
               DWORD file_system_name_size,
               DOKAN_FILE_INFO *file_info FSAPFSTOOLS_ATTRIBUTE_UNUSED )
#else
NTSTATUS __stdcall mount_dokan_GetVolumeInformation(
                    wchar_t *volume_name,
                    DWORD volume_name_size,
                    DWORD *volume_serial_number,
                    DWORD *maximum_filename_length,
                    DWORD *file_system_flags,
                    wchar_t *file_system_name,
                    DWORD file_system_name_size,
                    DOKAN_FILE_INFO *file_info FSAPFSTOOLS_ATTRIBUTE_UNUSED )
#endif
{
	libcerror_error_t *error = NULL;
	const wchar_t *name      = NULL;
	static char *function    = "mount_dokan_GetVolumeInformation";
	size_t name_size         = 0;
	int result               = 0;

	FSAPFSTOOLS_UNREFERENCED_PARAMETER( file_info )

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %ls\n",
		 function,
		 volume_name );
	}
#endif
	name      = L"FSAPFS";
	name_size = 1 + wide_string_length(
	                 name );

	if( ( volume_name != NULL )
	 && ( volume_name_size > (DWORD) name_size ) )
	{
		/* Using wcsncpy seems to cause strange behavior here
		 */
		if( memory_copy(
		     volume_name,
		     name,
		     name_size * sizeof( wchar_t ) ) == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy volume name.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
	}
	if( volume_serial_number != NULL )
	{
		/* If this value contains 0 it can crash the system is this an issue in Dokan?
		 */
		*volume_serial_number = 0x19831116;
	}
	if( maximum_filename_length != NULL )
	{
		*maximum_filename_length = 256;
	}
	if( file_system_flags != NULL )
	{
		*file_system_flags = FILE_CASE_SENSITIVE_SEARCH
		                   | FILE_CASE_PRESERVED_NAMES
		                   | FILE_UNICODE_ON_DISK
		                   | FILE_READ_ONLY_VOLUME;
	}
	name      = L"Dokan";
	name_size = 1 + wide_string_length(
	                 name );

	if( ( file_system_name != NULL )
	 && ( file_system_name_size > (DWORD) name_size ) )
	{
		/* Using wcsncpy seems to cause strange behavior here
		 */
		if( memory_copy(
		     file_system_name,
		     name,
		     name_size * sizeof( wchar_t ) ) == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy file system name.",
			 function );

			result = MOUNT_DOKAN_ERROR_GENERIC_FAILURE;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )

/* Unmounts the volume
 * Returns 0 if successful or an error code otherwise
 */
int __stdcall mount_dokan_Unmount(
               DOKAN_FILE_INFO *file_info FSAPFSTOOLS_ATTRIBUTE_UNUSED )
{
	static char *function = "mount_dokan_Unmount";

	FSAPFSTOOLS_UNREFERENCED_PARAMETER( file_info )

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s\n",
		 function );
	}
#endif
	return( 0 );
}

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

#endif /* defined( HAVE_LIBDOKAN ) */

