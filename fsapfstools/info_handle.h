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

#if !defined( _INFO_HANDLE_H )
#define _INFO_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "fsapfstools_libbfio.h"
#include "fsapfstools_libcerror.h"
#include "fsapfstools_libfsapfs.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct info_handle info_handle_t;

struct info_handle
{
	/* The file system index
	 */
	int file_system_index;

	/* The recovery password
	 */
	system_character_t *recovery_password;

	/* The recovery password size
	 */
	size_t recovery_password_size;

	/* The user password
	 */
	system_character_t *user_password;

	/* The user password size
	 */
	size_t user_password_size;

	/* The volume offset
	 */
	off64_t volume_offset;

	/* The libbfio input file IO handle
	 */
	libbfio_handle_t *input_file_io_handle;

	/* The libfsapfs input container
	 */
	libfsapfs_container_t *input_container;

	/* Value to indicate the container is locked
	 */
	uint8_t container_is_locked;

	/* The bodyfile output stream
	 */
	FILE *bodyfile_stream;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int fsapfstools_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error );

int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error );

int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error );

int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_set_bodyfile(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int info_handle_set_file_system_index(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_set_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_set_recovery_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_get_volume_by_index(
     info_handle_t *info_handle,
     int volume_index,
     libfsapfs_volume_t **volume,
     libcerror_error_t **error );

int info_handle_posix_time_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     int64_t value_64bit,
     libcerror_error_t **error );

int info_handle_uuid_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     const uint8_t *uuid_data,
     libcerror_error_t **error );

int info_handle_file_entry_value_fprint(
     info_handle_t *info_handle,
     libfsapfs_file_entry_t *file_entry,
     const system_character_t *path,
     libcerror_error_t **error );

int info_handle_file_entry_value_with_name_fprint(
     info_handle_t *info_handle,
     libfsapfs_file_entry_t *file_entry,
     const system_character_t *path,
     const system_character_t *file_entry_name,
     libcerror_error_t **error );

int info_handle_file_system_hierarchy_fprint_file_entry(
     info_handle_t *info_handle,
     libfsapfs_file_entry_t *file_entry,
     const system_character_t *path,
     libcerror_error_t **error );

int info_handle_file_system_hierarchy_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_file_entry_fprint_by_identifier(
     info_handle_t *info_handle,
     uint64_t file_system_identifier,
     libcerror_error_t **error );

int info_handle_file_entry_fprint_by_path(
     info_handle_t *info_handle,
     const system_character_t *path,
     libcerror_error_t **error );

int info_handle_file_entries_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_volume_fprint(
     info_handle_t *info_handle,
     int volume_index,
     libfsapfs_volume_t *volume,
     libcerror_error_t **error );

int info_handle_container_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _INFO_HANDLE_H ) */

