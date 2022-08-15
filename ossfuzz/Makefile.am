if HAVE_LIB_FUZZING_ENGINE
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

bin_PROGRAMS = \
	container_fuzzer \
	extended_attribute_fuzzer \
	file_entry_fuzzer \
	volume_fuzzer

container_fuzzer_SOURCES = \
	container_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libfsapfs.h

container_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsapfs/libfsapfs.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

extended_attribute_fuzzer_SOURCES = \
	extended_attribute_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libfsapfs.h

extended_attribute_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsapfs/libfsapfs.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

file_entry_fuzzer_SOURCES = \
	file_entry_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libfsapfs.h

file_entry_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsapfs/libfsapfs.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

volume_fuzzer_SOURCES = \
	ossfuzz_libbfio.h \
	ossfuzz_libfsapfs.h \
	volume_fuzzer.cc

volume_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsapfs/libfsapfs.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on container_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(container_fuzzer_SOURCES)
	@echo "Running splint on extended_attribute_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(extended_attribute_fuzzer_SOURCES)
	@echo "Running splint on file_entry_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(file_entry_fuzzer_SOURCES)
	@echo "Running splint on volume_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(volume_fuzzer_SOURCES)

