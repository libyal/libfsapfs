/*
 * The B-tree entry functions
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

#include "libfsapfs_btree_entry.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_btree.h"
#include "fsapfs_object.h"

/* Creates a B-tree entry
 * Make sure the value btree_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_entry_initialize(
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_entry_initialize";

	if( btree_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree entry.",
		 function );

		return( -1 );
	}
	if( *btree_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree entry value already set.",
		 function );

		return( -1 );
	}
	*btree_entry = memory_allocate_structure(
	                 libfsapfs_btree_entry_t );

	if( *btree_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_entry,
	     0,
	     sizeof( libfsapfs_btree_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_entry != NULL )
	{
		memory_free(
		 *btree_entry );

		*btree_entry = NULL;
	}
	return( -1 );
}

/* Frees a B-tree entry
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_entry_free(
     libfsapfs_btree_entry_t **btree_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_entry_free";

	if( btree_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree entry.",
		 function );

		return( -1 );
	}
	if( *btree_entry != NULL )
	{
		if( ( *btree_entry )->value_data != NULL )
		{
			memory_free(
			 ( *btree_entry )->value_data );
		}
		if( ( *btree_entry )->key_data != NULL )
		{
			memory_free(
			 ( *btree_entry )->key_data );
		}
		memory_free(
		 *btree_entry );

		*btree_entry = NULL;
	}
	return( 1 );
}

/* Sets the key data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_entry_set_key_data(
     libfsapfs_btree_entry_t *btree_entry,
     const uint8_t *key_data,
     size_t key_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_entry_set_key_data";

	if( btree_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree entry.",
		 function );

		return( -1 );
	}
	if( btree_entry->key_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree entry - key data value already set.",
		 function );

		return( -1 );
	}
	if( key_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key data.",
		 function );

		return( -1 );
	}
	if( key_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( key_data_size > 0 )
	{
		btree_entry->key_data = (uint8_t *) memory_allocate(
		                                     sizeof( uint8_t ) * key_data_size );

		if( btree_entry->key_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to create key data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     btree_entry->key_data,
		     key_data,
		     key_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy key data.",
			 function );

			goto on_error;
		}
		btree_entry->key_data_size = key_data_size;
	}
	return( 1 );

on_error:
	if( btree_entry->key_data != NULL )
	{
		memory_free(
		 btree_entry->key_data );

		btree_entry->key_data = NULL;
	}
	btree_entry->key_data_size = 0;

	return( -1 );
}

/* Sets the value data
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_entry_set_value_data(
     libfsapfs_btree_entry_t *btree_entry,
     const uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_entry_set_value_data";

	if( btree_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree entry.",
		 function );

		return( -1 );
	}
	if( btree_entry->value_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree entry - value data value already set.",
		 function );

		return( -1 );
	}
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_data_size > 0 )
	{
		btree_entry->value_data = (uint8_t *) memory_allocate(
		                                       sizeof( uint8_t ) * value_data_size );

		if( btree_entry->value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     btree_entry->value_data,
		     value_data,
		     value_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value data.",
			 function );

			goto on_error;
		}
		btree_entry->value_data_size = value_data_size;
	}
	return( 1 );

on_error:
	if( btree_entry->value_data != NULL )
	{
		memory_free(
		 btree_entry->value_data );

		btree_entry->value_data = NULL;
	}
	btree_entry->value_data_size = 0;

	return( -1 );
}

