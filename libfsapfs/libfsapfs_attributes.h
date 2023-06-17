/*
 * (Extended) attribute functions
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

#if !defined( _LIBFSAPFS_ATTRIBUTE_H )
#define _LIBFSAPFS_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_attribute_values.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsapfs_attributes_get_file_extents(
     libfsapfs_attribute_values_t *attribute_values,
     libbfio_handle_t *file_io_handle,
     libfsapfs_file_system_btree_t *file_system_btree,
     uint64_t transaction_identifier,
     libcerror_error_t **error );

int libfsapfs_attributes_get_data_stream(
     libfsapfs_attribute_values_t *attribute_values,
     libfsapfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfsapfs_file_system_btree_t *file_system_btree,
     uint64_t transaction_identifier,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_ATTRIBUTE_H ) */

