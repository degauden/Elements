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
import ElementsKernel.Logging as log
from ElementsKernel import Project, ProjectCommonRoutines
from ElementsKernel import Exit

def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
This script creates an <Elements> project in your current directory (by default)
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
    - If your project directory exists already (e.g. project cloned from git), the script will not erase it,
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
    parser.add_argument('-y', '--yes', default=False, action="store_true",
                        help='Answer <yes> by default to any question, useful when the script is called by another'\
                         'script')

    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """
    
    exit_code = Exit.Code["OK"]
    
    logger = log.getLogger('CreateElementsProject')

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the CreateElementsProject script')
    logger.info('#')

    proj_name = args.project_name
    proj_version = args.project_version
    dependant_projects = args.dependency
    destination_path = Project.setPath()
    dependency = args.dependency
    no_version_directory = args.no_version_directory
    standalone = args.standalone
    force_erase = args.erase
    answer_yes = args.yes

    logger.info('# Installation directory : %s', destination_path)

    try:
        # Set the project directory
        project_dir = Project.getProjectDirectory(no_version_directory, destination_path, proj_name, proj_version)
        Project.makeChecks(proj_name, proj_version, dependency, dependant_projects)

        Project.checkProjectExist(project_dir, no_version_directory, force_erase, answer_yes)

        # Create the project
        Project.createProject(project_dir, proj_name, proj_version, dependant_projects, standalone)

        # Print all files created
        ProjectCommonRoutines.printCreationList()

        logger.info('# <%s> project successfully created.', project_dir)

    except Exception as msg:
        if str(msg):
            logger.error(msg)
        logger.error('# Script aborted.')
        exit_code = Exit.Code["NOT_OK"]
    else:
        logger.info('# Script over.')


    return exit_code
