#!/usr/bin/env python3
#
# Script to build and install Python-bindings.
# Version: 20260510

import glob
import gzip
import os
import shlex
import shutil
import subprocess
import sys
import tarfile

from setuptools import Extension
from setuptools._distutils.ccompiler import new_compiler
from setuptools.command.build_ext import build_ext
from setuptools.command.sdist import sdist


class ProjectInformation:
    """Project information."""

    def __init__(self, compiler_type):
        """Initializes project information."""
        super().__init__()
        self._library_name = None
        self._library_names = None
        self._library_version = None
        self.define_macros = None
        self.include_directories = None
        self.sources = None

        self._read_configure_ac()
        self._read_makefile_am()
        self._determine_define_macros(compiler_type)
        self._determine_sources()

    @property
    def module_name(self):
        """The Python module name."""
        return f"py{self._library_name[3:]:s}"

    def _determine_define_macros(self, compiler_type):
        """Determines the define macros."""
        self.define_macros = []

        if compiler_type != "msvc":
            self.define_macros.append(("HAVE_CONFIG_H", "1"))
        else:
            self.define_macros.extend(
                [
                    ("_CRT_SECURE_NO_WARNINGS", "1"),
                    ("UNICODE", "1"),
                    ("WINVER", "0x0501"),
                ]
            )
            for library_name in self._library_names:
                if library_name != self._library_name:
                    self.define_macros.append(
                        (f"HAVE_LOCAL_{library_name:s}".upper(), "1")
                    )

    def _determine_sources(self):
        """Determines the source files."""
        self.sources = []

        for library_name in self._library_names:
            for source_file in glob.glob(os.path.join(library_name, "*.[ly]")):
                generated_source_file = f"{source_file[:-2]:s}.c"
                if not os.path.exists(generated_source_file):
                    raise RuntimeError(
                        f"Missing generated source file: {generated_source_file:s}"
                    )

            source_files = glob.glob(os.path.join(library_name, "*.c"))
            self.sources.extend(source_files)

        source_files = glob.glob(os.path.join(self.module_name, "*.c"))
        self.sources.extend(source_files)

    def _read_configure_ac(self):
        """Reads the configure.ac file to initialize the project information."""
        with open("configure.ac", encoding="utf-8") as file_object:
            found_ac_init = False
            found_library_name = False
            for line in file_object.readlines():
                line = line.strip()
                if found_library_name:
                    library_version = line[1:-2]
                    self._library_version = library_version
                    break

                elif found_ac_init:
                    library_name = line[1:-2]
                    self._library_name = library_name
                    found_library_name = True

                elif line.startswith("AC_INIT"):
                    found_ac_init = True

        if not self._library_name or not self._library_version:
            raise RuntimeError(
                "Unable to find library name and version in: configure.ac"
            )

    def _read_makefile_am(self):
        """Reads the Makefile.am file to initialize the project information."""
        if not self._library_name:
            raise RuntimeError("Missing library name")

        self._library_names = []
        self.include_directories = []
        with open("Makefile.am", encoding="utf-8") as file_object:
            found_subdirs = False
            for line in file_object.readlines():
                line = line.strip()
                if found_subdirs:
                    library_name, _, _ = line.partition(" ")

                    self.include_directories.append(library_name)

                    if library_name.startswith("lib"):
                        self._library_names.append(library_name)

                    if library_name == self._library_name:
                        break

                elif line.startswith("SUBDIRS"):
                    found_subdirs = True

        if not self.include_directories or not self._library_names:
            raise RuntimeError(
                "Unable to find include directories and library names in: "
                "Makefile.am"
            )


