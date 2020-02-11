/*
 * File entry functions
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

#if !defined( _LIBFSAPFS_FILE_ENTRY_H )
#define _LIBFSAPFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsapfs_directory_record.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_extern.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_internal_file_entry libfsapfs_internal_file_entry_t;

struct libfsapfs_internal_file_entry
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

	/* The inode
	 */
	libfsapfs_inode_t *inode;

	/* The directory record
	 */
	libfsapfs_directory_record_t *directory_record;

	/* The extended attributes
	 */
	libcdata_array_t *extended_attributes;

	/* The compressed data extended attribute
	 */
	libfsapfs_extended_attribute_t *compressed_data_extended_attribute;

	/* The resource fork extended attribute
	 */
	libfsapfs_extended_attribute_t *resource_fork_extended_attribute;

	/* The symbolic link extended attribute
	 */
	libfsapfs_extended_attribute_t *symbolic_link_extended_attribute;

	/* The symbolic link data
	 */
	uint8_t *symbolic_link_data;

	/* The symbolic link data size
	 */
	size_t symbolic_link_data_size;

	/* The directory entries
	 */
	libcdata_array_t *directory_entries;

	/* The file size
	 */
	size64_t file_size;

	/* The compression method
	 */
	uint32_t compression_method;

	/* The file extents
	 */
	libcdata_array_t *file_extents;

	/* The data stream
	 */
	libfdata_stream_t *data_stream;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsapfs_file_entry_initialize(
     libfsapfs_file_entry_t **file_entry,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_inode_t *inode,
     libfsapfs_directory_record_t *directory_record,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_free(
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_identifier(
     libfsapfs_file_entry_t *file_entry,
     uint64_t *identifier,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_parent_identifier(
     libfsapfs_file_entry_t *file_entry,
     uint64_t *parent_identifier,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_parent_file_entry(
     libfsapfs_file_entry_t *file_entry,
     libfsapfs_file_entry_t **parent_file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_creation_time(
     libfsapfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_modification_time(
     libfsapfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_access_time(
     libfsapfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_inode_change_time(
     libfsapfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_owner_identifier(
     libfsapfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_group_identifier(
     libfsapfs_file_entry_t *file_entry,
     uint32_t *group_identifier,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_file_mode(
     libfsapfs_file_entry_t *file_entry,
     uint16_t *file_mode,
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

int libfsapfs_internal_file_entry_get_extended_attributes(
     libfsapfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

int libfsapfs_internal_file_entry_get_symbolic_link_data(
     libfsapfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf8_symbolic_link_target_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf8_symbolic_link_target(
     libfsapfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf16_symbolic_link_target_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_utf16_symbolic_link_target(
     libfsapfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_number_of_extended_attributes(
     libfsapfs_file_entry_t *file_entry,
     int *number_of_extended_attributes,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_extended_attribute_by_index(
     libfsapfs_file_entry_t *file_entry,
     int extended_attribute_index,
     libfsapfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_has_extended_attribute_by_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_has_extended_attribute_by_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_extended_attribute_by_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_extended_attribute_by_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

int libfsapfs_internal_file_entry_get_directory_entries(
     libfsapfs_internal_file_entry_t *internal_file_entry,
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

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsapfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsapfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

int libfsapfs_internal_file_entry_get_file_extents(
     libfsapfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

int libfsapfs_internal_file_entry_get_data_stream(
     libfsapfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
ssize_t libfsapfs_file_entry_read_buffer(
         libfsapfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSAPFS_EXTERN \
ssize_t libfsapfs_file_entry_read_buffer_at_offset(
         libfsapfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSAPFS_EXTERN \
off64_t libfsapfs_file_entry_seek_offset(
         libfsapfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_offset(
     libfsapfs_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error );

int libfsapfs_internal_file_entry_get_file_size(
     libfsapfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_size(
     libfsapfs_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_number_of_extents(
     libfsapfs_file_entry_t *file_entry,
     int *number_of_extents,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_file_entry_get_extent_by_index(
     libfsapfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_FILE_ENTRY_H ) */

