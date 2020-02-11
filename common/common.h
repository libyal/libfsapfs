/*
 * Common include file
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

#if !defined( _COMMON_H )
#define _COMMON_H

#if defined( HAVE_CONFIG_H )
#include "config.h"
#endif

/* Include the Borland/CodeGear C++ Builder compiler specific configuration
 */
#if defined( __BORLANDC__ )
#include "config_borlandc.h"

/* Include the Microsoft Visual Studio C++ compiler specific configuration
 */
#elif defined( _MSC_VER )
#include "config_msc.h"
#endif

#include "config_winapi.h"

#endif /* !defined( _COMMON_H ) */

