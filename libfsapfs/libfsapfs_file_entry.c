/*
 * File entry functions
 *
 * Copyright (C) 2010-2018, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libfsapfs_directory_record.h"
#include "libfsapfs_file_entry.h"
#include "libfsapfs_file_extent.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_inode.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcthreads.h"
#include "libfsapfs_types.h"

/* Creates a file entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_entry_initialize(
     libfsapfs_file_entry_t **file_entry,
     libbfio_handle_t *file_io_handle,
     libfsapfs_file_system_btree_t *file_system_btree,
     libfsapfs_inode_t *inode,
     libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_initialize";

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
	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	internal_file_entry = memory_allocate_structure(
	                       libfsapfs_internal_file_entry_t );

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_entry,
	     0,
	     sizeof( libfsapfs_internal_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file entry.",
		 function );

		memory_free(
		 internal_file_entry );

		return( -1 );
	}
	internal_file_entry->file_io_handle    = file_io_handle;
	internal_file_entry->file_system_btree = file_system_btree;
	internal_file_entry->inode             = inode;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBFSAPFS )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_entry->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to intialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*file_entry = (libfsapfs_file_entry_t *) internal_file_entry;

	return( 1 );

on_error:
	if( internal_file_entry != NULL )
	{
		memory_free(
		 internal_file_entry );
	}
	return( -1 );
}

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_entry_free(
     libfsapfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_free";
	int result                                           = 1;

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
		internal_file_entry = (libfsapfs_internal_file_entry_t *) *file_entry;
		*file_entry         = NULL;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBFSAPFS )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_entry->read_write_lock ),
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
		/* The file_io_handle and file_system_btree references are freed elsewhere
		 */
		if( libcdata_array_free(
		     &( internal_file_entry->directory_entries ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_directory_record_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory entries array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( internal_file_entry->file_extents ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file extents array.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file_entry );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_file_entry_get_utf8_name_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_get_utf8_name_size";
	int result                                           = 1;

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsapfs_inode_get_utf8_name_size(
	     internal_file_entry->inode,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_file_entry_get_utf8_name(
     libfsapfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_get_utf8_name";
	int result                                           = 1;

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsapfs_inode_get_utf8_name(
	     internal_file_entry->inode,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_file_entry_get_utf16_name_size(
     libfsapfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_get_utf16_name_size";
	int result                                           = 1;

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsapfs_inode_get_utf16_name_size(
	     internal_file_entry->inode,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry $FILE_NAME attribute
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsapfs_file_entry_get_utf16_name(
     libfsapfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_get_utf16_name";
	int result                                           = 1;

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsapfs_inode_get_utf16_name(
	     internal_file_entry->inode,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_entry_get_number_of_sub_file_entries(
     libfsapfs_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libcerror_error_t **error )
{
	libcdata_array_t *directory_entries                  = NULL;
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_get_number_of_sub_file_entries";
	uint64_t file_system_identifier                      = 0;
	int result                                           = 0;

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entries == NULL )
	{
		if( libfsapfs_inode_get_identifier(
		     internal_file_entry->inode,
		     &file_system_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier from inode.",
			 function );

			goto on_error;
		}
		if( libcdata_array_initialize(
		     &directory_entries,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entries array.",
			 function );

			goto on_error;
		}
		result = libfsapfs_file_system_btree_get_directory_entries(
		          internal_file_entry->file_system_btree,
		          internal_file_entry->file_io_handle,
		          file_system_identifier,
		          directory_entries,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entries from file system B-tree.",
			 function );

			goto on_error;
		}
		internal_file_entry->directory_entries = directory_entries;
		directory_entries                      = NULL;
	}
	if( libcdata_array_get_number_of_entries(
	     internal_file_entry->directory_entries,
	     number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from array.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( directory_entries != NULL )
	{
		libcdata_array_free(
		 &directory_entries,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_directory_record_free,
		 NULL );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_file_entry->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_entry_get_sub_file_entry_by_index(
     libfsapfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsapfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libcdata_array_t *directory_entries                  = NULL;
	libfsapfs_directory_record_t *directory_record       = NULL;
	libfsapfs_inode_t *inode                             = NULL;
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_get_sub_file_entry_by_index";
	uint64_t file_system_identifier                      = 0;
	int result                                           = 0;

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entries == NULL )
	{
		if( libfsapfs_inode_get_identifier(
		     internal_file_entry->inode,
		     &file_system_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier from inode.",
			 function );

			goto on_error;
		}
		if( libcdata_array_initialize(
		     &directory_entries,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entries array.",
			 function );

			goto on_error;
		}
		result = libfsapfs_file_system_btree_get_directory_entries(
		          internal_file_entry->file_system_btree,
		          internal_file_entry->file_io_handle,
		          file_system_identifier,
		          directory_entries,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entries from file system B-tree.",
			 function );

			goto on_error;
		}
		internal_file_entry->directory_entries = directory_entries;
		directory_entries                      = NULL;
	}
	if( libcdata_array_get_entry_by_index(
	     internal_file_entry->directory_entries,
	     sub_file_entry_index,
	     (intptr_t **) &directory_record,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry: %d.",
		 function,
		 sub_file_entry_index );

		return( -1 );
	}
	if( libfsapfs_directory_record_get_identifier(
	     directory_record,
	     &file_system_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file system identifier from directory entry: %d.",
		 function,
		 sub_file_entry_index );

		goto on_error;
	}
	if( libfsapfs_file_system_btree_get_inode(
	     internal_file_entry->file_system_btree,
	     internal_file_entry->file_io_handle,
	     file_system_identifier,
	     &inode,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode: %" PRIu64 " from file system B-tree.",
		 function,
		 file_system_identifier );

		 goto on_error;
	}
	if( libfsapfs_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->file_system_btree,
	     inode,
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
	inode = NULL;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( inode != NULL )
	{
		libfsapfs_inode_free(
		 &inode,
		 NULL );
	}
	if( directory_entries != NULL )
	{
		libcdata_array_free(
		 &directory_entries,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_directory_record_free,
		 NULL );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_file_entry->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* TODO implement and add thread locks */

/* Reads data at the current offset from the data stream object
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsapfs_file_entry_read_buffer(
         libfsapfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_read_buffer";
	ssize_t read_count                                   = 0;

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

/* TODO implement */

	return( -1 );
}

/* Reads data at a specific offset from the data stream object
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsapfs_file_entry_read_buffer_at_offset(
         libfsapfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_entry_read_buffer_at_offset";
	ssize_t read_count    = 0;

	if( libfsapfs_file_entry_seek_offset(
	     file_entry,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libfsapfs_file_entry_read_buffer(
	              file_entry,
	              buffer,
	              buffer_size,
	              error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain offset in the data stream object
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsapfs_file_entry_seek_offset(
         libfsapfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_seek_offset";

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

/* TODO implement */

	return( -1 );
}

/* Retrieves the current offset of the data stream object
 * Returns the offset if successful or -1 on error
 */
int libfsapfs_file_entry_get_offset(
     libfsapfs_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_get_offset";

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

/* TODO implement */

	return( -1 );
}

/* Retrieves the size of the data stream object
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_entry_get_size(
     libfsapfs_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	libcdata_array_t *file_extents                       = NULL;
	libfsapfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsapfs_file_entry_get_size";
	uint64_t file_system_identifier                      = 0;
	int result                                           = 0;

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
	internal_file_entry = (libfsapfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->file_extents == NULL )
	{
		if( libfsapfs_inode_get_data_stream_identifier(
		     internal_file_entry->inode,
		     &file_system_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream identifier from inode.",
			 function );

			goto on_error;
		}
		if( libcdata_array_initialize(
		     &file_extents,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file extents array.",
			 function );

			goto on_error;
		}
		result = libfsapfs_file_system_btree_get_file_extents(
		          internal_file_entry->file_system_btree,
		          internal_file_entry->file_io_handle,
		          file_system_identifier,
		          file_extents,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file extents from file system B-tree.",
			 function );

			goto on_error;
		}
		internal_file_entry->file_extents = file_extents;
		file_extents                      = NULL;
	}
/* TODO implement */

#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( file_extents != NULL )
	{
		libcdata_array_free(
		 &file_extents,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsapfs_file_extent_free,
		 NULL );
	}
#if defined( HAVE_LIBFSAPFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_file_entry->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

