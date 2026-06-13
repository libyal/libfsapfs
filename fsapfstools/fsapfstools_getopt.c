/*
 * GetOpt functions
 *
 * Copyright (C) 2018-2026, Joachim Metz <joachim.metz@gmail.com>
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

/* Prints text word wrapped
 */
void fsapfstools_getopt_wrapped_fprint(
      FILE *stream,
      int maximum_line_length,
      int start_line_index,
      const char *indentation,
      const char *text,
      size_t text_length )
{
	size_t line_length      = 0;
	size_t line_offset      = 0;
	size_t next_word_offset = 0;
	size_t word_offset      = 0;

	if( stream == NULL )
	{
		return;
	}
	if( text == NULL )
	{
		return;
	}
	if( indentation == NULL )
	{
		return;
	}
	maximum_line_length -= start_line_index;

	while( line_offset < text_length )
	{
		while( ( next_word_offset - line_offset ) < (size_t) maximum_line_length )
		{
			while( next_word_offset < text_length )
			{
				if( ( text[ next_word_offset ] == ' ' )
				 || ( text[ next_word_offset ] == '\n' ) )
				{
					break;
				}
				next_word_offset++;
			}
			if( next_word_offset >= text_length )
			{
				word_offset = text_length;

				break;
			}
			next_word_offset++;

			word_offset = next_word_offset;
		}
		line_length = word_offset - line_offset;

		if( line_offset == 0 )
		{
			fprintf(
			 stream,
			 "%.*s\n",
			 (int) line_length,
			 &( text[ line_offset ] ) );
		}
		else
		{
			fprintf(
			 stream,
			 "%s%*s%.*s\n",
			 indentation,
			 start_line_index,
			 "",
			 (int) line_length,
			 &( text[ line_offset ] ) );
		}
		line_offset = word_offset;
		word_offset = next_word_offset;

		if( line_offset >= text_length )
		{
			break;
		}
	}
}

/* Prints usage information
 */
