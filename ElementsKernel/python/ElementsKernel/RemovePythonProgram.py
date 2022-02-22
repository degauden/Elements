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

""" This script will remove all files related to a python program

:file: ElementsKernel/RemovePythonProgram.py
:author: Nicolas Morisset

:date: 02/10/16

"""

import argparse
import os

from argparse import RawTextHelpFormatter

try:
    from builtins import input
except ImportError:
    from __builtin__ import input

from ElementsKernel import ProjectCommonRoutines
from ElementsKernel import ParseCmakeLists
from ElementsKernel import Logging

from ElementsKernel import Exit

CMAKE_LISTS_FILE = 'CMakeLists.txt'

LOGGER = Logging.getLogger(__name__)

################################################################################


def getAllFiles(program_name, module_directory, module_name):
    """
    Get all files related to a python program
    """
    delete_file_list = []
    file_name_conf = os.path.join(module_directory, 'conf', module_name, program_name) + '.conf'
    if os.path.exists(file_name_conf):
        delete_file_list.append(file_name_conf)
    file_name_cpp = os.path.join(module_directory, 'python', module_name, program_name) + '.py'
    if os.path.exists(file_name_cpp):
        delete_file_list.append(file_name_cpp)

    return delete_file_list

################################################################################


def updateCmakeListsFile(module_dir, program_name):
    """
    Update the <CMakeLists.txt> file
    """
    LOGGER.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        # Backup the file
        ProjectCommonRoutines.makeACopy(cmake_filename)
        f = open(cmake_filename)
        data = f.read()
        f.close()
        # Add the program to be removed
        cmake_object = ParseCmakeLists.CMakeLists(data)
        cmake_object.elements_remove_python_executable = program_name

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################


def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
    This script allows you to remove all files on disk related to a python
    program. Usually you use this script when you made a typo in the program
    name when calling the <AddPythonProgram> script. The <CMakeLists.txt> file
    is updated accordingly.
    """

    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=RawTextHelpFormatter)
    parser.add_argument('program_name', metavar='program-name',
                        type=str, help='Program name')

    return parser

################################################################################


def mainMethod(args):
    """
    Main
    """

    LOGGER.info('#')
    LOGGER.info('#  Logging from the mainMethod() of the RemovePythonProgram \
    script ')
    LOGGER.info('#')

    exit_code = Exit.Code["OK"]

    program_name = args.program_name

    # Default is the current directory
    module_dir = os.getcwd()

    LOGGER.info('Current directory : %s', module_dir)
    LOGGER.info('')

    try:
        # We absolutely need a Elements cmake file
        module_name = ProjectCommonRoutines.getElementsModuleName(module_dir)

        # Default is the current directory
        file_to_be_deleted = getAllFiles(program_name, module_dir, module_name)
        if file_to_be_deleted:
            LOGGER.info('File to be deleted:')
            for elt_file in file_to_be_deleted:
                LOGGER.info(' --> %s', elt_file)
            response_key = input('Do you want to continue?(y/n, default: n)')
            if response_key.lower() == 'y':
                ProjectCommonRoutines.removeFilesOnDisk(file_to_be_deleted)
                updateCmakeListsFile(module_dir, program_name)
        else:
            LOGGER.info('No file found for deletion!')
            LOGGER.info('')

    except Exception as msg:
        if str(msg):
            LOGGER.error(msg)
        LOGGER.error('# Script aborted.')
        exit_code = Exit.Code["NOT_OK"]
    else:
        LOGGER.info('# Script over.')

    return exit_code
