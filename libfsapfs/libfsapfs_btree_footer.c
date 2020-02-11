/*
 * The B-tree footer functions
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

#include "libfsapfs_btree_footer.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_btree.h"

/* Creates a B-tree footer
 * Make sure the value btree_footer is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_footer_initialize(
     libfsapfs_btree_footer_t **btree_footer,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_footer_initialize";

	if( btree_footer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree footer.",
		 function );

		return( -1 );
	}
	if( *btree_footer != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree footer value already set.",
		 function );

		return( -1 );
	}
	*btree_footer = memory_allocate_structure(
	                 libfsapfs_btree_footer_t );

	if( *btree_footer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree footer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_footer,
	     0,
	     sizeof( libfsapfs_btree_footer_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree footer.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_footer != NULL )
	{
		memory_free(
		 *btree_footer );

		*btree_footer = NULL;
	}
	return( -1 );
}

/* Frees a B-tree footer
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_footer_free(
     libfsapfs_btree_footer_t **btree_footer,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_footer_free";

	if( btree_footer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree footer.",
		 function );

		return( -1 );
	}
	if( *btree_footer != NULL )
	{
		memory_free(
		 *btree_footer );

		*btree_footer = NULL;
	}
	return( 1 );
}

/* Reads the B-tree footer
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_btree_footer_read_data(
     libfsapfs_btree_footer_t *btree_footer,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_btree_footer_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
#endif

	if( btree_footer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree footer.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsapfs_btree_footer_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B-tree footer data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_btree_footer_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_btree_footer_t *) data )->node_size,
	 btree_footer->node_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_btree_footer_t *) data )->key_size,
	 btree_footer->key_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_btree_footer_t *) data )->value_size,
	 btree_footer->value_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_btree_footer_t *) data )->maximum_key_size,
	 btree_footer->maximum_key_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_btree_footer_t *) data )->maximum_value_size,
	 btree_footer->maximum_value_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_btree_footer_t *) data )->flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
		libfsapfs_debug_print_btree_flags(
		 value_32bit );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: node size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 btree_footer->node_size );

		libcnotify_printf(
		 "%s: key size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 btree_footer->key_size );

		libcnotify_printf(
		 "%s: value size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 btree_footer->value_size );

		libcnotify_printf(
		 "%s: maximum key size\t\t\t: %" PRIu32 "\n",
		 function,
		 btree_footer->maximum_key_size );

		libcnotify_printf(
		 "%s: maximum value size\t\t\t: %" PRIu32 "\n",
		 function,
		 btree_footer->maximum_value_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_btree_footer_t *) data )->total_number_of_keys,
		 value_64bit );
		libcnotify_printf(
		 "%s: total number of keys\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_btree_footer_t *) data )->total_number_of_nodes,
		 value_64bit );
		libcnotify_printf(
		 "%s: total number of nodes\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

