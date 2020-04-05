""" Small example for pybind11 """

import argparse
import numpy as np
from ElementsKernel import Logging
from testCallBack import test

from ElementsKernel import Exit

def norm(x):
    if np.isscalar(x):
        return x
    else:
        return np.sqrt((np.asarray(x) ** 2).sum())


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
