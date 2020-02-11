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

#if !defined( _MOUNT_DOKAN_H )
#define _MOUNT_DOKAN_H

#include <common.h>
#include <types.h>

#if defined( HAVE_LIBDOKAN )
#include <dokan.h>
#endif

#include "fsapfstools_libcerror.h"
#include "fsapfstools_libfsapfs.h"
#include "mount_file_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LIBDOKAN )

int mount_dokan_set_file_information(
     BY_HANDLE_FILE_INFORMATION *file_information,
     size64_t size,
     uint16_t file_mode,
     uint64_t creation_time,
     uint64_t access_time,
     uint64_t modification_time,
     libcerror_error_t **error );

int mount_dokan_set_find_data(
     WIN32_FIND_DATAW *find_data,
     size64_t size,
     uint16_t file_mode,
     uint64_t creation_time,
     uint64_t access_time,
     uint64_t modification_time,
     libcerror_error_t **error );

int mount_dokan_filldir(
     PFillFindData fill_find_data,
     DOKAN_FILE_INFO *file_info,
     wchar_t *name,
     size_t name_size,
     WIN32_FIND_DATAW *find_data,
     mount_file_entry_t *file_entry,
     libcerror_error_t **error );

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )

int __stdcall mount_dokan_CreateFile(
               const wchar_t *path,
               DWORD desired_access,
               DWORD share_mode,
               DWORD creation_disposition,
               DWORD attribute_flags,
               DOKAN_FILE_INFO *file_info );

int __stdcall mount_dokan_OpenDirectory(
               const wchar_t *path,
               DOKAN_FILE_INFO *file_info );

int __stdcall mount_dokan_CloseFile(
               const wchar_t *path,
               DOKAN_FILE_INFO *file_info );

int __stdcall mount_dokan_ReadFile(
               const wchar_t *path,
               void *buffer,
               DWORD number_of_bytes_to_read,
               DWORD *number_of_bytes_read,
               LONGLONG offset,
               DOKAN_FILE_INFO *file_info );

int __stdcall mount_dokan_FindFiles(
               const wchar_t *path,
               PFillFindData fill_find_data,
               DOKAN_FILE_INFO *file_info );

int __stdcall mount_dokan_GetFileInformation(
               const wchar_t *path,
               BY_HANDLE_FILE_INFORMATION *file_information,
               DOKAN_FILE_INFO *file_info );

int __stdcall mount_dokan_GetVolumeInformation(
               wchar_t *volume_name,
               DWORD volume_name_size,
               DWORD *volume_serial_number,
               DWORD *maximum_filename_length,
               DWORD *file_system_flags,
               wchar_t *file_system_name,
               DWORD file_system_name_size,
               DOKAN_FILE_INFO *file_info );

int __stdcall mount_dokan_Unmount(
               DOKAN_FILE_INFO *file_info );

#else

NTSTATUS __stdcall mount_dokan_ZwCreateFile(
                    const wchar_t *path,
                    DOKAN_IO_SECURITY_CONTEXT *security_context,
                    ACCESS_MASK desired_access,
                    ULONG file_attributes,
                    ULONG share_access,
                    ULONG creation_disposition,
                    ULONG creation_options,
                    DOKAN_FILE_INFO *file_info );

NTSTATUS __stdcall mount_dokan_CloseFile(
                    const wchar_t *path,
                    DOKAN_FILE_INFO *file_info );

NTSTATUS __stdcall mount_dokan_ReadFile(
                    const wchar_t *path,
                    void *buffer,
                    DWORD number_of_bytes_to_read,
                    DWORD *number_of_bytes_read,
                    LONGLONG offset,
                    DOKAN_FILE_INFO *file_info );

NTSTATUS __stdcall mount_dokan_FindFiles(
                    const wchar_t *path,
                    PFillFindData fill_find_data,
                    DOKAN_FILE_INFO *file_info );

NTSTATUS __stdcall mount_dokan_GetFileInformation(
                    const wchar_t *path,
                    BY_HANDLE_FILE_INFORMATION *file_information,
                    DOKAN_FILE_INFO *file_info );

NTSTATUS __stdcall mount_dokan_GetVolumeInformation(
                    wchar_t *volume_name,
                    DWORD volume_name_size,
                    DWORD *volume_serial_number,
                    DWORD *maximum_filename_length,
                    DWORD *file_system_flags,
                    wchar_t *file_system_name,
                    DWORD file_system_name_size,
                    DOKAN_FILE_INFO *file_info );

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

#endif /* defined( HAVE_LIBDOKAN ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_DOKAN_H ) */

