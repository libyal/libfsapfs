/*
 * Volume functions
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

#if !defined( _LIBFSAPFS_VOLUME_H )
#define _LIBFSAPFS_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfsapfs_container_data_handle.h"
#include "libfsapfs_container_key_bag.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_extern.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_file_system_data_handle.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_object_map_btree.h"
#include "libfsapfs_snapshot_metadata_tree.h"
#include "libfsapfs_volume_key_bag.h"
#include "libfsapfs_volume_superblock.h"
#include "libfsapfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_internal_volume libfsapfs_internal_volume_t;

struct libfsapfs_internal_volume
{
	/* The volume superblock
	 */
	libfsapfs_volume_superblock_t *superblock;

	/* The container key bag
	 */
	libfsapfs_container_key_bag_t *container_key_bag;

	/* The container data handle
	 */
	libfsapfs_container_data_handle_t *container_data_handle;

	/* The container data block vector
	 */
	libfdata_vector_t *container_data_block_vector;

	/* The object map B-tree
	 */
	libfsapfs_object_map_btree_t *object_map_btree;

	/* The snapshot metadata tree
	 */
	libfsapfs_snapshot_metadata_tree_t *snapshot_metadata_tree;

	/* The snapshot
	 */
	libcdata_array_t *snapshots;

	/* The volume key bag
	 */
	libfsapfs_volume_key_bag_t *key_bag;

	/* The encryption context
	 */
	libfsapfs_encryption_context_t *encryption_context;

	/* The file system data handle
	 */
	libfsapfs_file_system_data_handle_t *file_system_data_handle;

	/* The file system data block vector
	 */
	libfdata_vector_t *file_system_data_block_vector;

	/* The file system B-tree
	 */
	libfsapfs_file_system_btree_t *file_system_btree;

	/* The IO handle
	 */
	libfsapfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the volume is locked
	 */
	uint8_t is_locked;

	/* The user password
	 */
	uint8_t *user_password;

        /* The user password size
	 */
	size_t user_password_size;

	/* Value to indicate the user password is set
	 */
	uint8_t user_password_is_set;

	/* The recovery password
	 */
	uint8_t *recovery_password;

        /* The recovery password size
	 */
	size_t recovery_password_size;

	/* Value to indicate the recovery password is set
	 */
	uint8_t recovery_password_is_set;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsapfs_volume_initialize(
     libfsapfs_volume_t **volume,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsapfs_container_key_bag_t *container_key_bag,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_free(
     libfsapfs_volume_t **volume,
     libcerror_error_t **error );

int libfsapfs_internal_volume_open_read(
     libfsapfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsapfs_internal_volume_close(
     libfsapfs_internal_volume_t *internal_volume,
     libcerror_error_t **error );

int libfsapfs_internal_volume_unlock(
     libfsapfs_internal_volume_t *internal_volume,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_unlock(
     libfsapfs_volume_t *volume,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_size(
     libfsapfs_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_identifier(
     libfsapfs_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_utf8_name_size(
     libfsapfs_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_utf8_name(
     libfsapfs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_utf16_name_size(
     libfsapfs_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_utf16_name(
     libfsapfs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_is_locked(
     libfsapfs_volume_t *volume,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_set_utf8_password(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_set_utf16_password(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_set_utf8_recovery_password(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_set_utf16_recovery_password(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_next_file_entry_identifier(
     libfsapfs_volume_t *volume,
     uint64_t *identifier,
     libcerror_error_t **error );

int libfsapfs_internal_volume_get_file_system_btree(
     libfsapfs_internal_volume_t *internal_volume,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_file_entry_by_identifier(
     libfsapfs_volume_t *volume,
     uint64_t identifier,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_root_directory(
     libfsapfs_volume_t *volume,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_file_entry_by_utf8_path(
     libfsapfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_file_entry_by_utf16_path(
     libfsapfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_number_of_snapshots(
     libfsapfs_volume_t *volume,
     int *number_of_snapshots,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_get_snapshot_by_index(
     libfsapfs_volume_t *volume,
     int snapshot_index,
     libfsapfs_snapshot_t **snapshot,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_VOLUME_H ) */

