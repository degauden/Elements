##
# @file: %(FILE)s
# @author: %(AUTHOR)s
# @date: %(DATE)s
##

import argparse
import ElementsKernel.Logging as log

def defineSpecificProgramOptions():
    
    parser = argparse.ArgumentParser()
    
    #
    # !!! Write your program options here !!!
    # e.g. parser.add_argument('--string-value', type=str, help='A string option')
    #
    
    return parser


def mainMethod(args):
    logger = log.getLogger('%(PROGRAMNAME)s')

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the %(PROGRAMNAME)s')
    logger.info('#')
    #
    # !!! Write you main code here !!! 
    #
