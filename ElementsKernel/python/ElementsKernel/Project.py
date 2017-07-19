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

logger = log.getLogger('CreateElementsProject')

AUX_CMAKE_LIST_IN = "CMakeLists.txt.in"
AUX_CMAKE_FILE_IN = "Makefile.in"
AUX_PROJ_RST_IN = "doc_project.rst.in"

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
    os.rename(cmake_list_file, cmake_list_file.replace('.in', ''))

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

    # Remove '.in'
    cmakefile = os.path.join(project_dir, AUX_CMAKE_FILE_IN)
    os.rename(cmakefile, cmakefile.replace('.in', ''))

    project_doc_dir = os.path.join(project_dir, "doc")
    if not os.path.exists(project_doc_dir):
        os.makedirs(project_doc_dir)
    epcr.copyAuxFile(project_doc_dir, AUX_PROJ_RST_IN)
    project_rst_file = os.path.join(project_doc_dir, AUX_PROJ_RST_IN)
    os.rename(project_rst_file, project_rst_file.replace('.in', ''))

    substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects, standalone)

################################################################################

def makeChecks(proj_name, proj_version, dependency, dependant_projects):
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
    epcr.checkNameInEuclidNamingDatabase(proj_name, nc.TYPES[0])


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
    Look for any version directory in the project directory e.g 1.0,1.2 etc...
    """
    matchList = []
    regex_pattern = r"\d+\.\d+(\.\d+)?"
    dirlist = [elt for elt in os.listdir(project_dir) if os.path.isdir(os.path.join(project_dir, elt)) ]
    version_array = [ m.group(0) for l in dirlist for m in [ re.search(regex_pattern, l) ] if m ]
    for elt in dirlist:
        match = re.search(regex_pattern, elt)
        if match:
            matchList.append(match.group(0))
    return matchList

################################################################################

def CheckProjectExist(project_dir, no_version_directory, force_erase):
    """
    Look for any version directory in the project directory e.g 1.0,1.2 etc...
    """
    if os.path.exists(project_dir) and not force_erase:
        logger.warning('<%s> Project ALREADY exists!!!', project_dir)
        version_dir_list = lookForDirectories(project_dir)
        # Warn user about directory under the project
        if no_version_directory and version_dir_list:
            logger.warning('Found the following version(s) directory(ies) : %s', version_dir_list)
        response_key = raw_input('Do you want to overwrite the existing project (y/n, default: n)?')
        if response_key.lower() == "yes" or response_key == "y":
            logger.info('# Overwriting the existing project: <%s>', project_dir)
        else:
            raise epcr.ErrorOccured
    elif force_erase:
        epcr.eraseDirectory(project_dir)

################################################################################

def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
This script creates an <Elements> project in your current directory(by default)
or at the location defined by the <$User_area> environment variable.
It means that all the necessary structure (directory structure, makefiles etc...)
will be automatically created for you. In addition, the project name will be registered (if any) into
the Naming Database with the URL defined by the ELEMENTS_NAMING_DB_URL environment variable.
The options are:

[-d]          Use this option if your project has some dependencies on other project(s).
[-n or -novd] Use this option if you do not want to create a version directory
[-e]          Use this option to erase an already existing project

e.g.
    > CreateElementsProject test_project 1.0
      This creates the following directories : "test_project/1.0"

    > CreateElementsProject test 1.0 -n (or -novd)
      This creates the following directory : "test_project"

    > CreateElementsProject test 1.0 -e
      With this option an already existing project ("test_project/1.0") will be fully erased before
      the creation of the new one.
      This creates the following directory : "test_project/1.0"

    The "test_project" project is created at your current directory or
    at the location pointed by the $User_area environment variable

Note:
    - If your project directory exists already(e.g. project cloned from git), the script will not erase it,
      it will just replace the files needed to be updated. For instance, the <.git> directory will
      not be erased (and/or whatever files/directory there).
      If you do not use the -n (or -novd) option, in such a case the <.git> directory for instance will not be
      copied under the version directory. It is the responsability of the user to do that and whatever file(s)
      needed to be copied.
            """

    from argparse import RawTextHelpFormatter

    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=RawTextHelpFormatter)
    parser.add_argument('project_name', metavar='project-name', type=str,
                        help='Project name')
    parser.add_argument('project_version', metavar='project-version',
                        type=str, default='1.0', help='Project version number')
    parser.add_argument('-d', '--dependency', metavar=('project_name', 'version'),
                        nargs=2, action='append', type=str,
                        help='Dependency project name and its version number"\
                         e.g "-d Elements x.x.x"')
    parser.add_argument('-n', '-novd', '--no-version-directory', action="store_true",
                        help='Does not create the <project-version> directory only\
                        the <project-name> directory will be created')
    parser.add_argument('-e', '--erase', action="store_true",
                        help='Erase the <project-version> directory if it does exists')
    parser.add_argument('-s', '--standalone', default=False, action="store_true",
                        help='Remove the implicit dependency onto the Elements project (expert only)')


    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """
    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the CreateElementsProject script')
    logger.info('#')

    proj_name = args.project_name
    proj_version = args.project_version
    dependant_projects = args.dependency
    destination_path = setPath()
    dependency = args.dependency
    no_version_directory = args.no_version_directory
    standalone = args.standalone
    force_erase = args.erase

    logger.info('# Installation directory : %s', destination_path)

    try:
        # Set the project directory
        project_dir = buildProjectDir(no_version_directory, destination_path, proj_name, proj_version)
        makeChecks(proj_name, proj_version, dependency, dependant_projects)

        CheckProjectExist(project_dir, no_version_directory, force_erase)

        # Create the project
        createProject(project_dir, proj_name, proj_version, dependant_projects, standalone)

        logger.info('# <%s> project successfully created.', project_dir)

    except epcr.ErrorOccured, msg:
        if str(msg):
           logger.error(msg)
        logger.error('# Script aborted.')
        return 1
    except Exception, msg:
        if str(msg):
            logger.error(msg)
        logger.error('# Script aborted.')
        return 1
    else:
        logger.info('# Script over.')
