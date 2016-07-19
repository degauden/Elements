"""
@file ElementsKernel/AddCppClass.py
@author Nicolas Morisset

@date 01/07/15

This script creates a new Elements C++ Class

@copyright: 2012-2020 Euclid Science Ground Segment

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3.0 of the License, or (at your option)
any later version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

"""

import argparse
import os
import time
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.ParseCmakeLists as pcl
import ElementsKernel.Logging as log

logger = log.getLogger('AddCppClass')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
H_TEMPLATE_FILE = 'ClassName_template.h'
CPP_TEMPLATE_FILE = 'ClassName_template.cpp'
UNITTEST_TEMPLATE_FILE = 'UnitTestFile_template.cpp'

H_TEMPLATE_FILE_IN = 'ClassName_template.h.in'
CPP_TEMPLATE_FILE_IN = 'ClassName_template.cpp.in'
UNITTEST_TEMPLATE_FILE_IN = 'UnitTestFile_template.cpp.in'

################################################################################

def getClassName(str_subdir_class):
    """
    Get the class name and sub directory if any
    """
    name_list = str_subdir_class.split(os.path.sep)
    class_name = name_list[-1]
    subdir = str_subdir_class.replace(class_name, '')
    # Remove end slash
    subdir = subdir[:-1]
    logger.info('Class name: %s', class_name)
    if subdir:
        logger.info('Sub directory: %s', subdir)
    return subdir, class_name

################################################################################

def createDirectories(module_dir, module_name, subdir):
    """
    Create directories needed for a module and a class
    """
    # Create Directories
    module_path = os.path.join(module_dir, module_name, subdir)
    if not os.path.exists(module_path):
        os.makedirs(module_path)
    src_lib_path = os.path.join(module_dir, 'src', 'lib', subdir)
    if not os.path.exists(src_lib_path):
        os.makedirs(src_lib_path)
    test_path = os.path.join(module_dir, 'tests', 'src', subdir)
    if not os.path.exists(test_path):
        os.makedirs(test_path)

################################################################################

def substituteStringsInDotH(file_path, class_name, module_name, subdir):
    """
    Substitute variables in the Header template file and rename it
    """
    template_file = os.path.join(file_path, H_TEMPLATE_FILE)
    os.rename(os.path.join(file_path, H_TEMPLATE_FILE_IN), template_file)

    # Substitute strings in h_template_file
    f = open(template_file, 'r')
    data = f.read()
    # Format all dependent projects
    # We put by default Elements dependency if no one is given
    date_str = time.strftime("%x")
    author_str = epcr.getAuthor()
    # Make some substitutions
    file_name_str = os.path.join(module_name, subdir, class_name + '.h')
    define_words_str = '_' + file_name_str
    define_words_str = define_words_str.replace(H_TEMPLATE_FILE, class_name + '.h')
    define_words_str = define_words_str.replace('.', '_')
    define_words_str = (define_words_str.replace(os.path.sep, '_')).upper()
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "DEFINE_WORDS": define_words_str,
                       "CLASSNAME": class_name,
                       "MODULENAME": module_name}

    f.close()
    # Save new data
    file_name = template_file.replace(H_TEMPLATE_FILE, class_name + '.h')
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)

################################################################################

def substituteStringsInDotCpp(file_path, class_name, module_name, subdir):
    """
    Substitute variables in the CPP template file and rename it
    """
    template_file = os.path.join(file_path, CPP_TEMPLATE_FILE)
    os.rename(os.path.join(file_path, CPP_TEMPLATE_FILE_IN), template_file)

    # Substitute strings in template_file
    f = open(template_file, 'r')
    data = f.read()
    author_str = epcr.getAuthor()
    date_str = time.strftime("%x")
    # This avoid double slashes
    ossep2 = os.sep
    if not subdir:
        ossep2 = ''
    file_name_str = os.path.join('src', 'lib', subdir, class_name + '.cpp')
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "OSSEP": os.sep,
                       "OSSEP2": ossep2,
                       "MODULENAME": module_name,
                       "SUBDIR": subdir,
                       "CLASSNAME": class_name}

    f.close()
    # Save new data
    file_name = template_file.replace(CPP_TEMPLATE_FILE, class_name + '.cpp')
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)

################################################################################

def substituteStringsInUnitTestFile(file_path, class_name, module_name, subdir):
    """
    Substitute variables in the Unit Test template file and rename it
    """
    template_file = os.path.join(file_path, UNITTEST_TEMPLATE_FILE)
    os.rename(os.path.join(file_path, UNITTEST_TEMPLATE_FILE_IN), template_file)

    # Substitute strings in template_file
    f = open(template_file, 'r')
    data = f.read()
    author_str = epcr.getAuthor()
    date_str = time.strftime("%x")
    file_name_str = os.path.join('tests', 'src', class_name + '_test.cpp')
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "OSSEP": os.sep,
                       "MODULENAME": module_name,
                       "SUBDIR": subdir,
                       "CLASSNAME": class_name}

    f.close()
    # Save new data
    file_name = template_file.replace(UNITTEST_TEMPLATE_FILE, class_name + '_test.cpp')
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)

################################################################################

