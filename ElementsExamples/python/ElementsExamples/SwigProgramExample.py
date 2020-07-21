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


""" Small example for a python based script 
"""

import argparse
import ElementsKernel.Logging as log
from ElementsKernel import Exit

from SwigExample import functionExample


def defineSpecificProgramOptions():
    """ Allows to define the (command line and configuration file) options specific to
    this program. See the ElementsProgram documentation for more details.
    
    :return: An  ArgumentParser.
    """
    parser = argparse.ArgumentParser()

    parser.add_argument('--int-option', type=int,
                        help='An int example option')

    return parser

def mainMethod(args):
    """ The "main" method.
    This method is the entry point to the program. In this sense, it is similar to a main
    (and it is why it is called mainMethod()). The code below contains the calls to the
    different classes created for the first developer's workshop. See the ElementsProgram 
    documentation for more details.
    """
    logger = log.getLogger('SwigProgramExample')
    logger.info('Entering SwigProgramExample mainMethod()')

    #
    #  Log some of the arguments
    #
    int_from_configuration = args.int_option
    if not int_from_configuration:
        int_from_configuration = 9
    logger.info('Example int : %d', int_from_configuration)
    func_result = functionExample(int_from_configuration)
    logger.info('functionExample(%d) : %d', int_from_configuration, func_result)

    logger.info('Exiting SwigProgramExample mainMethod()')

    return Exit.Code["OK"]
