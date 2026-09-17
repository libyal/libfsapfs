#!/bin/sh
# Script to generate configure and Makefile using the autotools.
#
# Version: 20260914

EXIT_SUCCESS=0
EXIT_FAILURE=1

BINDIR=$(which aclocal)

if [ -x "${BINDIR}" ]
then
    BINDIR=$(dirname "${BINDIR}")

elif [ -x "/usr/bin/aclocal" ]
then
    BINDIR="/usr/bin"

elif [ -x "/usr/local/bin/aclocal" ]
then
    BINDIR="/usr/local/bin"

# Try default location of MacPorts installed binaries.
elif [ -x "/opt/local/bin/aclocal" ]
then
    BINDIR="/opt/local/bin"

# Try default location of Homebrew installed binaries.
elif [ -x "/opt/homebrew/bin/aclocal" ]
then
    BINDIR="/opt/homebrew/bin"

# Try default location of 32-bit MSYS2-MinGW installed binaries.
elif [ -x "/mingw32/bin/aclocal" ]
then
    BINDIR="/mingw32/bin"

# Try default location of 64-bit MSYS2-MinGW installed binaries.
elif [ -x "/mingw64/bin/aclocal" ]
then
    BINDIR="/mingw64/bin"

else
    echo "Unable to find autotools"

    exit ${EXIT_FAILURE}
fi

ACLOCAL="${BINDIR}/aclocal"
AUTOCONF="${BINDIR}/autoconf"
AUTOHEADER="${BINDIR}/autoheader"
AUTOMAKE="${BINDIR}/automake"
AUTOPOINT="${BINDIR}/autopoint"
AUTORECONF="${BINDIR}/autoreconf"
LIBTOOLIZE="${BINDIR}/libtoolize"
PKGCONFIG="${BINDIR}/pkg-config"

# shellcheck disable=SC3028
if [ "${OSTYPE}" = "msys" ]
then
    # Work-around for autopoint failing to detect gettext version using
    # func_trace (which is not available) on MSYS by writing the gettext
    # version to intl/VERSION.
    if [ ! -d intl ]
    then
        mkdir intl
    fi
    GETTEXT_VERSION=$(gettext --version | head -n1 | sed 's/^.* //')

    echo "gettext-${GETTEXT_VERSION}" > intl/VERSION

elif [ ! -x "${PKGCONFIG}" ]
then
    if [ "${BINDIR}" != "/usr/bin" ]
    then
        # On OpenBSD most of the autotools are located in /usr/local/bin
        # while pkg-config is located in /usr/bin
        PKGCONFIG="/usr/bin/pkg-config"
    fi
    if [ ! -x "${PKGCONFIG}" ]
    then
        echo "Unable to find: pkg-config"

        exit ${EXIT_FAILURE}
    fi
fi

if [ -x "${AUTORECONF}" ]
then
    ${AUTORECONF} --force --install
    RESULT=$?
    [ "$RESULT" -ne 0 ] && exit "$RESULT"
else
    if [ ! -x "${ACLOCAL}" ]
    then
        echo "Unable to find: aclocal"

        exit ${EXIT_FAILURE}
    fi
    if [ ! -x "${AUTOCONF}" ]
        then
        echo "Unable to find: autoconf"

        exit ${EXIT_FAILURE}
    fi
    if [ ! -x "${AUTOHEADER}" ]
    then
        echo "Unable to find: autoheader"

        exit ${EXIT_FAILURE}
    fi
    if [ ! -x "${AUTOMAKE}" ]
    then
        echo "Unable to find: automake"

        exit ${EXIT_FAILURE}
    fi
    if [ ! -x "${AUTOPOINT}" ]
    then
        echo "Unable to find: autopoint"

        exit ${EXIT_FAILURE}
    fi
    if [ ! -x "${LIBTOOLIZE}" ]
    then
        echo "Unable to find: libtoolize"

        exit ${EXIT_FAILURE}
    fi
    ${AUTOPOINT} --force
    RESULT=$?
    [ "$RESULT" -ne 0 ] && exit "$RESULT"

    ${ACLOCAL} --force --install -I m4
    RESULT=$?
    [ "$RESULT" -ne 0 ] && exit "$RESULT"

    ${LIBTOOLIZE} --force
    RESULT=$?
    [ "$RESULT" -ne 0 ] && exit "$RESULT"

    ${AUTOHEADER} --force
    RESULT=$?
    [ "$RESULT" -ne 0 ] && exit "$RESULT"

    ${AUTOCONF} --force
    RESULT=$?
    [ "$RESULT" -ne 0 ] && exit "$RESULT"

    ${AUTOMAKE} --force --add-missing
    RESULT=$?
    [ "$RESULT" -ne 0 ] && exit "$RESULT"
fi

exit ${EXIT_SUCCESS}

