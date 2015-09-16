##
# @file: ElementsKernel/ElementsAddCppClass.py
# @author: Nicolas Morisset
#          Astronomy Department of the University of Geneva
#
# @date: 01/07/15
#
# This script will create a new Elements C++ Class
##

import argparse
import os
import re
import shutil
import time
import ElementsKernel.ElementsProjectCommonRoutines as epcr
import ElementsKernel.parseCmakeLists as pcl
import ElementsKernel.Logging as log

logger = log.getLogger('AddCppClass')

# Define constants
CMAKE_LISTS_FILE    = 'CMakeLists.txt'
CMAKE_LISTS_FILE_IN = 'CMakeLists.txt.mod.in'
H_TEMPLATE_FILE     = 'ClassName_template.h'
CPP_TEMPLATE_FILE   = 'className_template.cpp'


################################################################################
    
def getClassName(str_subdir_class):
    """
    Get the class name and sub directory if any
    """
    name_list = str_subdir_class.split(os.path.sep)
    className = name_list[-1]
    subdir = str_subdir_class.replace(className,'')
    logger.info('# Class name: %s' % className)
    logger.info('# Sub directory: %s' % subdir)
    return subdir, className

################################################################################
    
def isElementsModuleExist(module_dir):
    """
    """
    found_keyword = True
    module_name = ''
    cmake_file = os.path.join(os.path.sep, module_dir, CMAKE_LISTS_FILE)
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
            logger.error('# Can not find the module name in %s!' % cmake_file)
            found_keyword = False
        else:
            logger.info('# Module name found : %s' % module_name)
    
    return found_keyword, module_name

################################################################################

def createDirectories(module_dir, module_name, subdir):
    """
    Create directories needed for a module
    """
    logger.info('# Creating the directories ')
    # Create Directories
    module_path = os.path.join(os.path.sep, module_dir, module_name, subdir)
    if not os.path.exists(module_path):
        os.makedirs(module_path)
    src_lib_path = os.path.join(os.path.sep, module_dir, 'src/lib', subdir)
    if not os.path.exists(src_lib_path):
        os.makedirs(src_lib_path)
    test_path = os.path.join(os.path.sep, module_dir, 'tests/src', subdir)
    if not os.path.exists(test_path):
        os.makedirs(test_path)

################################################################################

def copyAuxFile(destination, file_name):
    """
    Copy all necessary auxiliary data to the <destination> directory
    """
    scripts_goes_on = True

    aux_path_filename = epcr.getAuxPathFile(file_name)
    if aux_path_filename:
        logger.info('# Copying AUX file : %s' % file_name)
        shutil.copy(aux_path_filename, os.path.join(os.path.sep, destination,
                                                        file_name))
    else:
        scripts_goes_on = False

    return scripts_goes_on

################################################################################
       
