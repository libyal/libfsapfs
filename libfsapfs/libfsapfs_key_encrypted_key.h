/*
 * The key encrypted key (KEK) functions
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

#if !defined( _LIBFSAPFS_KEY_ENCRYPTED_KEY_H )
#define _LIBFSAPFS_KEY_ENCRYPTED_KEY_H

#include <common.h>
#include <types.h>

#include "libfsapfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_key_encrypted_key libfsapfs_key_encrypted_key_t;

struct libfsapfs_key_encrypted_key
{
	/* The identifier
	 */
	uint8_t identifier[ 16 ];

	/* The HMAC
	 */
	uint8_t hmac[ 32 ];

	/* The number of iterations
	 */
	uint64_t number_of_iterations;

	/* The salt
	 */
	uint8_t salt[ 16 ];

	/* The encryption method
	 */
	uint32_t encryption_method;

	/* The wrapped key encrypted key (KEK)
	 */
	uint8_t wrapped_kek[ 40 ];
};

int libfsapfs_key_encrypted_key_initialize(
     libfsapfs_key_encrypted_key_t **key_encrypted_key,
     libcerror_error_t **error );

int libfsapfs_key_encrypted_key_free(
     libfsapfs_key_encrypted_key_t **key_encrypted_key,
     libcerror_error_t **error );

int libfsapfs_key_encrypted_key_read_data(
     libfsapfs_key_encrypted_key_t *key_encrypted_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsapfs_key_encrypted_key_unlock_with_key(
     libfsapfs_key_encrypted_key_t *key_encrypted_key,
     const uint8_t *key,
     size_t key_size,
     uint8_t *unlocked_key,
     size_t unlocked_key_size,
     libcerror_error_t **error );

int libfsapfs_key_encrypted_key_unlock_with_password(
     libfsapfs_key_encrypted_key_t *key_encrypted_key,
     const uint8_t *password,
     size_t password_length,
     uint8_t *unlocked_key,
     size_t unlocked_key_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_KEY_ENCRYPTED_KEY_H ) */

