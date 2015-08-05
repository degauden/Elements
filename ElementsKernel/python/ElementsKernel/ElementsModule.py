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
    cmake_lists = 'elements_project'
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


def createModule(project_dir, module_name, add_python, remove_lib, remove_exe):
    """
    Create a module, copy auxiliary files and substitute variables in the
    CMakefile.txt file
    """
    script_goes_on = True

    # Create module directory
    mod_path = os.path.join(os.path.sep, project_dir, module_name)
    logger.info('# Creating the module: <%s> ' % mod_path)
    if os.path.exists(mod_path):
        # Ask user
        logger.warning('<%s> module ALREADY exists!!!' % module_name)
        response_key = raw_input(
            'Do you want to replace the existing module (Yes/No, default: No)?')
        if response_key == "YES" or response_key == "yes" or response_key == "y":
            logger.info('# Replacing the existing module: <%s>' % module_name)
            ep.eraseDirectory(mod_path)
        else:
            logger.info("Script stopped by user!")
            script_goes_on = False

    if script_goes_on:
        createModuleDirectories(
            mod_path, module_name, remove_lib, remove_exe, add_python)
        ep.copyAuxFile(mod_path, AUX_CMAKELIST_MOD_IN)
        substituteModuleVariables(
            mod_path, module_name, remove_lib, remove_exe, add_python)

    return script_goes_on


def defineSpecificProgramOptions():
    description = """ 
This script creates an <Elements> module at your current directory
(default) but it must be inside a project directory. All necessary structure
(directory structure, makefiles etc...) will be automatically created 
for you. 
[-py] Use this option if you need the python structure
[--path pathname] Use this option if your project directory is not the current 
         directory. Give an absolute path.
[-h] help
           """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument(
        'module_name', metavar='module-name', type=str, help='Module name')
    parser.add_argument(
        '-py', '--python-stuff', action='store_true', help='Add python directory structure')
    parser.add_argument(
        '--path', type=str, help='Installation absolute path (different than current directory)')
    parser.add_argument(
        '-re', '--remove-exe', action='store_true', help='Structure for executable removed')
    parser.add_argument(
        '-rl', '--remove-lib', action='store_true', help='Structure for library removed')
    parser.add_argument(
        '-ex', '--example', action='store_true',  help='Add example files ')

    return parser


def mainMethod(args):

    logger.info('#')
    logger.info(
        '#  Logging from the mainMethod() of the CreateElementsModule script ')
    logger.info('#')

    try:
        script_goes_on = True
        module_name = args.module_name
        add_python = args.python_stuff
        add_example = args.example
        remove_lib = args.remove_lib
        remove_exe = args.remove_exe

        # TMP TMP
        if add_example:
            logger.warning('# Sorry NO EXAMPLES files available yet!')
        # TMP TMP

        # Default is the current directory
        project_dir = os.getcwd()
        if not args.path is None:
            project_dir = ep.setPath(args.path)

        logger.info('# Current directory : %s', project_dir)

        # We absolutely need a Elements cmake file
        script_goes_on = isElementsProjectExist(project_dir)

        # Module as no version number, '1.0' is just for using the routine
        if script_goes_on:
            script_goes_on = ep.isNameAndVersionValid(module_name, '1.0')

        # Check aux files exist
        if script_goes_on:
            script_goes_on = ep.isAuxFileExist(AUX_CMAKELIST_MOD_IN)

        if script_goes_on:
            if os.path.exists(project_dir):
                if createModule(project_dir, module_name, add_python, remove_lib, remove_exe):
                    logger.info(
                        '# <%s> module successfully created in <%s>.' % (module_name, project_dir))
            else:
                if not script_goes_on:
                    logger.error(
                        '# <%s> project directory does not exist!' % project_dir)
        else:
            logger.error('# Script aborted')

    except Exception as e:
        logger.exception(e)
        logger.info('# Script stopped...')
