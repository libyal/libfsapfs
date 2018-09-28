/*
 * File entry functions
 *
 * Copyright (C) 2010-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSAPFS_FILE_ENTRY_H )
#define _LIBFSAPFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsapfs_extern.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_internal_file_entry libfsapfs_internal_file_entry_t;

struct libfsapfs_internal_file_entry
{
	/* Inode
	 */
	libfsapfs_inode_t *inode;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The file system B-tree
	 */
	libfsapfs_file_system_btree_t *file_system_btree;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsapfs_file_entry_initialize(
     libfsapfs_file_entry_t **file_entry,
     libbfio_handle_t *file_io_handle,
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_inode_t *inode,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_free(
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf8_name_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf16_name_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_number_of_sub_file_entries(
     libfsapfs_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_sub_file_entry_by_index(
     libfsapfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsapfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_FILE_ENTRY_H ) */

