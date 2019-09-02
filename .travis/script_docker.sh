#!/bin/sh
# Script to run script step on Travis-CI with Docker
#
# Version: 20190809

# Exit on error.
set -e;

export DEBIAN_FRONTEND="noninteractive";

apt-get update -q;

# Install add-apt-repository and locale-gen.
apt-get install -y locales software-properties-common;

# Set locale to US English and UTF-8.
locale-gen en_US.UTF-8;

# Install packages essential for building.
apt-get install -y autoconf automake build-essential git libtool pkg-config autopoint;

export LANG="en_US.UTF-8";

./synclibs.sh --use-head;

./autogen.sh;

.travis/runtests.sh;

