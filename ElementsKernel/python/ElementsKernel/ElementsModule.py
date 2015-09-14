##
# @file: ElementsKernel/ElementsModule.py
# @author: Nicolas Morisset
#          Astronomy Department of the University of Geneva
#
# @date: 01/07/15
#
# This script will create a new Elements module

import argparse
import os
import ElementsKernel.ElementsProject as ep
import ElementsKernel.Logging as log

logger = log.getLogger('CreateElementsModule')

# Define constants
AUX_CMAKELIST_MOD_IN = 'CMakeLists.txt.mod.in'
CMAKE_LISTS_FILE = 'CMakeLists.txt'
LIB_KEY = '#LIB#'
EXE_KEY = '#EXE#'
PY_KEY = '#PY#'


def isElementsProjectExist(dir_project):
    """
    Checks if a CMakeLists.txt file exists and is really an Elements
    cmake file
    """
    file_exists = True
    cmake_file = os.path.join(os.path.sep, dir_project, CMAKE_LISTS_FILE)
    if not os.path.isfile(cmake_file):
        file_exists = False
        logger.error(
            '# %s cmake project file is missing! Are you inside a project directory?', cmake_file)
    else:
        # Check the make file is an Elements cmake file
        # it should contain the string : "elements_project"
        f = open(cmake_file, 'r')
        data = f.read()
        if not 'elements_project' in data:
            file_exists = False
            logger.error(
                '# %s is not an Elements project cmake file! Can not find the <elements_project> directive', cmake_file)
        f.close()

    return file_exists


def substituteModuleVariables(module_dir, module_name, remove_lib, remove_exe, add_python):
    """
    Substitute variables in CMakeList.txt.mod.in file and rename it to
    CMakeList.txt.
    """
    logger.info('# Substitute variables in <%s> file' % AUX_CMAKELIST_MOD_IN)
    cmake_list_file = os.path.join(
        os.path.sep, module_dir, AUX_CMAKELIST_MOD_IN)
    cmake_list_file_final = os.path.join(
        os.path.sep, module_dir, CMAKE_LISTS_FILE)

    # Make it a bit more logical
    add_lib = not remove_lib
    add_exe = not remove_exe

    # Remove all useless lines in CMakeLists.txt file following
    # the options chosen
    f_initial = open(cmake_list_file, 'r')
    f_final = open(cmake_list_file_final, 'w')

    for line in f_initial.readlines():
        line = line.replace('%(MODULE_NAME)s', module_name)
        # we got all structure
        if add_lib and add_exe and add_python:
            # We keep all structure with the library stuff
            if line:
                line = line.replace(LIB_KEY, '')
                line = line.replace(EXE_KEY, '')
                line = line.replace(PY_KEY, '')
            f_final.write(line)
        elif add_lib and add_exe and not add_python:
            if not PY_KEY in line:
                line = line.replace(EXE_KEY, '')
                line = line.replace(LIB_KEY, '')
                f_final.write(line)
        elif add_lib and not add_exe and not add_python:
            if not PY_KEY in line:
                if not EXE_KEY in line:
                    line = line.replace(LIB_KEY, '')
                    f_final.write(line)
        elif add_lib and not add_exe and add_python:
            if not EXE_KEY in line:
                line = line.replace(LIB_KEY, '')
                line = line.replace(PY_KEY, '')
                f_final.write(line)
        elif not add_lib and add_exe and add_python:
            if not LIB_KEY in line:
                line = line.replace(EXE_KEY, '')
                line = line.replace(PY_KEY, '')
                f_final.write(line)
        elif not add_lib and add_exe and not add_python:
            if not LIB_KEY in line:
                if not PY_KEY in line:
                    line = line.replace(EXE_KEY, '')
                    f_final.write(line)

    f_final.close()
    f_initial.close()
    os.remove(cmake_list_file)


def createPythonStuff(mod_path, module_name):
    """
    Create the directory structure for python
    """
    logger.info('# Creating the python directories ')
    os.makedirs(os.path.join(os.path.sep, mod_path, 'python', module_name))
    os.makedirs(os.path.join(os.path.sep, mod_path, 'scripts'))
    os.makedirs(os.path.join(os.path.sep, mod_path, 'tests', 'python'))


def createModuleDirectories(mod_path, module_name, remove_lib, remove_exe, add_python):
    """
    Create the directory structure for the module
    """
    # Create module directories
    logger.info('# Creating the module directories')
    os.makedirs(mod_path)

    os.makedirs(os.path.join(os.path.sep, mod_path, 'doc'))

    if not remove_lib:
        os.makedirs(os.path.join(os.path.sep, mod_path, 'src', 'lib'))

    os.makedirs(os.path.join(os.path.sep, mod_path, module_name))
    os.makedirs(os.path.join(os.path.sep, mod_path, 'tests', 'src'))

    if not remove_exe:
        os.makedirs(os.path.join(os.path.sep, mod_path, 'src', 'program'))
        os.makedirs(os.path.join(os.path.sep, mod_path, 'conf'))

    if add_python:
        createPythonStuff(mod_path, module_name)




