##
# @file: ElementsKernel/ElementsAddCppProgram.py
# @author: Nicolas Morisset
#          Astronomy Department of the University of Geneva
#
# @date: 01/07/15
#
# This script will create a new Elements C++ Program
##

import argparse
import os
import re
import shutil
import time
import ElementsKernel.ElementsProjectCommonRoutines as epcr
import ElementsKernel.parseCmakeLists as pcl
import ElementsKernel.Logging as log

logger = log.getLogger('AddCppProgram')

# Define constants
CMAKE_LISTS_FILE    = 'CMakeLists.txt'
CMAKE_LISTS_FILE_IN = 'CMakeLists.txt.mod.in'
H_TEMPLATE_FILE     = 'ClassName_template.h'
CPP_TEMPLATE_FILE   = 'className_template.cpp'

def createDirectories(module_dir, program_name):
    """
    Create directories needed for a program
    """
    logger.info('# Creating the directories ')
    # Create Directories
    module_path = os.path.join(os.path.sep, module_dir, module_name, subdir)
      

################################################################################
    
      
def createCppProgram(module_dir, module_name, program_name, module_dep_list,
                    library_dep_list):
    """
    """
    logger.info('# Creating the directories ')
    
    script_goes_on = True 
    
    createDirectories(module_dir, program_name)  
                     
    return script_goes_on

################################################################################
    
def defineSpecificProgramOptions():
    description = """
           """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('program_name', metavar='program-name', 
                        type=str, 
                        help='Program name')
    parser.add_argument('-md', '--module-dependency', metavar='module_name',
                        action='append', type=str,
                        help='Dependency module name e.g."-md ElementsKernel"')
    parser.add_argument('-ld', '--library-dependency', metavar='library_name', 
                        action='append', type=str,
                        help='Dependency library name e.g."-ld ElementsKernel"')

    return parser

################################################################################

def mainMethod(args):

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the ElementsAddCppProgram \
    script ')
    logger.info('#')

    try:
        # True: no error occured
        script_goes_on       = True 
        
        module_list          = args.module_dependency
        library_list         = args.library_dependency

        # Default is the current directory
        current_dir = os.getcwd()

        logger.info('# Current directory : %s', current_dir)

        # We absolutely need a Elements cmake file
        script_goes_on, module_name = epcr.isElementsModuleExist(current_dir)
        
        # Check aux files exist
        #if script_goes_on:
        #    script_goes_on = ep.isAuxFileExist(H_TEMPLATE_FILE)
        #if script_goes_on:
        #    script_goes_on = ep.isAuxFileExist(CPP_TEMPLATE_FILE)
        #if script_goes_on:
        #    script_goes_on = ep.isAuxFileExist(CMAKE_LISTS_FILE_IN)
        
        # Create CPP class    
        if script_goes_on:
            if createCppProgram(current_dir, module_name, sub_dir, class_name, 
                              module_list, library_list):
                logger.info('# <%s> program successfully created in <%s>.' % 
                            (class_name, module_dir))
            else:
                if not script_goes_on:
                    logger.error('# <%s> Script aborted!')
        else:
            logger.error('# Script aborted!')

    except Exception as e:
        logger.exception(e)
        logger.info('# Script stopped...')
