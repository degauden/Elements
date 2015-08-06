import argparse
import ElementsKernel.Logging as log


def defineSpecificProgramOptions():
    parser = argparse.ArgumentParser()
    parser.add_argument('--string-value', type=str, help='A string option')
    parser.add_argument('--int-value', type=int, help='An integer option')
    parser.add_argument('--f-value', type=float, help='A float option')
    parser.add_argument('--int-list', nargs='+', type=int, help='An int list')
    parser.add_argument(
        '--float-list', nargs='+', type=float, help='An float list')
    return parser


def mainMethod(args):
    logger = log.getLogger('PythonProgramExample')

    logger.info('#')
    logger.info(
        '#  Logging from the mainMethod() of the PythonProgramExample ')
    logger.info('#')

    try:
        logger.info('#')
        logger.info('# Adding the values of the int list')
        if not args.int_list:
            raise Exception('--int-list parameter missing')
        arg_sum = 0
        for i in args.int_list:
            logger.debug('Adding %d', i)
            arg_sum += i
        logger.info('Sum = %d', arg_sum)
    except Exception as e:
        logger.error('Something went wrong...')
        logger.exception(e)
