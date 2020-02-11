/*
 * The libfcache header wrapper
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

#if !defined( _LIBFSAPFS_LIBFCACHE_H )
#define _LIBFSAPFS_LIBFCACHE_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFCACHE for local use of libfcache
 */
#if defined( HAVE_LOCAL_LIBFCACHE )

#include <libfcache_cache.h>
#include <libfcache_date_time.h>
#include <libfcache_definitions.h>
#include <libfcache_types.h>

#else

/* If libtool DLL support is enabled set LIBFCACHE_DLL_IMPORT
 * before including libfcache.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFCACHE_DLL_IMPORT
#endif

#include <libfcache.h>

#endif /* defined( HAVE_LOCAL_LIBFCACHE ) */

#endif /* !defined( _LIBFSAPFS_LIBFCACHE_H ) */

