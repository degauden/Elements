"""
 @file: ElementsKernel/parseCmakeLists.py
 @author: Nikolaos Apostolakos
          Nicolas Morisset
          Astronomy Department of the University of Geneva

 @date: 01/07/15

 Purpose:
 This module parses and updates the <CMakeLists.txt> file
 All these classes are for parsing the <CMakeLists.txt> file. Each class
 represents a cmake macro. The "CMakeLists" class uses all the other classes.
"""
import re

################################################################################

class ElementsSubdir(object):
    """
    Decode the <elements_subdir> macro
    """
       
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return 'elements_subdir(' + self.name + ')'

################################################################################

class ElementsDependsOnSubdirs(object):
    """
    Decode the <elements_depends_on_subdirs> macro
    """

    def __init__(self, subdir_list):
        self.subdir_list = subdir_list

    def __str__(self):
        result = 'elements_depends_on_subdirs('
        for subdir in self.subdir_list:
            result += subdir + ' '
        result = result.strip() + ')'
        return result

################################################################################

class FindPackage(object):
    """
    Decode the <find_package> macro
    """

    def __init__(self, package, required_components):
        self.package = package
        self.required_components_list = required_components

    def __str__(self):
        result = 'find_package(' + self.package
        if self.required_components_list:
            result += ' REQUIRED COMPONENTS'
        for component in self.required_components_list:
            result += ' ' + component
        return result + ')'

################################################################################

class ElementsAddLibrary(object):
    """
    Decode the <elements_add_library> macro
    """

    def __init__(self, name, source_list, link_libraries, include_dirs, 
                 public_headers):
        self.name = name
        self.source_list = source_list
        self.link_libraries_list = link_libraries
        self.include_dirs_list = include_dirs
        self.public_headers_list = public_headers

    def __str__(self):
        result = 'elements_add_library(' + self.name
        for source in self.source_list:
            result += ' ' + source
        if self.link_libraries_list:
            result += '\n                     LINK_LIBRARIES'
            for name in self.link_libraries_list:
                result += ' ' + name
        if self.include_dirs_list:
            result += '\n                     INCLUDE_DIRS'
            for name in self.include_dirs_list:
                result += ' ' + name
        if self.public_headers_list:
            result += '\n                     PUBLIC_HEADERS'
            for name in self.public_headers_list:
                result += ' ' + name
        return result + ')'
    
################################################################################

class ElementsAddExecutable(object):
    """
    Decode the <elements_add_executable> macro
    """

    def __init__(self, name, source, link_libraries):
        self.name = name
        self.source = source
        self.link_libraries_list = link_libraries

    def __str__(self):
        result = 'elements_add_executable(' + self.name
        result += ' ' + self.source
        if self.link_libraries_list:
            result += '\n                     LINK_LIBRARIES'
            for name in self.link_libraries_list:
                result += ' ' + name
        result = result.strip() + ')'
        return result

################################################################################

class ElementsAddPythonExecutable(object):
    """
    Decode the <elements_add_python_program> macro
    """

    def __init__(self, name, module_name):
        self.name = name
        self.module_name = module_name

    def __str__(self):
        result = 'elements_add_python_program(' + self.name
        result += ' ' + self.module_name
        result = result.strip() + ')'
        return result

################################################################################

class ElementsAddUnitTest(object):
    """
    Decode the <elements_add_unit_test> macro
    """

    def __init__(self, class_name, source, link_libraries, include_dir, key_type):
        self.class_name = class_name
        self.source_list = source
        self.link_libraries_list = link_libraries
        self.include_dir_list = include_dir
        self.key_type = key_type

    def __str__(self):
        result = 'elements_add_unit_test(' + self.class_name + ' '
        if self.source_list:
            for source in self.source_list:
                result +=  source + ' '
        if self.link_libraries_list:
            result += '\n                     LINK_LIBRARIES'
            for name in self.link_libraries_list:
                result += ' ' + name
        if self.include_dir_list:
            result += '\n                     INCLUDE_DIRS'
            for include in self.include_dir_list:
                result += ' ' + include 
        if self.key_type:
            result +=  '\n                     TYPE ' + self.key_type
        result = result.strip() + ')'
        return result

################################################################################

