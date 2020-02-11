/*
 * Directory record functions
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

#if !defined( _LIBFSAPFS_DIRECTORY_RECORD_H )
#define _LIBFSAPFS_DIRECTORY_RECORD_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_directory_record libfsapfs_directory_record_t;

struct libfsapfs_directory_record
{
	/* Identifier
	 */
	uint64_t identifier;

	/* Name size
	 */
	uint16_t name_size;

	/* Name
	 */
	uint8_t *name;

	/* Name hash
	 */
	uint32_t name_hash;

	/* Added time
	 */
	uint64_t added_time;
};

int libfsapfs_directory_record_initialize(
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfsapfs_directory_record_free(
     libfsapfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfsapfs_directory_record_clone(
     libfsapfs_directory_record_t **destination_directory_record,
     libfsapfs_directory_record_t *source_directory_record,
     libcerror_error_t **error );

int libfsapfs_directory_record_read_key_data(
     libfsapfs_directory_record_t *directory_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_directory_record_read_value_data(
     libfsapfs_directory_record_t *directory_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_directory_record_get_identifier(
     libfsapfs_directory_record_t *directory_record,
     uint64_t *identifier,
     libcerror_error_t **error );

int libfsapfs_directory_record_get_utf8_name_size(
     libfsapfs_directory_record_t *directory_record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_directory_record_get_utf8_name(
     libfsapfs_directory_record_t *directory_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_directory_record_compare_name_with_utf8_string(
     libfsapfs_directory_record_t *directory_record,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t name_hash,
     uint8_t use_case_folding,
     libcerror_error_t **error );

int libfsapfs_directory_record_get_utf16_name_size(
     libfsapfs_directory_record_t *directory_record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_directory_record_get_utf16_name(
     libfsapfs_directory_record_t *directory_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_directory_record_compare_name_with_utf16_string(
     libfsapfs_directory_record_t *directory_record,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t name_hash,
     uint8_t use_case_folding,
     libcerror_error_t **error );

int libfsapfs_directory_record_get_added_time(
     libfsapfs_directory_record_t *directory_record,
     int64_t *posix_time,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_DIRECTORY_RECORD_H ) */