void fsapfstools_getopt_usage_fprint(
      FILE *stream,
      const char *program,
      const char *description,
      const fsapfstools_option_t *options,
      int number_of_options )
{
	const fsapfstools_option_t *option   = NULL;
	size_t description_length          = 0;
	size_t largest_string_length       = 0;
	size_t line_length                 = 0;
	size_t maximum_line_length         = 80;
	size_t prefix_length               = 0;
	size_t string_length               = 0;
	int number_of_options_without_name = 0;
	int option_index                   = 0;
	int print_count                    = 0;

	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
	if( options == NULL )
	{
		return;
	}
	if( description != NULL )
	{
		description_length = narrow_string_length(
		                      description );

		fsapfstools_getopt_wrapped_fprint(
		 stream,
		 maximum_line_length,
		 0,
		 "",
		 description,
		 description_length );

		fprintf(
		 stream,
		 "\n" );
	}
	fprintf(
	 stream,
	 "Usage: %s",
	 program );

	prefix_length = 7 + narrow_string_length(
	                     program );

	line_length = prefix_length;

	/* Print the options with an argument for the usage string
	 */
	for( option_index = 0;
	     option_index < number_of_options;
	     option_index++ )
	{
		option = &( options[ option_index ] );

		if( ( option->identifier == 0 )
		 || ( option->help == NULL ) )
		{
			continue;
		}
		if( option->name == NULL )
		{
			number_of_options_without_name++;

			continue;
		}
		string_length = 8 + narrow_string_length(
		                     option->name );

		if( ( line_length + string_length ) > maximum_line_length )
		{
			fprintf(
			 stream,
			 "\n%*s",
			 (int) prefix_length,
			 "" );

			line_length = prefix_length;
		}
		fprintf(
		 stream,
		 " [ -%c %s ]",
		 option->identifier,
		 option->name );

		line_length += string_length;
	}
	/* Print the options without an argument for the usage string
	 */
	if( number_of_options_without_name > 0 )
	{
		string_length += 6 + number_of_options_without_name;

		if( ( line_length + string_length ) > maximum_line_length )
		{
			fprintf(
			 stream,
			 "\n%*s",
			 (int) prefix_length,
			 "" );

			line_length = prefix_length;
		}
		fprintf(
		 stream,
		 " [ -" );

		for( option_index = 0;
		     option_index < number_of_options;
		     option_index++ )
		{
			option = &( options[ option_index ] );

			if( ( option->identifier == 0 )
			 || ( option->name != NULL )
			 || ( option->help == NULL ) )
			{
				continue;
			}
			fprintf(
			 stream,
			 "%c",
			 option->identifier );
		}
		fprintf(
		 stream,
		 " ]" );

		line_length += string_length;
	}
	/* Print the fixed position arguments for the usage string
	 */
	for( option_index = 0;
	     option_index < number_of_options;
	     option_index++ )
	{
		option = &( options[ option_index ] );

		if( ( option->identifier != 0 )
		 || ( option->name == NULL ) )
		{
			continue;
		}
		string_length = narrow_string_length(
		                 option->name );

		if( string_length > largest_string_length )
		{
			largest_string_length = string_length;
		}
		if( ( line_length - string_length ) > maximum_line_length )
		{
			fprintf(
			 stream,
			 "\n%*s",
			 (int) prefix_length,
			 "" );

			line_length = prefix_length;
		}
		fprintf(
		 stream,
		 " %s",
		 option->name );

		line_length += string_length;
	}
	fprintf(
	 stream,
	 "\n\n" );

	largest_string_length += 2;

	/* Print a description of the fixed position arguments
	 */
	for( option_index = 0;
	     option_index < number_of_options;
	     option_index++ )
	{
		option = &( options[ option_index ] );

		if( ( option->identifier == 0 )
		 && ( option->name != NULL ) )
		{
			string_length = narrow_string_length(
			                 option->name );

			fprintf(
			 stream,
			 "\t%s:%*s%s\n\n",
			 option->name,
			 (int) ( largest_string_length - string_length - 1 ),
			 "",
			 option->help );
		}
	}
	/* Print a description of the options
	 */
	for( option_index = 0;
	     option_index < number_of_options;
	     option_index++ )
	{
		option = &( options[ option_index ] );

		if( ( option->identifier == 0 )
		 || ( option->help == NULL ) )
		{
			continue;
		}
		print_count = fprintf(
		               stream,
		               "\t-%c:%*s",
		               option->identifier,
		               (int) ( largest_string_length - 3 ),
		               "" );

		if( print_count > 1 )
		{
			string_length = narrow_string_length(
			                 option->help );

			fsapfstools_getopt_wrapped_fprint(
			 stream,
			 maximum_line_length,
			 print_count - 1,
			 "\t",
			 option->help,
			 string_length );
		}
	}
}

/* Determines the getopt options string
 */
int fsapfstools_getopt_get_options_string(
     const fsapfstools_option_t *options,
     int number_of_options,
     system_character_t *options_string,
     size_t options_string_size )
{
	const fsapfstools_option_t *option = NULL;
	size_t options_string_index      = 0;
	int option_index                 = 0;

	if( options == NULL )
	{
		return( -1 );
	}
	if( options_string == NULL )
	{
		return( -1 );
	}
	for( option_index = 0;
	     option_index < number_of_options;
	     option_index++ )
	{
		option = &( options[ option_index ] );

		if( ( option->identifier == 0 )
		 || ( option->help == NULL ) )
		{
			continue;
		}
		if( options_string_index >= options_string_size )
		{
			return( -1 );
		}
		options_string[ options_string_index++ ] = (system_character_t) option->identifier;

		if( option->name != NULL )
		{
			if( options_string_index >= options_string_size )
			{
				return( -1 );
			}
			options_string[ options_string_index++ ] = (system_character_t) ':';
		}
	}
	if( options_string_index >= options_string_size )
	{
		return( -1 );
	}
	options_string[ options_string_index++ ] = 0;

	return( 1 );
}

