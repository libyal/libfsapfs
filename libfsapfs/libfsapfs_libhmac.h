/*
 * The internal libhmac header
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

#if !defined( _LIBFSAPFS_LIBHMAC_H )
#define _LIBFSAPFS_LIBHMAC_H

#include <common.h>

/* Define HAVE_LOCAL_LIBHMAC for local use of libhmac
 */
#if defined( HAVE_LOCAL_LIBHMAC )

#include <libhmac_definitions.h>
#include <libhmac_md5.h>
#include <libhmac_sha1.h>
#include <libhmac_sha256.h>
#include <libhmac_sha512.h>
#include <libhmac_support.h>
#include <libhmac_types.h>

#else

/* If libtool DLL support is enabled set LIBHMAC_DLL_IMPORT
 * before including libhmac.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBHMAC_DLL_IMPORT
#endif

#include <libhmac.h>

#endif

#endif

