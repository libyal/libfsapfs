/*
 * The profiler functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libfsapfs_libcerror.h"
#include "libfsapfs_profiler.h"

#if defined( HAVE_PROFILER )

/* Creates a profiler
 * Make sure the value profiler is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_profiler_initialize(
     libfsapfs_profiler_t **profiler,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_profiler_initialize";

	if( profiler == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid profiler.",
		 function );

		return( -1 );
	}
	if( *profiler != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid profiler value already set.",
		 function );

		return( -1 );
	}
	*profiler = memory_allocate_structure(
	             libfsapfs_profiler_t );

	if( *profiler == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create profiler.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *profiler,
	     0,
	     sizeof( libfsapfs_profiler_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear profiler.",
		 function );

		memory_free(
		 *profiler );

		*profiler = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *profiler != NULL )
	{
		memory_free(
		 *profiler );

		*profiler = NULL;
	}
	return( -1 );
}

/* Frees a profiler
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_profiler_free(
     libfsapfs_profiler_t **profiler,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_profiler_free";

	if( profiler == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid profiler.",
		 function );

		return( -1 );
	}
	if( *profiler != NULL )
	{
		memory_free(
		 *profiler );

		*profiler = NULL;
	}
	return( 1 );
}

/* Opens the profiler
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_profiler_open(
     libfsapfs_profiler_t *profiler,
     const char *filename,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_profiler_open";

	if( profiler == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid profiler.",
		 function );

		return( -1 );
	}
	if( profiler->output_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid profiler - output stream value already set.",
		 function );

		return( -1 );
	}
	profiler->output_stream = file_stream_open(
	                           filename,
	                           "w" );

	if( profiler->output_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open profiler.",
		 function );

		goto on_error;
	}
	if( fprintf(
	     profiler->output_stream,
	     "timestamp,name,offset,size,duration\n" ) <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( profiler->output_stream != NULL )
	{
		file_stream_close(
		 profiler->output_stream );

		profiler->output_stream = NULL;
	}
	return( -1 );
}

/* Closes the profiler
 * Returns 0 if successful or -1 on error
 */
int libfsapfs_profiler_close(
     libfsapfs_profiler_t *profiler,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_profiler_close";

	if( profiler == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid profiler.",
		 function );

		return( -1 );
	}
	if( profiler->output_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid profiler - missing output stream.",
		 function );

		return( -1 );
	}
	if( file_stream_close(
	     profiler->output_stream ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close profiler.",
		 function );

		return( -1 );
	}
	profiler->output_stream = NULL;

	return( 0 );
}

/* Starts timing
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_profiler_start_timing(
     libfsapfs_profiler_t *profiler,
     int64_t *start_timestamp,
     libcerror_error_t **error )
{
	struct timespec start_time;

	static char *function = "libfsapfs_profiler_start_timing";

	if( profiler == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid profiler.",
		 function );

		return( -1 );
	}
	if( start_timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid start timestamp.",
		 function );

		return( -1 );
	}
	if( clock_gettime(
	     CLOCK_REALTIME,
	     &start_time ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve start time.",
		 function );

		return( -1 );
	}
	*start_timestamp = ( (int64_t) start_time.tv_sec * 1000000000 ) + start_time.tv_nsec;

	return( 1 );
}

/* Stops timing
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_profiler_stop_timing(
     libfsapfs_profiler_t *profiler,
     int64_t start_timestamp,
     const char *name,
     off64_t offset,
     size64_t size,
     libcerror_error_t **error )
{
	struct timespec stop_time;

	static char *function = "libfsapfs_profiler_start_timing";
	int64_t sample_time   = 0;

	if( profiler == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid profiler.",
		 function );

		return( -1 );
	}
	if( profiler->output_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid profiler - missing output stream.",
		 function );

		return( -1 );
	}
	if( clock_gettime(
	     CLOCK_REALTIME,
	     &stop_time ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve stop time.",
		 function );

		return( -1 );
	}
	sample_time  = ( (int64_t) stop_time.tv_sec * 1000000000 ) + stop_time.tv_nsec;
	sample_time -= start_timestamp;

	if( fprintf(
	     profiler->output_stream,
	     "%" PRIi64 ",%s,%" PRIi64 ",%" PRIu64 ",%" PRIi64 "\n",
	     start_timestamp,
	     name,
	     offset,
	     size,
	     sample_time ) <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write sample.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* defined( HAVE_PROFILER ) */

