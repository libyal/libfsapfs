# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir2)"
pyexecdir = $(pyexecdir2)

if HAVE_PYTHON2
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFSAPFS_DLL_IMPORT@

am_pyfsapfs_la_rpath = -rpath $(pyexecdir2)

BUILT_SOURCES = \
	pyfsapfs.c pyfsapfs.h \
	pyfsapfs_container.c pyfsapfs_container.h \
	pyfsapfs_datetime.c pyfsapfs_datetime.h \
	pyfsapfs_error.c pyfsapfs_error.h \
	pyfsapfs_extended_attribute.c pyfsapfs_extended_attribute.h \
	pyfsapfs_extended_attributes.c pyfsapfs_extended_attributes.h \
	pyfsapfs_file_entries.c pyfsapfs_file_entries.h \
	pyfsapfs_file_entry.c pyfsapfs_file_entry.h \
	pyfsapfs_file_object_io_handle.c pyfsapfs_file_object_io_handle.h \
	pyfsapfs_guid.c pyfsapfs_guid.h \
	pyfsapfs_integer.c pyfsapfs_integer.h \
	pyfsapfs_libbfio.h \
	pyfsapfs_libcerror.h \
	pyfsapfs_libclocale.h \
	pyfsapfs_libfguid.h \
	pyfsapfs_libfsapfs.h \
	pyfsapfs_python.h \
	pyfsapfs_unused.h \
	pyfsapfs_volume.c pyfsapfs_volume.h \
	pyfsapfs_volumes.c pyfsapfs_volumes.h

pyexec_LTLIBRARIES = pyfsapfs.la

nodist_pyfsapfs_la_SOURCES = $(BUILT_SOURCES)

pyfsapfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libfsapfs/libfsapfs.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFGUID_LIBADD@

pyfsapfs_la_CPPFLAGS = $(PYTHON2_CPPFLAGS)
pyfsapfs_la_LDFLAGS  = -module -avoid-version $(PYTHON2_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pyfsapfs/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pyfsapfs*.[ch]

distclean: clean
	/bin/rm -f Makefile

