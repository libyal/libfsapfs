/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBFSAPFS_IO_HANDLE_H )
#define _LIBFSAPFS_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"
#include "libfsapfs_profiler.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char fsapfs_container_signature[ 4 ];
extern const char fsapfs_volume_signature[ 4 ];

typedef struct libfsapfs_io_handle libfsapfs_io_handle_t;

struct libfsapfs_io_handle
{
	/* The bytes per sector
	 */
	uint16_t bytes_per_sector;

	/* The block size
	 */
	uint32_t block_size;

	/* The container size
	 */
	size64_t container_size;

#if defined( HAVE_PROFILER )
	/* The profiler
	 */
	libfsapfs_profiler_t *profiler;
#endif

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfsapfs_io_handle_initialize(
     libfsapfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsapfs_io_handle_free(
     libfsapfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsapfs_io_handle_clear(
     libfsapfs_io_handle_t *io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_IO_HANDLE_H ) */

