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
from ElementsKernel import Logging
from ElementsKernel.Program import str_to_bool
from ElementsExamples.PythonModuleExample import ClassExample

from ElementsKernel import Exit


def myLocalLogTestFunc():
    """ example of logging from a function
    """

    logger = Logging.getLogger()
    logger.info("Test of Message")

    logger2 = Logging.getLogger("TestLogger")
    logger2.info("Test2 of Message")

    logger3 = Logging.getLogger(__name__)
    logger3.info("Test3 of Message")


def defineSpecificProgramOptions():
    """ Allows to define the (command line and configuration file) options specific to
    this program. See the ElementsProgram documentation for more details.

    :return: An  ArgumentParser.
    """
    parser = argparse.ArgumentParser()

    parser.add_argument(
        '--string-option',
        type=str,
        help='A string example option'
    )

    parser.add_argument(
        '--int-option',
        type=int,
        default=111,
        help='An int example option'
    )

    parser.add_argument(
        '--int-option-with-default-and-default-in-conf',
        type=int,
        default=222,
        help='A string example option'
    )

    parser.add_argument(
        '--int-option-with-default-no-default-in-conf',
        type=int,
        default=444,
        help='A string example option'
    )

    parser.add_argument(
        '--int-option-no-default-not-defined-in-conf',
        type=int,
        help='A string example option'
    )

    parser.add_argument(
        '--int-option-with-no-defaults-anywhere',
        type=int,
        help='A string example option'
    )

    parser.add_argument(
        '--float-option',
        type=float,
        default=99999.0,
        help='A float option'
    )

    parser.add_argument(
        '--int-list-option',
        nargs='+',
        type=int,
        default=[-1, -2, 9],
        help='A integer list option'
    )

    parser.add_argument(
        '--overwrite',
        nargs='?',
        dest='overwrite',
        default=False,
        const=True,
        type=str_to_bool,
        help='Overwrite the output files'
    )

    parser.add_argument(
        '--threshold', '-t',
        type=float,
        default=0.5,
        help="signal threshold")

    return parser


def mainMethod(args):
    """ The "main" method.
    This method is the entry point to the program. In this sense, it is similar to a main
    (and it is why it is called mainMethod()). The code below contains the calls to the
    different classes created for the first developer's workshop

        See the ElementsProgram documentation for more details.
    """
    logger = Logging.getLogger('ProgramExample')
    logger.info('Entering ProgramExample mainMethod()')

    #
    #  function with log messages
    #
    myLocalLogTestFunc()

    #
    #  Log some of the arguments
    #
    string_from_configuration = args.string_option
    logger.info('Example string : %s', string_from_configuration)
    second_element = args.int_list_option[1]
    logger.info('Second elements from the list : %s', str(second_element))

    logger.info("the int_option value %d", args.int_option)
    logger.info("the threshold value %.1f", args.threshold)

    #
    #  Calling a static method
    #
    result = ClassExample.product(3, 4)
    logger.info('Static method result : %s', str(result))

    #
    #  Calling the constructor and a method member
    #
    example_object = ClassExample(args.int_list_option)
    logger.info('Sum of the list elements : %s', str(example_object.sumListValues()))

    logger.info('Exiting ProgramExample mainMethod()')

    return Exit.Code["OK"]
