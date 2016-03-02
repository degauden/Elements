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

"""
File: python/ElementsKernel/RemovePythonProgram.py

Created on: 02/11/16
Author: Nicolas Morisset
"""

from __future__ import division, print_function
from future_builtins import *

import argparse
import os
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.Logging as log

logger = log.getLogger('RemovePythonProgram')

################################################################################

def getAllFiles(program_name, module_directory, module_name):
    """
    """
    delete_file_list=[]
    file_name_conf = os.path.join(module_directory, 'conf', module_name, program_name) + '.conf'
    if os.path.exists(file_name_conf):
        delete_file_list.append(file_name_conf)
    file_name_cpp = os.path.join(module_directory, 'python', module_name, program_name) + '.py'
    if os.path.exists(file_name_cpp):
        delete_file_list.append(file_name_cpp)

    return delete_file_list
           
################################################################################

def defineSpecificProgramOptions():
    description = """
    This script allows you to remove all files on disk related to a python 
    program. Usually you use this script when you made a typo in the program  
    name when calling the <AddPythonProgram> script.
    
    WARNING: The script can not remove things related to the python program in 
             the <CMakeLists.txt> file. You MUST edit it and remove all 
             unecessary stuff related to this program. 
    """
    from argparse import RawTextHelpFormatter

    parser = argparse.ArgumentParser(description=description, 
                                     formatter_class=RawTextHelpFormatter)
    parser.add_argument('program_name', metavar='program-name',
                        type=str,
                        help='Program name')

    return parser

################################################################################

def mainMethod(args):

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the RemovePythonProgram \
    script ')
    logger.info('#')

    try:
        # True: no error occured
        script_goes_on = True

        program_name = args.program_name

        # Default is the current directory
        module_dir = os.getcwd()

        logger.info('Current directory : %s', module_dir)

        # We absolutely need a Elements cmake file
        script_goes_on, module_name = epcr.isElementsModuleExist(module_dir)

        if script_goes_on:
            # Default is the current directory
            file_to_be_deleted = getAllFiles(program_name, module_dir, module_name)
            if file_to_be_deleted:
                epcr.removeFilesOnDisk(file_to_be_deleted)
                cmakefile = os.path.join(module_dir, 'CMakeLists.txt')
                logger.warning('# !!!!!!!!!!!!!!!')
                logger.warning(' Please remove all things related to the program name : %s ' % (program_name))
                logger.warning(' in the <CMakeLists.txt> file : %s ' % (cmakefile))
                logger.warning('# !!!!!!!!!!!!!!!')
            else:
                logger.info('')
                logger.info('No file found for deletion!')
                logger.info('Script over')
        else:
            logger.error(' No module name found at the current directory : %s' % (module_dir))
            logger.error(' Script stopped...')
    except Exception as e:
        logger.exception(e)
        logger.info('# Script stopped...')
