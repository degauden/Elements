"""
@file ElementsKernel/Project.py
@author Nicolas Morisset

@date 01/07/15

This script will create a new Elements project

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
import sys
import re
import ELEMENTS_VERSION  # @UnresolvedImport
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.NameCheck as nc
import ElementsKernel.Logging as log

try:
    from builtins import input
except:
    from __builtin__ import input

logger = log.getLogger('CreateElementsProject')

AUX_CMAKE_LIST_IN = "CMakeLists.txt.in"
AUX_CMAKE_FILE_IN = "Makefile.in"
AUX_PROJ_RST_IN = "doc_project.rst.in"
AUX_GITIGNORE_IN = "gitignore_template.in"

################################################################################

def setPath():
    """
    Set the installation path either the current directory or
    the directory set by the <User_area> environment variable
    """
    # Check if User_area environment variable exists
    user_area = os.environ.get('User_area')
    if not user_area is None:
        logger.debug('# <$User_area> environment variable defined to : <%s>', user_area)
        destination_path = user_area
    else:
        destination_path = os.getcwd()

    return destination_path

################################################################################

def checkDependencyProjectValid(str_list):
    """
    Check if the dependency project name and version list is valid
    """
    for i in range(len(str_list)):
        epcr.checkNameAndVersionValid(str_list[i][0], str_list[i][1])

################################################################################

def duplicate_elements(duplicate_list):
    """
    Look for duplicate element in a list
    """
    name_list = []
    not_found_duplicate = True
    for elt in duplicate_list:
        if not elt[0] in name_list:
            name_list.append(elt[0])
        else:
            raise epcr.ErrorOccured("Found twice the following dependency : < %s >" % elt[0])

################################################################################

def getElementsVersion():
    """
    Get the Elements version number
    """

    elt_version = ELEMENTS_VERSION.ELEMENTS_ORIGINAL_VERSION

    logger.info('# Elements version found : <%s>', elt_version)

    return str(elt_version)

################################################################################

def substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects, standalone=False):
    """
    Substitute variables in <CMakeList.txt.in> file and rename the file to
    <CMakeLists.txt>.
    """
    logger.debug('# Substitute variables in <%s> file', AUX_CMAKE_LIST_IN)
    cmake_list_file = os.path.join(project_dir, AUX_CMAKE_LIST_IN)

    # Substitute
    f = open(cmake_list_file, 'r')
    data = f.read()

    # Format all dependent projects
    # We put by default Elements dependency if no one is given
    if standalone:
        str_dep_projects = ""
    else:
        str_dep_projects = 'Elements ' + getElementsVersion()

    if not dep_projects is None:
        for dep in dep_projects:
            if not dep[0] in str_dep_projects:
                str_dep_projects += ' ' + dep[0] + ' ' + dep[1]
            else:
                logger.warning('<%s> dependency already exists. It is skipped!', dep[0])

    if str_dep_projects:
        str_dep_projects = "USE " + str_dep_projects


    new_data = data % {"PROJECT_NAME": proj_name,
                       "PROJECT_VERSION": proj_version,
                       "DEPENDANCE_LIST": str_dep_projects}

    f.close()

    # Save new data
    f = open(cmake_list_file, 'w')
    f.write(new_data)
    f.close()
    # Remove '.in'
    file_no_dot_in = cmake_list_file.replace('.in', '')
    os.rename(cmake_list_file, file_no_dot_in)
    epcr.addItemToCreationList(file_no_dot_in)

################################################################################

def createProject(project_dir, proj_name, proj_version, dep_projects, standalone=False):
    """
    Create the project structure
    """
    logger.info('# Creating the project')

    # Create project
    if not os.path.exists(project_dir):
        os.makedirs(project_dir)

    epcr.copyAuxFile(project_dir, AUX_CMAKE_LIST_IN)
    epcr.copyAuxFile(project_dir, AUX_CMAKE_FILE_IN)
    epcr.copyAuxFile(project_dir, AUX_GITIGNORE_IN)

    # Rename the git file as ".gitignore"
    gitfile = os.path.join(project_dir, AUX_GITIGNORE_IN)
    git_ignore_file_dot_name = os.path.join(project_dir, ('.' + AUX_GITIGNORE_IN.replace('_template.in', '')))
    os.rename(gitfile, git_ignore_file_dot_name)
    epcr.addItemToCreationList(git_ignore_file_dot_name)

    # Remove '.in'
    cmakefile = os.path.join(project_dir, AUX_CMAKE_FILE_IN)
    file_no_dot_in = cmakefile.replace('.in', '')
    os.rename(cmakefile, file_no_dot_in)
    epcr.addItemToCreationList(file_no_dot_in)

    project_doc_dir = os.path.join(project_dir, "doc")
    if not os.path.exists(project_doc_dir):
        os.makedirs(project_doc_dir)
    epcr.copyAuxFile(project_doc_dir, AUX_PROJ_RST_IN)
    project_rst_file = os.path.join(project_doc_dir, AUX_PROJ_RST_IN)

    file_no_dot_in = project_rst_file.replace('.in', '')
    os.rename(project_rst_file, file_no_dot_in)
    epcr.addItemToCreationList(file_no_dot_in)

    substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects, standalone)

################################################################################

def makeChecks(proj_name, proj_version, dependency, dependant_projects, answer_yes=False):
    """
    Make some checks
    """
    # Check project name and version
    epcr.checkNameAndVersionValid(proj_name, proj_version)
    if not dependency is None:
        checkDependencyProjectValid(dependant_projects)
    # Check for duplicate dependencies
    if not dependency is None:
        duplicate_elements(dependant_projects)
    # Check AUX files exist
    epcr.checkAuxFileExist(AUX_CMAKE_LIST_IN)
    epcr.checkAuxFileExist(AUX_CMAKE_FILE_IN)
    # Check name in the Element Naming Database
    epcr.checkNameInEuclidNamingDatabase(proj_name, nc.TYPES[0], answer_yes)


################################################################################

def buildProjectDir(no_version_directory, destination_path, proj_name, proj_version):
    """
    Build project directory path
    """
    if no_version_directory:
        project_dir = os.path.join(destination_path, proj_name)
    else:
        project_dir = os.path.join(destination_path, proj_name, proj_version)
    return project_dir

################################################################################

def lookForDirectories(project_dir):
    """
    Look for any version directory in the project directory e.g HEAD, 1.2.3, 1.0 etc...
    """
    match_list = []
    dirlist = [elt for elt in os.listdir(project_dir) if os.path.isdir(os.path.join(project_dir, elt)) ]
    for elt in dirlist:
        match = re.match(epcr.version_regex, elt)
        if match:
            match_list.append(match.group(0))
    return match_list

################################################################################

def checkProjectExist(project_dir, no_version_directory, force_erase, answer_yes=False):
    """
    Look for any version directory in the project directory e.g 1.0,1.2 etc...
    """
    if os.path.exists(project_dir) and not force_erase:
        logger.warning('<%s> Project ALREADY exists!!!', project_dir)
        version_dir_list = lookForDirectories(project_dir)
        # Warn user about directory under the project
        if no_version_directory and version_dir_list:
            logger.warning('Found the following version(s) directory(ies) : %s', version_dir_list)
        if not answer_yes:
            response_key = input('Do you want to overwrite the existing project (y/n, default: n)?')
        if answer_yes or response_key.lower() == "yes" or response_key == "y":
            logger.info('# Overwriting the existing project: <%s>', project_dir)
        else:
            raise epcr.ErrorOccured
    elif force_erase:
        epcr.eraseDirectory(project_dir)