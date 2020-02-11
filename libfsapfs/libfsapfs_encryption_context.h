/*
 * Encryption functions
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

#if !defined( _LIBFSAPFS_ENCRYPTION_H )
#define _LIBFSAPFS_ENCRYPTION_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcaes.h"
#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_encryption_context libfsapfs_encryption_context_t;

struct libfsapfs_encryption_context
{
	/* The encryption method
	 */
	uint32_t method;

	/* The AES-XTS decryption context
	 */
	libcaes_tweaked_context_t *decryption_context;
};

int libfsapfs_encryption_context_initialize(
     libfsapfs_encryption_context_t **context,
     uint32_t method,
     libcerror_error_t **error );

int libfsapfs_encryption_context_free(
     libfsapfs_encryption_context_t **context,
     libcerror_error_t **error );

int libfsapfs_encryption_context_set_keys(
     libfsapfs_encryption_context_t *context,
     const uint8_t *key,
     size_t key_size,
     const uint8_t *tweak_key,
     size_t tweak_key_size,
     libcerror_error_t **error );

int libfsapfs_encryption_context_crypt(
     libfsapfs_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t sector_number,
     uint16_t bytes_per_sector,
     libcerror_error_t **error );

int libfsapfs_encryption_aes_key_unwrap(
     const uint8_t *key,
     size_t key_size_bits,
     const uint8_t *wrapped_data,
     size_t wrapped_data_size,
     uint8_t *unwrapped_data,
     size_t unwrapped_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_ENCRYPTION_H ) */

