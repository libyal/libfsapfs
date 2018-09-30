ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	include \
	common \
	libcerror \
	libcthreads \
	libcdata \
	libclocale \
	libcnotify \
	libcsplit \
	libuna \
	libcfile \
	libcpath \
	libbfio \
	libfcache \
	libfdata \
	libfdatetime \
	libfguid \
	libhmac \
	libcaes \
	libfsapfs \
	pyfsapfs \
	pyfsapfs-python2 \
	pyfsapfs-python3 \
	fsapfstools \
	po \
	manuals \
	tests \
	msvscpp

DPKG_FILES = \
	dpkg/changelog \
	dpkg/changelog.in \
	dpkg/compat \
	dpkg/control \
	dpkg/copyright \
	dpkg/rules \
	dpkg/libfsapfs-dev.install \
	dpkg/libfsapfs.install \
	dpkg/libfsapfs-python.install \
	dpkg/libfsapfs-python3.install \
	dpkg/libfsapfs-tools.install \
	dpkg/source/format

GETTEXT_FILES = \
	config.rpath \
	po/Makevars.in

PKGCONFIG_FILES = \
	libfsapfs.pc.in

SETUP_PY_FILES = \
	setup.py

SPEC_FILES = \
	libfsapfs.spec \
	libfsapfs.spec.in

EXTRA_DIST = \
	$(DPKG_FILES) \
	$(GETTEXT_FILES) \
	$(PKGCONFIG_FILES) \
	$(SETUP_PY_FILES) \
	$(SPEC_FILES)

MAINTAINERCLEANFILES = \
	Makefile.in

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = \
	libfsapfs.pc

libtool: @LIBTOOL_DEPS@
	cd $(srcdir) && $(SHELL) ./config.status --recheck

lib: library

library:
	(cd $(srcdir)/common && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcerror && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcthreads && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcdata && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libclocale && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcnotify && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcsplit && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcfile && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcpath && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfcache && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdata && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdatetime && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfguid && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libhmac && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcaes && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfsapfs && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/po && $(MAKE) $(AM_MAKEFLAGS))

distclean: clean
	/bin/rm -f Makefile
	/bin/rm -f config.status
	/bin/rm -f config.cache
	/bin/rm -f config.log
	/bin/rm -f libfsapfs.pc
	/bin/rm -f libfsapfs.spec
	@for dir in ${subdirs}; do \
		(cd $$dir && $(MAKE) distclean) \
		|| case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

splint:
	(cd $(srcdir)/libcerror && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcthreads && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcdata && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libclocale && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcnotify && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcsplit && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcfile && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcpath && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfcache && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdata && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdatetime && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfguid && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libhmac && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcaes && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfsapfs && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/fsapfstools && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/po && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/tests && $(MAKE) splint $(AM_MAKEFLAGS))

