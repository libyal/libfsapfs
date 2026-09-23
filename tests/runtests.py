#!/usr/bin/env python3
#
# Script to run Python test scripts.
#
# Version: 20260923
#
# Copyright (C) 2018-2026, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.

import glob
import os
import sys
import unittest

test_profile = ".pyfsapfs"
input_glob = "*"
option_sets = ["offset", "password"]


def read_ignore_list(test_profile):
    """Reads the test profile ignore file if it exists.

    Args:
      test_profile (str): test profile.

    Returns:
      set[str]: ignore list.
    """
    ignore_file_path = os.path.join("tests", "input", test_profile, "ignore")
    if not os.path.isfile(ignore_file_path):
        return set()

    with open(ignore_file_path, encoding="utf-8") as file_object:
        return set([line.strip() for line in file_object.readlines()])


if __name__ == "__main__":
    print(f"Using Python version {sys.version!s}")

    test_loader = unittest.TestLoader()
    test_runner = unittest.TextTestRunner(verbosity=2)

    test_scripts = test_loader.discover("tests", pattern="*.py")

    ignore_list = read_ignore_list(test_profile)

    test_set = None
    source_file = None

    for test_set in glob.glob(os.path.join("tests", "input", "*")):
        test_set = test_set.rsplit(os.path.sep, maxsplit=1)[-1]
        if not test_set or test_set[0] == "." or test_set in ignore_list:
            continue

        source_files = glob.glob(os.path.join("tests", "input", test_set, input_glob))
        if source_files:
            source_file = source_files[0]
            break

    setattr(unittest, "source", source_file)

    if source_file:
        for option_set in option_sets:
            test_filename = ".".join([os.path.basename(source_file), option_set])
            test_options_file_path = os.path.join(
                "tests",
                "input",
                test_profile,
                test_set,
                test_filename,
            )
            if not os.path.isfile(test_options_file_path):
                continue

            with open(test_options_file_path, encoding="utf-8") as file_object:
                lines = [line.strip() for line in file_object.readlines()]
                if lines[0] == "# libyal test data options":
                    for line in lines[1:]:
                        key, value = line.split("=", maxsplit=1)
                        if key == "offset":
                            value = int(value)

                        setattr(unittest, key, value)

    test_results = test_runner.run(test_scripts)
    if not test_results.wasSuccessful():
        sys.exit(1)
