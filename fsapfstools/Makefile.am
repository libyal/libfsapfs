AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
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
	@LIBFUSE_CPPFLAGS@ \
	@LIBFSAPFS_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fsapfsinfo \
	fsapfsmount

fsapfsinfo_SOURCES = \
	fsapfsinfo.c \
	fsapfstools_getopt.c fsapfstools_getopt.h \
	fsapfstools_i18n.h \
	fsapfstools_libbfio.h \
	fsapfstools_libcerror.h \
	fsapfstools_libclocale.h \
	fsapfstools_libcnotify.h \
	fsapfstools_libcpath.h \
	fsapfstools_libfdatetime.h \
	fsapfstools_libfguid.h \
	fsapfstools_libfsapfs.h \
	fsapfstools_libuna.h \
	fsapfstools_output.c fsapfstools_output.h \
	fsapfstools_signal.c fsapfstools_signal.h \
	fsapfstools_unused.h \
	info_handle.c info_handle.h

fsapfsinfo_LDADD = \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsapfs/libfsapfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

fsapfsmount_SOURCES = \
	fsapfsmount.c \
	fsapfstools_getopt.c fsapfstools_getopt.h \
	fsapfstools_i18n.h \
	fsapfstools_libbfio.h \
	fsapfstools_libcdata.h \
	fsapfstools_libcerror.h \
	fsapfstools_libclocale.h \
	fsapfstools_libcnotify.h \
	fsapfstools_libcpath.h \
	fsapfstools_libfsapfs.h \
	fsapfstools_libuna.h \
	fsapfstools_output.c fsapfstools_output.h \
	fsapfstools_signal.c fsapfstools_signal.h \
	fsapfstools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

fsapfsmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsapfs/libfsapfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on fsapfsinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fsapfsinfo_SOURCES)

