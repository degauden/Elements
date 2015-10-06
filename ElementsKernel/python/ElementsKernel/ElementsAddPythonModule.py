"""
@file: ElementsKernel/ElementsAddPythonModule.py
@author: Nicolas Morisset
         Astronomy Department of the University of Geneva

@date: 01/07/15

This script creates a new Elements python module
"""

import os
import argparse
import time
import ElementsKernel.ElementsProjectCommonRoutines as epcr
import ElementsKernel.parseCmakeLists as pcl
import ElementsKernel.Logging as log

logger = log.getLogger('AddPythonModule')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'

################################################################################

def createDirectories(module_dir, module_name):
    """
    Create directories needed for a python module
    """
    # Create the executable directory
    python_module_path = os.path.join(module_dir, 'python', module_name)
    epcr.makeDirectory(python_module_path)
    # Create the python directory in tests
    tests_python_path = os.path.join(module_dir, 'tests', 'python')
    epcr.makeDirectory(tests_python_path)

################################################################################

def createFiles(module_dir, module_name, python_module_name):
    """
    Create files needed for a python module
    """
    # Create the init file 
    init_file = os.path.join(module_dir, 'python', module_name, '__init__.py')
    if not os.path.exists(init_file):
        f = open(init_file, 'w')
        f.close()

    python_module_file = os.path.join(module_dir, 'python', module_name, 
                                    python_module_name+'.py')
    # Create the module file 
    if not os.path.exists(python_module_file):
        f = open(python_module_file, 'w')
        f.write('"""\n')
        f.write('@file '+ os.path.join('python', module_name, python_module_name + '.py')+'\n')
        f.write('@date '+ time.strftime("%x") +'\n')
        f.write('@author '+epcr.getAuthor() +'\n')
        f.write('"""\n')
        f.close()

################################################################################

def updateCmakeListsFile(module_dir):
    """
    Update the CMakeList.txt file
    """
    logger.info('# Updating the <%s> file' % CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)

    # Backup the file
    epcr.makeACopy(cmake_filename)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        f = open(cmake_filename, 'r')
        data = f.read()
        f.close()
        cmake_object = pcl.CMakeLists(data)

        # Add elements_install_conf_files if any
        cmake_object.elements_install_python_modules = 'elements_install_python_modules()'
                                       
    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################

def createPythonModule(current_dir, module_name, python_module_name):
    """
    Create the python module
    """
    logger.info('#')
    script_goes_on  = True
    createDirectories(current_dir, module_name)
    createFiles(current_dir, module_name, python_module_name)
    module_path = os.path.join(current_dir, 'python', python_module_name)
    updateCmakeListsFile(current_dir)
    
    return script_goes_on

################################################################################

def defineSpecificProgramOptions():
    description = """
This script creates an <Elements> python module at your current directory
(default), this directory must be an <Elements> module.
           """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('module_name', metavar='module-name', 
                        type=str, 
                        help='Module name')

    return parser

################################################################################

def mainMethod(args):

    logger.info('#')
    logger.info(
        '#  Logging from the mainMethod() of the ElementsAddPythonModule script ')
    logger.info('#')

    try:
        script_goes_on     = True
        python_module_name = args.module_name

        # Default is the current directory
        current_dir = os.getcwd()

        logger.info('# Current directory : %s', current_dir)

        # We absolutely need a Elements cmake file
        script_goes_on, module_name = epcr.isElementsModuleExist(current_dir)

        # Module as no version number, '1.0' is just for using the routine
        if script_goes_on:
            script_goes_on = epcr.isNameAndVersionValid(python_module_name, '1.0')
        
        module_file_path = os.path.join(current_dir, 'python', module_name,
                                         python_module_name+'.py')
        
        # Make sure the program does not already exist
        if script_goes_on:
            script_goes_on = epcr.isFileAlreadyExist(module_file_path, python_module_name) 
                  
        if script_goes_on:
            if os.path.exists(current_dir):
                if createPythonModule(current_dir, module_name, python_module_name):
                    logger.info('# <%s> python module successfully created in <%s>.' % 
                                (python_module_name, module_file_path))
                    # Remove backup file
                    epcr.deleteFile(os.path.join(current_dir, CMAKE_LISTS_FILE)+'~')
                    logger.info('# Script over.')
            else:
                    logger.error('# <%s> project directory does not exist!'
                                  % current_dir)
        if not script_goes_on:
            logger.error('# Script aborted')

    except Exception as e:
        logger.exception(e)
        logger.info('# Script stopped...')
