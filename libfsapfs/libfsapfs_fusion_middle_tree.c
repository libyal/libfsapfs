/*
 * The Fusion middle tree functions
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

#include "libfsapfs_fusion_middle_tree.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_fusion_middle_tree.h"

/* Creates a Fusion middle tree
 * Make sure the value fusion_middle_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_fusion_middle_tree_initialize(
     libfsapfs_fusion_middle_tree_t **fusion_middle_tree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_fusion_middle_tree_initialize";

	if( fusion_middle_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Fusion middle tree.",
		 function );

		return( -1 );
	}
	if( *fusion_middle_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid Fusion middle tree value already set.",
		 function );

		return( -1 );
	}
	*fusion_middle_tree = memory_allocate_structure(
	                       libfsapfs_fusion_middle_tree_t );

	if( *fusion_middle_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create Fusion middle tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *fusion_middle_tree,
	     0,
	     sizeof( libfsapfs_fusion_middle_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear Fusion middle tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *fusion_middle_tree != NULL )
	{
		memory_free(
		 *fusion_middle_tree );

		*fusion_middle_tree = NULL;
	}
	return( -1 );
}

/* Frees a Fusion middle tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_fusion_middle_tree_free(
     libfsapfs_fusion_middle_tree_t **fusion_middle_tree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_fusion_middle_tree_free";

	if( fusion_middle_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Fusion middle tree.",
		 function );

		return( -1 );
	}
	if( *fusion_middle_tree != NULL )
	{
		memory_free(
		 *fusion_middle_tree );

		*fusion_middle_tree = NULL;
	}
	return( 1 );
}

/* Reads the Fusion middle tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_fusion_middle_tree_read_file_io_handle(
     libfsapfs_fusion_middle_tree_t *fusion_middle_tree,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t fusion_middle_tree_data[ 4096 ];

	static char *function = "libfsapfs_fusion_middle_tree_read_file_io_handle";
	ssize_t read_count    = 0;

	if( fusion_middle_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Fusion middle tree.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading Fusion middle tree at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek Fusion middle tree offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              fusion_middle_tree_data,
	              4096,
	              error );

	if( read_count != (ssize_t) 4096 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read Fusion middle tree data.",
		 function );

		return( -1 );
	}
	if( libfsapfs_fusion_middle_tree_read_data(
	     fusion_middle_tree,
	     fusion_middle_tree_data,
	     4096,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read Fusion middle tree data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the Fusion middle tree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_fusion_middle_tree_read_data(
     libfsapfs_fusion_middle_tree_t *fusion_middle_tree,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsapfs_fusion_middle_tree_read_data";
	uint32_t object_subtype = 0;
	uint32_t object_type    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit    = 0;
	uint32_t value_32bit    = 0;
#endif

	if( fusion_middle_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Fusion middle tree.",
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
	if( ( data_size < sizeof( fsapfs_fusion_middle_tree_t ) )
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
		 "%s: Fusion middle tree data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_fusion_middle_tree_t *) data )->object_type,
	 object_type );

	if( object_type != 0x40000002UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 object_type );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_fusion_middle_tree_t *) data )->object_subtype,
	 object_subtype );

	if( object_subtype != 0x00000015UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 object_subtype );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_fusion_middle_tree_t *) data )->object_checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: object checksum\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_fusion_middle_tree_t *) data )->object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_fusion_middle_tree_t *) data )->object_transaction_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object transaction identifier\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object type\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_subtype );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_fusion_middle_tree_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

