"""
@file: ElementsKernel/AddPythonProgram.py
@author: Nicolas Morisset

@date: 01/07/15

This script creates a new Elements module

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

import argparse
import os
import time
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.NameCheck as nc
import ElementsKernel.ParseCmakeLists as pcl
import ElementsKernel.ParseCmakeListsMacros as pclm
import ElementsKernel.Logging as log

logger = log.getLogger('AddPythonProgram')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
PROGRAM_TEMPLATE_FILE = 'PythonProgram_template.py'
PROGRAM_TEMPLATE_FILE_IN = 'PythonProgram_template.py.in'

################################################################################

def createDirectories(module_dir, module_name):
    """
    Create directories needed for a python program
    """
    # Create the executable directory
    program_path = os.path.join(module_dir, 'python', module_name)
    epcr.makeDirectory(program_path)
    # Create the conf directory
    conf_dir = os.path.join(module_dir, 'conf', module_name)
    epcr.makeDirectory(conf_dir)

################################################################################

def createFiles(module_dir, module_name, program_name):
    """
    Create files needed for a python program
    """
    # Create the executable directory
    epcr.createPythonInitFile(os.path.join(module_dir, 'python', module_name, '__init__.py'))

    conf_file = os.path.join(module_dir, 'conf', module_name, program_name + '.conf')
    if not os.path.exists(conf_file):
        f = open(conf_file, 'w')
        f.write('# Write your program options here. e.g. : option = string')
        f.close()

################################################################################

def subStringsInPythonProgramFile(file_path, program_name, module_name):
    """
    Substitute variables in the python template file and rename it
    """
    template_file = os.path.join(file_path, PROGRAM_TEMPLATE_FILE)
    os.rename(os.path.join(file_path, PROGRAM_TEMPLATE_FILE_IN), template_file)

    # Substitute strings in h_template_file
    f = open(template_file, 'r')
    data = f.read()
    # Format all dependent projects
    # We put by default Elements dependency if no one is given
    date_str = time.strftime("%x")
    author_str = epcr.getAuthor()
    # Make some substitutions
    file_name_str = os.path.join('python', module_name, program_name + '.py')
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "PROGRAMNAME": program_name}

    f.close()

    # Save new data
    file_name = template_file.replace(PROGRAM_TEMPLATE_FILE, program_name)
    file_name += '.py'
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)

################################################################################

def updateCmakeListsFile(module_dir, program_name):
    """
    Update the <CMakeList.txt> file
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
        module_name = cmake_object.elements_subdir_list[0].name + '.' + program_name

        # Add elements_install_conf_files if any
        cmake_object.elements_install_python_modules = 'elements_install_python_modules()'
        cmake_object.elements_install_conf_files = 'elements_install_conf_files()'

        program_object = pclm.ElementsAddPythonExecutable(program_name, module_name)
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
    program_path = os.path.join(current_dir, 'python', module_name)
    epcr.copyAuxFile(program_path, PROGRAM_TEMPLATE_FILE_IN)
    subStringsInPythonProgramFile(program_path, program_name, module_name)
    updateCmakeListsFile(current_dir, program_name)

################################################################################

def makeChecks(program_file_path, program_name):
    """
    Make some checks
    """
    # Module as no version number, '1.0' is just for using the routine
    epcr.checkNameAndVersionValid(program_name, '1.0')
    # Check aux file exist
    epcr.checkAuxFileExist(PROGRAM_TEMPLATE_FILE_IN)
    # Make sure the program does not already exist
    epcr.checkFileNotExist(program_file_path, program_name)
    # Check name in the Element Naming Database
    epcr.checkNameInEuclidNamingDatabase(program_name, nc.TYPES[2])

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

    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the AddPythonProgram script')
    logger.info('#')

    program_name = args.program_name

    try:
        # Default is the current directory
        current_dir = os.getcwd()

        logger.info('# Current directory : %s', current_dir)
        logger.info('')

        # We absolutely need a Elements cmake file
        module_name = epcr.checkElementsModuleNotExist(current_dir)

        program_file_path = os.path.join(current_dir, 'python', module_name, program_name + '.py')
        # Make checks
        makeChecks(program_file_path, program_name)

        # Create program
        createPythonProgram(current_dir, module_name, program_name)

        logger.info('< %s > program successfully created in < %s >.', program_name, program_file_path)
        # Remove backup file
        epcr.deleteFile(os.path.join(current_dir, CMAKE_LISTS_FILE) + '~')

    except:
        logger.info('# Script aborted.')
        return 1
    else:
        logger.info('# Script over.')
