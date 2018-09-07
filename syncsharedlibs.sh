#!/bin/sh
# Script that synchronizes the shared library dependencies
#
# Version: 20180728

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

GIT_URL_PREFIX="https://github.com/libyal";
SHARED_LIBS="libcerror libcthreads libcdata libclocale libcnotify libcsplit libuna libcfile libcpath libbfio libfdatetime";

if test "${TRAVIS_OS_NAME}" != "linux";
then
	echo "ERROR: This script is intended to be run on Travis CI.";

	exit 1;
fi

USE_HEAD="";

if test "$1" = "--use-head";
then
	USE_HEAD="--use-head";
fi

OLDIFS=$IFS;
IFS=" ";

for SHARED_LIB in ${SHARED_LIBS};
do
	GIT_URL="${GIT_URL_PREFIX}/${SHARED_LIB}.git";

	git clone --quiet ${GIT_URL} ${SHARED_LIB}-$$;

	if ! test -d ${SHARED_LIB}-$$;
	then
		echo "Unable to git clone: ${GIT_URL}";

		IFS=$OLDIFS;

		exit ${EXIT_FAILURE};
	fi
	(cd ${SHARED_LIB}-$$ && git fetch --quiet --all --tags --prune)

	LATEST_TAG=`cd ${SHARED_LIB}-$$ && git describe --tags --abbrev=0`;

	if test -n ${LATEST_TAG} && test -z ${USE_HEAD};
	then
		echo "Synchronizing: ${SHARED_LIB} from ${GIT_URL} tag ${LATEST_TAG}";

		(cd ${SHARED_LIB}-$$ && git checkout --quiet tags/${LATEST_TAG});
	else
		echo "Synchronizing: ${SHARED_LIB} from ${GIT_URL} HEAD";
	fi

	(cd ${SHARED_LIB}-$$ && ./synclibs.sh ${USE_HEAD} && ./autogen.sh);

	CONFIGURE_OPTIONS="";

	(cd ${SHARED_LIB}-$$ && ./configure --help | grep -- '--enable-wide-character-type' > /dev/null);

	if test $? -eq 0;
	then
		CONFIGURE_OPTIONS="${CONFIGURE_OPTIONS} --enable-wide-character-type";
	fi

	(cd ${SHARED_LIB}-$$ && ./configure --prefix=/usr ${CONFIGURE_OPTIONS} && make && sudo make install);

	rm -rf ${SHARED_LIB}-$$;
done

IFS=$OLDIFS;

exit ${EXIT_SUCCESS};

