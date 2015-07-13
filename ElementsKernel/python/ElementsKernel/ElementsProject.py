
import argparse
import os
import sys
import re
import shutil

import ElementsKernel.Logging as log

logger = log.getLogger('CreateElementsProject')

AUX_CMAKE_LIST_IN = "CMakeList.txt.in"
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
        logger.error("<%s> name not valid. It must follow this regex : < %s >" % (name, name_regex))
        valid = False

    version_regex = '^\d+?\.\d+?$'
    if re.match(version_regex, version) is None:
        logger.error("Version number not valid. It must follow this regex: < %s >" % version_regex)    
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
        
def copyAuxFile(aux_dir, destination, file):
    """
    Copy all necessary auxiliary data to the <destination> directory
    """
    logger.info('# Copying AUX file : %s' % file)
    shutil.copy( os.path.join(os.path.sep, aux_dir, file ), os.path.join(os.path.sep, destination, file))
    
def eraseDirectory(from_directory):
    """
    Erase the directory from disk
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
    # Format all dependant projects
    if not dep_projects is None:
        str_dep_projects = ' '.join(' '.join(s) for s in dep_projects)
    else:
        str_dep_projects = ' '
    new_data = data % { "PROJECT_NAME" : proj_name, "PROJECT_VERSION" : proj_version, "DEPENDANCE_LIST": str_dep_projects }
    f.close()
    # Save new data
    f = open(cmake_list_file, 'w')
    f.write(new_data)
    f.close()
    os.rename(cmake_list_file, cmake_list_file.replace('.in','')) 
         
def createProject(aux_path, project_dir, proj_name, proj_version, dep_projects):
    """
    Create the project structure
    """
    logger.info('# Creating the project')
    
    # Create project
    os.makedirs(project_dir)
       
    copyAuxFile(aux_path, project_dir, AUX_CMAKE_LIST_IN)
    copyAuxFile(aux_path, project_dir, AUX_CMAKE_FILE_IN)

    substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects)

def defineSpecificProgramOptions():
    usage = """
            PROG project_name project_version 
                 [-d dependency_project dependency_version]
                 [-p] project path
                 [-h]
            
            e.g. CreateElementsProject MyProject 1.0 -d Alexandria 2.0 -d PhosphorosCore 3.0 
            
            This script creates an <Elements> project in your current directory
            by default. It means all the necessary structure (directory structure,
            makefiles etc...) will be automatically created for you. 
            Use the <-p> option if you want to install your project somewhere else.
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
        script_goes_on   = True
        proj_name        = args.project_name
        proj_version     = args.project_version       
        destination_path = setPath(args.path)
 
        # Check project name and version   
        script_goes_on = isNameAndVersionValid(proj_name, proj_version)
                       
        # Set the project directory
        aux_path    = os.getenv("ELEMENTS_AUX_PATH")
        project_dir = os.path.join(os.path.sep, destination_path, proj_name, proj_version)
        
        # Make sure dependencies name and version are valid 
        if not args.dep_project_version is None:
             script_goes_on = isDependencyProjectValid(args.dep_project_version)

        if script_goes_on and os.path.exists(project_dir):
            # Ask user
            logger.warning('<%s> project ALREADY exists!!!' % project_dir)
            response_key = raw_input('Do you want to replace the existing project and associated module(s) (Yes/No, default: No)?')
            if response_key =="YES" or response_key =="yes" or response_key =="y":
                logger.info('# Replacing the existing project: <%s>' % project_dir)
                eraseDirectory(project_dir) 
            else:
                script_stopped = True
                logger.info('# Script stopped by user!')
        
        if script_goes_on:       
            logger.info('# Creating project: <%s>' % project_dir )             
            createProject(aux_path, project_dir, proj_name, proj_version, args.dep_project_version)
            logger.info('# <%s> project successfully created.' % project_dir )             
        
    except Exception as e:
        logger.exception(e)
        logger.error('# Script aborted...')
        
