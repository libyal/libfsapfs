/*
 * The file system functions
 *
 * Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libfsapfs_directory_record.h"
#include "libfsapfs_encryption_context.h"
#include "libfsapfs_file_entry.h"
#include "libfsapfs_file_system.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_types.h"

/* Creates file system
 * Make sure the value file_system is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_initialize(
     libfsapfs_file_system_t **file_system,
     libfsapfs_io_handle_t *io_handle,
     libfsapfs_encryption_context_t *encryption_context,
     libfsapfs_file_system_btree_t *file_system_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_system_initialize";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system value already set.",
		 function );

		return( -1 );
	}
	*file_system = memory_allocate_structure(
	                libfsapfs_file_system_t );

	if( *file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system,
	     0,
	     sizeof( libfsapfs_file_system_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system.",
		 function );

		memory_free(
		 *file_system );

		*file_system = NULL;

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBFSAPFS )
	if( libcthreads_read_write_lock_initialize(
	     &( ( *file_system )->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	( *file_system )->io_handle          = io_handle;
	( *file_system )->encryption_context = encryption_context;
	( *file_system )->file_system_btree  = file_system_btree;

	return( 1 );

on_error:
	if( *file_system != NULL )
	{
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( -1 );
}

/* Frees file system
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_free(
     libfsapfs_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_system_free";
	int result            = 1;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		if( ( *file_system )->file_system_btree != NULL )
		{
			if( libfsapfs_file_system_btree_free(
			     &( ( *file_system )->file_system_btree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file system B-tree.",
				 function );

				result = -1;
			}
		}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBFSAPFS )
		if( libcthreads_read_write_lock_free(
		     &( ( *file_system )->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
}

/* Retrieves a file entry for a specific identifier from the file system B-tree
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_get_file_entry_by_identifier(
     libfsapfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     uint64_t transaction_identifier,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsapfs_inode_t *inode = NULL;
	static char *function    = "libfsapfs_file_system_get_file_entry_by_identifier";
	int result               = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
	result = libfsapfs_file_system_btree_get_inode_by_identifier(
	          file_system->file_system_btree,
	          file_io_handle,
	          identifier,
	          transaction_identifier,
	          &inode,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode: %" PRIu64 " from file system B-tree.",
		 function,
		 identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsapfs_file_entry_initialize(
		     file_entry,
		     file_system->io_handle,
		     file_io_handle,
		     file_system->encryption_context,
		     file_system->file_system_btree,
		     inode,
		     NULL,
		     transaction_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( inode != NULL )
	{
		libfsapfs_inode_free(
		 &inode,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a file entry for an UTF-8 encoded path from the file system
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_get_file_entry_by_utf8_path(
     libfsapfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint64_t transaction_identifier,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsapfs_directory_record_t *directory_record = NULL;
	libfsapfs_inode_t *inode                       = NULL;
	static char *function                          = "libfsapfs_file_system_get_file_entry_by_utf8_path";
	int result                                     = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
	result = libfsapfs_file_system_btree_get_inode_by_utf8_path(
	          file_system->file_system_btree,
	          file_io_handle,
	          2,
	          utf8_string,
	          utf8_string_length,
	          transaction_identifier,
	          &inode,
	          &directory_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode from file system B-tree.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsapfs_file_entry_initialize(
		     file_entry,
		     file_system->io_handle,
		     file_io_handle,
		     file_system->encryption_context,
		     file_system->file_system_btree,
		     inode,
		     directory_record,
		     transaction_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			goto on_error;
		}
		inode            = NULL;
		directory_record = NULL;
	}
	return( result );

on_error:
	if( directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &directory_record,
		 NULL );
	}
	if( inode != NULL )
	{
		libfsapfs_inode_free(
		 &inode,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a file entry for an UTF-16 encoded path from the file system
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsapfs_file_system_get_file_entry_by_utf16_path(
     libfsapfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint64_t transaction_identifier,
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsapfs_directory_record_t *directory_record = NULL;
	libfsapfs_inode_t *inode                       = NULL;
	static char *function                          = "libfsapfs_file_system_get_file_entry_by_utf16_path";
	int result                                     = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
	result = libfsapfs_file_system_btree_get_inode_by_utf16_path(
	          file_system->file_system_btree,
	          file_io_handle,
	          2,
	          utf16_string,
	          utf16_string_length,
	          transaction_identifier,
	          &inode,
	          &directory_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode from file system B-tree.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsapfs_file_entry_initialize(
		     file_entry,
		     file_system->io_handle,
		     file_io_handle,
		     file_system->encryption_context,
		     file_system->file_system_btree,
		     inode,
		     directory_record,
		     transaction_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			goto on_error;
		}
		inode            = NULL;
		directory_record = NULL;
	}
	return( result );

on_error:
	if( directory_record != NULL )
	{
		libfsapfs_directory_record_free(
		 &directory_record,
		 NULL );
	}
	if( inode != NULL )
	{
		libfsapfs_inode_free(
		 &inode,
		 NULL );
	}
	return( -1 );
}

