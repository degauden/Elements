""" Small example for a python based script """

import argparse
from ElementsKernel import Logging, Exit


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

    return parser


def mainMethod(args):
    """
    @brief The "main" method.
    @details This method is the entry point to the program. In this sense, it is similar to a main
    (and it is why it is called mainMethod()). The code below contains the calls to the
    different classes created for the first developer's workshop

        See the ElementsProgram documentation for more details.
    """
    logger = Logging.getLogger('TestProgramExample')
    logger.info('Entering TestProgramExample mainMethod()')

    logger.info('Exiting TestProgramExample mainMethod()')

    return Exit.Code["OK"]
