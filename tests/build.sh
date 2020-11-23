#!/bin/sh
# Script to build from source
#
# Version: 20201121

set -e

./synclibs.sh --use-head
./autogen.sh
./configure "$@"
make > /dev/null

