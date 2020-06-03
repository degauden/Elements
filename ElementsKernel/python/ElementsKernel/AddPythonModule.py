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


""" This script creates a new Elements python module

:file: ElementsKernel/AddPythonModule.py
:author: Nicolas Morisset

:date: 01/07/15

"""

import os
import argparse
import time
from ElementsKernel import Auxiliary
from ElementsKernel import ProjectCommonRoutines
from ElementsKernel import ParseCmakeLists
from ElementsKernel import Logging

from ElementsKernel import Exit

logger = Logging.getLogger('AddPythonModule')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
PYTEST_TEMPLATE_FILE_IN = 'PythonTest_template.py.in'
PYMODULE_TEMPLATE_FILE_IN = 'PythonModule_template.py.in'

################################################################################

def createDirectories(module_dir, module_name):
    """
    Create directories needed for a python module
    """
    standalone_directories = [os.path.join('python', module_name),
                              os.path.join('tests', 'python')]
    for d in standalone_directories:
        target_dir = os.path.join(module_dir, d)
        if not os.path.exists(target_dir):
            os.makedirs(target_dir)

################################################################################

def updateCmakeListsFile(module_dir):
    """
    Update the CMakeList.txt file
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

        # Add elements_install_conf_files if any
        cmake_object.elements_install_python_modules = 'elements_install_python_modules()'

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################

def createPythonModule(module_dir, module_name, python_module_name):
    """
    Create the python module
    """
    print ('module_dir ',module_dir, 'module_name ',module_name, 'python_module_name ',python_module_name)
    createDirectories(module_dir, module_name)
    ProjectCommonRoutines.createPythonInitFile(os.path.join(module_dir, 'python', module_name, '__init__.py'))
    full_pymodule_name = os.path.join('python', module_name, python_module_name + '.py')
    pytest_name = os.path.join('tests', 'python', python_module_name + '_test.py')
    target_locations = { 
                       PYMODULE_TEMPLATE_FILE_IN: full_pymodule_name,
                       PYTEST_TEMPLATE_FILE_IN: pytest_name
                       }

    configuration = {  "FILE":  full_pymodule_name,
                       "FILETEST": pytest_name,
                       "DATE": time.strftime("%x"),
                       "AUTHOR": ProjectCommonRoutines.getAuthor(),
                       "MODULENAME": module_name,
                       "PYTHONMODULE": python_module_name
                    }
    # Put AUX files to their target and substitut
    for src in target_locations:
        file_name = os.path.join("ElementsKernel", "templates", src)
        tgt = target_locations[src]
        Auxiliary.configure(file_name, module_dir, tgt,
                            configuration=configuration,
                            create_missing_dir=True)
        ProjectCommonRoutines.addItemToCreationList(os.path.join(module_dir, tgt))

    updateCmakeListsFile(module_dir)

################################################################################

def makeChecks(module_file_path, python_module_name):
    """
    Make some checks
    """
    # Module as no version number? but '1.0' is just for using the routine
    ProjectCommonRoutines.checkNameAndVersionValid(python_module_name, '1.0')
    ProjectCommonRoutines.checkFileNotExist(module_file_path, python_module_name)
    ProjectCommonRoutines.checkAuxFileExist(PYTEST_TEMPLATE_FILE_IN)

################################################################################

def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
This script creates an <Elements> python module at your current directory
(default), this directory must be an <Elements> module.
           """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('module_name', metavar='module-name',
                        type=str,
                        help='Module name')
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
    logger.info('#  Logging from the mainMethod() of the AddPythonModule script')
    logger.info('#')

    exit_code = Exit.Code["OK"]

    python_module_name = args.module_name

    try:
        # Default is the current directory
        current_dir = os.getcwd()

        logger.info('Current directory : %s', current_dir)
        logger.info('')

        # We absolutely need a Elements cmake file
        module_name = ProjectCommonRoutines.getElementsModuleName(current_dir)

        module_file_path = os.path.join(current_dir, 'python', module_name,
                                        python_module_name + '.py')
        makeChecks(module_file_path, python_module_name)

        # Create module
        createPythonModule(current_dir, module_name, python_module_name)

        logger.info('< %s > python module successfully created in < %s >', python_module_name, module_file_path)
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