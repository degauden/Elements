#
# Copyright (C) 2012-2020 Euclid Science Ground Segment
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3.0 of the License, or (at your option)
# any later version.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#

""" All these classes are for parsing the macros in the CMakeLists.txt file. Each class
 represents a cmake macro.

:file: ElementsKernel/ParseCmakeListsMacros.py
:author: Nicolas Morisset

:date: 21/07/16


"""

################################################################################


class ElementsSubdir(object):
    """
    Decode the <elements_subdir> macro
    """

    def __init__(self, name):
        self.name = name

    def __str__(self):
        return 'elements_subdir(' + self.name + ')'

################################################################################


class ElementsDependsOnSubdirs(object):
    """
    Decode the <elements_depends_on_subdirs> macro
    """

    def __init__(self, subdir_list):
        self.subdir_list = subdir_list

    def __str__(self):
        result = 'elements_depends_on_subdirs('
        for subdir in self.subdir_list:
            result += subdir + ' '
        result = result.strip() + ')'
        return result

################################################################################


class FindPackage(object):
    """
    Decode the <find_package> macro
    """

    def __init__(self, package, required_components):
        self.package = package
        self.required_components_list = required_components

    def __str__(self):
        result = 'find_package(' + self.package
        if self.required_components_list:
            result += ' REQUIRED COMPONENTS'
        for component in self.required_components_list:
            result += ' ' + component
        return result + ')'

################################################################################


class ElementsAddLibrary(object):
    """
    Decode the <elements_add_library> macro
    """

    def __init__(self, name, source_list, link_libraries, include_dirs,
                 public_headers):
        self.name = name
        self.source_list = source_list
        self.link_libraries_list = link_libraries
        self.include_dirs_list = include_dirs
        self.public_headers_list = public_headers

    def __str__(self):
        result = 'elements_add_library(' + self.name
        for source in self.source_list:
            result += ' ' + source
        if self.include_dirs_list:
            result += '\n                     INCLUDE_DIRS'
            for name in self.include_dirs_list:
                result += ' ' + name
        if self.link_libraries_list:
            result += '\n                     LINK_LIBRARIES'
            for name in self.link_libraries_list:
                result += ' ' + name
        if self.public_headers_list:
            result += '\n                     PUBLIC_HEADERS'
            for name in self.public_headers_list:
                result += ' ' + name
        return result + ')'

################################################################################


class ElementsAddExecutable(object):
    """
    Decode the <elements_add_executable> macro
    """

    def __init__(self, name, source, link_libraries, include_dirs):
        self.name = name
        self.source = source
        self.link_libraries_list = link_libraries
        self.include_dirs_list = include_dirs

    def __str__(self):
        result = 'elements_add_executable(' + self.name
        result += ' ' + self.source
        if self.include_dirs_list:
            result += '\n                     INCLUDE_DIRS'
            for name in self.include_dirs_list:
                result += ' ' + name
        if self.link_libraries_list:
            result += '\n                     LINK_LIBRARIES'
            for name in self.link_libraries_list:
                result += ' ' + name
        result = result.strip() + ')'
        return result

################################################################################


class ElementsAddPythonExecutable(object):
    """
    Decode the <elements_add_python_program> macro
    """

    def __init__(self, name, module_name):
        self.name = name
        self.module_name = module_name

    def __str__(self):
        result = 'elements_add_python_program(' + self.name
        result += ' ' + self.module_name
        result = result.strip() + ')'
        return result

################################################################################


class ElementsAddUnitTest(object):
    """
    Decode the <elements_add_unit_test> macro
    """

    def __init__(self, class_name, source, link_libraries, include_dir, key_type, exec_name):
        self.class_name = class_name
        self.source_list = source
        self.link_libraries_list = link_libraries
        self.include_dir_list = include_dir
        self.key_type = key_type
        self.exec_name = exec_name

    def __str__(self):
        result = 'elements_add_unit_test(' + self.class_name + ' '
        if self.source_list:
            for source in self.source_list:
                result += source + ' '
        if self.exec_name:
            result += '\n                     EXECUTABLE ' + self.exec_name
        if self.link_libraries_list:
            result += '\n                     LINK_LIBRARIES'
            for name in self.link_libraries_list:
                result += ' ' + name
        if self.include_dir_list:
            result += '\n                     INCLUDE_DIRS'
            for include in self.include_dir_list:
                result += ' ' + include
        if self.key_type:
            result += '\n                     TYPE ' + self.key_type
        result = result.strip() + ')'
        return result

