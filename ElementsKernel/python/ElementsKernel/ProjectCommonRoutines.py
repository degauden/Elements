"""
file: ElementsKernel/ProjectCommonRoutines.py
author: Nicolas Morisset
         Astronomy Department of the University of Geneva

date: 01/07/15

This module offers some common routines used by scripts for creating C++
projects, modules, classes etc..
"""

import os
import re
import shutil
import ElementsKernel.Logging as log

logger = log.getLogger('ProjectCommonRoutines')

CMAKE_LISTS_FILE = 'CMakeLists.txt'

################################################################################

def removeFilesOnDisk(file_list): 
    """
    Remove all files on hard drive from the <file_list> list .  
    """
    for elt in file_list:
        logger.info('File deleted : %s' % elt)
        deleteFile(elt) 

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

def deleteFile(path_filename):
    """
    Delete the <path_filename> file if it does exist. <path_filename> includes
    the path and filename.
    """
    if os.path.exists(path_filename):
        os.remove(path_filename)

################################################################################

def makeACopy(cmakefile):
    """
    Make a copy(backup) of the <CMakeFileLists.txt> file. The copy is named
    <CMakeFileLists.txt~>, <cmakefile> includes the path of the file.
    """
    copy_file = cmakefile + '~'
    if os.path.exists(cmakefile):
        shutil.copy(cmakefile, copy_file)
    else:
        logger.warning('# File not found: <%s> Can not make a copy of this file!'
                        % cmakefile)

################################################################################

def isNameAndVersionValid(name, version):
    """
    Check that the <name> and <version> respect a regex
    """
    valid = True
    name_regex = "^[A-Za-z0-9][A-Za-z0-9_-]*$"
    if re.match(name_regex, name) is None:
        logger.error("# < %s %s > name not valid. It must follow this regex : < %s >"
                     % (name, version, name_regex))
        valid = False

    version_regex = "^\\d+\\.\\d+(\\.\\d+)?$"
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
    Look for the <auxdir> path in the <ELEMENTS_AUX_PATH> environment variable 
    where is located the <auxdir/file_name> file. It returns the filename with 
    the path or an empty string if not found.
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
        logger.error("# Auxiliary file NOT FOUND  : <%s>" % full_filename)
        full_filename = ''

    return full_filename

################################################################################

def copyAuxFile(destination, aux_file_name):
    """
    Copy the <aux_file_name> file to the <destination> directory.
    <aux_file_name> is just the name without path
    """
    scripts_goes_on = True
    aux_path_file = getAuxPathFile(aux_file_name)
    if aux_path_file:
        shutil.copy(aux_path_file, os.path.join(destination, aux_file_name))
    else:
        scripts_goes_on = False

    return scripts_goes_on

################################################################################

def isAuxFileExist(aux_file_name):
    """
    Make sure the <aux_file> auxiliary file exists. 
    <aux_file> is just the name without the path.
    """
    found = False
    aux_path_file = getAuxPathFile(aux_file_name)
    if aux_path_file:
        found = True

    return found

################################################################################

def getAuthor():
    """
    Get the contents of the <USER> environment variables
    """
    try:
        author_str = os.environ['USER']
    except KeyError:
        author_str = ''

    return author_str

################################################################################

def isElementsModuleExist(module_directory):
    """
    Get the module name in the <CMAKE_LISTS_FILE> file
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
                pos_start = line.find('(')
                pos_end = line.find(')')
                module_name = line[pos_start + 1:pos_end]
        f.close()

        if not module_name:
            logger.error('# Module name not found in the <%s> file!' % cmake_file)
            logger.error('# Maybe you are not in a module directory...')
            found_keyword = False

    return found_keyword, module_name

################################################################################

def isFileAlreadyExist(path_filename, name):
    """
    Check if the <path_filename> file does not already exist
    <path_filename> : path + filename
    """
    script_goes_on = True
    if os.path.exists(path_filename):
        script_goes_on = False
        logger.error('# The <%s> name already exists! ' % name)
        logger.error('# File found here : <%s>! ' % path_filename)

    return script_goes_on

################################################################################

################################################################################

def createPythonInitFile(init_path_filename):
    """
    Create on disk the __init__.py python file
    """
    if not os.path.exists(init_path_filename):
        f = open(init_path_filename, 'w')
        f.write("from pkgutil import extend_path\n")
        f.write("__path__ = extend_path(__path__, __name__)\n")
        f.close()
