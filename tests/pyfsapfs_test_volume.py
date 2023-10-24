#!/usr/bin/env python
#
# Python-bindings volume type test script
#
# Copyright (C) 2018-2023, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import os
import sys
import unittest

import pyfsapfs


class DataRangeFileObject(object):
  """File-like object that maps an in-file data range."""

  def __init__(self, path, range_offset, range_size):
    """Initializes a file-like object.

    Args:
      path (str): path of the file that contains the data range.
      range_offset (int): offset where the data range starts.
      range_size (int): size of the data range starts, or None to indicate
          the range should continue to the end of the parent file-like object.
    """
    super(DataRangeFileObject, self).__init__()
    self._current_offset = 0
    self._file_object = open(path, "rb")
    self._range_offset = range_offset
    self._range_size = range_size

  def __enter__(self):
    """Enters a with statement."""
    return self

  def __exit__(self, unused_type, unused_value, unused_traceback):
    """Exits a with statement."""
    return

  def close(self):
    """Closes the file-like object."""
    if self._file_object:
      self._file_object.close()
      self._file_object = None

  def get_offset(self):
    """Retrieves the current offset into the file-like object.

    Returns:
      int: current offset in the data range.
    """
    return self._current_offset

  def get_size(self):
    """Retrieves the size of the file-like object.

    Returns:
      int: size of the data range.
    """
    return self._range_size

  def read(self, size=None):
    """Reads a byte string from the file-like object at the current offset.

    The function will read a byte string of the specified size or
    all of the remaining data if no size was specified.

    Args:
      size (Optional[int]): number of bytes to read, where None is all
          remaining data.

    Returns:
      bytes: data read.

    Raises:
      IOError: if the read failed.
    """
    if (self._range_offset < 0 or
        (self._range_size is not None and self._range_size < 0)):
      raise IOError("Invalid data range.")

    if self._current_offset < 0:
      raise IOError(
          "Invalid current offset: {0:d} value less than zero.".format(
              self._current_offset))

    if (self._range_size is not None and
        self._current_offset >= self._range_size):
      return b""

    if size is None:
      size = self._range_size
    if self._range_size is not None and self._current_offset + size > self._range_size:
      size = self._range_size - self._current_offset

    self._file_object.seek(
        self._range_offset + self._current_offset, os.SEEK_SET)

    data = self._file_object.read(size)

    self._current_offset += len(data)

    return data

  def seek(self, offset, whence=os.SEEK_SET):
    """Seeks to an offset within the file-like object.

    Args:
      offset (int): offset to seek to.
      whence (Optional(int)): value that indicates whether offset is an absolute
          or relative position within the file.

    Raises:
      IOError: if the seek failed.
    """
    if self._current_offset < 0:
      raise IOError(
          "Invalid current offset: {0:d} value less than zero.".format(
              self._current_offset))

    if whence == os.SEEK_CUR:
      offset += self._current_offset
    elif whence == os.SEEK_END:
      offset += self._range_size
    elif whence != os.SEEK_SET:
      raise IOError("Unsupported whence.")
    if offset < 0:
      raise IOError("Invalid offset value less than zero.")

    self._current_offset = offset


class VolumeTypeTests(unittest.TestCase):
  """Tests the volume type."""

  def test_is_locked(self):
    """Tests the is_locked function."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      try:
        if not fsapfs_container.number_of_volumes:
          raise unittest.SkipTest("missing volumes")

        fsapfs_volume = fsapfs_container.get_volume(0)

        result = fsapfs_volume.is_locked()
        self.assertFalse(result)

      finally:
        fsapfs_container.close()

  def test_get_size(self):
    """Tests the get_size function and size property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      try:
        if not fsapfs_container.number_of_volumes:
          raise unittest.SkipTest("missing volumes")

        fsapfs_volume = fsapfs_container.get_volume(0)

        # Note this is not yet implemented.
        with self.assertRaises(IOError):
          size = fsapfs_volume.get_size()
          self.assertIsNotNone(size)

          self.assertIsNotNone(fsapfs_volume.size)

      finally:
        fsapfs_container.close()

  def test_get_name(self):
    """Tests the get_name function and name property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      try:
        if not fsapfs_container.number_of_volumes:
          raise unittest.SkipTest("missing volumes")

        fsapfs_volume = fsapfs_container.get_volume(0)

        name = fsapfs_volume.get_name()
        self.assertIsNotNone(name)

        self.assertIsNotNone(fsapfs_volume.name)

      finally:
        fsapfs_container.close()

  def test_get_next_file_entry_identifier(self):
    """Tests the get_next_file_entry_identifier function and next_file_entry_identifier property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      try:
        if not fsapfs_container.number_of_volumes:
          raise unittest.SkipTest("missing volumes")

        fsapfs_volume = fsapfs_container.get_volume(0)

        next_file_entry_identifier = fsapfs_volume.get_next_file_entry_identifier()
        self.assertIsNotNone(next_file_entry_identifier)

        self.assertIsNotNone(fsapfs_volume.next_file_entry_identifier)

      finally:
        fsapfs_container.close()

  def test_get_root_directory(self):
    """Tests the get_root_directory function and root_directory property."""
    test_source = getattr(unittest, "source", None)
    if not test_source:
      raise unittest.SkipTest("missing source")

    test_offset = getattr(unittest, "offset", None)

    with DataRangeFileObject(
        test_source, test_offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      try:
        if not fsapfs_container.number_of_volumes:
          raise unittest.SkipTest("missing volumes")

        fsapfs_volume = fsapfs_container.get_volume(0)

        root_directory = fsapfs_volume.get_root_directory()
        self.assertIsNotNone(root_directory)

        self.assertIsNotNone(fsapfs_volume.root_directory)

      finally:
        fsapfs_container.close()


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "-o", "--offset", dest="offset", action="store", default=None,
      type=int, help="offset of the source file.")

  argument_parser.add_argument(
      "-p", "--password", dest="password", action="store", default=None,
      type=str, help="password to unlock the source file.")

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "offset", options.offset)
  setattr(unittest, "password", options.password)
  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
