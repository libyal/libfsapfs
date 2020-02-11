/*
 * Mounts an Apple File System (APFS) container
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
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include "fsapfstools_getopt.h"
#include "fsapfstools_i18n.h"
#include "fsapfstools_libcerror.h"
#include "fsapfstools_libclocale.h"
#include "fsapfstools_libcnotify.h"
#include "fsapfstools_libfsapfs.h"
#include "fsapfstools_output.h"
#include "fsapfstools_signal.h"
#include "fsapfstools_unused.h"
#include "mount_dokan.h"
#include "mount_fuse.h"
#include "mount_handle.h"

mount_handle_t *fsapfsmount_mount_handle = NULL;
int fsapfsmount_abort                    = 0;

/* Prints usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use fsapfsmount to mount an Apple File System (APFS) container\n\n" );

	fprintf( stream, "Usage: fsapfsmount [ -f file_system_index ] [ -o offset ] [ -p password ]\n"
	                 "                   [ -r recovery_password ] [ -X extended_options ]\n"
	                 "                   [ -hvV ] container mount_point\n\n" );

	fprintf( stream, "\tcontainer:   an Apple File System (APFS) container\n\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-f:          mounts a specific file system or \"all\"\n" );
	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-o:          specify the container offset in bytes\n" );
	fprintf( stream, "\t-p:          specify the password/passphrase\n" );
	fprintf( stream, "\t-r:          specify the recovery password/passphrase\n" );
	fprintf( stream, "\t-v:          verbose output to stderr, while fsapfsmount will remain running in the\n"
	                 "\t             foreground\n" );
	fprintf( stream, "\t-V:          print version\n" );
	fprintf( stream, "\t-X:          extended options to pass to sub system\n" );
}

/* Signal handler for fsapfsmount
 */