def updateCmakeListsFile(module_dir, subdir, class_name, elements_dep_list,
                         library_dep_list):
    """
    Update the <CMakeLists.txt> file for a class
    """
    logger.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        cmake_object, module_name = epcr.updateCmakeCommonPart(cmake_filename, library_dep_list)

        # Put ElementsKernel as a default
        default_dependency = 'ElementsKernel'
        if elements_dep_list:
            if not default_dependency in elements_dep_list:
                elements_dep_list.insert(0, default_dependency)
        else:
            elements_dep_list = [default_dependency]

        # Update ElementsDependsOnSubdirs macro
        if elements_dep_list:
            for mod_dep in elements_dep_list:
                dep_object = pcl.ElementsDependsOnSubdirs([mod_dep])
                cmake_object.elements_depends_on_subdirs_list.append(dep_object)

        # Update elements_add_library macro
        if module_name:
            source = 'src' + os.sep + 'lib' + os.sep + subdir + '*.cpp'
            existing = [x for x in cmake_object.elements_add_library_list if x.name == module_name]
            link_libs = []
            if elements_dep_list:
                link_libs = link_libs + elements_dep_list
            if library_dep_list:
                link_libs = link_libs + library_dep_list
            if existing:
                if not source in existing[0].source_list:
                    existing[0].source_list.append(source)
                for lib in link_libs:
                    if not lib in existing[0].link_libraries_list:
                        existing[0].link_libraries_list.append(lib)
            else:
                source_list = [source]
                include_dirs_list = []
                public_headers_list = [module_name]
                lib_object = pcl.ElementsAddLibrary(module_name, source_list,
                                                    link_libs, include_dirs_list,
                                                    public_headers_list)
                cmake_object.elements_add_library_list.append(lib_object)

            # Add unit test
            source_name = 'tests' + os.sep + 'src' + os.sep + subdir + class_name +'_test.cpp'
            unittest_object = pcl.ElementsAddUnitTest(module_name +'_' + class_name,
                                                      [source_name], [module_name],
                                                      [], 'Boost')
            cmake_object.elements_add_unit_test_list.append(unittest_object)

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################

def isClassFileAlreadyExist(class_name, module_dir, module_name, subdir):
    """
    Check if the class file does not already exist
    """
    script_goes_on = True
    module_path = os.path.join(module_dir, module_name, subdir)
    file_name = class_name + '.h'
    file_name_path = os.path.join(module_path, file_name)
    if os.path.exists(file_name_path):
        script_goes_on = False
        logger.error('The <%s> class already exists! ', class_name)
        logger.error('The header file already exists: <%s>! ', file_name_path)

    return script_goes_on

################################################################################

def createCppClass(module_dir, module_name, subdir, class_name, elements_dep_list,
                   library_dep_list):
    """
    Create all necessary files for a cpp class
    """

    # Check the class does not exist already
    script_goes_on = isClassFileAlreadyExist(class_name, module_dir, module_name,
                                             subdir)
    if script_goes_on:

        createDirectories(module_dir, module_name, subdir)
        class_h_path = os.path.join(module_dir, module_name, subdir)
        script_goes_on = epcr.copyAuxFile(class_h_path, H_TEMPLATE_FILE_IN)
        class_cpp_path = os.path.join(module_dir, 'src', 'lib', subdir)
        if script_goes_on:
            script_goes_on = epcr.copyAuxFile(class_cpp_path, CPP_TEMPLATE_FILE_IN)
        unittest_path = os.path.join(module_dir, 'tests', 'src', subdir)
        if script_goes_on:
            script_goes_on = epcr.copyAuxFile(unittest_path, UNITTEST_TEMPLATE_FILE_IN)
        if script_goes_on:
            updateCmakeListsFile(module_dir, subdir, class_name,
                                 elements_dep_list, library_dep_list)

            substituteStringsInDotH(class_h_path, class_name, module_name, subdir)
            substituteStringsInDotCpp(class_cpp_path, class_name, module_name, subdir)
            substituteStringsInUnitTestFile(unittest_path, class_name, module_name, subdir)

    return script_goes_on

################################################################################

def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
This script creates an <Elements> class at your current directory (default).
All necessary structure (directory structure, makefiles etc...)
will be automatically created for you if any but you have to be inside an
<Elements> module.
    """
    from argparse import RawTextHelpFormatter

    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=RawTextHelpFormatter)

    parser.add_argument('class_name', metavar='class-name',
                        type=str,
                        help='Class name')
    parser.add_argument('-ed', '--elements-dependency', metavar='module_name',
                        action='append', type=str,
                        help='Dependency module name e.g. "-ed ElementsKernel"')
    parser.add_argument('-extd', '--external-dependency', metavar='library_name',
                        action='append', type=str,
                        help='External dependency library name e.g. "-extd ElementsKernel"')

    return parser

################################################################################

def mainMethod(args):
    """ Main method of the AddCppClass Script"""
    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the AddCppClass script ')
    logger.info('#')

    elements_dep_list = args.elements_dependency
    library_dep_list = args.external_dependency
    (subdir, class_name) = getClassName(args.class_name)

    # Default is the current directory
    module_dir = os.getcwd()

    logger.info('Current directory : %s', module_dir)
    logger.info('')

    # We absolutely need a Elements cmake file
    script_goes_on, module_name = epcr.isElementsModuleExist(module_dir)

    # Check aux files exist
    if script_goes_on:
        script_goes_on = epcr.isAuxFileExist(H_TEMPLATE_FILE_IN)
    if script_goes_on:
        script_goes_on = epcr.isAuxFileExist(CPP_TEMPLATE_FILE_IN)

    # Create CPP class
    if script_goes_on and createCppClass(module_dir, module_name, subdir,
                        class_name, elements_dep_list, library_dep_list):
        logger.info('<%s> class successfully created in <%s>.',
                    class_name, module_dir + os.sep + subdir)
        # Remove backup file
        epcr.deleteFile(os.path.join(module_dir, CMAKE_LISTS_FILE) + '~')
        logger.info('Script over.')
    else:
        logger.error('Script aborted!')
