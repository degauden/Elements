#!/usr/bin/env python

##
# @file: scripts/CreateElementsModule.py
# @author: Nicolas Morisset  
#          Astronomy Department of the University of Geneva 
#          Nicolas.Morisset@unige.ch
# @date: 01/07/15
#
# This script will create a new <Elements> project

import argparse
import numpy as np
import os
import re
import shutil
import ELEMENTS_VERSION
import ElementsKernel.Logging as log

logger = log.getLogger('CreateElementsProject')

AUX_CMAKE_LIST_IN = "CMakeLists.txt.in"
AUX_CMAKE_FILE_IN = "Makefile"

def setPath(path_defined_by_user):
    """
    Set the directory path
    """     
    # Set the path if any
    destination_path = ''
    if not path_defined_by_user is None:
        destination_path = path_defined_by_user
    else:
        destination_path = os.getcwd()
    
    return destination_path     

def isNameAndVersionValid(name, version):
    """
    Check that the <name> and <version> respect a regex
    """
    valid = True
    name_regex='^[A-Za-z0-9][A-Za-z0-9_-]*$' 
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

  
def isDependencyProjectValid(str_list):
    """
    Check if the dependency project name and version list is valid
    """
    valid = True
    for i in range(len(str_list)):
        if not isNameAndVersionValid(str_list[i][0], str_list[i][1]):
            valid = False
            break
            
                 
    return valid

def duplicate_elements(duplicate_list):
    """
    """
    name_list = []
    not_found_duplicate = True
    for elt in duplicate_list:
        if not elt[0] in name_list:
             name_list.append(elt[0])
        else:
            logger.error('# Found twice the following dependency : <%s>, script aborted' % elt[0]) 
            not_found_duplicate = False
            break     
                
    return not_found_duplicate
    
def getElementsVersion():
    """
    Get the elements version
    """
    patch_version = ELEMENTS_VERSION.ELEMENTS_PATCH_VERSION
    version       = [ str(ELEMENTS_VERSION.ELEMENTS_MAJOR_VERSION), 
                      str(ELEMENTS_VERSION.ELEMENTS_MINOR_VERSION)]
    version_patch = [ str(ELEMENTS_VERSION.ELEMENTS_MAJOR_VERSION), 
                      str(ELEMENTS_VERSION.ELEMENTS_MINOR_VERSION),
                      str(patch_version)]
    
    elt_version = '.'.join(version)
    if patch_version > 0:
        elt_version = '.'.join(version_patch)
        
    logger.info('# Elements version found : <%s>' % elt_version) 
        
    return str(elt_version)  

      
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
            full_filename = os.path.sep.join([elt, file_name])
            if os.path.exists(full_filename) and 'auxdir' in full_filename:
                found = True
                logger.info("# Auxiliary directory for this file : <%s>" % full_filename)
                break 
        
    if not found:
        full_filename =''
        logger.error("# Auxiliary directory NOT FOUND for this file : <%s>" % file_name)
        logger.error("# Auxiliary directory : <%s>" % aux_dir)
                                                                     
    return full_filename  

def isAuxFileExist(aux_file):
    """
    Make sure auxialiary is found
    """
    found = False
    aux_path_file = getAuxPathFile(aux_file)
    if aux_path_file:
            found = True
                
    return found

     
def copyAuxFile(destination, file_name):
    """
    Copy all necessary auxiliary data to the <destination> directory
    """
    scripts_goes_on = True
    
    aux_path_file = getAuxPathFile(file_name)
    if aux_path_file:
         logger.info('# Copying AUX file : %s' % file_name)
         shutil.copy(aux_path_file, os.path.join(os.path.sep, destination, file_name))
    else:
        scripts_goes_on = False
        
    return scripts_goes_on


def eraseDirectory(from_directory):
    """
    Erase a directory and its contents from disk
    """
    shutil.rmtree(from_directory)  
    logger.info('# <%s> Project erased!' % from_directory)

def substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects):
    """
    Substitute variables in CMakeList.txt.in file and rename the file to 
    CMakeList.txt.
    """
    logger.info('# substitute variables in <%s> file' % AUX_CMAKE_LIST_IN) 
    cmake_list_file = os.path.join(os.path.sep, project_dir, AUX_CMAKE_LIST_IN )
    
    # Substitute    
    f = open(cmake_list_file, 'r')
    data = f.read()
    # Format all dependent projects
    # We put by default Elements dependency if no one is given
    str_dep_projects = ''
    if not dep_projects is None:
        str_dep_projects = ' '.join(' '.join(s) for s in dep_projects)
    else:
        str_dep_projects = ''.join(['Elements ', getElementsVersion()])
        
    new_data = data % { "PROJECT_NAME"    : proj_name, 
                        "PROJECT_VERSION" : proj_version, 
                        "DEPENDANCE_LIST" : str_dep_projects }
    f.close()
    # Save new data
    f = open(cmake_list_file, 'w')
    f.write(new_data)
    f.close()
    os.rename(cmake_list_file, cmake_list_file.replace('.in','')) 
         
def createProject(project_dir, proj_name, proj_version, dep_projects):
    """
    Create the project structure
    """
    logger.info('# Creating the project')
    
    # Create project
    os.makedirs(project_dir)
       
    copyAuxFile(project_dir, AUX_CMAKE_LIST_IN)
    copyAuxFile(project_dir, AUX_CMAKE_FILE_IN)

    substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects)
        
        
def defineSpecificProgramOptions():
    usage = """
PROG project_name project_version 
     [-d dependency_project dependency_version]
     [-p] project path
     [-h]

e.g. CreateElementsProject MyProject 1.0 -d Alexandria 2.0 -d 
     PhosphorosCore 3.0 

This script creates an <Elements> project in your current directory
by default. It means all the necessary structure (directory 
structure, makefiles etc...) will be automatically created for you. 
Use the <-p> option if you want to install your project somewhere 
else.
Use the [-d] option if your project has some dependencies to other
project(s). 
            """
    parser = argparse.ArgumentParser(usage=usage)
    parser.add_argument('project_name', metavar='project-name', type=str, help='Project name')
    parser.add_argument('project_version', metavar='project-version', type=str, help='Project version number')
    parser.add_argument('-d','--dep-project-version', nargs=2, action='append', type=str , help='Dependency project name and its version number e.g "project_name 0.1"')
    parser.add_argument('-p','--path', type=str , help='Installation path(default : current directory)')
    return parser


def mainMethod(args):

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the CreateElementsProject script ')
    logger.info('#')
    
    try:
        script_goes_on     = True
        proj_name          = args.project_name
        proj_version       = args.project_version 
        dependant_projects = args.dep_project_version      
        destination_path   = setPath(args.path)
 
        # Check project name and version   
        script_goes_on = isNameAndVersionValid(proj_name, proj_version)

        # Check for duplicate dependencies
        if script_goes_on and not args.dep_project_version is None:
            script_goes_on = duplicate_elements(dependant_projects)
        
        # Check aux files exist
        if script_goes_on:
             script_goes_on = isAuxFileExist(AUX_CMAKE_LIST_IN)
        if script_goes_on:
             script_goes_on = isAuxFileExist(AUX_CMAKE_FILE_IN)
                    
        # Set the project directory
        project_dir = os.path.join(os.path.sep, destination_path, proj_name, proj_version)

        # Make sure dependencies name and version are valid 
        if script_goes_on and not args.dep_project_version is None:
            script_goes_on = isDependencyProjectValid(args.dep_project_version)

        if script_goes_on and os.path.exists(project_dir):
            logger.warning('<%s> project ALREADY exists!!!' % project_dir)
            response_key = raw_input('Do you want to replace the existing project and associated module(s) (Yes/No, default: No)?')
            if response_key =="YES" or response_key =="yes" or response_key =="y":
                logger.info('# Replacing the existing project: <%s>' % project_dir)
                eraseDirectory(project_dir) 
            else:
                script_goes_on = False
                logger.info('# Script stopped by user!')
                
        if script_goes_on:       
             createProject(project_dir, proj_name, proj_version, dependant_projects)
             logger.info('# <%s> project successfully created.' % project_dir )             
                 
    except Exception as e:
        logger.exception(e)
        logger.error('# Script aborted...')
        
