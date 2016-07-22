"""
@file: ElementsKernel/AddModule.py
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
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.ParseCmakeLists as pcl
import ElementsKernel.ParseCmakeListsMacros as pclm
import ElementsKernel.Logging as log

logger = log.getLogger('AddElementsModule')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
CMAKE_LISTS_FILE_IN = 'CMakeLists.txt.mod.in'

################################################################################

def isElementsProjectExist(project_directory):
    """
    Checks if a <CMakeLists.txt> file exists and is really an Elements
    cmake file
    """
    file_exists = True
    cmake_file = os.path.join(project_directory, CMAKE_LISTS_FILE)
    if not os.path.isfile(cmake_file):
        file_exists = False
        logger.error('# <%s> cmake project file is missing! Are you inside a '\
                     'project directory?', cmake_file)
    else:
        # Check the make file is an Elements cmake file
        # it should contain the string : "elements_project"
        f = open(cmake_file, 'r')
        data = f.read()
        if not 'elements_project' in data:
            file_exists = False
            logger.error('# <%s> is not an Elements project cmake file! Can '\
                         'not find the <elements_project> directive', cmake_file)
        f.close()

    return file_exists

################################################################################

def createModuleDirectories(mod_path, module_name):
    """
    Create the directory structure for the module
    """
    # Create module directories
    logger.info('# Creating the module directories')
    os.makedirs(mod_path)
    os.makedirs(os.path.join(mod_path, module_name))
    os.makedirs(os.path.join(mod_path, 'doc'))
    os.makedirs(os.path.join(mod_path, 'conf', module_name))
    os.makedirs(os.path.join(mod_path, 'tests', 'src'))

################################################################################

def createCmakeListFile(module_dir, module_name, module_dep_list):
    """
    Create the <CMakeList.txt> file and add dependencies to it
    """
    logger.info('# Create the <%s> File', CMAKE_LISTS_FILE)
    cmake_list_file_final = os.path.join(module_dir, CMAKE_LISTS_FILE)

    # Copy aux file to destination
    epcr.copyAuxFile(module_dir, CMAKE_LISTS_FILE_IN)
    # Rename it
    file_template = os.path.join(module_dir, CMAKE_LISTS_FILE)
    os.rename(os.path.join(module_dir, CMAKE_LISTS_FILE_IN),
              file_template)

    # Read the template file
    fo = open(file_template, 'r')
    template_data = fo.read()
    fo.close()

    cmake_object = pcl.CMakeLists(template_data)

    # Add elements_subdir macro
    subdir_obj = pclm.ElementsSubdir(module_name)
    cmake_object._elements_subdir_list.append(subdir_obj)

    # Set <ElementsKernel> as a default
    default_dependency = 'ElementsKernel'
    if module_dep_list:
        if not default_dependency in module_dep_list:
            module_dep_list.insert(0, default_dependency)
    else:
        module_dep_list = [default_dependency]

    # Update ElementsDependsOnSubdirs macro
    if module_dep_list:
        for mod_dep in module_dep_list:
            dep_object = pclm.ElementsDependsOnSubdirs([mod_dep])
            cmake_object._elements_depends_on_subdirs_list.append(dep_object)

    # Write new data
    f = open(cmake_list_file_final, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################

def createModule(project_dir, module_name, dependency_list):
    """
    Create a module, copy auxiliary files and substitute variables in the
    CMakefile.txt file
    """
    script_goes_on = True

    # Create module directory
    mod_path = os.path.join(project_dir, module_name)
    logger.info('# Creating the module: <%s> ', mod_path)
    if os.path.exists(mod_path):
        # Ask user
        logger.warning('<%s> module ALREADY exists!!!', module_name)
        response_key = raw_input(
            'Do you want to replace the existing module (yes/y/no/n), default: no)?')
        if response_key == "yes" or response_key == "y":
            logger.info('# Replacing the existing module: <%s>', module_name)
            epcr.eraseDirectory(mod_path)
        else:
            logger.info("Script stopped by user!")
            script_goes_on = False

    if script_goes_on:
        createModuleDirectories(mod_path, module_name)
        createCmakeListFile(mod_path, module_name, dependency_list)

    return script_goes_on

################################################################################

def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
This script creates an <Elements> module at your current directory
(default) but it must be inside a project directory. All necessary structure
(directory structure, makefiles etc...) will be automatically created for you.
           """
    from argparse import RawTextHelpFormatter

    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=RawTextHelpFormatter)

    parser.add_argument('module_name', metavar='module-name',
                        type=str,
                        help='Module name')
    parser.add_argument('-md', '--module-dependency', metavar='module_name',
                        action='append', type=str,
                        help='Dependency module name e.g "-md ElementsKernel"')

    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the AddModule script ')
    logger.info('#')

    module_name = args.module_name
    dependency_list = args.module_dependency

    # Default is the current directory
    project_dir = os.getcwd()

    logger.info('# Current directory : %s', project_dir)

    # We absolutely need a Elements cmake file
    script_goes_on = isElementsProjectExist(project_dir)

    # Module as no version number, '1.0' is just for using the routine
    if script_goes_on:
        script_goes_on = epcr.isNameAndVersionValid(module_name, '1.0')

    if script_goes_on and createModule(project_dir, module_name, dependency_list):
        logger.info('# <%s> module successfully created in <%s>.', module_name, project_dir)
        logger.info('# Script over.')
    else:
        logger.warning('# Script aborted')
