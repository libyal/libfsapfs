/*
 * The data block vector functions
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

#if !defined( _LIBFSAPFS_DATA_BLOCK_VECTOR_H )
#define _LIBFSAPFS_DATA_BLOCK_VECTOR_H

#include <common.h>
#include <types.h>

#include "libfsapfs_file_system_data_handle.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsapfs_data_block_vector_initialize(
     libfdata_vector_t **vector,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_file_system_data_handle_t *data_handle,
     libcdata_array_t *file_extents,
     uint8_t is_sparse,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_DATA_BLOCK_VECTOR_H ) */

