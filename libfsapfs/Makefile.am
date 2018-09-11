AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSAPFS_DLL_EXPORT@

lib_LTLIBRARIES = libfsapfs.la

libfsapfs_la_SOURCES = \
	fsapfs_btree.h \
	fsapfs_container_physical_map.h \
	fsapfs_container_reaper.h \
	fsapfs_container_space_manager.h \
	fsapfs_container_superblock.h \
	fsapfs_file_system.h \
	fsapfs_object.h \
	fsapfs_object_map.h \
	fsapfs_volume_superblock.h \
	libfsapfs.c \
	libfsapfs_btree_entry.c libfsapfs_btree_entry.h \
	libfsapfs_btree_footer.c libfsapfs_btree_footer.h \
	libfsapfs_btree_header.c libfsapfs_btree_header.h \
	libfsapfs_btree_root.c libfsapfs_btree_root.h \
	libfsapfs_container.c libfsapfs_container.h \
	libfsapfs_container_physical_map.c libfsapfs_container_physical_map.h \
	libfsapfs_container_physical_map_entry.c libfsapfs_container_physical_map_entry.h \
	libfsapfs_container_reaper.c libfsapfs_container_reaper.h \
	libfsapfs_container_space_manager.c libfsapfs_container_space_manager.h \
	libfsapfs_container_superblock.c libfsapfs_container_superblock.h \
	libfsapfs_debug.c libfsapfs_debug.h \
	libfsapfs_definitions.h \
	libfsapfs_error.c libfsapfs_error.h \
	libfsapfs_extern.h \
	libfsapfs_file_entry.c libfsapfs_file_entry.h \
	libfsapfs_file_system_btree.c libfsapfs_file_system_btree.h \
	libfsapfs_io_handle.c libfsapfs_io_handle.h \
	libfsapfs_libbfio.h \
	libfsapfs_libcdata.h \
	libfsapfs_libcerror.h \
	libfsapfs_libclocale.h \
	libfsapfs_libcnotify.h \
	libfsapfs_libcthreads.h \
	libfsapfs_libfdatetime.h \
	libfsapfs_libfguid.h \
	libfsapfs_libuna.h \
	libfsapfs_notify.c libfsapfs_notify.h \
	libfsapfs_object.c libfsapfs_object.h \
	libfsapfs_object_map.c libfsapfs_object_map.h \
	libfsapfs_object_map_btree.c libfsapfs_object_map_btree.h \
	libfsapfs_object_map_descriptor.c libfsapfs_object_map_descriptor.h \
	libfsapfs_support.c libfsapfs_support.h \
	libfsapfs_types.h \
	libfsapfs_unused.h \
	libfsapfs_volume.c libfsapfs_volume.h \
	libfsapfs_volume_superblock.c libfsapfs_volume_superblock.h

libfsapfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@PTHREAD_LIBADD@

libfsapfs_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libfsapfs_definitions.h.in \
	libfsapfs.rc \
	libfsapfs.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfsapfs_definitions.h
	/bin/rm -f libfsapfs.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libfsapfs ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libfsapfs_la_SOURCES)

