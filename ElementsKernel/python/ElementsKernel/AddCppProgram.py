"""
@file ElementsKernel/AddCppProgram.py
@author Nicolas Morisset

@date 01/07/15

This script creates a new Elements C++ Program

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
import ElementsKernel.ParseCmakeListsMacros as pclm
import ElementsKernel.NameCheck as nc
import ElementsKernel.Logging as log

logger = log.getLogger('AddCppProgram')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
PROGRAM_TEMPLATE_FILE = 'Program_template.cpp'
PROGRAM_TEMPLATE_FILE_IN = 'Program_template.cpp.in'

def createDirectories(module_dir, module_name):
    """
    Create directories needed for a program
    """
    # Create the executable directory
    program_path = os.path.join(module_dir, 'src', 'program')
    epcr.makeDirectory(program_path)
    # Create the conf directory
    conf_dir = os.path.join(module_dir, 'conf', module_name)
    epcr.makeDirectory(conf_dir)


################################################################################

def addConfFile(module_dir, module_name, program_name):
    """
    Create the configuration file by default
    """
    conf_file = os.path.join(module_dir, 'conf', module_name, program_name + '.conf')
    # check file does not exist
    if not os.path.exists(conf_file):
        f = open(conf_file, 'w')
        f.write('###############################################################################\n')
        f.write('#\n')
        f.write('# Configuration file for the <' + program_name + '> executable \n')
        f.write('#\n')
        f.write('###############################################################################\n')
        f.close()
        epcr.addItemToElementsCreationList(conf_file)
    else:
        logger.warning('The < %s > conf file has been kept as it already exists!', conf_file)
        logger.warning('The < %s > conf file already exists!', conf_file)

################################################################################

def substituteStringsInProgramFile(file_path, program_name):
    """
    Substitute variables in template file and rename the file
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
    file_name_str = os.path.join('src', 'program', program_name + '.cpp')
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "PROGRAMNAME": program_name}

    f.close()
    # Save new data
    file_name = template_file.replace(PROGRAM_TEMPLATE_FILE, program_name)
    file_name += '.cpp'
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)
    epcr.addItemToElementsCreationList(file_name)

################################################################################

def updateCmakeListsFile(module_dir, module_name, program_name,
                         module_dep_list, library_dep_list):
    """
    Update CMakeLists.txt file
    """
    logger.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)
    epcr.addItemToElementsCreationList(cmake_filename)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        cmake_object, module_name = epcr.updateCmakeCommonPart(cmake_filename, library_dep_list)

        # Update ElementsDependsOnSubdirs macro
        if module_dep_list:
            for mod_dep in module_dep_list:
                dep_object = pclm.ElementsDependsOnSubdirs([mod_dep])
                cmake_object.elements_depends_on_subdirs_list.append(dep_object)

        # Add elements_install_conf_files if any
        cmake_object.elements_install_conf_files = 'elements_install_conf_files()'

        # Update elements_add_executable macro
        source = 'src' + os.sep + 'program' + os.sep + program_name + '.cpp'
        existing_exe = [x for x in cmake_object.elements_add_executable_list if x.name == program_name]
        existing_add_lib = [x for x in cmake_object.elements_add_library_list if x.name == module_name]
        link_libs = ['ElementsKernel']
        if module_dep_list:
            link_libs = link_libs + module_dep_list
        if existing_add_lib:
            link_libs += [module_name]
        if library_dep_list:
            link_libs = link_libs + library_dep_list
        if existing_exe:
            for lib in link_libs:
                if not lib in existing_exe[0].link_libraries_list:
                    existing_exe[0].link_libraries_list.append(lib)
        else:
            exe_object = pclm.ElementsAddExecutable(program_name, source,
                                                   link_libs)
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
    createDirectories(module_dir, module_name)
    program_path = os.path.join(module_dir, 'src', 'program')
    epcr.copyAuxFile(program_path, PROGRAM_TEMPLATE_FILE_IN)
    substituteStringsInProgramFile(program_path, program_name)
    addConfFile(module_dir, module_name, program_name)
    updateCmakeListsFile(module_dir, module_name, program_name,
                         module_dep_list, library_dep_list)

################################################################################

def makeChecks(current_dir, program_name):
    """
    Make some checks
    """
    # Check name in the Element Naming Database
    epcr.checkNameInEuclidNamingDatabase(program_name, nc.TYPES[2])
    # Check if file exits
    program_file_path = os.path.join(current_dir, 'src', 'program', program_name + '.cpp')
    epcr.checkFileNotExist(program_file_path, program_name)
    # Check program name is valid
    epcr.checkNameAndVersionValid(program_name, '1.0')
    # Check aux file exist
    epcr.checkAuxFileExist(PROGRAM_TEMPLATE_FILE_IN)


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

    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the AddCppProgram script')
    logger.info('#')

    program_name = args.program_name
    module_list = args.module_dependency
    library_list = args.library_dependency

    try:
        # Default is the current directory
        current_dir = os.getcwd()

        logger.info('Current directory : %s', current_dir)
        logger.info('')
        # We absolutely need a Elements cmake file
        module_name = epcr.getElementsModuleName(current_dir)
        # make some checks
        makeChecks(current_dir, program_name)

        # Create CPP program
        createCppProgram(current_dir, module_name, program_name, module_list, library_list)

        location = current_dir + os.sep + 'src' + os.sep + 'program'
        logger.info('< %s > program successfully created in < %s >.', program_name, location)

        # Remove backup file
        epcr.deleteFile(os.path.join(current_dir, CMAKE_LISTS_FILE) + '~')

        # Print all files created
        epcr.printElementsCreationList()

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
