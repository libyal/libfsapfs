include_HEADERS = \
	libfsapfs.h

pkginclude_HEADERS = \
	libfsapfs/codepage.h \
	libfsapfs/definitions.h \
	libfsapfs/error.h \
	libfsapfs/extern.h \
	libfsapfs/features.h \
	libfsapfs/types.h

EXTRA_DIST = \
	libfsapfs.h.in \
	libfsapfs/definitions.h.in \
	libfsapfs/features.h.in \
	libfsapfs/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libfsapfs.h
	-rm -f libfsapfs/definitions.h
	-rm -f libfsapfs/features.h
	-rm -f libfsapfs/types.h
	-rm -f Makefile

