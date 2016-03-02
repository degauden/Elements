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
File: python/ElementsKernel/RemoveCppClass.py

Created on: 02/10/16
Author: Nicolas Morisset

"""

from __future__ import division, print_function
from future_builtins import *

import argparse
import os
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.Logging as log

logger = log.getLogger('RemoveCppClass')

################################################################################

def getAllFiles(class_name, module_directory, module_name):
    """
    """
    delete_file_list=[]
    file_name_h = os.path.join(module_directory, module_name, class_name) + '.h'
    if os.path.exists(file_name_h):
        delete_file_list.append(file_name_h)
    file_name_cpp = os.path.join(module_directory, 'src', 'lib', class_name) + '.cpp'
    if os.path.exists(file_name_cpp):
        delete_file_list.append(file_name_cpp)
    file_name_test = os.path.join(module_directory, 'tests', 'src', class_name) + '_test.cpp'
    if os.path.exists(file_name_test):
        delete_file_list.append(file_name_test)

    return delete_file_list
           
################################################################################

def defineSpecificProgramOptions():
    description = """
    This script allows you to remove all files on disk related to a class name.
    Usually you use this script when you made a typo in the class name when 
    calling the <AddcppClass>.
    
    WARNING: The script can not remove things related to the class in the 
             <CMakeLists.txt> file. You MUST edit it and remove all unecessary
             stuff related to this class. 
    """
    from argparse import RawTextHelpFormatter

    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=RawTextHelpFormatter)
    parser.add_argument('class_name', metavar='class-name',
                        type=str, help='Class name')

    return parser

################################################################################

def mainMethod(args):

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the RemoveCppClass \
    script ')
    logger.info('#')

    try:
        # True: no error occured
        script_goes_on = True

        class_name = args.class_name

        # Default is the current directory
        module_dir = os.getcwd()

        logger.info('Current directory : %s', module_dir)

        # We absolutely need a Elements cmake file
        script_goes_on, module_name = epcr.isElementsModuleExist(module_dir)

        if script_goes_on:
            # Default is the current directory
            file_to_be_deleted = getAllFiles(class_name, module_dir, module_name)
            if file_to_be_deleted:
                epcr.removeFilesOnDisk(file_to_be_deleted)
                cmakefile = os.path.join(module_dir, 'CMakeLists.txt')
                logger.info('')
                logger.warning('# !!!!!!!!!!!!!!!')
                logger.warning(' Please remove all things related to the class name : %s ' % (class_name))
                logger.warning(' in the <CMakeLists.txt> file : %s ' % (cmakefile))
                logger.warning('# !!!!!!!!!!!!!!!')
            else:
                logger.info('')
                logger.info('No file found for deletion!')
            logger.info('Script over')
        else:
            logger.error(' No module name found at the current directory : %s' \
                         % (module_dir))
            logger.error(' Script stopped...')
    except Exception as e:
        logger.exception(e)
        logger.info('# Script stopped...')
