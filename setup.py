#!/usr/bin/env python
#
# Script to build and install Python-bindings.
# Version: 20230909

from __future__ import print_function

import copy
import datetime
import glob
import gzip
import platform
import os
import shlex
import shutil
import subprocess
import sys
import tarfile
import zipfile

from distutils.ccompiler import new_compiler

from setuptools import Extension
from setuptools import setup
from setuptools.command.build_ext import build_ext
from setuptools.command.sdist import sdist


if (sys.version_info[0], sys.version_info[1]) < (3, 7):
  print(("Unsupported Python version: {0:s}, version 3.7 or higher "
         "required.").format(sys.version))
  sys.exit(1)


class custom_build_ext(build_ext):
  """Custom handler for the build_ext command."""

  def _RunCommand(self, command):
    """Runs the command."""
    arguments = shlex.split(command)
    process = subprocess.Popen(
        arguments, stderr=subprocess.PIPE, stdout=subprocess.PIPE,
        universal_newlines=True)
    if not process:
      raise RuntimeError("Running: {0:s} failed.".format(command))

    output, error = process.communicate()
    if process.returncode != 0:
      error = "\n".join(error.split("\n")[-5:])
      raise RuntimeError("Running: {0:s} failed with error:\n{1:s}.".format(
          command, error))

    return output

  def build_extensions(self):
    """Set up the build extensions."""
    # TODO: move build customization here?
    build_ext.build_extensions(self)

  def run(self):
    """Runs the build extension."""
    compiler = new_compiler(compiler=self.compiler)
    if compiler.compiler_type == "msvc":
      self.define = [
          ("_CRT_SECURE_NO_WARNINGS", ""),
          ("UNICODE", ""),
      ]

    else:
      command = "sh configure --disable-nls --disable-shared-libs"
      output = self._RunCommand(command)

      print_line = False
      for line in output.split("\n"):
        line = line.rstrip()
        if line == "configure:":
          print_line = True

        if print_line:
          print(line)

      self.define = [
          ("HAVE_CONFIG_H", ""),
      ]

    build_ext.run(self)


class custom_sdist(sdist):
  """Custom handler for the sdist command."""

  def run(self):
    """Builds a source distribution (sdist) package."""
    if self.formats != ["gztar"] and self.formats != ["zip"]:
      print("'setup.py sdist' unsupported format.")
      sys.exit(1)

    if glob.glob("*.tar.gz"):
      print("'setup.py sdist' remove existing *.tar.gz files from "
            "source directory.")
      sys.exit(1)

    command = "make dist"
    exit_code = subprocess.call(command, shell=True)
    if exit_code != 0:
      raise RuntimeError("Running: {0:s} failed.".format(command))

    if not os.path.exists(self.dist_dir):
      os.mkdir(self.dist_dir)

    source_package_file = glob.glob("*.tar.gz")[0]
    source_package_prefix, _, source_package_suffix = (
        source_package_file.partition("-"))
    sdist_package_file = "{0:s}-python-{1:s}".format(
        source_package_prefix, source_package_suffix)
    sdist_package_file = os.path.join(self.dist_dir, sdist_package_file)
    os.rename(source_package_file, sdist_package_file)

    # Create and add the PKG-INFO file to the source package.
    with gzip.open(sdist_package_file, "rb") as input_file:
      with open(sdist_package_file[:-3], "wb") as output_file:
        shutil.copyfileobj(input_file, output_file)
    os.remove(sdist_package_file)

    self.distribution.metadata.write_pkg_info(".")
    pkg_info_path = "{0:s}-{1:s}/PKG-INFO".format(
        source_package_prefix, source_package_suffix[:-7])
    with tarfile.open(sdist_package_file[:-3], "a:") as tar_file:
      tar_file.add("PKG-INFO", arcname=pkg_info_path)
    os.remove("PKG-INFO")

    with open(sdist_package_file[:-3], "rb") as input_file:
      with gzip.open(sdist_package_file, "wb") as output_file:
        shutil.copyfileobj(input_file, output_file)
    os.remove(sdist_package_file[:-3])

    # Convert the .tar.gz into a .zip
    if self.formats == ["zip"]:
      zip_sdist_package_file = "{0:s}.zip".format(sdist_package_file[:-7])

      with tarfile.open(sdist_package_file, "r|gz") as tar_file:
        with zipfile.ZipFile(
            zip_sdist_package_file, "w", zipfile.ZIP_DEFLATED) as zip_file:
          for tar_file_entry in tar_file:
            file_entry = tar_file.extractfile(tar_file_entry)
            if tar_file_entry.isfile():
              modification_time = datetime.datetime.fromtimestamp(
                  tar_file_entry.mtime)
              zip_modification_time = (
                  modification_time.year, modification_time.month,
                  modification_time.day, modification_time.hour,
                  modification_time.minute, modification_time.second)
              zip_info = zipfile.ZipInfo(
                  date_time=zip_modification_time,
                  filename=tar_file_entry.name)
              zip_info.external_attr = (tar_file_entry.mode & 0xff) << 16

              file_data = file_entry.read()
              zip_file.writestr(zip_info, file_data)

      os.remove(sdist_package_file)
      sdist_package_file = zip_sdist_package_file

    # Inform distutils what files were created.
    dist_files = getattr(self.distribution, "dist_files", [])
    dist_files.append(("sdist", "", sdist_package_file))


