/*
 * Mount tool fuse functions
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

#if !defined( _MOUNT_FUSE_H )
#define _MOUNT_FUSE_H

#include <common.h>
#include <types.h>

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
#define FUSE_USE_VERSION	26

#if defined( HAVE_LIBFUSE )
#include <fuse.h>

#elif defined( HAVE_LIBOSXFUSE )
#include <osxfuse/fuse.h>
#endif

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

#include "fsapfstools_libcerror.h"
#include "fsapfstools_libfsapfs.h"
#include "mount_file_entry.h"
#include "mount_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )

int mount_fuse_set_stat_info(
     struct stat *stat_info,
     size64_t size,
     uint16_t file_mode,
     int64_t access_time,
     int64_t inode_change_time,
     int64_t modification_time,
     libcerror_error_t **error );

int mount_fuse_filldir(
     void *buffer,
     fuse_fill_dir_t filler,
     const char *name,
     struct stat *stat_info,
     mount_file_entry_t *file_entry,
     libcerror_error_t **error );

int mount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info );

int mount_fuse_release(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_opendir(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_readdir(
     const char *path,
     void *buffer,
     fuse_fill_dir_t filler,
     off_t offset,
     struct fuse_file_info *file_info );

int mount_fuse_releasedir(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_getattr(
     const char *path,
     struct stat *stat_info );

int mount_fuse_readlink(
     const char *path,
     char *buffer,
     size_t size );

void mount_fuse_destroy(
      void *private_data );

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_FUSE_H ) */

