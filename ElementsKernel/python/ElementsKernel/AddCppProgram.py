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

""" This script creates a new Elements C++ Program

:file: ElementsKernel/AddCppProgram.py
:author: Nicolas Morisset

:date: 01/07/15


"""

import argparse
import os
import time
from ElementsKernel import Auxiliary
from ElementsKernel import ProjectCommonRoutines
from ElementsKernel import ParseCmakeListsMacros
from ElementsKernel import Logging

from ElementsKernel import Exit

LOGGER = Logging.getLogger(__name__)

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
PROGRAM_TEMPLATE_FILE = 'Program_template.cpp'
PROGRAM_TEMPLATE_FILE_IN = 'Program_template.cpp.in'


def createDirectories(module_dir):
    """
    Create directories needed for a program
    """

    standalone_directories = [os.path.join('src', 'program'),
                              'conf']
    for d in standalone_directories:
        target_dir = os.path.join(module_dir, d)
        if not os.path.exists(target_dir):
            os.makedirs(target_dir)

################################################################################


def addConfFile(module_dir, program_name):
    """
    Create the configuration file by default
    """
    conf_file = os.path.join(module_dir, 'conf', program_name + '.conf')
    # check file does not exist
    if not os.path.exists(conf_file):
        f = open(conf_file, 'w')
        f.write('###############################################################################\n')
        f.write('#\n')
        f.write('# Configuration file for the <' + program_name + '> executable \n')
        f.write('#\n')
        f.write('###############################################################################\n')
        f.close()
        ProjectCommonRoutines.addItemToCreationList(conf_file)
    else:
        LOGGER.warning('The < %s > conf file has been kept as it already exists!', conf_file)
        LOGGER.warning('The < %s > conf file already exists!', conf_file)

################################################################################


def substituteAuxFiles(module_dir, program_name):
    """
    Copy AUX file(s) and substitutes keyworks
    """
    target_location = os.path.join('src', 'program', program_name + '.cpp')
    configuration = {  "FILE": target_location,
                       "DATE": time.strftime("%x"),
                       "AUTHOR": ProjectCommonRoutines.getAuthor(),
                       "PROGRAMNAME": program_name
                    }

    Auxiliary.configure(os.path.join("ElementsKernel", "templates", PROGRAM_TEMPLATE_FILE_IN),
                        module_dir, target_location,
                        configuration=configuration,
                        create_missing_dir=True)
    ProjectCommonRoutines.addItemToCreationList(os.path.join(module_dir, target_location))

################################################################################


def updateCmakeListsFile(module_dir, module_name, program_name,
                         module_dep_list, library_dep_list):
    """
    Update CMakeLists.txt file
    """
    LOGGER.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)
    ProjectCommonRoutines.addItemToCreationList(cmake_filename)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        cmake_object, module_name = ProjectCommonRoutines.updateCmakeCommonPart(cmake_filename, library_dep_list)

        # Update ElementsDependsOnSubdirs macro
        if module_dep_list:
            for mod_dep in module_dep_list:
                dep_object = ParseCmakeListsMacros.ElementsDependsOnSubdirs([mod_dep])
                cmake_object.elements_depends_on_subdirs_list.append(dep_object)

        # Add elements_install_conf_files if any
        cmake_object.elements_install_conf_files = 'elements_install_conf_files()'

        # Update elements_add_executable macro
        source = os.path.join('src', 'program', program_name + '.cpp')
        existing_exe = [x for x in cmake_object.elements_add_executable_list if x.name == program_name]
        existing_add_lib = [x for x in cmake_object.elements_add_library_list if x.name == module_name]
        link_libs = ['ElementsKernel']
        include_dirs = ['ElementsKernel']
        if module_dep_list:
            link_libs = link_libs + module_dep_list
            include_dirs = include_dirs + module_dep_list
        if existing_add_lib:
            link_libs += [module_name]
            include_dirs += [module_name]
        if library_dep_list:
            link_libs = link_libs + library_dep_list
            include_dirs = include_dirs + library_dep_list
        if existing_exe:
            for lib in link_libs:
                if not lib in existing_exe[0].link_libraries_list:
                    existing_exe[0].link_libraries_list.append(lib)
            for incd in include_dirs:
                if not incd in existing_exe[0].include_dirs_list:
                    existing_exe[0].include_dirs_list.append(incd)
        else:
            exe_object = ParseCmakeListsMacros.ElementsAddExecutable(program_name, source,
                                                   link_libs, include_dirs)
            cmake_object.elements_add_executable_list.append(exe_object)

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################


def createCppProgram(module_dir, module_name, program_name, module_dep_list, library_dep_list):
    """
    Creates all necessary files for a program
    """
    createDirectories(module_dir)
    substituteAuxFiles(module_dir, program_name)
    addConfFile(module_dir, program_name)
    updateCmakeListsFile(module_dir, module_name, program_name,
                         module_dep_list, library_dep_list)

################################################################################


def makeChecks(current_dir, program_name):
    """
    Make some checks
    """
    # Check if file exits
    program_file_path = os.path.join(current_dir, 'src', 'program', program_name + '.cpp')
    ProjectCommonRoutines.checkFileNotExist(program_file_path, program_name)
    ProjectCommonRoutines.checkNameAndVersionValid(program_name, '1.0')
    ProjectCommonRoutines.checkAuxFileExist(PROGRAM_TEMPLATE_FILE_IN)

################################################################################


def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
This script creates an <Elements> C++ program at your current directory(default)
All necessary structure (directory structure, makefiles etc...) will be automat-
ically created for you if any but you have to be inside an <Elements> module.
           """
    from argparse import RawTextHelpFormatter

    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=RawTextHelpFormatter)

    parser.add_argument('program_name', metavar='program-name',
                        type=str,
                        help='Program name')
    parser.add_argument('-md', '--module-dependency', metavar='module_name',
                        action='append', type=str,
                        help='Dependency module name e.g."-md ElementsKernel"')
    parser.add_argument('-ld', '--library-dependency', metavar='library_name',
                        action='append', type=str,
                        help='Dependency library name e.g."-ld ElementsKernel"')
    parser.add_argument('-y', '--yes', default=False, action="store_true",
                        help='Answer <yes> by default to any question, useful when the script is called by another'\
                         'script')

    return parser

################################################################################


def mainMethod(args):
    """
    Main
    """

    LOGGER.info('#')
    LOGGER.info('#  Logging from the mainMethod() of the AddCppProgram script')
    LOGGER.info('#')

    exit_code = Exit.Code["OK"]

    program_name = args.program_name
    module_list = args.module_dependency
    library_list = args.library_dependency

    try:
        # Default is the current directory
        current_dir = os.getcwd()

        LOGGER.info('Current directory : %s', current_dir)
        LOGGER.info('')
        # We absolutely need a Elements cmake file
        module_name = ProjectCommonRoutines.getElementsModuleName(current_dir)
        # make some checks
        makeChecks(current_dir, program_name)
        # Create CPP program
        createCppProgram(current_dir, module_name, program_name, module_list, library_list)

        location = os.path.join(current_dir, 'src', 'program')
        LOGGER.info('< %s > program successfully created in < %s >.', program_name, location)

        # Remove backup file
        ProjectCommonRoutines.deleteFile(os.path.join(current_dir, CMAKE_LISTS_FILE) + '~')

        # Print all files created
        ProjectCommonRoutines.printCreationList()

    except Exception as msg:
        if str(msg):
            LOGGER.error(msg)
        LOGGER.error('# Script aborted.')
        exit_code = Exit.Code["NOT_OK"]
    else:
        LOGGER.info('# Script over.')

    return exit_code
