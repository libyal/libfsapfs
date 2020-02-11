/*
 * GetOpt functions
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

#if !defined( _FSAPFSTOOLS_GETOPT_H )
#define _FSAPFSTOOLS_GETOPT_H

#include <common.h>
#include <types.h>

/* unistd.h is included here to export getopt, optarg, optind and optopt
 */
#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_GETOPT )
#define fsapfstools_getopt( argument_count, argument_values, options_string ) \
	getopt( argument_count, argument_values, options_string )

#else

#if !defined( __CYGWIN__ )
extern int optind;
extern system_character_t *optarg;
extern system_integer_t optopt;

#else
int optind;
system_character_t *optarg;
system_integer_t optopt;

#endif /* !defined( __CYGWIN__ ) */

system_integer_t fsapfstools_getopt(
                  int argument_count,
                  system_character_t * const argument_values[],
                  const system_character_t *options_string );

#endif /* defined( HAVE_GETOPT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFSTOOLS_GETOPT_H ) */

