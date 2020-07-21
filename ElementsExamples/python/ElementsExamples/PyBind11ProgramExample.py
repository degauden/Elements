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


""" Small example for pybind11

"""

import argparse
import numpy as np

from ElementsKernel import Logging
from ElementsKernel import Exit

from testCallBack import test # pylint: disable=no-name-in-module

def norm(x):
    """ function to return the norm of a scalar or a vector
    """

    n = 0

    if np.isscalar(x):
        n = x
    else:
        n = np.sqrt((np.asarray(x) ** 2).sum())

    return n

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
    logger = Logging.getLogger('PyBind11ProgramExample')
    logger.info('Entering PyBind11ProgramExample mainMethod()')

    logger.info('Testing callback with an input tuple')

    x = [1., 2.]
    test(x, norm)

    logger.info('Testing callback with an input array')

    x = np.array([1., 2.])
    test(x, norm)

    logger.info('Exiting PyBind11ProgramExample mainMethod()')

    return Exit.Code["OK"]