def substituteStringsInDotH(path, class_name, module_name):
    """
    Substitute variables in template file and rename the file
    """
    logger.info('# Substitute variables in <%s> file' % H_TEMPLATE_FILE)
    full_file_name = os.path.join(os.path.sep, path, H_TEMPLATE_FILE)
    # Substitute strings in h_template_file
    f = open(full_file_name, 'r')
    data = f.read()
    # Format all dependent projects
    # We put by default Elements dependency if no one is given
    date_str = time.strftime("%x")
    author_str = epcr.getAuthor()
    # Make some substitutions
    full_file_name_str = full_file_name.replace(H_TEMPLATE_FILE, class_name+'.h')
    define_words_str = '_' + full_file_name
    define_words_str = define_words_str.replace(H_TEMPLATE_FILE, class_name+'.h')
    define_words_str = define_words_str.replace('.','_')
    define_words_str = (define_words_str.replace(os.path.sep,'_')).upper()
    new_data = data % {"FILE": full_file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "DEFINE_WORDS": define_words_str,
                       "CLASSNAME": class_name,
                       "MODULENAME": module_name}

    f.close()
    # Save new data
    h_file_name = full_file_name.replace(H_TEMPLATE_FILE, class_name + '.h')
    f = open(h_file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(full_file_name)

################################################################################

def substituteStringsDotInCpp(path, class_name, module_name):
    """
    """
    logger.info('# Substitute variables in <%s> file' % CPP_TEMPLATE_FILE)
    full_file_name = os.path.join(os.path.sep, path, CPP_TEMPLATE_FILE)
    
    # Substitute strings in template_file
    f = open(full_file_name, 'r')
    data = f.read()
    author_str = epcr.getAuthor()
    date_str   = time.strftime("%x")
    full_file_name_str = full_file_name.replace(CPP_TEMPLATE_FILE, class_name+'.cpp')
    new_data = data % {"FILE": full_file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "MODULENAME": module_name,
                       "CLASSNAME": class_name}

    f.close()
    # Save new data
    cpp_file_name = full_file_name.replace(CPP_TEMPLATE_FILE, class_name +'.cpp')
    f = open(cpp_file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(full_file_name)

################################################################################

def buildCmakeListsFile(module_dir, module_name, subdir, class_name, 
                        module_dep_list, library_dep_list):
    """
    """
    logger.info('# Creating or updating the CMakeLists.txt file ')
    cmake_filename = os.path.join(os.path.sep, module_dir, CMAKE_LISTS_FILE)
    
    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        f = open(cmake_filename, 'r')
        data = f.read()
        f.close()
        cmake_object = pcl.CMakeLists(data)
        module_name = cmake_object.elements_subdir_list[0].name
        
        # Update find_package macro
        if library_dep_list:
            for lib in library_dep_list:
                package_object = pcl.FindPackage(lib, [])
                cmake_object.find_package_list.append(package_object)
                
        # Update elements_subdir macro
        if module_dep_list:
            for mod_dep in module_dep_list:
                dep_object = pcl.ElementsDependsOnSubdirs([mod_dep])
                cmake_object.elements_depends_on_subdirs_list.append(dep_object)
                
        # Update elements_add_library macro
        if module_name:
            source = 'src' + os.sep + 'lib' + os.sep + subdir + '*.cpp'
            existing = [x for x in cmake_object.elements_add_library_list if x.name==module_name]
            link_libs = []
            if library_dep_list:
                 link_libs = link_libs + library_dep_list
            if module_dep_list:
                 link_libs = link_libs + module_dep_list
            if existing:
                if not source in existing[0].source_list:
                    existing[0].source_list.append(source)
                for lib in link_libs:
                    if not lib in existing[0].link_libraries_list:
                        existing[0].link_libraries_list.append(lib)
            else:
                source_list    = [source]
                include_dirs_list   = []
                public_headers_list = [module_name]
                lib_object = pcl.ElementsAddLibrary(module_name, source_list, 
                                                    link_libs, include_dirs_list,
                                                    public_headers_list)
                cmake_object.elements_add_library_list.append(lib_object)
            
            # Add unit test
            source_name = 'tests' + os.sep + subdir + class_name + '_test.cpp'
            unittest_object = pcl.ElementsAddUnitTest(class_name, [source_name], [module_name], [], 'Boost')
            cmake_object.elements_add_unit_test_list.append(unittest_object)
                   
    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################
      
def createCppClass(module_dir, module_name, subdir, class_name, module_dep_list,
                    library_dep_list):
    """
    """
    
    script_goes_on = True 
    
    createDirectories(module_dir, module_name, subdir)  
                     
    class_h_path = os.path.join(os.path.sep, module_dir, module_name, subdir)
    copyAuxFile(class_h_path, H_TEMPLATE_FILE)    
    class_cpp_path = os.path.join(os.path.sep, module_dir,'src/lib', subdir)
    copyAuxFile(class_cpp_path,CPP_TEMPLATE_FILE)
        
    buildCmakeListsFile(module_dir, module_name, subdir, class_name, 
                        module_dep_list, library_dep_list) 
     
    substituteStringsInDotH(class_h_path, class_name, module_name)  
    substituteStringsDotInCpp(class_cpp_path, class_name, 
                              module_name)  
    return script_goes_on

################################################################################
    
def defineSpecificProgramOptions():
    description = """
           """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('class_name', metavar='class-name', 
                        type=str, 
                        help='Module name')
    parser.add_argument('-md', '--module-dependency', action='append', 
                        type=str,
                        help='Dependency module name')
    parser.add_argument('-ld', '--library-dependency', action='append', 
                        type=str,
                        help='Dependency library name')

    return parser

################################################################################

def mainMethod(args):

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the ElementsAddCppClass \
    script ')
    logger.info('#')

    try:
        # True: no error occured
        script_goes_on       = True 
        
        module_list          = args.module_dependency
        library_list         = args.library_dependency
        (sub_dir,class_name) = getClassName(args.class_name)

        # Default is the current directory
        module_dir = os.getcwd()

        logger.info('# Current directory : %s', module_dir)

        # We absolutely need a Elements cmake file
        script_goes_on, module_name = isElementsModuleExist(module_dir)
        
        # Check aux files exist
        if script_goes_on:
            script_goes_on = epcr.isAuxFileExist(H_TEMPLATE_FILE)
        if script_goes_on:
            script_goes_on = epcr.isAuxFileExist(CPP_TEMPLATE_FILE)
        if script_goes_on:
            script_goes_on = epcr.isAuxFileExist(CMAKE_LISTS_FILE_IN)
        
        # Create CPP class    
        if script_goes_on:
            if createCppClass(module_dir, module_name, sub_dir, class_name, 
                              module_list, library_list):
                logger.info('# <%s> class successfully created in <%s>.' % 
                            (class_name, module_dir))
            else:
                if not script_goes_on:
                    logger.error('# <%s> Script aborted!')
        else:
            logger.error('# Script aborted!')

    except Exception as e:
        logger.exception(e)
        logger.info('# Script stopped...')
