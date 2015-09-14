##
# @file: ElementsKernel/ElementsAddModule.py
# @author: Nicolas Morisset
#          Astronomy Department of the University of Geneva
#
# @date: 01/07/15
#
# This script will create a new Elements module

import argparse
import os
import ElementsKernel.ElementsProject as ep
import ElementsKernel.ElementsModule as em
import ElementsKernel.Logging as log

logger = log.getLogger('AddElementsModule')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'

def createModuleDirectories(mod_path, module_name):
    """
    Create the directory structure for the module
    """
    # Create module directories
    logger.info('# Creating the module directories')
    os.makedirs(mod_path)
    os.makedirs(os.path.join(os.path.sep, mod_path, module_name))
    os.makedirs(os.path.join(os.path.sep, mod_path, 'doc'))
    os.makedirs(os.path.join(os.path.sep, mod_path, 'conf'))
    os.makedirs(os.path.join(os.path.sep, mod_path, 'tests', 'src'))

def createCmakeListFile(module_dir, module_name, dependency_list):
    """
    Create the CMakeList.txt file and add dependencies to it
    """
    logger.info('# Create the %s File' % CMAKE_LISTS_FILE)
    cmake_list_file_final = os.path.join(os.path.sep, module_dir, CMAKE_LISTS_FILE)

    f_final = open(cmake_list_file_final, 'w')
    line = 'elements_subdir(' + module_name + ') \n'
    f_final.write(line)
    if not dependency_list is None:
        for dep_module in dependency_list:
            line ='elements_depends_on_subdirs(' + dep_module[0] + ')\n'
            f_final.write(line) 
    f_final.close()


def createModule(project_dir, module_name, dependency_list):
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
        createModuleDirectories(mod_path, module_name)
        createCmakeListFile(mod_path, module_name, dependency_list)

    return script_goes_on


def defineSpecificProgramOptions():
    description = """
This script creates an <Elements> module at your current directory
(default) but it must be inside a project directory. All necessary structure
(directory structure, makefiles etc...) will be automatically created 
for you. Use the [-md] option for module dependency and [-h] for help.
           """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('module_name', metavar='module-name', 
                        type=str, 
                        help='Module name')
    parser.add_argument('-md', '--module-dependency', nargs=1, action='append', 
                        type=str,
                        help='Dependency module name e.g "dep_module_name"')

    return parser

def mainMethod(args):

    logger.info('#')
    logger.info(
        '#  Logging from the mainMethod() of the ElementsAddModule script ')
    logger.info('#')

    try:
        script_goes_on  = True
        module_name     = args.module_name
        dependency_list = args.module_dependency

        # Default is the current directory
        project_dir = os.getcwd()

        logger.info('# Current directory : %s', project_dir)

        # We absolutely need a Elements cmake file
        script_goes_on = em.isElementsProjectExist(project_dir)

        # Module as no version number, '1.0' is just for using the routine
        if script_goes_on:
            script_goes_on = ep.isNameAndVersionValid(module_name, '1.0')
                
        if script_goes_on:
            if os.path.exists(project_dir):
                if createModule(project_dir, module_name, dependency_list):
                    logger.info('# <%s> module successfully created in <%s>.' % 
                                (module_name, project_dir))
            else:
                if not script_goes_on:
                    logger.error(
                        '# <%s> project directory does not exist!' % project_dir)
        else:
            logger.error('# Script aborted')

    except Exception as e:
        logger.exception(e)
        logger.info('# Script stopped...')
