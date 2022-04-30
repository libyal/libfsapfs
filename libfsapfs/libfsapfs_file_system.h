/*
 * The file system functions
 *
 * Copyright (C) 2018-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_file_system libfsapfs_file_system_t;

struct libfsapfs_file_system
{
	/* Dummy value
	 */
	int dummy;
};

int libfsapfs_file_system_initialize(
     libfsapfs_file_system_t **file_system,
     libcerror_error_t **error );

int libfsapfs_file_system_free(
     libfsapfs_file_system_t **file_system,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_FILE_SYSTEM_H ) */

