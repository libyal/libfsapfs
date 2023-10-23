AM_CPPFLAGS = -I$(top_srcdir)/include

EXTRA_DIST = \
	byte_stream.h \
	common.h \
	config.h \
	config_borlandc.h \
	config_msc.h \
	config_winapi.h \
	file_stream.h \
	memory.h \
	narrow_string.h \
	system_string.h \
	types.h \
	types.h.in \
	wide_string.h

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f config.h
	-rm -f types.h
	-rm -f Makefile

