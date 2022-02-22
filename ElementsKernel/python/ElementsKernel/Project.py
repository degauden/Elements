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

""" This script will create a new Elements project

:file: ElementsKernel/Project.py
:author: Nicolas Morisset
:date: 01/07/15
"""

import os
import re

import ELEMENTS_VERSION  # @UnresolvedImport pylint: disable=import-error

from ElementsKernel import Logging
from ElementsKernel import Auxiliary
from ElementsKernel import ProjectCommonRoutines

try:
    from builtins import input
except ImportError:
    from __builtin__ import input

LOGGER = Logging.getLogger(__name__)

AUX_CMAKE_LIST_IN = "CMakeLists.txt.in"
AUX_MAKE_FILE_IN = "Makefile.in"
AUX_PROJ_RST_IN = "doc_project.rst.in"
AUX_PROJ_MAINPAGE_IN = "mainpage.dox.in"
AUX_GITIGNORE_IN = "gitignore_template.in"
AUX_GITATTRIBUTES_IN = "gitattributes_template.in"
AUX_EDITOR_CONFIG = "editorconfig"
AUX_CLANG_FORMAT = "clang-format"

target_locations = { AUX_CMAKE_LIST_IN: "CMakeLists.txt",
                     AUX_MAKE_FILE_IN: "Makefile",
                     AUX_PROJ_RST_IN: "doc/doc_project.rst",
                     AUX_PROJ_MAINPAGE_IN: "doc/mainpage.dox",
                     AUX_GITIGNORE_IN: ".gitignore",
                     AUX_GITATTRIBUTES_IN: ".gitattributes",
                     AUX_EDITOR_CONFIG: ".editorconfig",
                     AUX_CLANG_FORMAT: ".clang-format"
                   }

################################################################################


def setPath():
    """
    Set the installation path either the current directory or
    the directory set by the <User_area> environment variable
    """
    # Check if User_area environment variable exists
    user_area = os.environ.get('User_area')
    if not user_area is None:
        LOGGER.debug('# <$User_area> environment variable defined to : <%s>', user_area)
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
        ProjectCommonRoutines.checkNameAndVersionValid(str_list[i][0], str_list[i][1])

################################################################################


def duplicate_elements(duplicate_list):
    """
    Look for duplicate element in a list
    """
    name_list = []
    for elt in duplicate_list:
        if not elt[0] in name_list:
            name_list.append(elt[0])
        else:
            raise Exception("Found twice the following dependency : < %s >" % elt[0])

################################################################################


def getElementsVersion():
    """
    Get the Elements version number
    """

    elt_version = ELEMENTS_VERSION.ELEMENTS_ORIGINAL_VERSION

    LOGGER.info('# Elements version found : <%s>', elt_version)

    return str(elt_version)

################################################################################


def getSubstituteConfiguration(proj_name, proj_version, dep_projects, standalone=False, visibility=False):
    """
    Format all dependent projects
    We put by default Elements dependency if no one is given
    """
    if standalone:
        str_dep_projects = ""
    else:
        str_dep_projects = 'Elements ' + getElementsVersion()
    if dep_projects:
        for dep in dep_projects:
            if not dep[0] in str_dep_projects:
                str_dep_projects += ' ' + dep[0] + ' ' + dep[1]
            else:
                LOGGER.warning('<%s> dependency already exists. It is skipped!', dep[0])

    if str_dep_projects:
        str_dep_projects = "USE " + str_dep_projects

    str_visibility = ""
    if visibility:
        str_visibility = """set(ELEMENTS_HIDE_SYMBOLS ON
    CACHE STRING "Enable explicit symbol visibility on gcc-4"
    FORCE)"""

    configuration = {"PROJECT_NAME":proj_name,
                     "PROJECT_VERSION":proj_version,
                     "DEPENDANCE_LIST":str_dep_projects,
                     "ELEMENTS_HIDE_SYMBOLS":str_visibility}

    return configuration

################################################################################


def createProject(project_dir, proj_name, proj_version, dep_projects, standalone=False, visibility=False):
    """
    Create the project structure
    """
    LOGGER.info('# Creating the project')

    configuration = getSubstituteConfiguration(proj_name, proj_version, dep_projects, standalone, visibility)

    for src in target_locations:
        file_name = os.path.join("ElementsKernel", "templates", src)
        tgt = target_locations[src]
        Auxiliary.configure(file_name, project_dir, tgt,
                            configuration=configuration,
                            create_missing_dir=True)
        ProjectCommonRoutines.addItemToCreationList(os.path.join(project_dir, tgt))


def makeChecks(proj_name, proj_version, dependency, dependant_projects):
    """
    Make some checks
    """
    # Check project name and version
    ProjectCommonRoutines.checkNameAndVersionValid(proj_name, proj_version)
    if not dependency is None:
        checkDependencyProjectValid(dependant_projects)
    # Check for duplicate dependencies
    if not dependency is None:
        duplicate_elements(dependant_projects)
    # Check AUX files exist
    ProjectCommonRoutines.checkAuxFileExist(AUX_CMAKE_LIST_IN)
    ProjectCommonRoutines.checkAuxFileExist(AUX_MAKE_FILE_IN)

################################################################################


def getProjectDirectory(no_version_directory, destination_path, proj_name, proj_version):
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
        match = re.match(ProjectCommonRoutines.VERSION_REGEX, elt)
        if match:
            match_list.append(match.group(0))
    return match_list

################################################################################


def checkProjectExist(project_dir, no_version_directory, force_erase, answer_yes=False):
    """
    Look for any version directory in the project directory e.g 1.0,1.2 etc...
    """
    if os.path.exists(project_dir) and not force_erase:
        LOGGER.warning('<%s> Project ALREADY exists!!!', project_dir)
        version_dir_list = lookForDirectories(project_dir)
        # Warn user about directory under the project
        if no_version_directory and version_dir_list:
            LOGGER.warning('Found the following version(s) directory(ies) : %s', version_dir_list)
        if not answer_yes:
            response_key = input('Do you want to overwrite the existing project (y/n, default: n)?')
        if answer_yes or response_key.lower() == "yes" or response_key == "y":
            LOGGER.info('# Overwriting the existing project: <%s>', project_dir)
        else:
            raise Exception()
    elif force_erase:
        ProjectCommonRoutines.eraseDirectory(project_dir)