class ProjectInformation(object):
  """Project information."""

  def __init__(self):
    """Initializes project information."""
    super(ProjectInformation, self).__init__()
    self.include_directories = []
    self.library_name = None
    self.library_names = []
    self.library_version = None

    self._ReadConfigureAc()
    self._ReadMakefileAm()

  @property
  def module_name(self):
    """The Python module name."""
    return "py{0:s}".format(self.library_name[3:])

  def _ReadConfigureAc(self):
    """Reads configure.ac to initialize the project information."""
    with open("configure.ac", "r", encoding="utf-8") as file_object:
      found_ac_init = False
      found_library_name = False
      for line in file_object.readlines():
        line = line.strip()
        if found_library_name:
          library_version = line[1:-2]
          self.library_version = library_version
          break

        elif found_ac_init:
          library_name = line[1:-2]
          self.library_name = library_name
          found_library_name = True

        elif line.startswith("AC_INIT"):
          found_ac_init = True

    if not self.library_name or not self.library_version:
      raise RuntimeError(
          "Unable to find library name and version in: configure.ac")

  def _ReadMakefileAm(self):
    """Reads Makefile.am to initialize the project information."""
    if not self.library_name:
      raise RuntimeError("Missing library name")

    with open("Makefile.am", "r", encoding="utf-8") as file_object:
      found_subdirs = False
      for line in file_object.readlines():
        line = line.strip()
        if found_subdirs:
          library_name, _, _ = line.partition(" ")

          self.include_directories.append(library_name)

          if library_name.startswith("lib"):
            self.library_names.append(library_name)

          if library_name == self.library_name:
            break

        elif line.startswith("SUBDIRS"):
          found_subdirs = True

    if not self.include_directories or not self.library_names:
      raise RuntimeError(
          "Unable to find include directories and library names in: "
          "Makefile.am")


project_information = ProjectInformation()

CMDCLASS = {
  "build_ext": custom_build_ext,
  "sdist": custom_sdist}

SOURCES = []

# TODO: replace by detection of MSC
DEFINE_MACROS = []
if platform.system() == "Windows":
  DEFINE_MACROS.append(("WINVER", "0x0501"))
  # TODO: determine how to handle third party DLLs.
  for library_name in project_information.library_names:
    if library_name != project_information.library_name:
      definition = "HAVE_LOCAL_{0:s}".format(library_name.upper())

    DEFINE_MACROS.append((definition, ""))

# Put everything inside the Python module to prevent issues with finding
# shared libaries since pip does not integrate well with the system package
# management.
for library_name in project_information.library_names:
  for source_file in glob.glob(os.path.join(library_name, "*.[ly]")):
    generated_source_file = "{0:s}.c".format(source_file[:-2])
    if not os.path.exists(generated_source_file):
      raise RuntimeError("Missing generated source file: {0:s}".format(
          generated_source_file))

  source_files = glob.glob(os.path.join(library_name, "*.c"))
  SOURCES.extend(source_files)

source_files = glob.glob(os.path.join(project_information.module_name, "*.c"))
SOURCES.extend(source_files)

# TODO: find a way to detect missing python.h
# e.g. on Ubuntu python-dev is not installed by python-pip

setup_args = dict(
    cmdclass=CMDCLASS,
    ext_modules=[
        Extension(
            project_information.module_name,
            define_macros=DEFINE_MACROS,
            include_dirs=project_information.include_directories,
            libraries=[],
            library_dirs=[],
            sources=SOURCES
        )
    ]
)
setup(**setup_args)

