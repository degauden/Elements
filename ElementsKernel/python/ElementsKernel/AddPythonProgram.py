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

""" This script creates a new Elements module

:file: ElementsKernel/AddPythonProgram.py
:author: Nicolas Morisset

:date: 01/07/15

"""

import argparse
import os
import time

from ElementsKernel import Auxiliary
from ElementsKernel import ProjectCommonRoutines
from ElementsKernel import ParseCmakeLists
from ElementsKernel import ParseCmakeListsMacros
from ElementsKernel import Logging

from ElementsKernel import Exit

logger = Logging.getLogger('AddPythonProgram')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
PROGRAM_TEMPLATE_FILE = 'PythonProgram_template.py'
PROGRAM_TEMPLATE_FILE_IN = 'PythonProgram_template.py.in'

################################################################################


def createDirectories(module_dir, module_name):
    """
    Create directories needed for a python program
    """
    standalone_directories = [os.path.join('python', module_name),
                              os.path.join('conf', module_name)]
    for d in standalone_directories:
        target_dir = os.path.join(module_dir, d)
        if not os.path.exists(target_dir):
            os.makedirs(target_dir)

################################################################################


def createFiles(module_dir, module_name, program_name):
    """
    Create files needed for a python program
    """
    init_file = os.path.join(module_dir, 'python', module_name, '__init__.py')
    ProjectCommonRoutines.createPythonInitFile(init_file)
    ProjectCommonRoutines.addItemToCreationList(init_file)

    conf_file = os.path.join(module_dir, 'conf', module_name, program_name + '.conf')
    ProjectCommonRoutines.addItemToCreationList(conf_file)
    if not os.path.exists(conf_file):
        f = open(conf_file, 'w')
        f.write('# Write your program options here. e.g. : option = string')
        f.close()

################################################################################


def substituteAuxFiles(module_dir, program_name, module_name):
    """
    Copy AUX file(s) and substitutes keyworks
    """
    filename = program_name + ".py"
    configuration = {  "FILE":  os.path.join('python', module_name, filename),
                       "DATE": time.strftime("%x"),
                       "AUTHOR": ProjectCommonRoutines.getAuthor(),
                       "PROGRAMNAME": program_name
                    }
    # Put AUX files to their target and substitut
    tgt = os.path.join('python', module_name, program_name + '.py')
    Auxiliary.configure(os.path.join("ElementsKernel", "templates", PROGRAM_TEMPLATE_FILE_IN),
                        module_dir, tgt,
                        configuration=configuration,
                        create_missing_dir=True)
    ProjectCommonRoutines.addItemToCreationList(os.path.join(module_dir, tgt))

################################################################################


def updateCmakeListsFile(module_dir, program_name):
    """
    Update the <CMakeList.txt> file
    """
    logger.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)
    ProjectCommonRoutines.addItemToCreationList(cmake_filename)

    # Backup the file
    ProjectCommonRoutines.makeACopy(cmake_filename)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        f = open(cmake_filename)
        data = f.read()
        f.close()
        cmake_object = ParseCmakeLists.CMakeLists(data)
        module_name = cmake_object.elements_subdir_list[0].name + '.' + program_name

        # Add elements_install_conf_files if any
        cmake_object.elements_install_python_modules = 'elements_install_python_modules()'
        cmake_object.elements_install_conf_files = 'elements_install_conf_files()'

        program_object = ParseCmakeListsMacros.ElementsAddPythonExecutable(program_name, module_name)
        cmake_object.elements_add_python_executable_list.append(program_object)

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################


def createPythonProgram(current_dir, module_name, program_name):
    """
    Create the python program
    """
    createDirectories(current_dir, module_name)
    createFiles(current_dir, module_name, program_name)
    substituteAuxFiles(current_dir, program_name, module_name)
    updateCmakeListsFile(current_dir, program_name)

################################################################################


def makeChecks(program_file_path, program_name):
    """
    Make some checks
    """
    # Module as no version number, '1.0' is just for using the routine
    ProjectCommonRoutines.checkNameAndVersionValid(program_name, '1.0')
    # Check aux file exist
    ProjectCommonRoutines.checkAuxFileExist(PROGRAM_TEMPLATE_FILE_IN)
    # Make sure the program does not already exist
    ProjectCommonRoutines.checkFileNotExist(program_file_path, program_name)

################################################################################


def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
    This script creates an <Elements> python program at your current directory
    (default), this directory must be an <Elements> module.
           """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('program_name', metavar='program-name',
                        type=str,
                        help='Program name')
    parser.add_argument('-y', '--yes', default=False, action="store_true",
                        help='Answer <yes> by default to any question, useful when the script is called by another'\
                         'script')

    return parser

################################################################################


def mainMethod(args):
    """
    Main
    """

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the AddPythonProgram script')
    logger.info('#')

    exit_code = Exit.Code["OK"]

    program_name = args.program_name

    try:
        # Default is the current directory
        current_dir = os.getcwd()

        logger.info('# Current directory : %s', current_dir)
        logger.info('')

        # We absolutely need a Elements cmake file
        module_name = ProjectCommonRoutines.getElementsModuleName(current_dir)

        program_file_path = os.path.join(current_dir, 'python', module_name, program_name + '.py')
        # Make checks
        makeChecks(program_file_path, program_name)

        # Create program
        createPythonProgram(current_dir, module_name, program_name)

        logger.info('< %s > program successfully created in < %s >.', program_name, program_file_path)
        # Remove backup file
        ProjectCommonRoutines.deleteFile(os.path.join(current_dir, CMAKE_LISTS_FILE) + '~')

        # Print all files created
        ProjectCommonRoutines.printCreationList()

    except Exception as msg:
        if str(msg):
            logger.error(msg)
        logger.error('# Script aborted.')
        exit_code = Exit.Code["NOT_OK"]
    else:
        logger.info('# Script over.')

    return exit_code
