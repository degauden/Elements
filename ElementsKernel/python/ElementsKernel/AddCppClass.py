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

""" This script creates a new Elements C++ Class

:file: ElementsKernel/AddCppClass.py
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

logger = Logging.getLogger('AddCppClass')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
H_TEMPLATE_FILE = 'ClassName_template.h'
CPP_TEMPLATE_FILE = 'ClassName_template.cpp'
UNITTEST_TEMPLATE_FILE = 'UnitTestFile_template.cpp'

H_TEMPLATE_FILE_IN = 'ClassName_template.h.in'
CPP_TEMPLATE_FILE_IN = 'ClassName_template.cpp.in'
UNITTEST_TEMPLATE_FILE_IN = 'UnitTestFile_template.cpp.in'

################################################################################


def getClassName(subdir_class):
    """
    Get the class name and sub directory if any
    """
    (subdir, class_name) = os.path.split(subdir_class)
    logger.info('Class name: %s', class_name)
    if subdir:
        logger.info('Sub directory: %s', subdir)
    return subdir, class_name

################################################################################


def createDirectories(module_dir, module_name, subdir):
    """
    Create directories needed for a module and a class
    """
    standalone_directories = [os.path.join(module_name, subdir),
                              os.path.join('src', 'lib', subdir),
                              os.path.join('tests', 'src', subdir)]
    for d in standalone_directories:
        target_dir = os.path.join(module_dir, d)
        if not os.path.exists(target_dir):
            os.makedirs(target_dir)

################################################################################


def substituteAuxFiles(module_dir, class_name, module_name, subdir):
    """
    Copy AUX file(s) and substitutes keyworks
    """
    target_locations = {
                       H_TEMPLATE_FILE_IN: os.path.join(module_name, subdir, class_name + ".h"),
                       CPP_TEMPLATE_FILE_IN: os.path.join('src', 'lib', subdir, class_name + ".cpp"),
                       UNITTEST_TEMPLATE_FILE_IN: os.path.join('tests', 'src', subdir, class_name + "_test.cpp")
                       }

    ossep2 = "" if not subdir else os.sep

    configuration = {  "FILE_H": os.path.join(module_name, subdir, class_name + '.h'),
                       "FILE_CPP": os.path.join('src', 'lib', subdir, class_name + '.cpp'),
                       "FILE_TEST": os.path.join('tests', 'src', subdir, class_name + '_test.cpp'),
                       "DATE": time.strftime("%x"),
                       "AUTHOR": ProjectCommonRoutines.getAuthor(),
                       "DEFINE_WORDS": ("_" + module_name + "_" + class_name + "_H").upper(),
                       "CLASSNAME": class_name,
                       "OSSEP": os.sep,
                       "OSSEP2": ossep2,
                       "MODULENAME": module_name,
                       "SUBDIR": subdir
                    }
    # Put AUX files to their target
    for src in target_locations:
        file_name = os.path.join("ElementsKernel", "templates", src)
        tgt = target_locations[src]
        Auxiliary.configure(file_name, module_dir, tgt,
                            configuration=configuration,
                            create_missing_dir=True)
        ProjectCommonRoutines.addItemToCreationList(os.path.join(module_dir, tgt))

# # ###############################################################################


def updateCmakeListsFile(module_dir, subdir, class_name, elements_dep_list,
                         library_dep_list):
    """
    Update the <CMakeLists.txt> file for a class
    """
    logger.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)
    ProjectCommonRoutines.addItemToCreationList(cmake_filename)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        cmake_object, module_name = ProjectCommonRoutines.updateCmakeCommonPart(cmake_filename, library_dep_list)

        # Put ElementsKernel as a default
        default_dependency = 'ElementsKernel'
        if elements_dep_list:
            if not default_dependency in elements_dep_list:
                elements_dep_list.insert(0, default_dependency)
        else:
            elements_dep_list = [default_dependency]

        # Update ElementsDependsOnSubdirs macro
        if elements_dep_list:
            for mod_dep in elements_dep_list:
                dep_object = ParseCmakeListsMacros.ElementsDependsOnSubdirs([mod_dep])
                cmake_object.elements_depends_on_subdirs_list.append(dep_object)

        # Update elements_add_library macro
        if module_name:
            source = os.path.join('src', 'lib', subdir, '*.cpp')
            existing = [x for x in cmake_object.elements_add_library_list if x.name == module_name]
            link_libs = []
            include_dirs = []
            if elements_dep_list:
                link_libs = link_libs + elements_dep_list
                include_dirs = include_dirs + elements_dep_list
            if library_dep_list:
                link_libs = link_libs + library_dep_list
                include_dirs = include_dirs + library_dep_list
            if existing:
                if not source in existing[0].source_list:
                    existing[0].source_list.append(source)
                for lib in link_libs:
                    if not lib in existing[0].link_libraries_list:
                        existing[0].link_libraries_list.append(lib)
                for incd in include_dirs:
                    if not incd in existing[0].include_dirs_list:
                        existing[0].include_dirs_list.append(incd)
            else:
                source_list = [source]
                public_headers_list = [module_name]
                lib_object = ParseCmakeListsMacros.ElementsAddLibrary(module_name, source_list,
                                                    link_libs, include_dirs,
                                                    public_headers_list)
                cmake_object.elements_add_library_list.append(lib_object)

            # Add unit test
            source_name = os.path.join('tests', 'src', subdir, class_name + '_test.cpp')
            if subdir:
                exec_test_name = module_name + "_" + subdir + "_" + class_name + '_test'
                test_name = subdir + "_" + class_name
            else:
                exec_test_name = module_name + "_" + class_name + '_test'
                test_name = class_name

            unittest_object = ParseCmakeListsMacros.ElementsAddUnitTest(test_name,
                                                      [source_name], [module_name],
                                                      [], 'Boost', exec_test_name)
            cmake_object.elements_add_unit_test_list.append(unittest_object)

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################


def checkClassFileNotExist(class_name, module_dir, module_name, subdir):
    """
    Check if the class file does not already exist
    """
    module_path = os.path.join(module_dir, module_name, subdir)
    file_name = class_name + '.h'
    file_name_path = os.path.join(module_path, file_name)
    if os.path.exists(file_name_path):
        full_name = os.path.join(subdir, class_name)
        raise Exception("The <%s> class already exists! "
                        "Header file found here : < %s >" % (full_name, file_name_path))

################################################################################


def createCppClass(module_dir, module_name, subdir, class_name, elements_dep_list,
                   library_dep_list):
    """
    Create all necessary files for a cpp class
    """

    # Check the class does not exist already
    checkClassFileNotExist(class_name, module_dir, module_name, subdir)

    createDirectories(module_dir, module_name, subdir)

    # Update cmake file
    updateCmakeListsFile(module_dir, subdir, class_name, elements_dep_list, library_dep_list)
    # Substitue strings in files
    substituteAuxFiles(module_dir, class_name, module_name, subdir)

################################################################################


def makeChecks():
    """
    Make some checks
    """
    # Check aux files exist
    ProjectCommonRoutines.checkAuxFileExist(H_TEMPLATE_FILE_IN)
    ProjectCommonRoutines.checkAuxFileExist(CPP_TEMPLATE_FILE_IN)

################################################################################


def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
This script creates an <Elements> class at your current directory (default).
All necessary structure (directory structure, makefiles etc...)
will be automatically created for you if any but you have to be inside an
<Elements> module. You can specify a sub-directory where you want your class files (.h, .cpp).
e.g AddCppClass class_name or
    AddCppClass subdir/class_name
    """
    from argparse import RawTextHelpFormatter

    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=RawTextHelpFormatter)

    parser.add_argument('class_name', metavar='class-name',
                        type=str,
                        help='Class name without extention. e.g my_class_name or subdir/my_class_name')
    parser.add_argument('-ed', '--elements-dependency', metavar='module_name',
                        action='append', type=str,
                        help='Dependency module name e.g. "-ed ElementsKernel"')
    parser.add_argument('-extd', '--external-dependency', metavar='library_name',
                        action='append', type=str,
                        help='External dependency library name e.g. "-extd ElementsKernel"')

    return parser

################################################################################


def mainMethod(args):
    """ Main method of the AddCppClass Script"""

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the AddCppClass script ')
    logger.info('#')

    exit_code = Exit.Code["OK"]

    elements_dep_list = args.elements_dependency
    library_dep_list = args.external_dependency
    (subdir, class_name) = getClassName(args.class_name)

    try:
        # Default is the current directory
        module_dir = os.getcwd()
        # Make checks
        makeChecks()
        # We absolutely need a Elements cmake file
        module_name = ProjectCommonRoutines.getElementsModuleName(module_dir)

        logger.info('Current directory : %s', module_dir)
        logger.info('')

        # Create CPP class
        createCppClass(module_dir, module_name, subdir, class_name, elements_dep_list, library_dep_list)

        logger.info('<%s> class successfully created in <%s>.', class_name, os.path.join(module_dir, subdir))

        # Remove backup file
        ProjectCommonRoutines.deleteFile(os.path.join(module_dir, CMAKE_LISTS_FILE) + '~')

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
