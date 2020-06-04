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

:file: ElementsKernel/AddScript.py
:author: Hubert Degaudenzi

:date: 03/08/17

"""

import argparse
import os
import time
import stat
from ElementsKernel import Auxiliary
from ElementsKernel import ProjectCommonRoutines
from ElementsKernel import ParseCmakeLists
from ElementsKernel import Logging

from ElementsKernel import Exit

logger = Logging.getLogger('AddScript')

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
    ProjectCommonRoutines.makeDirectory(scripts_path)

################################################################################
def substituteAuxFiles(module_dir, program_name, module_name):
    """
    Copy AUX file(s) and substitutes keyworks
    """
    configuration = {  "FILE": os.path.join('scripts', program_name),
                       "DATE": time.strftime("%x"),
                       "AUTHOR": ProjectCommonRoutines.getAuthor(),
                    }
    # Put AUX files to their target and substitute
    tgt = os.path.join('scripts', program_name)
    Auxiliary.configure(os.path.join("ElementsKernel", "templates", PROGRAM_TEMPLATE_FILE_IN),
                        module_dir, tgt,
                        configuration=configuration,
                        create_missing_dir=True)
    
    full_tgt = os.path.join(module_dir, tgt)
    
    # Add the execution flag
    tgt_stat = os.stat(full_tgt)
    os.chmod(full_tgt, tgt_stat.st_mode | stat.S_IEXEC)
    
    ProjectCommonRoutines.addItemToCreationList(full_tgt)

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
    substituteAuxFiles(current_dir, program_name, module_name)
    updateCmakeListsFile(current_dir, program_name)

################################################################################

def makeChecks(program_file_path, program_name):
    """
    Make some checks
    """
    # Module as no version number, '1.0' is just for using the routine
    ProjectCommonRoutines.checkNameAndVersionValid(program_name, '1.0')
    ProjectCommonRoutines.checkFileNotExist(program_file_path, program_name)
    ProjectCommonRoutines.checkAuxFileExist(PROGRAM_TEMPLATE_FILE_IN)

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

    exit_code = Exit.Code["OK"]

    program_name = args.program_name

    # Default is the current directory
    current_dir = os.getcwd()

    logger.info('# Current directory : %s', current_dir)
    logger.info('')

    try:
        # We absolutely need a Elements cmake file
        module_name = ProjectCommonRoutines.getElementsModuleName(current_dir)
        # Check name in the Element Naming Database
        program_file_path = os.path.join(current_dir, 'scripts', program_name)
        # Make checks
        makeChecks(program_file_path, program_name)

        createScript(current_dir, module_name, program_name)
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