/*
 * Extended attribute functions
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

#if !defined( _LIBFSAPFS_EXTENDED_ATTRIBUTE_H )
#define _LIBFSAPFS_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_attribute_values.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_extern.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_libfdata.h"
#include "libfsapfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_internal_extended_attribute libfsapfs_internal_extended_attribute_t;

struct libfsapfs_internal_extended_attribute
{
	/* The IO handle
	 */
	libfsapfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The encryption context
	 */
	libfsapfs_encryption_context_t *encryption_context;

	/* The file system B-tree
	 */
	libfsapfs_file_system_btree_t *file_system_btree;

	/* The identifier
	 */
	uint64_t identifier;

	/* The attribute values
	 */
	libfsapfs_attribute_values_t *attribute_values;

	/* The transaction identifier
	 */
	uint64_t transaction_identifier;

	/* The data stream
	 */
	libfdata_stream_t *data_stream;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsapfs_extended_attribute_initialize(
     libfsapfs_extended_attribute_t **extended_attribute,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_attribute_values_t *attribute_values,
     uint64_t transaction_identifier,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_free(
     libfsapfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

int libfsapfs_extended_attribute_get_data_stream(
     libfsapfs_extended_attribute_t *extended_attribute,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_identifier(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint64_t *identifier,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf8_name_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf8_name(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf16_name_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf16_name(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_internal_extended_attribute_get_data_stream(
     libfsapfs_internal_extended_attribute_t *internal_extended_attribute,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
ssize_t libfsapfs_extended_attribute_read_buffer(
         libfsapfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSAPFS_EXTERN \
ssize_t libfsapfs_extended_attribute_read_buffer_at_offset(
         libfsapfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSAPFS_EXTERN \
off64_t libfsapfs_extended_attribute_seek_offset(
         libfsapfs_extended_attribute_t *extended_attribute,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_offset(
     libfsapfs_extended_attribute_t *extended_attribute,
     off64_t *offset,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size64_t *size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_number_of_extents(
     libfsapfs_extended_attribute_t *extended_attribute,
     int *number_of_extents,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_extent_by_index(
     libfsapfs_extended_attribute_t *extended_attribute,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_EXTENDED_ATTRIBUTE_H ) */

