/*
 * The libfmos header wrapper
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

#if !defined( _LIBFSAPFS_LIBFMOS_H )
#define _LIBFSAPFS_LIBFMOS_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFMOS for local use of libfmos
 */
#if defined( HAVE_LOCAL_LIBFMOS )

#include <libfmos_definitions.h>
#include <libfmos_adc.h>
#include <libfmos_lzfse.h>
#include <libfmos_lzvn.h>
#include <libfmos_types.h>

#else

/* If libtool DLL support is enabled set LIBFMOS_DLL_IMPORT
 * before including libfmos.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFMOS_DLL_IMPORT
#endif

#include <libfmos.h>

#endif /* defined( HAVE_LOCAL_LIBFMOS ) */

#endif /* !defined( _LIBFSAPFS_LIBFMOS_H ) */

