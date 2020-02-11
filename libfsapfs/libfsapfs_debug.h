/*
 * Debug functions
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

#if !defined( _LIBFSAPFS_DEBUG_H )
#define _LIBFSAPFS_DEBUG_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

void libfsapfs_debug_print_btree_flags(
      uint32_t btree_flags );

void libfsapfs_debug_print_btree_node_flags(
      uint16_t btree_node_flags );

void libfsapfs_debug_print_checkpoint_flags(
      uint32_t checkpoint_flags );

void libfsapfs_debug_print_container_compatible_features_flags(
      uint64_t compatible_features_flags );

void libfsapfs_debug_print_container_incompatible_features_flags(
      uint64_t incompatible_features_flags );

void libfsapfs_debug_print_container_read_only_compatible_features_flags(
      uint64_t read_only_compatible_features_flags );

void libfsapfs_debug_print_directory_entry_flags(
      uint16_t directory_entry_flags );

void libfsapfs_debug_print_extended_attribute_flags(
      uint16_t extended_attribute_flags );

void libfsapfs_debug_print_extended_field_flags(
      uint8_t extended_field_flags );

void libfsapfs_debug_print_inode_flags(
      uint64_t inode_flags );

void libfsapfs_debug_print_volume_compatible_features_flags(
      uint64_t compatible_features_flags );

void libfsapfs_debug_print_volume_flags(
      uint64_t volume_flags );

void libfsapfs_debug_print_volume_incompatible_features_flags(
      uint64_t incompatible_features_flags );

void libfsapfs_debug_print_volume_read_only_compatible_features_flags(
      uint64_t read_only_compatible_features_flags );

const char *libfsapfs_debug_print_file_system_data_type(
             uint8_t file_system_data_type );

const char *libfsapfs_debug_print_directory_record_extended_field_type(
             uint8_t extended_field_type );

const char *libfsapfs_debug_print_inode_extended_field_type(
             uint8_t extended_field_type );

int libfsapfs_debug_print_posix_time_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint8_t value_type,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfsapfs_debug_print_guid_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfsapfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_DEBUG_H ) */

