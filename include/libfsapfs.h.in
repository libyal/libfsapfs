/*
 * Library to access the Apple File System (APFS) format
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

#if !defined( _LIBFSAPFS_H )
#define _LIBFSAPFS_H

#include <libfsapfs/codepage.h>
#include <libfsapfs/definitions.h>
#include <libfsapfs/error.h>
#include <libfsapfs/extern.h>
#include <libfsapfs/features.h>
#include <libfsapfs/types.h>

#include <stdio.h>

#if defined( LIBFSAPFS_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBFSAPFS_EXTERN \
const char *libfsapfs_get_version(
             void );

/* Returns the access flags for reading
 */
LIBFSAPFS_EXTERN \
int libfsapfs_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_get_codepage(
     int *codepage,
     libfsapfs_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_set_codepage(
     int codepage,
     libfsapfs_error_t **error );

/* Determines if a file contains an APFS container signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_check_container_signature(
     const char *filename,
     libfsapfs_error_t **error );

/* Determines if a file contains an APFS volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_check_volume_signature(
     const char *filename,
     libfsapfs_error_t **error );

#if defined( LIBFSAPFS_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains an APFS container signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_check_container_signature_wide(
     const wchar_t *filename,
     libfsapfs_error_t **error );

/* Determines if a file contains an APFS volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_check_volume_signature_wide(
     const wchar_t *filename,
     libfsapfs_error_t **error );

#endif /* defined( LIBFSAPFS_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFSAPFS_HAVE_BFIO )

/* Determines if a file contains an APFS container signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_check_container_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libfsapfs_error_t **error );

/* Determines if a file contains an APFS volume signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libfsapfs_error_t **error );

#endif /* defined( LIBFSAPFS_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBFSAPFS_EXTERN \
void libfsapfs_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_notify_set_stream(
     FILE *stream,
     libfsapfs_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_notify_stream_open(
     const char *filename,
     libfsapfs_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_notify_stream_close(
     libfsapfs_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBFSAPFS_EXTERN \
void libfsapfs_error_free(
      libfsapfs_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_error_fprint(
     libfsapfs_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_error_sprint(
     libfsapfs_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_error_backtrace_fprint(
     libfsapfs_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_error_backtrace_sprint(
     libfsapfs_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Container functions
 * ------------------------------------------------------------------------- */

/* Creates a container
 * Make sure the value container is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_initialize(
     libfsapfs_container_t **container,
     libfsapfs_error_t **error );

/* Frees a container
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_free(
     libfsapfs_container_t **container,
     libfsapfs_error_t **error );

/* Signals the container to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_signal_abort(
     libfsapfs_container_t *container,
     libfsapfs_error_t **error );

/* Opens a container
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_open(
     libfsapfs_container_t *container,
     const char *filename,
     int access_flags,
     libfsapfs_error_t **error );

#if defined( LIBFSAPFS_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a container
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_open_wide(
     libfsapfs_container_t *container,
     const wchar_t *filename,
     int access_flags,
     libfsapfs_error_t **error );

#endif /* defined( LIBFSAPFS_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFSAPFS_HAVE_BFIO )

/* Opens a container using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_open_file_io_handle(
     libfsapfs_container_t *container,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsapfs_error_t **error );

#endif /* defined( LIBFSAPFS_HAVE_BFIO ) */

/* Closes a container
 * Returns 0 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_close(
     libfsapfs_container_t *container,
     libfsapfs_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_get_size(
     libfsapfs_container_t *container,
     size64_t *size,
     libfsapfs_error_t **error );

/* Retrieves the identifier
 * The identifier is an UUID stored in big-endian and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_get_identifier(
     libfsapfs_container_t *container,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libfsapfs_error_t **error );

/* Determines if the container is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_is_locked(
     libfsapfs_container_t *container,
     libfsapfs_error_t **error );

/* Retrieves the number of volumes
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_get_number_of_volumes(
     libfsapfs_container_t *container,
     int *number_of_volumes,
     libfsapfs_error_t **error );

/* Retrieves a specific of volume
 * The volume reference must be freed after use with libfsapfs_volume_free
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_container_get_volume_by_index(
     libfsapfs_container_t *container,
     int volume_index,
     libfsapfs_volume_t **volume,
     libfsapfs_error_t **error );

/* -------------------------------------------------------------------------
 * Volume functions
 * ------------------------------------------------------------------------- */

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_free(
     libfsapfs_volume_t **volume,
     libfsapfs_error_t **error );

/* Unlocks the volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_unlock(
     libfsapfs_volume_t *volume,
     libfsapfs_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_size(
     libfsapfs_volume_t *volume,
     size64_t *size,
     libfsapfs_error_t **error );

/* Retrieves the identifier
 * The identifier is an UUID stored in big-endian and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_identifier(
     libfsapfs_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_utf8_name_size(
     libfsapfs_volume_t *volume,
     size_t *utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_utf8_name(
     libfsapfs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_utf16_name_size(
     libfsapfs_volume_t *volume,
     size_t *utf16_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_utf16_name(
     libfsapfs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsapfs_error_t **error );

/* Determines if the volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_is_locked(
     libfsapfs_volume_t *volume,
     libfsapfs_error_t **error );

/* Sets an UTF-8 formatted password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_set_utf8_password(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_error_t **error );

/* Sets an UTF-16 formatted password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_set_utf16_password(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_error_t **error );

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_set_utf8_recovery_password(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_error_t **error );

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_set_utf16_recovery_password(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_error_t **error );

/* Retrieves the next file entry identifier
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_next_file_entry_identifier(
     libfsapfs_volume_t *volume,
     uint64_t *identifier,
     libfsapfs_error_t **error );

/* Retrieves a specific file entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_file_entry_by_identifier(
     libfsapfs_volume_t *volume,
     uint64_t identifier,
     libfsapfs_file_entry_t **file_entry,
     libfsapfs_error_t **error );

/* Retrieves the root directory file entry
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_root_directory(
     libfsapfs_volume_t *volume,
     libfsapfs_file_entry_t **file_entry,
     libfsapfs_error_t **error );

/* Retrieves the file entry for an UTF-8 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_file_entry_by_utf8_path(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_file_entry_t **file_entry,
     libfsapfs_error_t **error );

/* Retrieves the file entry for an UTF-16 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_file_entry_by_utf16_path(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_file_entry_t **file_entry,
     libfsapfs_error_t **error );

/* Retrieves the number of snapshots
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_number_of_snapshots(
     libfsapfs_volume_t *volume,
     int *number_of_snapshots,
     libfsapfs_error_t **error );

/* Retrieves a specific of snapshot
 * The snapshot reference must be freed after use with libfsapfs_snapshot_free
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_snapshot_by_index(
     libfsapfs_volume_t *volume,
     int snapshot_index,
     libfsapfs_snapshot_t **snapshot,
     libfsapfs_error_t **error );

/* -------------------------------------------------------------------------
 * Snapshot functions
 * ------------------------------------------------------------------------- */

/* Frees a snapshot
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_snapshot_free(
     libfsapfs_snapshot_t **snapshot,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_snapshot_get_utf8_name_size(
     libfsapfs_snapshot_t *snapshot,
     size_t *utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_snapshot_get_utf8_name(
     libfsapfs_snapshot_t *snapshot,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_snapshot_get_utf16_name_size(
     libfsapfs_snapshot_t *snapshot,
     size_t *utf16_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_snapshot_get_utf16_name(
     libfsapfs_snapshot_t *snapshot,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsapfs_error_t **error );

/* -------------------------------------------------------------------------
 * File entry functions
 * ------------------------------------------------------------------------- */

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_free(
     libfsapfs_file_entry_t **file_entry,
     libfsapfs_error_t **error );

/* Retrieves the identifier (or inode number)
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_identifier(
     libfsapfs_file_entry_t *file_entry,
     uint64_t *identifier,
     libfsapfs_error_t **error );

/* Retrieves the parent identifier (or inode number)
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_parent_identifier(
     libfsapfs_file_entry_t *file_entry,
     uint64_t *parent_identifier,
     libfsapfs_error_t **error );

/* Retrieves the parent file entry
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_parent_file_entry(
     libfsapfs_file_entry_t *file_entry,
     libfsapfs_file_entry_t **parent_file_entry,
     libfsapfs_error_t **error );

/* Retrieves the creation date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_creation_time(
     libfsapfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libfsapfs_error_t **error );

/* Retrieves the modification date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_modification_time(
     libfsapfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libfsapfs_error_t **error );

/* Retrieves the inode change date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_inode_change_time(
     libfsapfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libfsapfs_error_t **error );

/* Retrieves the access date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_access_time(
     libfsapfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libfsapfs_error_t **error );

/* Retrieves the user identifier
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_owner_identifier(
     libfsapfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libfsapfs_error_t **error );

/* Retrieves the group identifier
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_group_identifier(
     libfsapfs_file_entry_t *file_entry,
     uint32_t *group_identifier,
     libfsapfs_error_t **error );

/* Retrieves the file mode
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_file_mode(
     libfsapfs_file_entry_t *file_entry,
     uint16_t *file_mode,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf8_name_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf16_name_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * This value is retrieved from the com.apple.fs.symlink extended attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf8_symbolic_link_target_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * This value is retrieved from the com.apple.fs.symlink extended attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf8_symbolic_link_target(
     libfsapfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * This value is retrieved from the com.apple.fs.symlink extended attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf16_symbolic_link_target_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * This value is retrieved from the com.apple.fs.symlink extended attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf16_symbolic_link_target(
     libfsapfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsapfs_error_t **error );

/* Retrieves the number of extended attributes
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_number_of_extended_attributes(
     libfsapfs_file_entry_t *file_entry,
     int *number_of_extended_attributes,
     libfsapfs_error_t **error );

/* Retrieves the extended attribute for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_extended_attribute_by_index(
     libfsapfs_file_entry_t *file_entry,
     int extended_attribute_index,
     libfsapfs_extended_attribute_t **extended_attribute,
     libfsapfs_error_t **error );

/* Determines if there is an extended attribute for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_has_extended_attribute_by_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_error_t **error );

/* Determines if there is an extended attribute for an UTF-16 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_has_extended_attribute_by_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_error_t **error );

/* Retrieves the extended attribute for an UTF-8 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_extended_attribute_by_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_extended_attribute_t **extended_attribute,
     libfsapfs_error_t **error );

/* Retrieves the extended attribute for an UTF-16 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_extended_attribute_by_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_extended_attribute_t **extended_attribute,
     libfsapfs_error_t **error );

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_number_of_sub_file_entries(
     libfsapfs_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libfsapfs_error_t **error );

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_sub_file_entry_by_index(
     libfsapfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsapfs_file_entry_t **sub_file_entry,
     libfsapfs_error_t **error );

/* Retrieves the sub file entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_file_entry_t **sub_file_entry,
     libfsapfs_error_t **error );

/* Retrieves the sub file entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_file_entry_t **sub_file_entry,
     libfsapfs_error_t **error );

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSAPFS_EXTERN \
ssize_t libfsapfs_file_entry_read_buffer(
         libfsapfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libfsapfs_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSAPFS_EXTERN \
ssize_t libfsapfs_file_entry_read_buffer_at_offset(
         libfsapfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfsapfs_error_t **error );

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
LIBFSAPFS_EXTERN \
off64_t libfsapfs_file_entry_seek_offset(
         libfsapfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libfsapfs_error_t **error );

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_offset(
     libfsapfs_file_entry_t *file_entry,
     off64_t *offset,
     libfsapfs_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_size(
     libfsapfs_file_entry_t *file_entry,
     size64_t *size,
     libfsapfs_error_t **error );

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_number_of_extents(
     libfsapfs_file_entry_t *file_entry,
     int *number_of_extents,
     libfsapfs_error_t **error );

/* Retrieves a specific extent
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_extent_by_index(
     libfsapfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libfsapfs_error_t **error );

/* -------------------------------------------------------------------------
 * Extended attribute functions
 * ------------------------------------------------------------------------- */

/* Frees an extended attribute
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_free(
     libfsapfs_extended_attribute_t **extended_attribute,
     libfsapfs_error_t **error );

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_identifier(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint64_t *identifier,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf8_name_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf8_name(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsapfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf16_name_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libfsapfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf16_name(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsapfs_error_t **error );

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBFSAPFS_EXTERN \
ssize_t libfsapfs_extended_attribute_read_buffer(
         libfsapfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         libfsapfs_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSAPFS_EXTERN \
ssize_t libfsapfs_extended_attribute_read_buffer_at_offset(
         libfsapfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfsapfs_error_t **error );

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
LIBFSAPFS_EXTERN \
off64_t libfsapfs_extended_attribute_seek_offset(
         libfsapfs_extended_attribute_t *extended_attribute,
         off64_t offset,
         int whence,
         libfsapfs_error_t **error );

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_offset(
     libfsapfs_extended_attribute_t *extended_attribute,
     off64_t *offset,
     libfsapfs_error_t **error );

/* Retrieves the size of the data stream object
 * Returns 1 if successful or -1 on error
 */
LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size64_t *size,
     libfsapfs_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_H ) */

