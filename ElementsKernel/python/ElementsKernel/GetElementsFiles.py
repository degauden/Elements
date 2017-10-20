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
File: python/ElementsKernel/GetElementsFiles.py

Created on: 10/10/17
Author: hubert
"""

from __future__ import division, print_function
import sys
if sys.version_info[0] < 3:
    from future_builtins import *

import argparse
import ElementsKernel.Logging as log

def defineSpecificProgramOptions():
    """
    @brief Allows to define the (command line and configuration file) options
    specific to this program

    @details
        See the Elements documentation for more details.
    @return
        An  ArgumentParser.
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('-s', '--self', default=False, action="store_true",
                        help='Get only the files from the current project. No recursion.')

    parser.add_argument('-v', '--variable', default="PATH",
                        help='The variable to search in [default: PATH]')


    #
    # !!! Write your program options here !!!
    # e.g. parser.add_argument('--string-value', type=str, help='A string option')
    #

    return parser


def mainMethod(args):
    """
    @brief The "main" method.
    @details
        This method is the entry point to the program. In this sense, it is
        similar to a main (and it is why it is called mainMethod()).
    """

    logger = log.getLogger('GetElementsFiles')

    logger.info('#')
    logger.info('# Entering GetElementsFiles mainMethod()')
    logger.info('#')

    # !! Getting the option from the example option in defineSpecificProgramOption
    # !! e.g string_option = args.string_value

    #
    # !!! Write you main code here !!!
    #

    logger.info('#')
    logger.info('# Exiting GetElementsFiles mainMethod()')
    logger.info('#')
