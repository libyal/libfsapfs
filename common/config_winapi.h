/*
 * Configuration file for WINAPI
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

#if !defined( _CONFIG_WINAPI_H )
#define _CONFIG_WINAPI_H

/* Define the earliest supported WINAPI version
#define WINVER				0x0501
 */

/* If necessary make sure WINAPI is defined
 */
#if defined( HAVE_WINDOWS_H ) || defined( __BORLANDC__ ) || defined( _MSC_VER )
#include <windows.h>
#endif

#if defined( WINAPI )

/* Define to the address where bug reports for this package should be sent.
 */
#define PACKAGE_BUGREPORT		"joachim.metz@gmail.com"

/* Define the size of the integer for WINAPI
 */
#if !defined( SIZEOF_INT )
#define SIZEOF_INT			4
#endif

/* Define the size of size_t for WINAPI
 */
#if !defined( SIZEOF_SIZE_T )
#if __WORDSIZE == 64
#define SIZEOF_SIZE_T			8
#else
#define SIZEOF_SIZE_T			4
#endif
#endif

/* Define the size of the wide character for WINAPI
 */
#if !defined( SIZEOF_WCHAR_T )
#define SIZEOF_WCHAR_T			2
#endif

/* Enable the DllMain function
 */
#define HAVE_DLLMAIN			1

/* Enable verbose output
#define HAVE_VERBOSE_OUTPUT		1
 */

/* Enable debug output
#define HAVE_DEBUG_OUTPUT		1
 */

/* Enable both the narrow and wide character functions
 */
#if !defined( HAVE_WIDE_CHARACTER_TYPE )
#define HAVE_WIDE_CHARACTER_TYPE	1
#endif

/* If not controlled by config.h enable multi-thread support
 */
#if !defined( HAVE_CONFIG_H ) && !defined( HAVE_MULTI_THREAD_SUPPORT )
#define HAVE_MULTI_THREAD_SUPPORT	1
#endif

#endif /* defined( WINAPI ) */

#endif /* !defined( _CONFIG_WINAPI_H ) */

