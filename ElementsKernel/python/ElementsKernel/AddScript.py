"""
@file: ElementsKernel/AddScript.py
@author: Hubert Degaudenzi

@date: 03/08/17

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
import ElementsKernel.Logging as log

logger = log.getLogger('AddScript')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
PROGRAM_TEMPLATE_FILE = 'Script_template'
PROGRAM_TEMPLATE_FILE_IN = 'Script_template.in'

################################################################################

def createDirectories(module_dir, module_name):
    """
    Create directories needed for a python program
    """

    # Create the scripts directory
    scripts_path = os.path.join(module_dir, 'scripts')
    epcr.makeDirectory(scripts_path)

################################################################################

def subStringsInScriptFile(file_path, program_name, module_name):
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
    file_name_str = os.path.join('scripts', program_name)
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "PROGRAMNAME": program_name}

    f.close()

    # Save new data
    file_name = template_file.replace(PROGRAM_TEMPLATE_FILE, program_name)
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)
    epcr.addItemToCreationList(file_name)

################################################################################

def updateCmakeListsFile(module_dir, program_name):
    """
    Update the <CMakeList.txt> file
    """
    logger.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)
    epcr.addItemToCreationList(cmake_filename)

    # Backup the file
    epcr.makeACopy(cmake_filename)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        f = open(cmake_filename, 'r')
        data = f.read()
        f.close()
        cmake_object = pcl.CMakeLists(data)

        cmake_object.elements_install_scripts = 'elements_install_scripts()'

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################

def createScript(current_dir, module_name, program_name):
    """
    Create the python program
    """
    createDirectories(current_dir, module_name)
    program_path = os.path.join(current_dir, 'scripts')
    epcr.copyAuxFile(program_path, PROGRAM_TEMPLATE_FILE_IN)
    subStringsInScriptFile(program_path, program_name, module_name)
    updateCmakeListsFile(current_dir, program_name)

################################################################################

def makeChecks(program_file_path, program_name, answer_yes=False):
    """
    Make some checks
    """
    # Module as no version number, '1.0' is just for using the routine
    epcr.checkNameAndVersionValid(program_name, '1.0')
    # Make sure the program does not already exist
    epcr.checkFileNotExist(program_file_path, program_name)
    # Check aux file exist
    epcr.checkAuxFileExist(PROGRAM_TEMPLATE_FILE_IN)
    # Check name in DB
    epcr.checkNameInEuclidNamingDatabase(program_name, nc.TYPES[2], answer_yes)

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
    parser.add_argument('program_name', metavar='program-name', type=str, help='Program name')
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
    logger.info('#  Logging from the mainMethod() of the AddScript script')
    logger.info('#')

    program_name = args.program_name
    answer_yes = args.yes

    # Default is the current directory
    current_dir = os.getcwd()

    logger.info('# Current directory : %s', current_dir)
    logger.info('')

    try:
        # We absolutely need a Elements cmake file
        module_name = epcr.getElementsModuleName(current_dir)
        # Check name in the Element Naming Database
        program_file_path = os.path.join(current_dir, 'scripts', program_name)
        # Make checks
        makeChecks(program_file_path, program_name, answer_yes)

        createScript(current_dir, module_name, program_name)
        logger.info('< %s > program successfully created in < %s >.', program_name, program_file_path)

        # Remove backup file
        epcr.deleteFile(os.path.join(current_dir, CMAKE_LISTS_FILE) + '~')

        # Print all files created
        epcr.printCreationList()

    except epcr.ErrorOccured as msg:
        if str(msg):
            logger.error(msg)
        logger.error('# Script aborted.')
        return 1
    except Exception as msg:
        if str(msg):
            logger.error(msg)
        logger.error('# Script aborted.')
        return 1
    else:
        logger.info('# Script over.')
