/*
 * The internal type definitions
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

#if !defined( _LIBFSAPFS_INTERNAL_TYPES_H )
#define _LIBFSAPFS_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBFSAPFS for local use of libfsapfs
 * The definitions in <libfsapfs/types.h> are copied here
 * for local use of libfsapfs
 */
#if defined( HAVE_LOCAL_LIBFSAPFS )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libfsapfs_container {}		libfsapfs_container_t;
typedef struct libfsapfs_extended_attribute {}	libfsapfs_extended_attribute_t;
typedef struct libfsapfs_file_entry {}		libfsapfs_file_entry_t;
typedef struct libfsapfs_snapshot {}		libfsapfs_snapshot_t;
typedef struct libfsapfs_volume {}		libfsapfs_volume_t;

#else
typedef intptr_t libfsapfs_container_t;
typedef intptr_t libfsapfs_extended_attribute_t;
typedef intptr_t libfsapfs_file_entry_t;
typedef intptr_t libfsapfs_snapshot_t;
typedef intptr_t libfsapfs_volume_t;

#endif /* defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI ) */

#endif /* defined( HAVE_LOCAL_LIBFSAPFS ) */

#endif /* !defined( _LIBFSAPFS_INTERNAL_TYPES_H ) */

