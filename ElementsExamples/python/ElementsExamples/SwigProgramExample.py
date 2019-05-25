""" Small example for a python based script """

import argparse
import ElementsKernel.Logging as log
from ElementsKernel import Exit

from SwigExample import functionExample


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

    parser.add_argument('--int-option', type=int,
                        help='An int example option')

    return parser

def mainMethod(args):
    """
    @brief The "main" method.
    @details This method is the entry point to the program. In this sense, it is similar to a main
    (and it is why it is called mainMethod()). The code below contains the calls to the
    different classes created for the first developer's workshop

        See the ElementsProgram documentation for more details.
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

    return Exit.Code.OK
