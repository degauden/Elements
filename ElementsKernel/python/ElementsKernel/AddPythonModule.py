#
"""
@file ElementsKernel/AddPythonModule.py
@author Nicolas Morisset

@date 01/07/15

@brief This script creates a new Elements python module

@copyright: 2012-2020 Euclid Science Ground Segment

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3.0 of the License, or (at your option)
any later version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA


"""

import os
import argparse
import time
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.ParseCmakeLists as pcl
import ElementsKernel.Logging as log

logger = log.getLogger('AddPythonModule')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
PYTEST_TEMPLATE_FILE = 'PythonTest_template.py'
PYTEST_TEMPLATE_FILE_IN = 'PythonTest_template.py.in'
PYMODULE_TEMPLATE_FILE = 'PythonModule_template.py'
PYMODULE_TEMPLATE_FILE_IN = 'PythonModule_template.py.in'

################################################################################

def createDirectories(module_dir, module_name):
    """
    Create directories needed for a python module
    """
    # Create the executable directory
    python_module_path = os.path.join(module_dir, 'python', module_name)
    epcr.makeDirectory(python_module_path)
    # Create the python directory in tests
    tests_python_path = os.path.join(module_dir, 'tests', 'python')
    epcr.makeDirectory(tests_python_path)

################################################################################

def updateCmakeListsFile(module_dir):
    """
    Update the CMakeList.txt file
    """
    logger.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)

    # Backup the file
    epcr.makeACopy(cmake_filename)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        f = open(cmake_filename, 'r')
        data = f.read()
        f.close()
        cmake_object = pcl.CMakeLists(data)

        # Add elements_install_conf_files if any
        cmake_object.elements_install_python_modules = 'elements_install_python_modules()'

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()
################################################################################

def substituteStringsInPyModuleFile(pymodule_path, module_name, python_module_name):
    """
    Substitute variables in the python test template file and rename it
    """
    template_file = os.path.join(pymodule_path, PYMODULE_TEMPLATE_FILE)
    os.rename(os.path.join(pymodule_path, PYMODULE_TEMPLATE_FILE_IN), template_file)

    # Substitute strings in template_file
    f = open(template_file, 'r')
    data = f.read()
    author_str = epcr.getAuthor()
    date_str = time.strftime("%x")
    file_name_str = os.path.join('python', module_name, python_module_name + '.py')
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str}
    f.close()
    # Save new data
    file_name = template_file.replace(PYMODULE_TEMPLATE_FILE, python_module_name + '.py')
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)

################################################################################

def substituteStringsInPyTestFile(pytest_path, module_name, python_module_name):
    """
    Substitute variables in the python test template file and rename it
    """
    template_file = os.path.join(pytest_path, PYTEST_TEMPLATE_FILE)
    os.rename(os.path.join(pytest_path, PYTEST_TEMPLATE_FILE_IN), template_file)

    # Substitute strings in template_file
    f = open(template_file, 'r')
    data = f.read()
    author_str = epcr.getAuthor()
    date_str = time.strftime("%x")
    file_name_str = os.path.join('tests', 'python', python_module_name + '_test.py')
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "MODULENAME": module_name,
                       "PYTHONMODULE": python_module_name}

    f.close()
    # Save new data
    file_name = template_file.replace(PYTEST_TEMPLATE_FILE, python_module_name + '_test.py')
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)

################################################################################

def createPythonModule(current_dir, module_name, python_module_name):
    """
    Create the python module
    """
    createDirectories(current_dir, module_name)
    epcr.createPythonInitFile(os.path.join(current_dir, 'python', module_name, '__init__.py'))
    pytest_path = os.path.join(current_dir, 'tests', 'python')
    epcr.copyAuxFile(pytest_path, PYTEST_TEMPLATE_FILE_IN)
    pymodule_path = os.path.join(current_dir, 'python', module_name)
    script_goes_on = epcr.copyAuxFile(pymodule_path, PYMODULE_TEMPLATE_FILE_IN)

    if script_goes_on:
        substituteStringsInPyModuleFile(pymodule_path, module_name, python_module_name)
        substituteStringsInPyTestFile(pytest_path, module_name, python_module_name)
        updateCmakeListsFile(current_dir)

    return script_goes_on

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

    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the AddPythonModule script')
    logger.info('#')

    python_module_name = args.module_name

    # Default is the current directory
    current_dir = os.getcwd()

    logger.info('Current directory : %s', current_dir)
    logger.info('')

    # We absolutely need a Elements cmake file
    script_goes_on, module_name = epcr.isElementsModuleExist(current_dir)

    # Module as no version number, '1.0' is just for using the routine
    if script_goes_on:
        script_goes_on = epcr.isNameAndVersionValid(python_module_name, '1.0')

    module_file_path = os.path.join(current_dir, 'python', module_name,
                                    python_module_name + '.py')

    # Make sure the program does not already exist
    if script_goes_on:
        script_goes_on = epcr.isFileAlreadyExist(module_file_path, python_module_name)

    # Check aux file exist
    if script_goes_on:
        script_goes_on = epcr.isAuxFileExist(PYTEST_TEMPLATE_FILE_IN)

    if script_goes_on:
        if os.path.exists(current_dir):
            if createPythonModule(current_dir, module_name, python_module_name):
                logger.info('< %s > python module successfully created in < %s >',
                            python_module_name, module_file_path)
                # Remove backup file
                epcr.deleteFile(os.path.join(current_dir, CMAKE_LISTS_FILE) + '~')
                logger.info('Script over.')
        else:
            logger.error('< %s > project directory does not exist!', current_dir)
    if not script_goes_on:
        logger.error('Script aborted')
