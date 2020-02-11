/*
 * Container functions
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

#if !defined( _LIBFSAPFS_CONTAINER_H )
#define _LIBFSAPFS_CONTAINER_H

#include <common.h>
#include <types.h>

#include "libfsapfs_checkpoint_map.h"
#include "libfsapfs_container_data_handle.h"
#include "libfsapfs_container_key_bag.h"
#include "libfsapfs_container_superblock.h"
#include "libfsapfs_extern.h"
#include "libfsapfs_fusion_middle_tree.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_object_map_btree.h"
#include "libfsapfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_internal_container libfsapfs_internal_container_t;

struct libfsapfs_internal_container
{
	/* The container superblock
	 */
	libfsapfs_container_superblock_t *superblock;

	/* The Fusion middle tree
	 */
	libfsapfs_fusion_middle_tree_t *fusion_middle_tree;

	/* The checkpoint map
	 */
	libfsapfs_checkpoint_map_t *checkpoint_map;

	/* The container data handle
	 */
	libfsapfs_container_data_handle_t *container_data_handle;

	/* The data block vector
	 */
	libfdata_vector_t *data_block_vector;

	/* The object map B-tree
	 */
	libfsapfs_object_map_btree_t *object_map_btree;

	/* The container key bag
	 */
	libfsapfs_container_key_bag_t *key_bag;

	/* The IO handle
	 */
	libfsapfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBFSAPFS_EXTERN \
int libfsapfs_container_initialize(
     libfsapfs_container_t **container,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_free(
     libfsapfs_container_t **container,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_signal_abort(
     libfsapfs_container_t *container,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_open(
     libfsapfs_container_t *container,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFSAPFS_EXTERN \
int libfsapfs_container_open_wide(
     libfsapfs_container_t *container,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFSAPFS_EXTERN \
int libfsapfs_container_open_file_io_handle(
     libfsapfs_container_t *container,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_close(
     libfsapfs_container_t *container,
     libcerror_error_t **error );

int libfsapfs_internal_container_open_read(
     libfsapfs_internal_container_t *internal_container,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_get_size(
     libfsapfs_container_t *container,
     size64_t *size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_get_identifier(
     libfsapfs_container_t *container,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_is_locked(
     libfsapfs_container_t *container,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_get_number_of_volumes(
     libfsapfs_container_t *container,
     int *number_of_volumes,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_container_get_volume_by_index(
     libfsapfs_container_t *container,
     int volume_index,
     libfsapfs_volume_t **volume,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_CONTAINER_H ) */

