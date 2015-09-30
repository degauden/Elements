##
# @file: %(FILE)s
# @author: %(AUTHOR)s
# @date: %(DATE)s
##

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

    logger = log.getLogger('%(PROGRAMNAME)s')
    
    logger.info('#')
    logger.info('# Entering %(PROGRAMNAME)s mainMethod()')
    logger.info('#')
    
    # !! Getting the option from the example option in defineSpecificProgramOption 
    # !! e.g string_option = args.string_value
    
    #
    # !!! Write you main code here !!! 
    #
    
    logger.info('Exiting %(PROGRAMNAME)s mainMethod()')