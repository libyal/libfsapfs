/*
 * The file system functions
 *
 * Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSAPFS_FILE_SYSTEM_H )
#define _LIBFSAPFS_FILE_SYSTEM_H

#include <common.h>
#include <types.h>

#include "libfsapfs_encryption_context.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_file_system libfsapfs_file_system_t;

struct libfsapfs_file_system
{
	/* The IO handle
	 */
	libfsapfs_io_handle_t *io_handle;

	/* The encryption context
	 */
	libfsapfs_encryption_context_t *encryption_context;

	/* The file system B-tree
	 */
	libfsapfs_file_system_btree_t *file_system_btree;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsapfs_file_system_initialize(
     libfsapfs_file_system_t **file_system,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfsapfs_file_system_btree_t *file_system_btree,
     libcerror_error_t **error );

int libfsapfs_file_system_free(
     libfsapfs_file_system_t **file_system,
     libcerror_error_t **error );

int libfsapfs_file_system_get_file_entry_by_identifier(
     libfsapfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     uint64_t transaction_identifier,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsapfs_file_system_get_file_entry_by_utf8_path(
     libfsapfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint64_t transaction_identifier,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsapfs_file_system_get_file_entry_by_utf16_path(
     libfsapfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint64_t transaction_identifier,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_FILE_SYSTEM_H ) */

