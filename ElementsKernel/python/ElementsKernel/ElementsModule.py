#!/usr/bin/env python

##
# @file: scripts/CreateElementsModule.py
# @author: Nicolas Morisset  
#          Astronomy Department of the University of Geneva 
#          Nicolas.Morisset@unige.ch
# @date: 01/07/15
#
# This script will create a new <Elements> module  inside an existing project

import argparse
import os
import re
import shutil
import ElementsKernel.ElementsProject as ep
import ElementsKernel.Logging as log

logger = log.getLogger('CreateElementsModule')
       
AUX_CMAKELIST_MOD_IN = "CMakeList.txt.mod.in"

def substituteModuleVariables(module_dir, module_name):
    """
    Substitute variables in CMakeList.txt.mod.in file and rename it to 
    CMakeList.txt.
    """
    logger.info('# Substitute variables in <%s> file' % AUX_CMAKELIST_MOD_IN) 
    cmake_list_file = os.path.join(os.path.sep, module_dir, AUX_CMAKELIST_MOD_IN )
    # Substitute    
    f = open(cmake_list_file, 'r')
    data = f.read()
    f.close()
    # Substitute
    new_data = data % dict({ "MODULE_NAME" : module_name })
    # Save new data
    f = open(cmake_list_file, 'w')
    f.write(new_data)
    f.close()
    os.rename(cmake_list_file, cmake_list_file.replace('.mod.in','')) 

def createPythonStuff(mod_path, module_name):
    """
    Create the directory structure for python
    """
    logger.info('# Creating the python directories ')
    os.makedirs(os.path.join(os.path.sep, mod_path, 'python', module_name)) 
    os.makedirs(os.path.join(os.path.sep, mod_path, 'scripts')) 
    os.makedirs(os.path.join(os.path.sep, mod_path, 'tests', 'python')) 

def createModuleDirectories(mod_path, module_name):
    """
    Create the directory structure for the module
    """
    # Create module directories
    logger.info('# Creating the module directories')
    os.makedirs(mod_path) 
    os.makedirs(os.path.join(os.path.sep, mod_path, module_name)) 
    os.makedirs(os.path.join(os.path.sep, mod_path, 'src','lib')) 
    os.makedirs(os.path.join(os.path.sep, mod_path, 'src','program')) 
    os.makedirs(os.path.join(os.path.sep, mod_path,'tests', 'src')) 
    os.makedirs(os.path.join(os.path.sep, mod_path, 'doc')) 
      
   
def createModule(aux_path, project_dir, module_name, module_version, add_python):
    """
    Create a module, copy auxiliary files and substitute variables in the
    CMakefile.txt file
    """
    script_stopped = False
    # Create module directory
    mod_path = os.path.join(os.path.sep, project_dir, module_name)
    logger.info('# Creating the module: <%s> ' % mod_path)
    if os.path.exists(mod_path):
        # Ask user
        logger.warning('<%s> module ALREADY exists!!!' % module_name)
        response_key = raw_input('Do you want to replace the existing module (Yes/No, default: No)?')
        if response_key =="YES" or response_key =="yes" or response_key =="y":
            logger.info('# Replacing the existing module: <%s>' % module_name)
            ep.eraseDirectory(mod_path) 
        else:
            script_stopped = True

    if not script_stopped:
        createModuleDirectories(mod_path, module_name)
        if add_python:        
            createPythonStuff(mod_path, module_name)
            
        ep.copyAuxFile(aux_path, mod_path, AUX_CMAKELIST_MOD_IN)
        substituteModuleVariables(mod_path, module_name)
    
    return script_stopped
    
                
def defineSpecificProgramOptions():
    usage = """ 
            PROG [-h] module_name module_version [-pv project_name project_version] 
                 [--path] project path [-py] 
            
            e.g. CreateElementsModule MyModule 1.0
            
            This script creates an <Elements> module at your current directory.
            It means all the necessary structure (directory structure, makefiles
            etc...) will be automatically created for you. If you need to put 
            your module inside a project, use the [-pv] option. It will put your
            module inside the project in your current directory. The [--path] 
            option specifies the path of your project or where you want the 
            module to be installed.             
            If you need the python structure inside a module use the [-py] option.
            
           """
    parser = argparse.ArgumentParser(usage)
    parser.add_argument('module_name', metavar='module-name', type=str, help='Module name')
    parser.add_argument('module_version', metavar='module-version', type=str, help='Module version number')
    parser.add_argument('-pv','--project-version', nargs=2, type=str , help='Project name and its version number e.g "project_name 0.1"')
    parser.add_argument('-py','--python-stuff', action='store_true', help='Add python directories')
    parser.add_argument('--path', type=str , help='Installation path(default : current directory)')
    return parser


def mainMethod(args):
    
    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the CreateElementsModule script ')
    logger.info('#')
    
    try:
        script_stopped = False
        
        module_name      = args.module_name
        module_version   = args.module_version
        
        project_name     = ''
        project_version  = ''
        if not args.project_version is None:
             project_name     = args.project_version[0]
             project_version  = args.project_version[1]
             
        destination_path = ep.setPath(args.path)
        add_python       = args.python_stuff
        
        script_stopped = ep.isNameAndVersionValid(module_name, module_version)
        if project_name and project_version:
             script_stopped = ep.isNameAndVersionValid(project_name, project_version)
       
        aux_path      = os.getenv("ELEMENTS_AUX_PATH")
                    
        project_dir = os.path.join(os.path.sep, destination_path, project_name, project_version)
        if os.path.exists(project_dir):
            if not createModule(aux_path, project_dir, module_name, module_version, add_python):            
                 logger.info('# <%s> module successfully created in <%s>.' % (module_name, project_dir))
            else:
                 logger.info("Script stopped by user!")
     
        else:
             logger.error('<%s> project does not exist!!! Script aborted' % project_dir)
                   
    except Exception as e:
        logger.exception(e)
        logger.info('# Script stopped...')
       
