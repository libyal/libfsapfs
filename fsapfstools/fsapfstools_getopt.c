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

#include <common.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfstools_getopt.h"
#include "fsapfstools_libcnotify.h"

#if !defined( HAVE_GETOPT )

/* The option index
 * Start with argument 1 (argument 0 is the program name)
 */
int optind = 1;

/* The current option argument
 */
system_character_t *optarg = NULL;

/* Value to indicate the current option
 */
system_integer_t optopt = 0;

/* The next option in a group
 */
system_character_t *next_option = NULL;

/* Get the program options
 * Function for platforms that do not have the getopt function
 * Returns the option character processed, or -1 on error,
 * ? if the option was not in the options string, : if the option argument was missing
 */
system_integer_t fsapfstools_getopt(
                  int argument_count,
                  system_character_t * const argument_values[],
                  const system_character_t *options_string )
{
	system_character_t *argument_value = NULL;
	system_character_t *option_value   = NULL;
	static char *function              = "fsapfstools_getopt";
	size_t options_string_length       = 0;

	if( next_option != NULL )
	{
		argument_value = next_option;
		next_option    = NULL;
	}
	else if( optind >= argument_count )
	{
		return( (system_integer_t) -1 );
	}
	else
	{
		argument_value = argument_values[ optind ];

		/* Check if the argument value is not an empty string
		 */
		if( *argument_value == (system_character_t) '\0' )
		{
			return( (system_integer_t) -1 );
		}
		/* Check if the first character is a option marker '-'
		 */
		if( *argument_value != (system_character_t) '-' )
		{
			return( (system_integer_t) -1 );
		}
		argument_value++;

		/* Check if long options are provided '--'
		 */
		if( *argument_value == (system_character_t) '-' )
		{
			optind++;

			return( (system_integer_t) -1 );
		}
	}
	options_string_length = system_string_length(
	                         options_string );

	optopt       = *argument_value;
	option_value = system_string_search_character(
	                options_string,
	                optopt,
	                options_string_length );

	argument_value++;

	/* Check if an argument was specified or that the option was not found
	 * in the option string
	 */
	if( ( optopt == (system_integer_t) ':' )
	 || ( option_value == NULL ) )
	{
		if( *argument_value == (system_character_t) '\0' )
		{
			optind++;
		}
		if( ( *options_string != (system_character_t) ':' )
		 && ( optopt != (system_integer_t) '?' ) )
		{
			libcnotify_printf(
			 "%s: no such option: %" PRIc_SYSTEM ".\n",
			 function,
			 optopt );
		}
		return( (system_integer_t) '?' );
	}
	option_value++;

	/* Check if no option argument is required
	 */
	if( *option_value != (system_character_t) ':' )
	{
		optarg = NULL;

		if( *argument_value == (system_character_t) '\0' )
		{
			optind++;
		}
		else
		{
			/* Multiple options are grouped
			 */
			next_option = argument_value;
		}
	}
	/* Check if the argument is right after the option flag with no space in between
	 */
	else if( *argument_value != (system_character_t) '\0' )
	{
		optarg = argument_value;

		optind++;
	}
	else
	{
		optind++;

		/* Check if the argument was provided as the next argument value
		 */
		if( argument_count <= optind )
		{
			if( *option_value == ':' )
			{
				return( (system_integer_t) ':' );
			}
			libcnotify_printf(
			 "%s: option: %" PRIc_SYSTEM " requires an argument.\n",
			 function,
			 optopt );

			return( (system_integer_t) '?' );
		}
		optarg = argument_values[ optind ];

		optind++;
	}
	return( optopt );
}

#endif /* !defined( HAVE_GETOPT ) */

