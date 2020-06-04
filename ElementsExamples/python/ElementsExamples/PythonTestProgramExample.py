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
from ElementsKernel import Logging, Exit


def defineSpecificProgramOptions():
    """ Allows to define the (command line and configuration file) options specific to
    this program. See the ElementsProgram documentation for more details.

    :return: An  ArgumentParser.
    """
    parser = argparse.ArgumentParser()

    return parser


def mainMethod(args): #pylint: disable=unused-argument
    """ The "main" method.
    This method is the entry point to the program. In this sense, it is similar to a main
    (and it is why it is called mainMethod()). The code below contains the calls to the
    different classes created for the first developer's workshop. See the ElementsProgram
    documentation for more details.
    """
    logger = Logging.getLogger('TestProgramExample')
    logger.info('Entering TestProgramExample mainMethod()')

    logger.info('Exiting TestProgramExample mainMethod()')

    return Exit.Code["OK"]
