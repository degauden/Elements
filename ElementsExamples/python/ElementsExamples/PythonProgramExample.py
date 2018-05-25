""" Small example for a python based script """

import argparse
from ElementsKernel import Logging
from ElementsKernel.Program import str_to_bool
from ElementsExamples.PythonModuleExample import ClassExample


def myLocalLogTestFunc():

    logger = Logging.getLogger()
    logger.info("Test of Message")

    logger2 = Logging.getLogger("TestLogger")
    logger2.info("Test2 of Message")

    logger3 = Logging.getLogger(__name__)
    logger3.info("Test3 of Message")


def defineSpecificProgramOptions():
    """
    @brief Allows to define the (command line and configuration file) options specific to
    this program

    @details
        See the ElementsProgram documentation for more details.
    @return
        An  ArgumentParser.
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

    return parser


def mainMethod(args):
    """
    @brief The "main" method.
    @details This method is the entry point to the program. In this sense, it is similar to a main
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
    logger.info('Example string : ' + string_from_configuration)
    second_element = args.int_list_option[1]
    logger.info('Second elements from the list : ' + str(second_element))

    #
    #  Calling a static method
    #
    result = ClassExample.product(3, 4)
    logger.info('Static method result : ' + str(result))

    #
    #  Calling the constructor and a method member
    #
    example_object = ClassExample(args.int_list_option)
    logger.info('Sum of the list elements : ' + str(example_object.sumListValues()))

    logger.info('Exiting ProgramExample mainMethod()')

    return 0
