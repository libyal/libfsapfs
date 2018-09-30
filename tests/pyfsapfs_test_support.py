#!/usr/bin/env python
#
# Python-bindings support functions test script
#
# Copyright (C) 2018, Joachim Metz <joachim.metz@gmail.com>
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

import unittest

import pyfsapfs


class SupportFunctionsTests(unittest.TestCase):
  """Tests the support functions."""

  def test_get_version(self):
    """Tests the get_version function."""
    version = pyfsapfs.get_version()

    # TODO: check version.
    # self.assertEqual(version, "00000000")


if __name__ == "__main__":
  unittest.main(verbosity=2)