class CMakeLists(object):
    """
    Parse the <CMakeLists.txt> file
    """

    def __init__(self, text=''):
        self.text = text
        self.elements_install_conf_files = None
        self.elements_install_python_modules = None
        self.elements_install_scripts = None
        self.elements_subdir_list = []
        self.elements_depends_on_subdirs_list = []
        self.find_package_list = []
        self.elements_add_library_list = []
        self.elements_add_executable_list = []
        self.elements_add_unit_test_list = []
        self.elements_add_python_executable_list = []
        
        # Remove all comment lines
        for_parsing = ''
        for line in text.splitlines():
            if '#' in line:
                for_parsing += line[:line.find('#')] + '\n'
            else:
                for_parsing += line + '\n'

        text = for_parsing
        
        elements_install_conf_files = re.findall(r"elements_install_conf_files\(.*?\)", text)
        if elements_install_conf_files:
            self.elements_install_conf_files = 'elements_install_conf_files()'

        elements_install_python_modules = re.findall(r"elements_install_python_modules\(.*?\)", text)
        if elements_install_python_modules:
            self.elements_install_python_modules = 'elements_install_python_modules()'

        elements_install_scripts = re.findall(r"elements_install_scripts\(.*?\)", text)
        if elements_install_scripts:
            self.elements_install_scripts = 'elements_install_scripts()'

        elements_subdir_list = re.findall(r"\nelements_subdir\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_subdir in elements_subdir_list:
            name = elements_subdir.replace('\n', ' ').replace('elements_subdir(', '')[:-1].strip()
            self.elements_subdir_list.append(ElementsSubdir(name))
        
        elements_depends_on_subdirs_list = re.findall(r"elements_depends_on_subdirs\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_depends_on_subdirs in elements_depends_on_subdirs_list:
            names = elements_depends_on_subdirs.replace('\n', ' ').replace('\s+', ' ').replace('elements_depends_on_subdirs(', '')[:-1].strip()
            self.elements_depends_on_subdirs_list.append(ElementsDependsOnSubdirs(names.split()))
        
        find_package_list = re.findall(r"find_package\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for find_package in find_package_list:
            name = find_package.replace('\n', ' ').replace('find_package(', '')[:-1].strip()
            components = []
            if 'REQUIRED COMPONENTS' in name:
                components = name.split('REQUIRED COMPONENTS')[1].split()
                name = name.split('REQUIRED COMPONENTS')[0].strip()
            self.find_package_list.append(FindPackage(name, components))
        
        elements_add_library_list = re.findall(r"elements_add_library\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_add_library in elements_add_library_list:
            content = elements_add_library.replace('\n', ' ').replace('elements_add_library(', '')[:-1].strip().split()
            name = content[0]
            source_list = []
            link_libraries = []
            include_dirs = []
            public_headers = []
            location = 'SOURCE'
            for word in content[1:]:
                if word == 'LINK_LIBRARIES':
                    location = 'LINK_LIBRARIES'
                    continue
                if word == 'INCLUDE_DIRS':
                    location = 'INCLUDE_DIRS'
                    continue
                if word == 'PUBLIC_HEADERS':
                    location = 'PUBLIC_HEADERS'
                    continue
                if location == 'SOURCE':
                    source_list.append(word)
                if location == 'LINK_LIBRARIES':
                    link_libraries.append(word)
                if location == 'INCLUDE_DIRS':
                    include_dirs.append(word)
                if location == 'PUBLIC_HEADERS':
                    public_headers.append(word)
            self.elements_add_library_list.append(ElementsAddLibrary(name, source_list, link_libraries, include_dirs, public_headers))

        elements_add_executable_list = re.findall(r"elements_add_executable\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_add_executable in elements_add_executable_list:
            content = elements_add_executable.replace('\n', ' ').replace('elements_add_executable(', '')[:-1].strip().split()
            name = content[0]
            source = ''
            link_libraries = []
            location = 'SOURCE'
            for word in content[1:]:
                if word == 'LINK_LIBRARIES':
                    location = 'LINK_LIBRARIES'
                    continue
                if location == 'SOURCE':
                    source = word
                if location == 'LINK_LIBRARIES':
                    link_libraries.append(word)
            self.elements_add_executable_list.append(ElementsAddExecutable(name, source, link_libraries))

        elements_add_unit_test_list = re.findall(r"elements_add_unit_test\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_add_unit_test in elements_add_unit_test_list:
            content = elements_add_unit_test.replace('\n', ' ').replace('elements_add_unit_test(', '')[:-1].strip().split()
            name = content[0]
            source_list = []
            link_libraries = []
            include_dirs = []
            key_type = ''
            location = 'SOURCE'
            for word in content[1:]:
                if word == 'LINK_LIBRARIES':
                    location = 'LINK_LIBRARIES'
                    continue
                if word == 'INCLUDE_DIRS':
                    location = 'INCLUDE_DIRS'
                    continue
                if word == 'TYPE':
                    location = 'TYPE'
                    continue
                if location == 'SOURCE':
                    source_list.append(word)
                if location == 'LINK_LIBRARIES':
                    link_libraries.append(word)
                if location == 'INCLUDE_DIRS':
                    include_dirs.append(word)
                if location == 'TYPE':
                    key_type = word
            self.elements_add_unit_test_list.append(ElementsAddUnitTest(name, source_list, link_libraries, include_dirs, key_type))
        #
        # Python stuff
        #
        elements_add_python_executable_list = re.findall(r"elements_add_python_program\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_add_python_program in elements_add_python_executable_list:
            content = elements_add_python_program.replace('\n', ' ').replace('elements_add_python_program(', '')[:-1].strip().split()
            name = content[0]
            module_name = content[1]
            self.elements_add_python_executable_list.append(ElementsAddPythonExecutable(name, module_name))
   

    # Look for the right location for adding the text just after this position
    @staticmethod
    def _addAfter(text, tag, to_add):
        if tag in text:
            for match in re.finditer(tag+r"\(.*?\)", text, re.MULTILINE|re.DOTALL):
                pass
            temp = text[:match.end()+1]
            rest = text[match.end()+1:].splitlines()
            while rest and rest[0].startswith('#'):
                temp += rest[0] + '\n'
                rest = rest[1:]
            temp += to_add + '\n'
            for line in rest:
                temp += line + '\n'
            text = temp
        else:
            text += '\n' + to_add + '\n'

        return text


    def __str__(self):
        result = self.text
        
        for find_package in self.find_package_list:
            if not re.search(r"(?<=\n)\s*find_package\(\s*"+ find_package.package + \
                            r"(?=[\s\)]).*?\)", result, re.MULTILINE|re.DOTALL):
                result = CMakeLists._addAfter(result, 'find_package', str(find_package))

        for elements_subdir in self.elements_subdir_list:
            if not re.search(r"(?<=\n)\s*elements_subdir\(\s*" + elements_subdir.name+ \
                            r"(?=[\s\)]).*?\)", result, re.MULTILINE|re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_subdir', str(elements_subdir))
                
        for elements_depends_on_subdirs in self.elements_depends_on_subdirs_list:
            if not re.search(r"(?<=\n)\s*elements_depends_on_subdirs\(\s*"+ \
                            elements_depends_on_subdirs.subdir_list[0]+ r"(?=[\s\)]).*?\)",\
                            result, re.MULTILINE|re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_depends_on_subdirs', str(elements_depends_on_subdirs))

        for library in self.elements_add_library_list:
            if not re.search(r"(?<=\n)\s*elements_add_library\(\s*" + library.name + \
                             r"(?=[\s\)]).*?\)", result, re.MULTILINE|re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_add_library', str(library))
            else:
                result = re.sub(r"(?<=\n)\s*elements_add_library\(\s*"+ library.name +\
                                r"(?=[\s\)]).*?\)", str(library), result, flags=re.MULTILINE|re.DOTALL)

        for exe in self.elements_add_executable_list:
            if not re.search(r"(?<=\n)\s*elements_add_executable\(\s*" + exe.name + r"(?=[\s\)]).*?\)",\
                            result, re.MULTILINE|re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_add_executable', str(exe))
            else:
                result = re.sub(r"(?<=\n)\s*elements_add_executable\(\s*"+ exe.name +\
                                r"(?=[\s\)]).*?\)", str(exe), result, flags=re.MULTILINE|re.DOTALL)

        for unit_test in self.elements_add_unit_test_list:
            if not re.search(r"(?<=\n)\s*elements_add_unit_test\(\s*" + unit_test.class_name +\
                            r"(?=[\s\)]).*?\)", result, re.MULTILINE|re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_add_unit_test', str(unit_test))
            else:
                result = re.sub(r"(?<=\n)\s*elements_add_unit_test\(\s*"+ unit_test.class_name +\
                                r"(?=[\s\)]).*?\)", str(unit_test), result, flags=re.MULTILINE|re.DOTALL)

        for pyexe in self.elements_add_python_executable_list:
            if not re.search(r"(?<=\n)\s*elements_add_python_program\(\s*" + pyexe.name +\
                            r"(?=[\s\)]).*?\)", result, re.MULTILINE|re.DOTALL):
                result = CMakeLists._addAfter(result, 'elements_add_python_program', str(pyexe))
            else:
                result = re.sub(r"(?<=\n)\s*elements_add_python_program\(\s*"+ pyexe.name +\
                                r"(?=[\s\)]).*?\)", str(pyexe), result, flags=re.MULTILINE|re.DOTALL)
 
        if not re.search(r"(?<=\n)\s*elements_install_python_modules\(.*?\)", result, \
                        re.MULTILINE|re.DOTALL) and self.elements_install_python_modules:
                result = CMakeLists._addAfter(result, 'elements_install_python_modules', self.elements_install_python_modules)

        if not re.search(r"(?<=\n)\s*elements_install_scripts\(.*?\)", result, re.MULTILINE|re.DOTALL) and \
        self.elements_install_scripts:
                result = CMakeLists._addAfter(result, 'elements_install_scripts', self.elements_install_scripts)

        if not re.search(r"(?<=\n)\s*elements_install_conf_files\(.*?\)", result, re.MULTILINE|re.DOTALL) and \
        self.elements_install_conf_files:
                result = CMakeLists._addAfter(result, 'elements_install_conf_files', self.elements_install_conf_files)

        return result

