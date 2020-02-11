/*
 * Inode functions
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

#if !defined( _LIBFSAPFS_INODE_H )
#define _LIBFSAPFS_INODE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_inode libfsapfs_inode_t;

struct libfsapfs_inode
{
	/* The identifier
	 */
	uint64_t identifier;

	/* The parent identifier
	 */
	uint64_t parent_identifier;

	/* The modification time
	 */
	uint64_t modification_time;

	/* The creation time
	 */
	uint64_t creation_time;

	/* The inode change time
	 */
	uint64_t inode_change_time;

	/* The access time
	 */
	uint64_t access_time;

	/* The owner identifier
	 */
	uint32_t owner_identifier;

	/* The group identifier
	 */
	uint32_t group_identifier;

	/* The file mode
	 */
	uint16_t file_mode;

	/* The name size
	 */
	uint16_t name_size;

	/* The name
	 */
	uint8_t *name;

	/* The inode flags
	 */
	uint64_t flags;

	/* The data stream identifier
	 */
	uint64_t data_stream_identifier;

	/* The data stream size
	 */
	uint64_t data_stream_size;
};

int libfsapfs_inode_initialize(
     libfsapfs_inode_t **inode,
     libcerror_error_t **error );

int libfsapfs_inode_free(
     libfsapfs_inode_t **inode,
     libcerror_error_t **error );

int libfsapfs_inode_read_key_data(
     libfsapfs_inode_t *inode,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_inode_read_value_data(
     libfsapfs_inode_t *inode,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_inode_get_identifier(
     libfsapfs_inode_t *inode,
     uint64_t *identifier,
     libcerror_error_t **error );

int libfsapfs_inode_get_parent_identifier(
     libfsapfs_inode_t *inode,
     uint64_t *parent_identifier,
     libcerror_error_t **error );

int libfsapfs_inode_get_creation_time(
     libfsapfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error );

int libfsapfs_inode_get_modification_time(
     libfsapfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error );

int libfsapfs_inode_get_inode_change_time(
     libfsapfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error );

int libfsapfs_inode_get_access_time(
     libfsapfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error );

int libfsapfs_inode_get_owner_identifier(
     libfsapfs_inode_t *inode,
     uint32_t *owner_identifier,
     libcerror_error_t **error );

int libfsapfs_inode_get_group_identifier(
     libfsapfs_inode_t *inode,
     uint32_t *group_identifier,
     libcerror_error_t **error );

int libfsapfs_inode_get_file_mode(
     libfsapfs_inode_t *inode,
     uint16_t *file_mode,
     libcerror_error_t **error );

int libfsapfs_inode_get_utf8_name_size(
     libfsapfs_inode_t *inode,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_inode_get_utf8_name(
     libfsapfs_inode_t *inode,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_inode_get_utf16_name_size(
     libfsapfs_inode_t *inode,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_inode_get_utf16_name(
     libfsapfs_inode_t *inode,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_inode_get_flags(
     libfsapfs_inode_t *inode,
     uint64_t *flags,
     libcerror_error_t **error );

int libfsapfs_inode_get_data_stream_identifier(
     libfsapfs_inode_t *inode,
     uint64_t *data_stream_identifier,
     libcerror_error_t **error );

int libfsapfs_inode_get_data_stream_size(
     libfsapfs_inode_t *inode,
     uint64_t *data_stream_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_INODE_H ) */

