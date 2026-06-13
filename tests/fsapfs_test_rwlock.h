/*
 * Read/Write lock functions for testing
 *
 * Copyright (C) 2018-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _FSAPFS_TEST_RWLOCK_H )
#define _FSAPFS_TEST_RWLOCK_H

#include <common.h>

#include "fsapfs_test_extern.h"
#include "fsapfs_test_libfsapfs.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( LIBFSAPFS_HAVE_MULTI_THREAD_SUPPORT )
#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
#define HAVE_FSAPFS_TEST_RWLOCK_HOOK	1
#endif
#endif

#if defined( HAVE_FSAPFS_TEST_RWLOCK_HOOK )

FSAPFS_TEST_INTERNAL \
int fsapfs_test_pthread_rwlock_init_attempts_before_fail;

FSAPFS_TEST_INTERNAL \
int fsapfs_test_pthread_rwlock_destroy_attempts_before_fail;

FSAPFS_TEST_INTERNAL \
int fsapfs_test_pthread_rwlock_rdlock_attempts_before_fail;

FSAPFS_TEST_INTERNAL \
int fsapfs_test_pthread_rwlock_wrlock_attempts_before_fail;

#if defined( HAVE_PTHREAD_RWLOCK_UNLOCK_HOOK )

FSAPFS_TEST_INTERNAL \
int fsapfs_test_pthread_rwlock_unlock_attempts_before_fail;

#endif /* defined( HAVE_PTHREAD_RWLOCK_UNLOCK_HOOK ) */

#endif /* defined( HAVE_FSAPFS_TEST_RWLOCK_HOOK ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSAPFS_TEST_RWLOCK_H ) */

