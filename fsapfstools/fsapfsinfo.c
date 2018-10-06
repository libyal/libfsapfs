/*
 * Shows information obtained from an Apple File System (APFS)
 *
 * Copyright (C) 2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfstools_getopt.h"
#include "fsapfstools_libcerror.h"
#include "fsapfstools_libclocale.h"
#include "fsapfstools_libcnotify.h"
#include "fsapfstools_libfsapfs.h"
#include "fsapfstools_output.h"
#include "fsapfstools_signal.h"
#include "fsapfstools_unused.h"
#include "info_handle.h"

enum FSAPFSINFO_MODES
{
	FSAPFSINFO_MODE_BODYFILE,
	FSAPFSINFO_MODE_CONTAINER,
	FSAPFSINFO_MODE_FILE_ENTRY,
	FSAPFSINFO_MODE_FILE_SYSTEM_HIERARCHY,
	FSAPFSINFO_MODE_INODE
};

info_handle_t *fsapfsinfo_info_handle = NULL;
int fsapfsinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use fsapfsinfo to determine information about an Apple\n"
	                 " File System (APFS).\n\n" );

	fprintf( stream, "Usage: fsapfsinfo [ -B bodyfile ] [ -E inode_number ]\n"
	                 "                  [ -F file_entry ] [ -o offset ]\n"
	                 "                  [ -p password ] [ -BhHvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-B:     output file system information as a bodyfile.\n" );
	fprintf( stream, "\t-E:     show information about a specific inode or \"all\".\n" );
	fprintf( stream, "\t-F:     show information about a specific file entry path.\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-H:     shows the file system hierarchy\n" );
	fprintf( stream, "\t-o:     specify the volume offset\n" );
	fprintf( stream, "\t-p:     specify the password\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for fsapfsinfo
 */
void fsapfsinfo_signal_handler(
      fsapfstools_signal_t signal FSAPFSTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "fsapfsinfo_signal_handler";

	FSAPFSTOOLS_UNREFERENCED_PARAMETER( signal )

	fsapfsinfo_abort = 1;

	if( fsapfsinfo_info_handle != NULL )
	{
		if( info_handle_signal_abort(
		     fsapfsinfo_info_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal info handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libfsapfs_error_t *error                 = NULL;
	system_character_t *option_bodyfile      = NULL;
	system_character_t *option_file_entry    = NULL;
	system_character_t *option_inode_number  = NULL;
	system_character_t *option_password      = NULL;
	system_character_t *option_volume_offset = NULL;
	system_character_t *source               = NULL;
	char *program                            = "fsapfsinfo";
	system_integer_t option                  = 0;
	size_t string_length                     = 0;
	uint64_t inode_number                    = 0;
	int option_mode                          = FSAPFSINFO_MODE_CONTAINER;
	int verbose                              = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "fsapfstools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( fsapfstools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	fsapfsoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = fsapfstools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "B:E:F:hHo:p:vV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'B':
				option_mode     = FSAPFSINFO_MODE_BODYFILE;
				option_bodyfile = optarg;

				break;

			case (system_integer_t) 'E':
				option_mode         = FSAPFSINFO_MODE_INODE;
				option_inode_number = optarg;

				break;

			case (system_integer_t) 'F':
				option_mode       = FSAPFSINFO_MODE_FILE_ENTRY;
				option_file_entry = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'H':
				option_mode = FSAPFSINFO_MODE_FILE_SYSTEM_HIERARCHY;

				break;

			case (system_integer_t) 'o':
				option_volume_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_password = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				fsapfsoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libfsapfs_notify_set_stream(
	 stderr,
	 NULL );
	libfsapfs_notify_set_verbose(
	 verbose );

	if( info_handle_initialize(
	     &fsapfsinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize info handle.\n" );

		goto on_error;
	}
	if( option_password != NULL )
	{
		if( info_handle_set_password(
		     fsapfsinfo_info_handle,
		     option_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set password.\n" );

			goto on_error;
		}
	}
	if( option_volume_offset != NULL )
	{
		if( info_handle_set_volume_offset(
		     fsapfsinfo_info_handle,
		     option_volume_offset,
		     &error ) != 1 )
		{
			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			fprintf(
			 stderr,
			 "Unsupported volume offset defaulting to: %" PRIi64 ".\n",
			 fsapfsinfo_info_handle->volume_offset );
		}
	}
	if( info_handle_open_input(
	     fsapfsinfo_info_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	switch( option_mode )
	{
		case FSAPFSINFO_MODE_BODYFILE:
			if( info_handle_create_bodyfile(
			     fsapfsinfo_info_handle,
			     option_bodyfile,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to output to bodyfile.\n" );

				goto on_error;
			}
			break;

		case FSAPFSINFO_MODE_FILE_ENTRY:
			if( info_handle_file_entry_fprint(
			     fsapfsinfo_info_handle,
			     option_file_entry,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print file entry information.\n" );

				goto on_error;
			}
			break;

		case FSAPFSINFO_MODE_FILE_SYSTEM_HIERARCHY:
			if( info_handle_file_system_hierarchy_fprint(
			     fsapfsinfo_info_handle,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print file system hierarchy.\n" );

				goto on_error;
			}
			break;

		case FSAPFSINFO_MODE_INODE:
			if( option_inode_number == NULL )
			{
				fprintf(
				 stderr,
				 "Mising inode number string.\n" );

				goto on_error;
			}
			string_length = system_string_length(
					 option_inode_number );

			if( ( string_length == 3 )
			 && ( system_string_compare(
			       option_inode_number,
			       _SYSTEM_STRING( "all" ),
			       3 ) == 0 ) )
			{
				if( info_handle_inodes_fprint(
				     fsapfsinfo_info_handle,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unable to print inodes.\n" );

					goto on_error;
				}
			}
			else if( fsapfstools_system_string_copy_from_64_bit_in_decimal(
			          option_inode_number,
			          string_length + 1,
			          &inode_number,
			          &error ) == 1 )
			{
				if( inode_number > (uint64_t) 0x0fffffffffffffffUL )
				{
					fprintf(
					 stderr,
					 "Invalid inode number value out of bounds." );

					goto on_error;
				}
				if( info_handle_inode_fprint(
				     fsapfsinfo_info_handle,
				     (uint32_t) inode_number,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unable to print inode: %" PRIu64 ".\n",
					 inode_number );

					goto on_error;
				}
			}
			else
			{
				fprintf(
				 stderr,
				 "Unable to copy inode number string to 64-bit decimal.\n" );

				goto on_error;
			}
			break;

		case FSAPFSINFO_MODE_CONTAINER:
		default:
			if( info_handle_container_fprint(
			     fsapfsinfo_info_handle,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print container information.\n" );

				goto on_error;
			}
			break;
	}
	if( info_handle_close_input(
	     fsapfsinfo_info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( info_handle_free(
	     &fsapfsinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( fsapfsinfo_info_handle != NULL )
	{
		info_handle_free(
		 &fsapfsinfo_info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