void fsapfsmount_signal_handler(
      fsapfstools_signal_t signal FSAPFSTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "fsapfsmount_signal_handler";

	FSAPFSTOOLS_UNREFERENCED_PARAMETER( signal )

	fsapfsmount_abort = 1;

	if( fsapfsmount_mount_handle != NULL )
	{
		if( mount_handle_signal_abort(
		     fsapfsmount_mount_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal mount handle to abort.\n",
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
	libfsapfs_error_t *error                     = NULL;
	system_character_t *mount_point              = NULL;
	system_character_t *option_extended_options  = NULL;
	system_character_t *option_file_system_index = NULL;
	system_character_t *option_offset            = NULL;
	system_character_t *option_password          = NULL;
	system_character_t *option_recovery_password = NULL;
	system_character_t *source                   = NULL;
	char *program                                = "fsapfsmount";
	system_integer_t option                      = 0;
	int result                                   = 0;
	int verbose                                  = 0;

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	struct fuse_operations fsapfsmount_fuse_operations;

	struct fuse_args fsapfsmount_fuse_arguments  = FUSE_ARGS_INIT(0, NULL);
	struct fuse_chan *fsapfsmount_fuse_channel   = NULL;
	struct fuse *fsapfsmount_fuse_handle         = NULL;

#elif defined( HAVE_LIBDOKAN )
	DOKAN_OPERATIONS fsapfsmount_dokan_operations;
	DOKAN_OPTIONS fsapfsmount_dokan_options;
#endif

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
	fsapfstools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = fsapfstools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "f:ho:p:r:vVX:" ) ) ) != (system_integer_t) -1 )
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

			case (system_integer_t) 'f':
				option_file_system_index = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'o':
				option_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_password = optarg;

				break;

			case (system_integer_t) 'r':
				option_recovery_password = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				fsapfstools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'X':
				option_extended_options = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source container.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind++ ];

	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libfsapfs_notify_set_stream(
	 stderr,
	 NULL );
	libfsapfs_notify_set_verbose(
	 verbose );

	if( mount_handle_initialize(
	     &fsapfsmount_mount_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
	if( option_file_system_index != NULL )
	{
		if( mount_handle_set_file_system_index(
		     fsapfsmount_mount_handle,
		     option_file_system_index,
		     &error ) != 1 )
		{
			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			fprintf(
			 stderr,
			 "Unsupported file system index defaulting to: all.\n" );
		}
	}
	if( option_offset != NULL )
	{
		if( mount_handle_set_offset(
		     fsapfsmount_mount_handle,
		     option_offset,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set container offset.\n" );

			goto on_error;
		}
	}
	if( option_password != NULL )
	{
		if( mount_handle_set_password(
		     fsapfsmount_mount_handle,
		     option_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set password.\n" );

			goto on_error;
		}
	}
	if( option_recovery_password != NULL )
	{
		if( mount_handle_set_recovery_password(
		     fsapfsmount_mount_handle,
		     option_recovery_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set recovery password.\n" );

			goto on_error;
		}
	}
	if( mount_handle_open(
	     fsapfsmount_mount_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open source container\n" );

		goto on_error;
	}
	if( mount_handle_is_locked(
	     fsapfsmount_mount_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to unlock source container\n" );

		goto on_error;
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( option_extended_options != NULL )
	{
		/* This argument is required but ignored
		 */
		if( fuse_opt_add_arg(
		     &fsapfsmount_fuse_arguments,
		     "" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &fsapfsmount_fuse_arguments,
		     "-o" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &fsapfsmount_fuse_arguments,
		     option_extended_options ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
	}
	if( memory_set(
	     &fsapfsmount_fuse_operations,
	     0,
	     sizeof( struct fuse_operations ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear fuse operations.\n" );

		goto on_error;
	}
	fsapfsmount_fuse_operations.open       = &mount_fuse_open;
	fsapfsmount_fuse_operations.read       = &mount_fuse_read;
	fsapfsmount_fuse_operations.release    = &mount_fuse_release;
	fsapfsmount_fuse_operations.opendir    = &mount_fuse_opendir;
	fsapfsmount_fuse_operations.readdir    = &mount_fuse_readdir;
	fsapfsmount_fuse_operations.releasedir = &mount_fuse_releasedir;
	fsapfsmount_fuse_operations.getattr    = &mount_fuse_getattr;
	fsapfsmount_fuse_operations.readlink   = &mount_fuse_readlink;
	fsapfsmount_fuse_operations.destroy    = &mount_fuse_destroy;

	fsapfsmount_fuse_channel = fuse_mount(
	                            mount_point,
	                            &fsapfsmount_fuse_arguments );

	if( fsapfsmount_fuse_channel == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse channel.\n" );

		goto on_error;
	}
	fsapfsmount_fuse_handle = fuse_new(
	                           fsapfsmount_fuse_channel,
	                           &fsapfsmount_fuse_arguments,
	                           &fsapfsmount_fuse_operations,
	                           sizeof( struct fuse_operations ),
	                           fsapfsmount_mount_handle );

	if( fsapfsmount_fuse_handle == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse handle.\n" );

		goto on_error;
	}
	if( verbose == 0 )
	{
		if( fuse_daemonize(
		     0 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to daemonize fuse.\n" );

			goto on_error;
		}
	}
	result = fuse_loop(
	          fsapfsmount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse loop.\n" );

		goto on_error;
	}
	fuse_destroy(
	 fsapfsmount_fuse_handle );

	fuse_opt_free_args(
	 &fsapfsmount_fuse_arguments );

	return( EXIT_SUCCESS );

#elif defined( HAVE_LIBDOKAN )
	if( memory_set(
	     &fsapfsmount_dokan_operations,
	     0,
	     sizeof( DOKAN_OPERATIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan operations.\n" );

		goto on_error;
	}
	if( memory_set(
	     &fsapfsmount_dokan_options,
	     0,
	     sizeof( DOKAN_OPTIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan options.\n" );

		goto on_error;
	}
	fsapfsmount_dokan_options.Version     = DOKAN_VERSION;
	fsapfsmount_dokan_options.ThreadCount = 0;
	fsapfsmount_dokan_options.MountPoint  = mount_point;

	if( verbose != 0 )
	{
		fsapfsmount_dokan_options.Options |= DOKAN_OPTION_STDERR;
#if defined( HAVE_DEBUG_OUTPUT )
		fsapfsmount_dokan_options.Options |= DOKAN_OPTION_DEBUG;
#endif
	}
/* This will only affect the drive properties
	fsapfsmount_dokan_options.Options |= DOKAN_OPTION_REMOVABLE;
*/

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	fsapfsmount_dokan_options.Options |= DOKAN_OPTION_KEEP_ALIVE;

	fsapfsmount_dokan_operations.CreateFile           = &mount_dokan_CreateFile;
	fsapfsmount_dokan_operations.OpenDirectory        = &mount_dokan_OpenDirectory;
	fsapfsmount_dokan_operations.CreateDirectory      = NULL;
	fsapfsmount_dokan_operations.Cleanup              = NULL;
	fsapfsmount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	fsapfsmount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	fsapfsmount_dokan_operations.WriteFile            = NULL;
	fsapfsmount_dokan_operations.FlushFileBuffers     = NULL;
	fsapfsmount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	fsapfsmount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	fsapfsmount_dokan_operations.FindFilesWithPattern = NULL;
	fsapfsmount_dokan_operations.SetFileAttributes    = NULL;
	fsapfsmount_dokan_operations.SetFileTime          = NULL;
	fsapfsmount_dokan_operations.DeleteFile           = NULL;
	fsapfsmount_dokan_operations.DeleteDirectory      = NULL;
	fsapfsmount_dokan_operations.MoveFile             = NULL;
	fsapfsmount_dokan_operations.SetEndOfFile         = NULL;
	fsapfsmount_dokan_operations.SetAllocationSize    = NULL;
	fsapfsmount_dokan_operations.LockFile             = NULL;
	fsapfsmount_dokan_operations.UnlockFile           = NULL;
	fsapfsmount_dokan_operations.GetFileSecurity      = NULL;
	fsapfsmount_dokan_operations.SetFileSecurity      = NULL;
	fsapfsmount_dokan_operations.GetDiskFreeSpace     = NULL;
	fsapfsmount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	fsapfsmount_dokan_operations.Unmount              = &mount_dokan_Unmount;

#else
	fsapfsmount_dokan_operations.ZwCreateFile         = &mount_dokan_ZwCreateFile;
	fsapfsmount_dokan_operations.Cleanup              = NULL;
	fsapfsmount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	fsapfsmount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	fsapfsmount_dokan_operations.WriteFile            = NULL;
	fsapfsmount_dokan_operations.FlushFileBuffers     = NULL;
	fsapfsmount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	fsapfsmount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	fsapfsmount_dokan_operations.FindFilesWithPattern = NULL;
	fsapfsmount_dokan_operations.SetFileAttributes    = NULL;
	fsapfsmount_dokan_operations.SetFileTime          = NULL;
	fsapfsmount_dokan_operations.DeleteFile           = NULL;
	fsapfsmount_dokan_operations.DeleteDirectory      = NULL;
	fsapfsmount_dokan_operations.MoveFile             = NULL;
	fsapfsmount_dokan_operations.SetEndOfFile         = NULL;
	fsapfsmount_dokan_operations.SetAllocationSize    = NULL;
	fsapfsmount_dokan_operations.LockFile             = NULL;
	fsapfsmount_dokan_operations.UnlockFile           = NULL;
	fsapfsmount_dokan_operations.GetFileSecurity      = NULL;
	fsapfsmount_dokan_operations.SetFileSecurity      = NULL;
	fsapfsmount_dokan_operations.GetDiskFreeSpace     = NULL;
	fsapfsmount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	fsapfsmount_dokan_operations.Unmounted            = NULL;
	fsapfsmount_dokan_operations.FindStreams          = NULL;
	fsapfsmount_dokan_operations.Mounted              = NULL;

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

	result = DokanMain(
	          &fsapfsmount_dokan_options,
	          &fsapfsmount_dokan_operations );

	switch( result )
	{
		case DOKAN_SUCCESS:
			break;

		case DOKAN_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: generic error\n" );
			break;

		case DOKAN_DRIVE_LETTER_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: bad drive letter\n" );
			break;

		case DOKAN_DRIVER_INSTALL_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to load driver\n" );
			break;

		case DOKAN_START_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: driver error\n" );
			break;

		case DOKAN_MOUNT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to assign drive letter\n" );
			break;

		case DOKAN_MOUNT_POINT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: mount point error\n" );
			break;

		default:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unknown error: %d\n",
			 result );
			break;
	}
	return( EXIT_SUCCESS );

#else
	fprintf(
	 stderr,
	 "No sub system to mount APFS format.\n" );

	return( EXIT_FAILURE );

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( fsapfsmount_fuse_handle != NULL )
	{
		fuse_destroy(
		 fsapfsmount_fuse_handle );
	}
	fuse_opt_free_args(
	 &fsapfsmount_fuse_arguments );
#endif
	if( fsapfsmount_mount_handle != NULL )
	{
		mount_handle_free(
		 &fsapfsmount_mount_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

