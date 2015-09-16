##
# @file: ElementsKernel/ElementsProject.py
# @author: Nicolas Morisset
#          Astronomy Department of the University of Geneva
#
# @date: 01/07/15
#
# This script will create a new Elements project
##

import argparse
import os
import re
import shutil
import ELEMENTS_VERSION  # @UnresolvedImport
import ElementsKernel.ElementsProjectCommonRoutines as epcr
import ElementsKernel.Logging as log

logger = log.getLogger('CreateElementsProject')

AUX_CMAKE_LIST_IN = "CMakeLists.txt.in"
AUX_CMAKE_FILE = "Makefile"

################################################################################

def setPath():
    """
    Set the installation path
    """
    # Check if User_area environment variable exists 
    user_area = os.environ.get('User_area')
    if not user_area is None:
        logger.info('# User_area environment variable defined to : <%s>' % user_area)
        destination_path = user_area         
    else:
        destination_path = os.getcwd()

    return destination_path

################################################################################

def isDependencyProjectValid(str_list):
    """
    Check if the dependency project name and version list is valid
    """
    valid = True
    for i in range(len(str_list)):
        if not epcr.isNameAndVersionValid(str_list[i][0], str_list[i][1]):
            valid = False
            break

    return valid

################################################################################

def duplicate_elements(duplicate_list):
    """
    """
    name_list = []
    not_found_duplicate = True
    for elt in duplicate_list:
        if not elt[0] in name_list:
            name_list.append(elt[0])
        else:
            logger.error(
                '# Found twice the following dependency : <%s>, script aborted' % elt[0])
            not_found_duplicate = False
            break

    return not_found_duplicate

################################################################################

def getElementsVersion():
    """
    Get the elements version
    """
    patch_version = ELEMENTS_VERSION.ELEMENTS_PATCH_VERSION
    version = [str(ELEMENTS_VERSION.ELEMENTS_MAJOR_VERSION),
               str(ELEMENTS_VERSION.ELEMENTS_MINOR_VERSION)]
    version_patch = [str(ELEMENTS_VERSION.ELEMENTS_MAJOR_VERSION),
                     str(ELEMENTS_VERSION.ELEMENTS_MINOR_VERSION),
                     str(patch_version)]

    elt_version = '.'.join(version)
    if patch_version > 0:
        elt_version = '.'.join(version_patch)

    logger.info('# Elements version found : <%s>' % elt_version)

    return str(elt_version)

################################################################################

def copyAuxFile(destination, file_name):
    """
    Copy all necessary auxiliary data to the <destination> directory
    """
    scripts_goes_on = True

    aux_path_file = epcr.getAuxPathFile(file_name)
    if aux_path_file:
        logger.info('# Copying AUX file : %s' % file_name)
        shutil.copy(
            aux_path_file, os.path.join(os.path.sep, destination, file_name))
    else:
        scripts_goes_on = False

    return scripts_goes_on

################################################################################

def substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects):
    """
    Substitute variables in CMakeList.txt.in file and rename the file to
    CMakeList.txt.
    """
    logger.info('# substitute variables in <%s> file' % AUX_CMAKE_LIST_IN)
    cmake_list_file = os.path.join(os.path.sep, project_dir, AUX_CMAKE_LIST_IN)

    # Substitute
    f = open(cmake_list_file, 'r')
    data = f.read()
    # Format all dependent projects
    # We put by default Elements dependency if no one is given
    str_dep_projects = ''
    if not dep_projects is None:
        str_dep_projects = ' '.join(' '.join(s) for s in dep_projects)
        new_data = data % {"PROJECT_NAME": proj_name,
                           "PROJECT_VERSION": proj_version,
                           "DEPENDANCE_LIST": str_dep_projects}
    else:
        # Add an comment example with the USE keyword
        
        new_data = data % {"PROJECT_NAME": proj_name,
                           "PROJECT_VERSION": proj_version,
                           "DEPENDANCE_LIST": str_dep_projects}
        # Remove <USE> keyword
        str_to_look_for = proj_name + ' ' + proj_version + ' USE '
        str_to_be_replaced = proj_name + ' ' + proj_version
        new_data = new_data.replace(str_to_look_for, str_to_be_replaced)

    f.close()
    # Save new data
    f = open(cmake_list_file, 'w')
    f.write(new_data)
    f.close()
    os.rename(cmake_list_file, cmake_list_file.replace('.in', ''))

################################################################################

def createProject(project_dir, proj_name, proj_version, dep_projects):
    """
    Create the project structure
    """
    logger.info('# Creating the project')

    # Create project
    os.makedirs(project_dir)

    copyAuxFile(project_dir, AUX_CMAKE_LIST_IN)
    copyAuxFile(project_dir, AUX_CMAKE_FILE)

    substituteProjectVariables(
        project_dir, proj_name, proj_version, dep_projects)

################################################################################

def defineSpecificProgramOptions():
    description = """
This script creates an <Elements> project in your current directory
by default or at the location defined by the <$User_area> environment variable.
It means all the necessary structure (directory
structure, makefiles etc...) will be automatically created for you.
Use the [-d] option if your project has some dependencies to other
project(s). 
            """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument(
        'project_name', metavar='project-name', type=str, help='Project name')
    parser.add_argument('project_version', metavar='project-version',
                        type=str, default='1.0', help='Project version number')
    parser.add_argument('-d', '--dependency', metavar=('project_name','version'), nargs=2, action='append', type=str,
                        help='Dependency project name and its version number e.g "project_name 0.1"')
    return parser

################################################################################

def mainMethod(args):

    logger.info('#')
    logger.info(
        '#  Logging from the mainMethod() of the CreateElementsProject script ')
    logger.info('#')

    try:
        script_goes_on = True
        proj_name = args.project_name
        proj_version = args.project_version
        dependant_projects = args.dependency
        destination_path = setPath()
        logger.info('# Installation directory : %s' % destination_path)
        
        # Check project name and version
        script_goes_on = epcr.isNameAndVersionValid(proj_name, proj_version)

        # Check for duplicate dependencies
        if script_goes_on and not args.dependency is None:
            script_goes_on = duplicate_elements(dependant_projects)

        # Check aux files exist
        if script_goes_on:
            script_goes_on = epcr.isAuxFileExist(AUX_CMAKE_LIST_IN)
        if script_goes_on:
            script_goes_on = epcr.isAuxFileExist(AUX_CMAKE_FILE)

        # Set the project directory
        project_dir = os.path.join(
            os.path.sep, destination_path, proj_name, proj_version)

        # Make sure dependencies name and version are valid
        if script_goes_on and not args.dependency is None:
            script_goes_on = isDependencyProjectValid(dependant_projects)

        if script_goes_on and os.path.exists(project_dir):
            logger.warning('<%s> project ALREADY exists!!!' % project_dir)
            response_key = raw_input(
                'Do you want to replace the existing project and associated module(s) (Yes/No, default: No)?')
            if response_key == "YES" or response_key == "yes" or response_key == "y":
                logger.info(
                    '# Replacing the existing project: <%s>' % project_dir)
                epcr.eraseDirectory(project_dir)
            else:
                script_goes_on = False
                logger.info('# Script stopped by user!')

        if script_goes_on:
            createProject(
                project_dir, proj_name, proj_version, dependant_projects)
            logger.info('# <%s> project successfully created.' % project_dir)
            logger.info('# Script over.')
    except Exception as e:
        logger.exception(e)
        logger.error('# Script aborted...')
