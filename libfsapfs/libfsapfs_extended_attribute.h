/*
 * Extended attribute functions
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

#if !defined( _LIBFSAPFS_EXTENDED_ATTRIBUTE_H )
#define _LIBFSAPFS_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsapfs_extern.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_internal_extended_attribute libfsapfs_internal_extended_attribute_t;

struct libfsapfs_internal_extended_attribute
{
	/* The identifier
	 */
	uint64_t identifier;

	/* The name size
	 */
	uint16_t name_size;

	/* The name
	 */
	uint8_t *name;

	/* The data size
	 */
	uint16_t data_size;

	/* The data
	 */
	uint8_t *data;
};

int libfsapfs_extended_attribute_initialize(
     libfsapfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_free(
     libfsapfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

int libfsapfs_internal_extended_attribute_free(
     libfsapfs_internal_extended_attribute_t **internal_extended_attribute,
     libcerror_error_t **error );

int libfsapfs_extended_attribute_read_key_data(
     libfsapfs_extended_attribute_t *extended_attribute,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_extended_attribute_read_value_data(
     libfsapfs_extended_attribute_t *extended_attribute,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_identifier(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint64_t *identifier,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf8_name_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf8_name(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsapfs_extended_attribute_compare_name_with_utf8_string(
     libfsapfs_extended_attribute_t *extended_attribute,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf16_name_size(
     libfsapfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_extended_attribute_get_utf16_name(
     libfsapfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsapfs_extended_attribute_compare_name_with_utf16_string(
     libfsapfs_extended_attribute_t *extended_attribute,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_EXTENDED_ATTRIBUTE_H ) */

