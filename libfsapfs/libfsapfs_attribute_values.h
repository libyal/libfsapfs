/*
 * (Extended) attribute values functions
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

#if !defined( _LIBFSAPFS_ATTRIBUTE_VALUES_H )
#define _LIBFSAPFS_ATTRIBUTE_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsapfs_file_extent.h"
#include "libfsapfs_libcdata.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_attribute_values libfsapfs_attribute_values_t;

struct libfsapfs_attribute_values
{
	/* The flags
	 */
	uint16_t flags;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	uint16_t name_size;

	/* The value data
	 */
	uint8_t *value_data;

	/* Value data size
	 */
	size64_t value_data_size;

	/* Value data stream identifier
	 */
	uint64_t value_data_stream_identifier;

	/* The value data file extents
	 */
	libcdata_array_t *value_data_file_extents;
};

int libfsapfs_attribute_values_initialize(
     libfsapfs_attribute_values_t **attribute_values,
     libcerror_error_t **error );

int libfsapfs_attribute_values_free(
     libfsapfs_attribute_values_t **attribute_values,
     libcerror_error_t **error );

int libfsapfs_attribute_values_read_key_data(
     libfsapfs_attribute_values_t *attribute_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_attribute_values_read_value_data(
     libfsapfs_attribute_values_t *attribute_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_attribute_values_compare_name_with_utf8_string(
     libfsapfs_attribute_values_t *attribute_values,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libfsapfs_attribute_values_get_utf8_name_size(
     libfsapfs_attribute_values_t *attribute_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_attribute_values_get_utf8_name(
     libfsapfs_attribute_values_t *attribute_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_attribute_values_compare_name_with_utf16_string(
     libfsapfs_attribute_values_t *attribute_values,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

int libfsapfs_attribute_values_get_utf16_name_size(
     libfsapfs_attribute_values_t *attribute_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_attribute_values_get_utf16_name(
     libfsapfs_attribute_values_t *attribute_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_attribute_values_get_number_of_extents(
     libfsapfs_attribute_values_t *attribute_values,
     int *number_of_extents,
     libcerror_error_t **error );

int libfsapfs_attribute_values_get_extent_by_index(
     libfsapfs_attribute_values_t *attribute_values,
     int extent_index,
     libfsapfs_file_extent_t **file_extent,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_ATTRIBUTE_VALUES_H ) */

