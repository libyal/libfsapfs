/*
 * Mount file entry
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

#if !defined( _MOUNT_FILE_ENTRY_H )
#define _MOUNT_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "fsapfstools_libcerror.h"
#include "fsapfstools_libfsapfs.h"
#include "mount_file_system.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct mount_file_entry mount_file_entry_t;

struct mount_file_entry
{
	/* The file system
	 */
	mount_file_system_t *file_system;

	/* The name
	 */
	system_character_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The file entry
	 */
	libfsapfs_file_entry_t *fsapfs_file_entry;
};

int mount_file_entry_initialize(
     mount_file_entry_t **file_entry,
     mount_file_system_t *file_system,
     const system_character_t *name,
     size_t name_length,
     libfsapfs_file_entry_t *fsapfs_file_entry,
     libcerror_error_t **error );

int mount_file_entry_free(
     mount_file_entry_t **file_entry,
     libcerror_error_t **error );

int mount_file_entry_get_parent_file_entry(
     mount_file_entry_t *file_entry,
     mount_file_entry_t **parent_file_entry,
     libcerror_error_t **error );

int mount_file_entry_get_creation_time(
     mount_file_entry_t *file_entry,
     uint64_t *creation_time,
     libcerror_error_t **error );

int mount_file_entry_get_access_time(
     mount_file_entry_t *file_entry,
     uint64_t *access_time,
     libcerror_error_t **error );

int mount_file_entry_get_modification_time(
     mount_file_entry_t *file_entry,
     uint64_t *modification_time,
     libcerror_error_t **error );

int mount_file_entry_get_inode_change_time(
     mount_file_entry_t *file_entry,
     uint64_t *inode_change_time,
     libcerror_error_t **error );

int mount_file_entry_get_file_mode(
     mount_file_entry_t *file_entry,
     uint16_t *file_mode,
     libcerror_error_t **error );

int mount_file_entry_get_name_size(
     mount_file_entry_t *file_entry,
     size_t *string_size,
     libcerror_error_t **error );

int mount_file_entry_get_name(
     mount_file_entry_t *file_entry,
     system_character_t *string,
     size_t string_size,
     libcerror_error_t **error );

int mount_file_entry_get_symbolic_link_target(
     mount_file_entry_t *file_entry,
     system_character_t *string,
     size_t string_size,
     libcerror_error_t **error );

int mount_file_entry_get_number_of_sub_file_entries(
     mount_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libcerror_error_t **error );

int mount_file_entry_get_sub_file_entry_by_index(
     mount_file_entry_t *file_entry,
     int sub_file_entry_index,
     mount_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

ssize_t mount_file_entry_read_buffer_at_offset(
         mount_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

int mount_file_entry_get_size(
     mount_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_FILE_ENTRY_H ) */

