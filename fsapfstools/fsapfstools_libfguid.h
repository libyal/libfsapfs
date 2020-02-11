/*
 * The libfguid header wrapper
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

#if !defined( _FSAPFSTOOLS_LIBFGUID_H )
#define _FSAPFSTOOLS_LIBFGUID_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFGUID for local use of libfguid
 */
#if defined( HAVE_LOCAL_LIBFGUID )

#include <libfguid_definitions.h>
#include <libfguid_identifier.h>
#include <libfguid_types.h>

#else

/* If libtool DLL support is enabled set LIBFGUID_DLL_IMPORT
 * before including libfguid.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT ) && !defined( HAVE_STATIC_EXECUTABLES )
#define LIBFGUID_DLL_IMPORT
#endif

#include <libfguid.h>

#endif /* defined( HAVE_LOCAL_LIBFGUID ) */

#endif /* !defined( _FSAPFSTOOLS_LIBFGUID_H ) */

