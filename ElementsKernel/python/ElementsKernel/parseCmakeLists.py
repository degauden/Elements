#!/usr/bin/env python

import re
import sys

# Purpose:
# All these classes are for parsing the CMakeLists.txt file of a module.
# Each class is designed for a cmake tag and builds a list for each tag.  
# The "CMakeLists" class uses all the others.

class ElementsSubdir:
    
    def __init__(self, name):
        self.name = name
    
    def __str__(self):
        return 'elements_subdir(' + self.name + ')\n'
    
    
##########################################
    
class ElemenentsDependsOnSubdirs:
    
    def __init__(self, subdir_list):
        self.subdir_list = subdir_list

    def __str__(self):
        result = 'elements_depends_on_subdirs('
        for subdir in self.subdir_list:
            result += subdir + ' '
        result = result.strip() + ')'
        return result

##########################################

class FindPackage:
    
    def __init__(self, package, required_components):
        self.package = package
        self.required_components = required_components
    
    def __str__(self):
        result = 'find_package(' + self.package
        if self.required_components:
            result += ' REQUIRED COMPONENTS'
        for component in self.required_components:
            result += ' ' + component
        return result + ')\n'

##########################################

class ElementsAddLibrary:
    
    def __init__(self, name, source_list, link_libraries, include_dirs, public_headers):
        self.name = name
        self.source_list = source_list
        self.link_libraries = link_libraries
        self.include_dirs = include_dirs
        self.public_headers = public_headers
    
    def __str__(self):
        result = 'elements_add_library(' + self.name
        for source in self.source_list:
            result += ' ' + source
        if self.link_libraries:
            result += '\n                     LINK_LIBRARIES'
            for name in self.link_libraries:
                result += ' ' + name
        if self.include_dirs:
            result += '\n                     INCLUDE_DIRS'
            for name in self.include_dirs:
                result += ' ' + name
        if self.public_headers:
            result += '\n                     PUBLIC_HEADERS'
            for name in self.public_headers:
                result += ' ' + name
        return result + ')\n'
    
##########################################
    
class ElementsAddUnitTest:
    
    def __init__(self, class_name, source, module_name, type_list):
        self.class_name  = class_name
        self.source      = source
        self.module_name = module_name
        self.type_list   = type_list

    def __str__(self):
        result = 'elements_add_unit_test(' + self.class_name + '_test ' + self.source + \
                '\n                     LINK_LIBRARIES ' + self.module_name + \
                ' TYPE '
        if self.type_list:
            for type_name in self.type_list:
                result +=  type_name + ' '
        result = result.strip() + ')'
        return result
    
##########################################

class CMakeLists:
    
    def __init__(self, text=''):
        self.text = text
        self.elements_subdir_list = []
        self.elements_depends_on_subdirs_list = []
        self.find_package_list = []
        self.elements_add_library_list = []
        self.elements_add_unit_test_list = []
        
        elements_subdir_list = re.findall(r"elements_subdir\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_subdir in elements_subdir_list:
            name = elements_subdir.replace('\n', ' ').replace('elements_subdir(', '')[:-1].strip()
            self.elements_subdir_list.append(ElementsSubdir(name))
        
        elements_depends_on_subdirs_list = re.findall(r"elements_depends_on_subdirs\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_depends_on_subdirs in elements_depends_on_subdirs_list:
            names = elements_depends_on_subdirs.replace('\n', ' ').replace('\s+', ' ').replace('elements_depends_on_subdirs(', '')[:-1].strip()
            self.elements_depends_on_subdirs_list.append(ElemenentsDependsOnSubdirs(names.split()))
        
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
            
        elements_add_unit_test_list = re.findall(r"elements_add_unit_test\(.*?\)", text, re.MULTILINE|re.DOTALL)
        for elements_add_unit_test in elements_add_unit_test_list:
            content = elements_add_unit_test.replace('\n', ' ').replace('elements_add_unit_test(', '')[:-1].strip().split()
            class_name = content[0]
            module_name = content[1]
            source = content[2]
            type_list = []
            location = 'SOURCE'
            for word in content[3:]:
                type_list.append(word)
                
            self.elements_add_unit_test_list.append(ElementsAddUnitTest(class_name, module_name, source, type_list))
 
    def _addAfter(self, text, tag, to_add):
        if tag in text:
            for match in re.finditer(tag+r"\(.*?\)", text, re.MULTILINE|re.DOTALL):
                pass
            text = text[:match.end()+1] + '\n' + to_add + text[match.end():]
        else:
            text += '\n' + to_add
        return text
       
    def __str__(self):
        result = self.text

        for elements_subdir in self.elements_subdir_list:
            if not re.search(r"elements_subdir\(\s*"+elements_subdir.name+r".*?\)", result, re.MULTILINE|re.DOTALL):
                result = self._addAfter(result, 'elements_subdir', str(elements_subdir))

        for elements_depends_on_subdirs in self.elements_depends_on_subdirs_list:
            if not re.search(r"elements_depends_on_subdirs\(\s*"+elements_depends_on_subdirs.subdir_list[0]+   r".*?\)"   , result, re.MULTILINE|re.DOTALL):
                result = self._addAfter(result, 'elements_depends_on_subdirs', str(elements_depends_on_subdirs))

        for library in self.elements_add_library_list:
            if not re.search(r"elements_add_library\(\s*" + library.name + r".*?\)", result, re.MULTILINE|re.DOTALL):
                result = self._addAfter(result, 'elements_add_library', str(library))
            else:
                result = re.sub(r"elements_add_library\(\s*"+library.name+   r".*?\)", str(library), result, flags=re.MULTILINE|re.DOTALL)

        for unit_test in self.elements_add_unit_test_list:
            if not re.search(r"elements_add_unit_test\(\s*" + unit_test.class_name + r".*?\)", result, re.MULTILINE|re.DOTALL):
                result = self._addAfter(result, 'elements_add_unit_test', str(unit_test))
            else:
                result = re.sub(r"elements_add_unit_test\(\s*"+unit_test.class_name+   r".*?\)", str(unit_test), result, flags=re.MULTILINE|re.DOTALL)
        
        return result

