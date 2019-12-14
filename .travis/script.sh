#!/bin/sh
# Script to run script step on Travis-CI
#
# Version: 20191209

# Exit on error.
set -e;

if test ${TRAVIS_OS_NAME} = "linux";
then
	export PATH=$(echo $PATH | tr ":" "\n" | sed '/\/opt\/python/d' | tr "\n" ":" | sed "s/::/:/g");
fi

if test ${TARGET} = "docker";
then
	docker run -t -v "${PWD}:/libfsapfs" ${DOCKERHUB_REPO}:${DOCKERHUB_TAG} sh -c "cd libfsapfs && .travis/script_docker.sh";

elif test ${TARGET} = "linux-gcc-python-setup-py" || test ${TARGET} = "macos-gcc-python-setup-py";
then
	./configure ${CONFIGURE_OPTIONS};
	make > /dev/null;
	python ./setup.py build;

elif test ${TARGET} = "macos-gcc-python-setup-py37";
then
	./configure ${CONFIGURE_OPTIONS};
	make > /dev/null;
	python3 ./setup.py bdist_wheel;

elif test ${TARGET} != "coverity";
then
	.travis/runtests.sh;

	if test ${TARGET} = "macos-gcc-pkgbuild";
	then
		export VERSION=`sed '5!d; s/^ \[//;s/\],$//' configure.ac`;

		make install DESTDIR=${PWD}/osx-pkg;
		mkdir -p ${PWD}/osx-pkg/usr/share/doc/libfsapfs;
		cp AUTHORS COPYING COPYING.LESSER NEWS README ${PWD}/osx-pkg/usr/share/doc/libfsapfs;

		pkgbuild --root osx-pkg --identifier com.github.libyal.libfsapfs --version ${VERSION} --ownership recommended ../libfsapfs-${VERSION}.pkg;
	fi
fi

