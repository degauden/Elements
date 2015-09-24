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

import os
import re
import shutil
import ElementsKernel.Logging as log

logger = log.getLogger('ElementsProjectCommonRoutines')

CMAKE_LISTS_FILE    = 'CMakeLists.txt'

################################################################################

def makeDirectory(directory_path):
    """
    Create a directory on disk if any
    """
    if not os.path.exists(directory_path):
        try:
            os.makedirs(directory_path)
        except OSError as e:
            raise e
                

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
    logger.info('# <%s> directory erased!' % directory)
    
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
                break

    if not found:
        full_filename = ''
        logger.error(
            "# Auxiliary directory NOT FOUND for this file : <%s>" % file_name)
        logger.error("# Auxiliary directory : <%s>" % aux_dir)

    return full_filename

################################################################################

def copyAuxFile(destination, aux_file_name):
    """
    Copy all necessary auxiliary data to the <destination> directory
    """
    scripts_goes_on = True

    aux_path_file = getAuxPathFile(aux_file_name)
    if aux_path_file:
        shutil.copy(aux_path_file, os.path.join(destination,aux_file_name))
    else:
        scripts_goes_on = False

    return scripts_goes_on

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
    Get the contents of the use environment variables
    """
    try:
        author_str = os.environ['USER']
    except KeyError:
        author_str = ''

    return author_str

################################################################################
    
def isElementsModuleExist(module_directory):
    """
    """
    found_keyword = True
    module_name = ''
    cmake_file = os.path.join(module_directory, CMAKE_LISTS_FILE)
    if not os.path.isfile(cmake_file):
        found_keyword = False
        logger.error('# %s cmake module file is missing! Are you inside a ' \
        'module directory?' % cmake_file)
    else:
        # Check the make file is an Elements cmake file
        # it should contain the string : "elements_project"
        f = open(cmake_file, 'r')
        for line in f.readlines():
            if 'elements_subdir' in line:
                posStart = line.find('(')        
                posEnd = line.find(')')        
                module_name = line[posStart+1:posEnd]
        f.close()
                
        if not module_name:
            logger.error('# Can not find the module name in the <%s> file!' % cmake_file)
            logger.error('# Maybe you are not in the expected directory...')
            found_keyword = False
    
    return found_keyword, module_name

################################################################################

def isFileAlreadyExist(path_filename, name):
    """
    Check if the program file does not already exist
    """
    script_goes_on = True
    if os.path.exists(path_filename):
        script_goes_on = False
        logger.error('# The <%s> name already exists! ' % name)
        logger.error('# as the file has been found: <%s>! ' % path_filename)

    return script_goes_on

################################################################################
