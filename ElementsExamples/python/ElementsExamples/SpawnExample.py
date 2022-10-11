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

""" Small example for a python based script using multiprocessing

"""

import argparse
import multiprocessing
import time
import ElementsKernel.Logging as log


def worker_func(i):

    logger = log.getLogger('SpawnTest')
    logger.info("Hello from %d", i)


def defineSpecificProgramOptions():
    """
    @brief Allows to define the (command line and configuration file) options
    specific to this program

    @details See the Elements documentation for more details.
    @return An  ArgumentParser.
    """

    parser = argparse.ArgumentParser()

    #
    # !!! Write your program options here !!!
    # e.g. parser.add_argument('--string-value', type=str, help='A string option')
    #

    return parser


def mainMethod(args):
    """
    @brief The "main" method.

    @details This method is the entry point to the program. In this sense, it is
    similar to a main (and it is why it is called mainMethod()).
    """

    logger = log.getLogger('SpawnTest')

    logger.info('#')
    logger.info('# Entering SpawnTest mainMethod()')
    logger.info('#')

    num_procs = 4
    time.sleep(2)

    with multiprocessing.get_context("spawn").Pool(num_procs) as p:
        p.map(worker_func, [i for i in range(num_procs)])

    logger.info('#')
    logger.info('# Exiting SpawnTest mainMethod()')
    logger.info('#')
