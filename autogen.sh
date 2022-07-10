#!/bin/sh
# Script to generate ./configure using the autotools
#
# Version: 20220709

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

BINDIR="/usr/bin";

if ! test -x "${BINDIR}/aclocal";
then
	BINDIR="/usr/local/bin";
fi
if ! test -x "${BINDIR}/aclocal";
then
	BINDIR="/usr/local/bin";
fi
if ! test -x "${BINDIR}/aclocal";
then
	# Default location of MacPorts installed binaries.
	BINDIR="/opt/local/bin";
fi
if ! test -x "${BINDIR}/aclocal";
then
	# Default location of 32-bit MSYS2-MinGW installed binaries.
	BINDIR="/mingw32/bin";
fi
if ! test -x "${BINDIR}/aclocal";
then
	# Default location of 64-bit MSYS2-MinGW installed binaries.
	BINDIR="/mingw64/bin";
fi

if ! test -x "${BINDIR}/aclocal";
then
	echo "Unable to find autotools";

	exit ${EXIT_FAILURE};
fi

ACLOCAL="${BINDIR}/aclocal";
AUTOCONF="${BINDIR}/autoconf";
AUTOHEADER="${BINDIR}/autoheader";
AUTOMAKE="${BINDIR}/automake";
AUTOPOINT="${BINDIR}/autopoint";
AUTORECONF="${BINDIR}/autoreconf";
LIBTOOLIZE="${BINDIR}/libtoolize";
PKGCONFIG="${BINDIR}/pkg-config";

if test "${OSTYPE}" = "msys";
then
	# Work-around for autopoint failing to detect gettext version
	# using func_trace (which is not available) on MSYS by writing
	# the gettext version to intl/VERSION.
	if ! test -d intl;
	then
		mkdir intl;
	fi
	GETTEXT_VERSION=`gettext --version | head -n1 | sed 's/^.* //'`;

	echo "gettext-${GETTEXT_VERSION}" > intl/VERSION;

elif ! test -x "${PKGCONFIG}";
then
	if test "${BINDIR}" != "/usr/bin";
	then
		# On OpenBSD most of the autotools are located in
		# /usr/local/bin while pkg-config is located in /usr/bin
		PKGCONFIG="/usr/bin/pkg-config";
	fi
	if ! test -x "${PKGCONFIG}";
	then
		echo "Unable to find: pkg-config";

		exit ${EXIT_FAILURE};
	fi
fi

if test -x "${AUTORECONF}";
then
	${AUTORECONF} --force --install
	if test $? -ne 0;
	then
		exit $?;
	fi
else
	if ! test -x "${ACLOCAL}";
	then
		echo "Unable to find: aclocal";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOCONF}";
		then
		echo "Unable to find: autoconf";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOHEADER}";
	then
		echo "Unable to find: autoheader";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOMAKE}";
	then
		echo "Unable to find: automake";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOPOINT}";
	then
		echo "Unable to find: autopoint";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${LIBTOOLIZE}";
	then
		echo "Unable to find: libtoolize";

		exit ${EXIT_FAILURE};
	fi

	${AUTOPOINT} --force;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${ACLOCAL} --force -I m4;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${LIBTOOLIZE} --force;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${AUTOHEADER} --force;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${AUTOCONF} --force;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${AUTOMAKE} --force --add-missing;
	if test $? -ne 0;
	then
		exit $?;
	fi

fi

exit ${EXIT_SUCCESS};

