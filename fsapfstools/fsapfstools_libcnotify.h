/*
 * The libcnotify header wrapper
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

#if !defined( _FSAPFSTOOLS_LIBCNOTIFY_H )
#define _FSAPFSTOOLS_LIBCNOTIFY_H

#include <common.h>

/* Define HAVE_LOCAL_LIBCNOTIFY for local use of libcnotify
 */
#if defined( HAVE_LOCAL_LIBCNOTIFY )

#include <libcnotify_definitions.h>
#include <libcnotify_print.h>
#include <libcnotify_stream.h>
#include <libcnotify_verbose.h>

#else

/* If libtool DLL support is enabled set LIBCNOTIFY_DLL_IMPORT
 * before including libcnotify.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT ) && !defined( HAVE_STATIC_EXECUTABLES )
#define LIBCNOTIFY_DLL_IMPORT
#endif

#include <libcnotify.h>

#endif /* defined( HAVE_LOCAL_LIBCNOTIFY ) */

#endif /* !defined( _FSAPFSTOOLS_LIBCNOTIFY_H ) */

