#!/usr/bin/env python
#
# Python-bindings container type test script
#
# Copyright (C) 2018-2022, Joachim Metz <joachim.metz@gmail.com>
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


class ContainerTypeTests(unittest.TestCase):
  """Tests the container type."""

  def test_signal_abort(self):
    """Tests the signal_abort function."""
    fsapfs_container = pyfsapfs.container()

    fsapfs_container.signal_abort()

  def test_open(self):
    """Tests the open function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if unittest.offset:
      raise unittest.SkipTest("source defines offset")

    fsapfs_container = pyfsapfs.container()
    fsapfs_container.open(test_source)

    with self.assertRaises(IOError):
      fsapfs_container.open(test_source)

    fsapfs_container.close()

    with self.assertRaises(TypeError):
      fsapfs_container.open(None)

    with self.assertRaises(ValueError):
      fsapfs_container.open(test_source, mode="w")

  def test_open_file_object(self):
    """Tests the open_file_object function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      with self.assertRaises(IOError):
        fsapfs_container.open_file_object(file_object)

      fsapfs_container.close()

      with self.assertRaises(TypeError):
        fsapfs_container.open_file_object(None)

      with self.assertRaises(ValueError):
        fsapfs_container.open_file_object(file_object, mode="w")

  def test_close(self):
    """Tests the close function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    fsapfs_container = pyfsapfs.container()

    with self.assertRaises(IOError):
      fsapfs_container.close()

  def test_open_close(self):
    """Tests the open and close functions."""
    test_source = unittest.source
    if not test_source:
      return

    if unittest.offset:
      raise unittest.SkipTest("source defines offset")

    fsapfs_container = pyfsapfs.container()

    # Test open and close.
    fsapfs_container.open(test_source)
    fsapfs_container.close()

    # Test open and close a second time to validate clean up on close.
    fsapfs_container.open(test_source)
    fsapfs_container.close()

    if os.path.isfile(test_source):
      with open(test_source, "rb") as file_object:

        # Test open_file_object and close.
        fsapfs_container.open_file_object(file_object)
        fsapfs_container.close()

        # Test open_file_object and close a second time to validate clean up on close.
        fsapfs_container.open_file_object(file_object)
        fsapfs_container.close()

        # Test open_file_object and close and dereferencing file_object.
        fsapfs_container.open_file_object(file_object)
        del file_object
        fsapfs_container.close()

  def test_is_locked(self):
    """Tests the is_locked function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      _ = fsapfs_container.is_locked()

      fsapfs_container.close()

    if unittest.password:
      with DataRangeFileObject(
          test_source, unittest.offset or 0, None) as file_object:

        fsapfs_container = pyfsapfs.container()
        fsapfs_container.open_file_object(file_object)

        _ = fsapfs_container.is_locked()

        fsapfs_container.close()

  def test_get_size(self):
    """Tests the get_size function and size property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      size = fsapfs_container.get_size()
      self.assertIsNotNone(size)

      self.assertIsNotNone(fsapfs_container.size)

      fsapfs_container.close()

  def test_get_number_of_volumes(self):
    """Tests the get_number_of_volumes function and number_of_volumes property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fsapfs_container = pyfsapfs.container()
      fsapfs_container.open_file_object(file_object)

      number_of_volumes = fsapfs_container.get_number_of_volumes()
      self.assertIsNotNone(number_of_volumes)

      self.assertIsNotNone(fsapfs_container.number_of_volumes)

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
