/*
 * Mount handle
 *
 * Copyright (C) 2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _MOUNT_HANDLE_H )
#define _MOUNT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "fsapfstools_libbfio.h"
#include "fsapfstools_libcerror.h"
#include "fsapfstools_libfsapfs.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct mount_handle mount_handle_t;

struct mount_handle
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

	/* The libfsapfs input volume
	 */
	libfsapfs_volume_t *input_volume;

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

int mount_handle_initialize(
     mount_handle_t **mount_handle,
     libcerror_error_t **error );

int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error );

int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error );

int mount_handle_set_file_system_index(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_set_password(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_set_recovery_password(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_set_volume_offset(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_open_input(
     mount_handle_t *mount_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int mount_handle_close_input(
     mount_handle_t *mount_handle,
     libcerror_error_t **error );

int mount_handle_get_volume_by_index(
     mount_handle_t *mount_handle,
     int volume_index,
     libfsapfs_volume_t **volume,
     libcerror_error_t **error );

int mount_handle_get_file_entry_by_path(
     mount_handle_t *mount_handle,
     const system_character_t *path,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_HANDLE_H ) */

