##
# @file: ElementsKernel/ElementsProjectCommonRoutines.py
# @author: Nicolas Morisset
#          Astronomy Department of the University of Geneva
#
# @date: 01/07/15
#
# This module offers some common routines used by scripts for creating C++ 
# projects, modules, classes etc..
##

import argparse
import os
import re
import shutil
import ElementsKernel.Logging as log

logger = log.getLogger('ElementsProjectCommonRoutines')

################################################################################

def isNameAndVersionValid(name, version):
    """
    Check that the <name> and <version> respect a regex
    """
    valid = True
    name_regex = '^[A-Za-z0-9][A-Za-z0-9_-]*$'
    if re.match(name_regex, name) is None:
        logger.error("# < %s %s > name not valid. It must follow this regex : < %s >"
                     % (name, version, name_regex))
        valid = False

    version_regex = '^\d+\.\d+(\.\d+)?$'
    if re.match(version_regex, version) is None:
        logger.error("# < %s %s > ,Version number not valid. It must follow this regex: < %s >"
                     % (name, version, version_regex))
        valid = False

    return valid

################################################################################

def eraseDirectory(directory):
    """
    Erase a directory and its contents from disk
    """
    shutil.rmtree(directory)
    logger.info('# <%s> Project erased!' % directory)
    
################################################################################

def getAuxPathFile(file_name):
    """
    Look for in path in the <ELEMENTS_AUX_PATH> environment variable where is
    located the <auxdir/file_name> file. It returns the filename with the path or
    an empty string if not found.
    """
    found = False
    full_filename = ''
    aux_dir = os.environ.get('ELEMENTS_AUX_PATH')
    if not aux_dir is None:
        for elt in aux_dir.split(os.pathsep):
            # look for the first valid path
            full_filename = os.path.sep.join([elt, 'templates', file_name])
            if os.path.exists(full_filename) and 'auxdir' in full_filename:
                found = True
                logger.debug(
                    "# Auxiliary directory for this file : <%s>" % full_filename)
                break

    if not found:
        full_filename = ''
        logger.error(
            "# Auxiliary directory NOT FOUND for this file : <%s>" % file_name)
        logger.error("# Auxiliary directory : <%s>" % aux_dir)

    return full_filename

################################################################################

def isAuxFileExist(aux_file):
    """
    Make sure auxiliary file exists
    """
    found = False
    aux_path_file = getAuxPathFile(aux_file)
    if aux_path_file:
        found = True

    return found

################################################################################

def getAuthor():
    """
    """
    try:
        author_str = os.environ['USER']
    except KeyError:
        author_str = ''

    return author_str