class custom_build_ext(build_ext):
    """Custom build_ext command."""

    def _print_configure_summary(self, output):
        """Prints the configure summary."""
        print_line = False
        for line in output.split("\n"):
            line = line.rstrip()
            if line == "configure:":
                print_line = True

            if print_line:
                print(line)

    def _run_shell_command(self, command):
        """Runs a command."""
        arguments = shlex.split(f"sh {command:s}")
        process = subprocess.Popen(
            arguments,
            stderr=subprocess.PIPE,
            stdout=subprocess.PIPE,
            universal_newlines=True,
        )
        if not process:
            raise RuntimeError(f"Running: {command:s} failed.")

        output, error = process.communicate()
        if process.returncode != 0:
            error = "\n".join(error.split("\n")[-5:])
            raise RuntimeError(f"Running: {command:s} failed with error:\n{error:s}.")

        return output

    def initialize_options(self):
        """Initialize build options."""
        super().initialize_options()

        compiler = new_compiler(compiler=self.compiler)
        project_information = ProjectInformation(compiler.compiler_type)

        # ext_module can be defined multiple times. It is currently assumed that
        # this is due to the experimental nature of tool.setuptools.ext-modules
        # at this time. Hence ext_modules is redefined as a single extension.
        self.distribution.ext_modules = [
            Extension(
                project_information.module_name,
                define_macros=project_information.define_macros,
                include_dirs=project_information.include_directories,
                sources=project_information.sources,
            )
        ]

    def run(self):
        """Runs the build."""
        compiler = new_compiler(compiler=self.compiler)
        if compiler.compiler_type != "msvc":
            output = self._run_shell_command(
                "configure --disable-nls --disable-shared-libs"
            )
            self._print_configure_summary(output)

        super().run()


class custom_sdist(sdist):
    """Custom sdist command."""

    def _repackage(self, source_package_file):
        """Repackages the source package file."""
        name_prefix, _, name_suffix = source_package_file[:-4].partition("-")

        source_prefix = f"{name_prefix:s}-{name_suffix:s}"
        sdist_prefix = f"{name_prefix:s}_python-{name_suffix:s}"
        sdist_package_file = os.path.join(self.dist_dir, f"{sdist_prefix:s}.tar.gz")

        with tarfile.open(source_package_file, "r") as input_file:
            with tarfile.open(sdist_package_file, "w:gz") as output_file:
                for member in input_file.getmembers():
                    if member.name == source_prefix:
                        member.name = sdist_prefix
                    elif member.name.startswith(f"{source_prefix:s}/"):
                        member.name = member.name.replace(
                            f"{source_prefix:s}/", f"{sdist_prefix:s}/", 1
                        )

                    if member.isfile():
                        file_object = input_file.extractfile(member)
                        output_file.addfile(member, file_object)
                    else:
                        output_file.addfile(member)

        return sdist_package_file

    def run(self):
        """Runs the build."""
        if self.formats != ["gztar"]:
            print(f"Unsupported sdist format: {self.formats!s}")
            sys.exit(1)

        if glob.glob("*.tar.gz"):
            print("Remove existing *.tar.gz files from source directory")
            sys.exit(1)

        exit_code = subprocess.call("make dist", shell=True)
        if exit_code != 0:
            raise RuntimeError("Failed to run: make dist")

        if not os.path.exists(self.dist_dir):
            os.mkdir(self.dist_dir)

        source_package_file = glob.glob("*.tar.gz")[0]

        name_prefix, _, name_suffix = source_package_file.partition("-")

        sdist_package_file = os.path.join(
            self.dist_dir, f"{name_prefix:s}_python-{name_suffix:s}"
        )
        # Decompression the source package.
        with gzip.open(source_package_file, "rb") as input_file:
            with open(source_package_file[:-3], "wb") as output_file:
                shutil.copyfileobj(input_file, output_file)
        os.remove(source_package_file)

        # Create a PKG-INFO file and add it to the source package.
        self.distribution.metadata.write_pkg_info(".")

        pkg_info_path = f"{name_prefix:s}-{name_suffix[:-7]:s}/PKG-INFO"
        with tarfile.open(source_package_file[:-3], "a:") as tar_file:
            tar_file.add("PKG-INFO", arcname=pkg_info_path)
        os.remove("PKG-INFO")

        sdist_package_file = self._repackage(source_package_file[:-3])
        os.remove(source_package_file[:-3])

        # Inform what files were created.
        dist_files = getattr(self.distribution, "dist_files", [])
        dist_files.append(("sdist", "", sdist_package_file))
