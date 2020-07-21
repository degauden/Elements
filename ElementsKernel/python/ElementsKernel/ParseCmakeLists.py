
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

""" This module parses and updates the <CMakeLists.txt> file. It uses the
 ParseCmakeListsMacros module to parse each cmake macro.

:file: ElementsKernel/ParseCmakeLists.py
:author: Nikolaos Apostolakos, Nicolas Morisset

:date: 01/07/15

"""

import ElementsKernel.ParseCmakeListsMacros as pclm
import re


class CMakeLists(object):
    """
    Parse the <CMakeLists.txt> file. The contents for each macro is stored
    into a list
    """

    def __init__(self, text=''):
        self.text = text
        self.elements_install_conf_files = None
        self.elements_install_python_modules = None
        self.elements_install_scripts = None
        self.elements_subdir_list = []
        self.elements_depends_on_subdirs_list = []
        self.find_package_list = []
        self.elements_add_library_list = []
        self.elements_add_executable_list = []
        self.elements_add_unit_test_list = []
        self.elements_add_python_executable_list = []
        self.elements_remove_python_executable = None
        self.elements_remove_python_module = None
        self.elements_remove_cpp_class = None
        self.elements_remove_cpp_program = None

        # Remove all comment lines
        text_with_no_comment = self._removeComments()
        #
        # Here we parse the CMakeLists.txt file
        #
        self._getMacroListWithNoParams(text_with_no_comment)
        self._getMacroListWithParams(text_with_no_comment)

    def _getMacroListWithParams(self, text):
        """
        Look for specific Elements macros (with parameters) and set the object members accordingly
        """
        regex_pattern = r"elements_add_library\(.*?\)"
        for elements_add_library in self._findAllPattern(regex_pattern, text, True):
            content = elements_add_library.replace('\n', ' ').replace('elements_add_library(', '')[:-1].strip().split()
            name = content[0]
            source_list = []
            link_libraries = []
            include_dirs = []
            public_headers = []
            location = 'SOURCE'
            for word in content[1:]:
                if word == 'LINK_LIBRARIES':
                    location = 'LINK_LIBRARIES'
                    continue
                if word == 'INCLUDE_DIRS':
                    location = 'INCLUDE_DIRS'
                    continue
                if word == 'PUBLIC_HEADERS':
                    location = 'PUBLIC_HEADERS'
                    continue
                if location == 'SOURCE':
                    source_list.append(word)
                if location == 'LINK_LIBRARIES':
                    link_libraries.append(word)
                if location == 'INCLUDE_DIRS':
                    include_dirs.append(word)
                if location == 'PUBLIC_HEADERS':
                    public_headers.append(word)
            self.elements_add_library_list.append(pclm.ElementsAddLibrary(name, source_list,
                                                                     link_libraries,
                                                                     include_dirs,
                                                                     public_headers))

        regex_pattern = r"elements_add_executable\(.*?\)"
        for elements_add_executable in self._findAllPattern(regex_pattern, text, True):
            content = elements_add_executable.replace('\n', ' ').replace('elements_add_executable(', '')[:-1]\
            .strip().split()
            name = content[0]
            source = ''
            link_libraries = []
            include_dirs = []
            location = 'SOURCE'
            for word in content[1:]:
                if word == 'LINK_LIBRARIES':
                    location = 'LINK_LIBRARIES'
                    continue
                if word == 'INCLUDE_DIRS':
                    location = 'INCLUDE_DIRS'
                    continue
                if location == 'SOURCE':
                    source = word
                if location == 'LINK_LIBRARIES':
                    link_libraries.append(word)
                if location == 'INCLUDE_DIRS':
                    include_dirs.append(word)
            self.elements_add_executable_list.append(pclm.ElementsAddExecutable(name, source, link_libraries, include_dirs))

        regex_pattern = r"elements_add_unit_test\(.*?\)"
        for elements_add_unit_test in self._findAllPattern(regex_pattern, text, True):
            content = elements_add_unit_test.replace('\n', ' ').replace('elements_add_unit_test(', '')[:-1]\
            .strip().split()
            name = content[0]
            source_list = []
            link_libraries = []
            include_dirs = []
            key_type = ''
            exec_name = ''
            location = 'SOURCE'
            for word in content[1:]:
                if word == 'LINK_LIBRARIES':
                    location = 'LINK_LIBRARIES'
                    continue
                if word == 'INCLUDE_DIRS':
                    location = 'INCLUDE_DIRS'
                    continue
                if word == 'TYPE':
                    location = 'TYPE'
                    continue
                if word == 'EXECUTABLE':
                    location = 'EXECUTABLE'
                    continue
                if location == 'SOURCE':
                    source_list.append(word)
                if location == 'LINK_LIBRARIES':
                    link_libraries.append(word)
                if location == 'INCLUDE_DIRS':
                    include_dirs.append(word)
                if location == 'TYPE':
                    key_type = word
                if location == 'EXECUTABLE':
                    exec_name = word
            self.elements_add_unit_test_list.append(pclm.ElementsAddUnitTest(name, source_list, link_libraries,
                                                                             include_dirs, key_type, exec_name))
        #
        # Python stuff
        #
        regex_pattern = r"elements_add_python_program\(.*?\)"
        for elements_add_python_program in self._findAllPattern(regex_pattern, text, True):
            content = elements_add_python_program.replace('\n', ' ').replace('elements_add_python_program(', '')[:-1]\
            .strip().split()
            name = content[0]
            module_name = content[1]
            self.elements_add_python_executable_list.append(pclm.ElementsAddPythonExecutable(name, module_name))

    def _getMacroListWithNoParams(self, text):
        """
        Look for specific Elements macros with no parameters and set the object members accordingly
        """
        regex_pattern = r"elements_install_conf_files\(.*?\)"
        if self._findAllPattern(regex_pattern, text, False):
            self.elements_install_conf_files = 'elements_install_conf_files()'

        regex_pattern = r"elements_install_python_modules\(.*?\)"
        if self._findAllPattern(regex_pattern, text, False):
            self.elements_install_python_modules = 'elements_install_python_modules()'

        regex_pattern = r"elements_install_scripts\(.*?\)"
        if self._findAllPattern(regex_pattern, text, False):
            self.elements_install_scripts = 'elements_install_scripts()'

        regex_pattern = r"elements_subdir\(.*?\)"
        for elements_subdir in self._findAllPattern(regex_pattern, text, True):
            name = elements_subdir.replace('\n', ' ').replace('elements_subdir(', '')[:-1].strip()
            self.elements_subdir_list.append(pclm.ElementsSubdir(name))

        regex_pattern = r"elements_depends_on_subdirs\(.*?\)"
        for elements_depends_on_subdirs in self._findAllPattern(regex_pattern, text, True):
            names = elements_depends_on_subdirs.replace('elements_depends_on_subdirs(', '')[:-1].strip()
            self.elements_depends_on_subdirs_list.append(pclm.ElementsDependsOnSubdirs(names.split()))

        regex_pattern = r"find_package\(.*?\)"
        for find_package in self._findAllPattern(regex_pattern, text, True):
            name = find_package.replace('\n', ' ').replace('find_package(', '')[:-1].strip()
            components = []
            if 'REQUIRED COMPONENTS' in name:
                components = name.split('REQUIRED COMPONENTS')[1].split()
                name = name.split('REQUIRED COMPONENTS')[0].strip()
            self.find_package_list.append(pclm.FindPackage(name, components))

    def _removeComments(self):
        """
        Remove all comments (with the <#> character et the beginning of the line)
        """
        for_parsing = ''
        for line in self.text.splitlines():
            if '#' in line:
                for_parsing += line[:line.find('#')] + '\n'
            else:
                for_parsing += line + '\n'
        return for_parsing

    @staticmethod
    def _findAllPattern(pattern, text_with_nocomments, with_options=True):
        """
        Look for a specific pattern in the text
        """
        found_pattern = []
        if with_options:
            found_pattern = re.findall(pattern, text_with_nocomments, re.MULTILINE | re.DOTALL)
        else:
            found_pattern = re.findall(pattern, text_with_nocomments)
        return found_pattern

    # Look for the right location for adding the text just after this position
    @staticmethod
    def _addAfter(text, tag, to_add):
        if tag in text:
            for match in re.finditer(tag + r"\(.*?\)", text, re.MULTILINE | re.DOTALL):
                # This is an empty loop in order to get the last match.
                pass
            temp = text[:match.end() + 1]
            rest = text[match.end() + 1:].splitlines()
            while rest and rest[0].startswith('#'):
                temp += rest[0] + '\n'
                rest = rest[1:]
            temp += to_add + '\n'
            for line in rest:
                temp += line + '\n'
            text = temp
        else:
            text += '\n' + to_add + '\n'

        return text

    def _remove_elements_macros(self, result):
        """
        Remove Elements macros if any from the CMakeLists object
        """
        # Remove python program from the list if any
        if not self.elements_remove_python_executable is None:
            remove_exe = self.elements_remove_python_executable
            for elt in self.elements_add_python_executable_list:
                str_elt = str(elt)
                if remove_exe in str_elt:
                    self.elements_add_python_executable_list.remove(elt)
                    result = result.replace(str_elt + '\n', '')
                    self.elements_remove_python_executable = None

        # Remove cpp class macro from the list if any
        if not self.elements_remove_cpp_class is None:
            remove_class = self.elements_remove_cpp_class
            for elt in self.elements_add_unit_test_list:
                str_elt = str(elt)
                if remove_class in str_elt:
                    self.elements_add_unit_test_list.remove(elt)
                    result = result.replace(str_elt + '\n', '')
                    self.elements_remove_cpp_class = None

        # Remove cpp program macro from the list if any
        if not self.elements_remove_cpp_program is None:
            remove_prog = self.elements_remove_cpp_program
            for elt in self.elements_add_executable_list:
                str_elt = str(elt)
                if remove_prog in str_elt:
                    self.elements_add_executable_list.remove(elt)
                    result = result.replace(str_elt + '\n', '')
                    self.elements_remove_cpp_program = None

        return result

    def _add_elements_macro_contents(self, result):
        """
        Add Elements macro contents from the CMakeLists object
        """
        closing_parenthesis = r"(?=[\s\)]).*?\)"
        leading_spaces = r"((^\s*)|((?<=\n)\s*))"
        open_parenthesis = r"\(\s*"

        for find_package in self.find_package_list:
            if not re.search(leading_spaces + "find_package" + open_parenthesis + find_package.package + \
                            closing_parenthesis, result, re.MULTILINE | re.DOTALL):
                result = CMakeLists._addAfter(result, 'find_package', str(find_package))

        for elements_subdir in self.elements_subdir_list:
            if not re.search(leading_spaces + "elements_subdir" + open_parenthesis + elements_subdir.name + \
                            closing_parenthesis, result, re.MULTILINE | re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_subdir', str(elements_subdir))

        for elements_depends_on_subdirs in self.elements_depends_on_subdirs_list:
            if not re.search(leading_spaces + "elements_depends_on_subdirs" + open_parenthesis + \
                            elements_depends_on_subdirs.subdir_list[0] + closing_parenthesis, \
                            result, re.MULTILINE | re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_depends_on_subdirs', str(elements_depends_on_subdirs))

        for library in self.elements_add_library_list:
            if not re.search(leading_spaces + "elements_add_library" + open_parenthesis + library.name + \
                             closing_parenthesis, result, re.MULTILINE | re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_add_library', str(library))
            else:
                result = re.sub(leading_spaces + "elements_add_library" + open_parenthesis + library.name + \
                                closing_parenthesis, str(library), result, flags=re.MULTILINE | re.DOTALL)

        for exe in self.elements_add_executable_list:
            if not re.search(leading_spaces + "elements_add_executable" + open_parenthesis + exe.name + \
                             closing_parenthesis, result, re.MULTILINE | re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_add_executable', str(exe))
            else:
                result = re.sub(leading_spaces + "elements_add_executable" + open_parenthesis + exe.name + \
                                closing_parenthesis, str(exe), result, flags=re.MULTILINE | re.DOTALL)

        for unit_test in self.elements_add_unit_test_list:
            if not re.search(leading_spaces + "elements_add_unit_test" + open_parenthesis + unit_test.class_name + \
                            closing_parenthesis, result, re.MULTILINE | re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_add_unit_test', str(unit_test))
            else:
                result = re.sub(leading_spaces + "elements_add_unit_test" + open_parenthesis + unit_test.class_name + \
                                closing_parenthesis, str(unit_test), result, flags=re.MULTILINE | re.DOTALL)

        for pyexe in self.elements_add_python_executable_list:
            if not re.search(leading_spaces + "elements_add_python_program" + open_parenthesis + pyexe.name + \
                            closing_parenthesis, result, re.MULTILINE | re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_add_python_program', str(pyexe))
            else:
                result = re.sub(leading_spaces + "elements_add_python_program" + open_parenthesis + pyexe.name + \
                                closing_parenthesis, str(pyexe), result, flags=re.MULTILINE | re.DOTALL)

        if not re.search(leading_spaces + r"elements_install_python_modules\(.*?\)", result,
                         re.MULTILINE | re.DOTALL) and self.elements_install_python_modules:
            result = CMakeLists._addAfter(result, 'elements_install_python_modules',
                                          self.elements_install_python_modules)

        if not re.search(leading_spaces + r"elements_install_scripts\(.*?\)", result, re.MULTILINE | re.DOTALL) and \
           self.elements_install_scripts:
            result = CMakeLists._addAfter(result, 'elements_install_scripts', self.elements_install_scripts)

        if not re.search(leading_spaces + r"elements_install_conf_files\(.*?\)", result, re.MULTILINE | re.DOTALL) and\
           self.elements_install_conf_files:
            result = CMakeLists._addAfter(result, 'elements_install_conf_files', self.elements_install_conf_files)

        return result

    def __str__(self):
        # Here we built the CMakeLists.txt file
        # <text> contains the original contents of the CMakeLists.txt file
        result = self.text
        result = self._remove_elements_macros(result)
        result = self._add_elements_macro_contents(result)

        return result

