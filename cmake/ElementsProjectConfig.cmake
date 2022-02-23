# - ElementsProject
# Define the macros used by Elements-based projects.
#
# Authors: Hubert Degaudenzi
#
# Commit Id: $Format:%H$

cmake_minimum_required(VERSION 2.8.12)

# FIXME: use of LOCATION property is deprecated and should be replaced with the
#        generator expression $<TARGET_FILE>, but the way we use it requires
#        CMake >= 2.8.12, so we must keep the old behavior until we bump the
#        cmake_minimum_required version. (policy added in CMake 3.0)
if(NOT CMAKE_VERSION VERSION_LESS 3.0) # i.e CMAKE_VERSION >= 3.0
  if(NOT CMAKE_VERSION VERSION_LESS 3.9.0)
    cmake_policy(SET CMP0026 NEW)
  else()  
    cmake_policy(SET CMP0026 OLD)
  endif()
  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    cmake_policy(SET CMP0042 OLD)
  endif()
endif()

if(POLICY CMP0063)
  # this policy is related to the symbol visibility
  # please run "cmake --help-policy CMP0063" for more details
  if(NOT CMAKE_VERSION VERSION_LESS 3.3) # i.e CMAKE_VERSION >= 3.3
    cmake_policy(SET CMP0063 NEW)
  endif()
endif()

if(POLICY CMP0048)
  # this policy is related to the behavior of the project() macro
  # please run "cmake --help-policy CMP0048" for more details
  if(NOT CMAKE_VERSION VERSION_LESS 3.12.1) # i.e CMAKE_VERSION >= 3.3
    cmake_policy(SET CMP0048 NEW)
  else()
    cmake_policy(SET CMP0048 OLD)
  endif()
endif()

if(POLICY CMP0054)
  # this policy is related to the string comparison
  # please run "cmake --help-policy CMP0054" for more details
  if(NOT CMAKE_VERSION VERSION_LESS 3.1) # i.e CMAKE_VERSION >= 3.3
    cmake_policy(SET CMP0054 NEW)
  else()
    cmake_policy(SET CMP0054 OLD)
  endif()
endif()


if (NOT HAS_ELEMENTS_TOOLCHAIN)
  # this is the call to the preload_local_module_path is the toolchain has not been called
  # Preset the CMAKE_MODULE_PATH from the environment, if not already defined.
  if(NOT CMAKE_MODULE_PATH)
    # Note: this works even if the environment variable is not set.
    file(TO_CMAKE_PATH "$ENV{CMAKE_MODULE_PATH}" CMAKE_MODULE_PATH)
  endif()

  # Add the directory containing this file and the to the modules search path
  set(CMAKE_MODULE_PATH ${ElementsProject_DIR} ${ElementsProject_DIR}/modules ${CMAKE_MODULE_PATH})
  # Automatically add the modules directory provided by the project.
  if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/modules)
      set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
    endif()
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
  endif()

  # Remove duplicates
  list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)

endif()

include(ElementsUtils)

debug_message("    <---- Elements Main config: ${CMAKE_CURRENT_LIST_FILE} ---->   ")

#-------------------------------------------------------------------------------
# Basic configuration
#-------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Ensure that the include directories added are always taken first.
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)


#---------------------------------------------------------------------------------------------------
# Programs and utilities needed for the build
#---------------------------------------------------------------------------------------------------
include(CMakeParseArguments)

find_package(PythonInterp ${PYTHON_EXPLICIT_VERSION} QUIET)

if(PYTHON_EXECUTABLE MATCHES "^(/usr)?/bin/python")
  set(PYTHON_HAS_STD_LOCATION TRUE)
else()
  set(PYTHON_HAS_STD_LOCATION FALSE)
endif()

#-------------------------------------------------------------------------------
# elements_project(project version
#                  [USE proj1 vers1 [proj2 vers2 ...]]
#                  [DATA package [VERSION vers] [package [VERSION vers] ...]])
#
# Main macro for a Elements-based project.
# Each project must call this macro once in the top-level CMakeLists.txt,
# stating the project name and the version in the regular format (X.Y[.Z]).
#
# The USE list can be used to declare which Elements-based projects are required by
# the broject being compiled.
#
# The DATA list can be used to declare the data packages required by the project
# runtime.
#-------------------------------------------------------------------------------
macro(elements_project project version)

  set(project_vers_format OLD)
  if(POLICY CMP0048)
    cmake_policy(GET CMP0048 project_vers_format)
  endif()
  
  if("${project_vers_format}" STREQUAL "NEW" AND (NOT ${version} MATCHES "^HEAD.*"))
    project(${project} VERSION ${version})
  else()
    project(${project})  
  endif()
  #----For some reason this is not set by calling 'project()'
  set(CMAKE_PROJECT_NAME ${project})
  if("${PROJECT_NAME}" STREQUAL "")
    set(PROJECT_NAME ${CMAKE_PROJECT_NAME} CACHE STRING "Name of the project")
  endif()

  #--- Define the version of the project - can be used to generate sources,
  set(CMAKE_PROJECT_VERSION ${version} CACHE STRING "Version of the project")
  if("${PROJECT_VERSION}" STREQUAL "")
    set(PROJECT_VERSION ${CMAKE_PROJECT_VERSION} CACHE STRING "Version of the project")
  endif()


  #--- Parse the other arguments on the
  CMAKE_PARSE_ARGUMENTS(PROJECT "" "DESCRIPTION" "USE;DATA" ${ARGN})
  if (PROJECT_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Wrong arguments.")
  endif()

  if(NOT CMAKE_PROJECT_VERSION MATCHES "^HEAD.*")
    string(REGEX MATCH ${version_regex} _version ${CMAKE_PROJECT_VERSION})
    set(CMAKE_PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1} CACHE INTERNAL "Major version of project")
    set(CMAKE_PROJECT_VERSION_MINOR ${CMAKE_MATCH_2} CACHE INTERNAL "Minor version of project")
    set(CMAKE_PROJECT_VERSION_PATCH ${CMAKE_MATCH_4} CACHE INTERNAL "Patch version of project")
  else()
    # 'HEAD' version is special
    set(CMAKE_PROJECT_VERSION_MAJOR 999)
    set(CMAKE_PROJECT_VERSION_MINOR 999)
    set(CMAKE_PROJECT_VERSION_PATCH "")
  endif()

  include(ElementsLocations)

  set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "-DSQUEEZED_INSTALL:BOOL=${SQUEEZED_INSTALL}")
  set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "-DINSTALL_DOC:BOOL=${INSTALL_DOC}")
  set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "-DUSE_VERSIONED_LIBRARIES:BOOL=${USE_VERSIONED_LIBRARIES}")
  set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "-DUSE_SPHINX:BOOL=${USE_SPHINX}")
  set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "-DINSTALL_TESTS:BOOL=${INSTALL_TESTS}")
  set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "-DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}")
  set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "--no-warn-unused-cli")
  

  set(env_xml ${CMAKE_BINARY_DIR}/${project}BuildEnvironment.xml
     CACHE STRING "path to the XML file for the environment to be used in building and testing")

  if(SQUEEZED_INSTALL)
    set(installed_env_xml \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${XML_INSTALL_SUFFIX}/${project}BuildEnvironment.xml
       CACHE STRING "path to the XML file for the environment to be used for installation")
  else()
    set(installed_env_xml \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${project}BuildEnvironment.xml
       CACHE STRING "path to the XML file for the environment to be used for installation")  
  endif()


  set(env_release_xml ${CMAKE_BINARY_DIR}/${project}Environment.xml
     CACHE STRING "path to the XML file for the environment to be used once the project is installed")

  if(SQUEEZED_INSTALL)
    set(installed_env_release_xml \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${XML_INSTALL_SUFFIX}/${project}Environment.xml
       CACHE STRING "path to the XML file for the environment to be used once the project is installed")
  else()
    set(installed_env_release_xml \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${project}Environment.xml
       CACHE STRING "path to the XML file for the environment to be used once the project is installed")  
  endif()



  mark_as_advanced(CMAKE_RUNTIME_OUTPUT_DIRECTORY CMAKE_LIBRARY_OUTPUT_DIRECTORY
                   env_xml env_release_xml
                   installed_env_xml installed_env_release_xml)


  if(ELEMENTS_BUILD_TESTS)
    find_package(Valgrind QUIET)
    if(VALGRIND_FOUND)
      set(MEMORYCHECK_COMMAND ${VALGRIND_EXECUTABLE})
      set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full --show-leak-kinds=all" )
    endif()
    enable_testing()
    include(CTest)
  endif()

  #--- Find subdirectories
  message(STATUS "Looking for local directories...")
  # Locate packages
  elements_get_packages(packages)
  message(STATUS "Found:")
  foreach(package ${packages})
    message(STATUS "  ${package}")
  endforeach()

  # List of all known packages, including those exported by other projects
  set(known_packages ${packages} ${override_subdirs})

  # paths where to locate scripts and executables
  set(binary_paths)
  foreach(modp ${CMAKE_MODULE_PATH})
    if(EXISTS ${modp}/scripts)
      set(binary_paths ${binary_paths} ${modp}/scripts)
    endif()
  endforeach()

  list(REMOVE_DUPLICATES binary_paths)


  # environment description
  set(project_environment)

  set(used_elements_projects)
  # Locate and import used projects.
  if(PROJECT_USE)
    _elements_use_other_projects(${PROJECT_USE})
  endif()
  if(used_elements_projects)
    list(REMOVE_DUPLICATES used_elements_projects)
  endif()

  if(NOT PROJECT_DESCRIPTION)
    set(PROJECT_DESCRIPTION "Please provide a description of the project.")
  endif()

  # Ensure that we have the correct order of the modules search path.
  # (the included <project>Config.cmake files are prepending their entries to
  # the module path).
  foreach(_p ${used_elements_projects})
    if(IS_DIRECTORY ${${_p}_DIR}/cmake)
      if(IS_DIRECTORY ${${_p}_DIR}/cmake/modules)
        set(CMAKE_MODULE_PATH ${${_p}_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
      endif()
      set(CMAKE_MODULE_PATH ${${_p}_DIR}/cmake ${CMAKE_MODULE_PATH})
    endif()
  endforeach()
  if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/modules)
      set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
    endif()
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
  endif()
  if(CMAKE_MODULE_PATH)
    list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)
  endif()
  #message(STATUS "CMAKE_MODULE_PATH -> ${CMAKE_MODULE_PATH}")

  # Find the required data packages and add them to the environment.
  set(data_packages)
  if(PROJECT_DATA)
    _elements_handle_data_packages(${PROJECT_DATA})
  endif()
  if(data_packages)
    list(REMOVE_DUPLICATES data_packages)
  endif()


  #--- commands required to build cached variable
  # (python scripts are located as such but run through python)
  set(binary_paths ${CMAKE_SOURCE_DIR}/cmake/scripts ${binary_paths})

  find_program(env_cmd env.py HINTS ${binary_paths})
  set(env_cmd ${PYTHON_EXECUTABLE} ${env_cmd})

  find_program(merge_cmd merge_files.py HINTS ${binary_paths})
  set(merge_cmd ${PYTHON_EXECUTABLE} ${merge_cmd} --no-stamp)

  find_program(versheader_cmd createProjVersHeader.py HINTS ${binary_paths})
  if(versheader_cmd)
    set(versheader_cmd ${PYTHON_EXECUTABLE} ${versheader_cmd})
  endif()

  find_program(instheader_cmd createProjInstHeader.py HINTS ${binary_paths})
  if(instheader_cmd)
    set(instheader_cmd ${PYTHON_EXECUTABLE} ${instheader_cmd})
  endif()

  find_program(expheader_cmd createProjExpHeader.py HINTS ${binary_paths})
  if(expheader_cmd)
    set(expheader_cmd ${PYTHON_EXECUTABLE} ${expheader_cmd})
  endif()


  find_program(versmodule_cmd createProjVersModule.py HINTS ${binary_paths})
  if(versmodule_cmd)
    set(versmodule_cmd ${PYTHON_EXECUTABLE} ${versmodule_cmd})
  endif()

  find_program(instmodule_cmd createProjInstModule.py HINTS ${binary_paths})
  if(instmodule_cmd)
    set(instmodule_cmd ${PYTHON_EXECUTABLE} ${instmodule_cmd})
  endif()


  find_program(thisheader_cmd createThisProjHeader.py HINTS ${binary_paths})
  if(thisheader_cmd)
    set(thisheader_cmd ${PYTHON_EXECUTABLE} ${thisheader_cmd})
  endif()

  find_program(thismodule_cmd createThisProjModule.py HINTS ${binary_paths})
  if(thismodule_cmd)
    set(thismodule_cmd ${PYTHON_EXECUTABLE} ${thismodule_cmd})
  endif()


  find_program(thismodheader_cmd createThisModHeader.py HINTS ${binary_paths})
  if(thismodheader_cmd)
    set(thismodheader_cmd ${PYTHON_EXECUTABLE} ${thismodheader_cmd})
  endif()

  find_program(Boost_testmain_cmd createBoostTestMain.py HINTS ${binary_paths})
  if(Boost_testmain_cmd)
    set(Boost_testmain_cmd ${PYTHON_EXECUTABLE} ${Boost_testmain_cmd})
  endif()

  find_program(CppUnit_testmain_cmd createCppUnitTestMain.py HINTS ${binary_paths})
  if(CppUnit_testmain_cmd)
    set(CppUnit_testmain_cmd ${PYTHON_EXECUTABLE} ${CppUnit_testmain_cmd})
  endif()

  find_program(elementsrun_cmd elementsrun.py HINTS ${binary_paths})
  if(elementsrun_cmd)
    set(elementsrun_cmd ${PYTHON_EXECUTABLE} ${elementsrun_cmd})
  endif()

  find_package(RPMBuild)
  if(RPMBUILD_FOUND)
    find_program(rpmbuild_wrap_cmd rpmbuild_wrap.py HINTS ${binary_paths})
    set(rpmbuild_wrap_cmd ${PYTHON_EXECUTABLE} ${rpmbuild_wrap_cmd})
	  mark_as_advanced(rpmbuild_wrap_cmd)
	  if (NOT RPMBUILD_VERSION VERSION_LESS 4.14)
	    set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "-DELEMENTS_DETACHED_DEBINFO=OFF")
    endif()
  endif()

  find_program(pythonprogramscript_cmd createPythonProgramScript.py HINTS ${binary_paths})
  if(pythonprogramscript_cmd)
    set(pythonprogramscript_cmd ${PYTHON_EXECUTABLE} ${pythonprogramscript_cmd})
  endif()

  find_program(ctest2junit_cmd ctest2JUnit.py HINTS ${binary_paths})
  if(ctest2junit_cmd)
    set(ctest2junit_cmd ${PYTHON_EXECUTABLE} ${ctest2junit_cmd})
  endif()

  find_program(ctestxml2html_cmd CTestXML2HTML.py HINTS ${binary_paths})
  if(ctestxml2html_cmd)
    set(ctestxml2html_cmd ${PYTHON_EXECUTABLE} ${ctestxml2html_cmd})
  endif()

  mark_as_advanced(env_cmd merge_cmd versheader_cmd instheader_cmd expheader_cmd 
                   versmodule_cmd instmodule_cmd
                   thisheader_cmd thismodule_cmd
                   thismodheader_cmd
                   Boost_testmain_cmd CppUnit_testmain_cmd elementsrun_cmd
                   pythonprogramscript_cmd ctest2junit_cmd ctestxml2html_cmd)


  #--- Project Installations------------------------------------------------------------------------
  if(NOT SQUEEZED_INSTALL OR ("${CMAKE_PROJECT_NAME}" STREQUAL "Elements"))

  install(DIRECTORY cmake/ DESTINATION ${CMAKE_INSTALL_SUFFIX}
                           FILES_MATCHING
                             PATTERN "*.cmake"
                             PATTERN "*.in"
                             PATTERN "*.dox"
                             PATTERN "*.py"
                             PATTERN "*.sh"
                             PATTERN "*.csh"
                             PATTERN "*.bat"
                             PATTERN ".svn" EXCLUDE
                             PATTERN ".git" EXCLUDE)

  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)

  file(GLOB cm_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake/*)
  foreach(cm ${cm_list})
    set_property(GLOBAL APPEND PROPERTY REGULAR_CMAKE_OBJECTS ${cm})
  endforeach()

  file(GLOB m_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${MAKE_DIR_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${MAKE_DIR_NAME}/*)
  foreach(m ${m_list})
    install(FILES ${MAKE_DIR_NAME}/${m} DESTINATION ${MAKE_INSTALL_SUFFIX})
    message(STATUS "Installing ${m} in ${MAKE_INSTALL_SUFFIX}")
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_MAKE TRUE)
    set_property(GLOBAL APPEND PROPERTY REGULAR_MAKE_OBJECTS ${m}) 
  endforeach()

  endif()

  #------------------------------------------------------------------------------------------------

  # get the python test framework

  if(PYTHONINTERP_FOUND)
    set(PYFRMK_TEST)
    set(PYFRMK_NAME)
    if(USE_PYTHON_NOSE)
      find_package(Nose QUIET)
      if(NOSE_FOUND)
        message(STATUS "Using the Nose python test framework")
        set(PYFRMK_TEST ${NOSE_EXECUTABLE})
        set(PYFRMK_NAME "PythonNose")
      else()
        message(WARNING "The Nose python test framework cannot be found")
        find_package(PyTest QUIET)
        if(PYTEST_FOUND)
          message(WARNING "Using Py.Test instead")
          set(PYFRMK_TEST ${PYTEST_EXECUTABLE})
          set(PYFRMK_NAME "PyTest")
        endif()
      endif()
    else()
      find_package(PyTest QUIET)
      if(PYTEST_FOUND)
        message(STATUS "Using the Py.Test python test framework")
        set(PYFRMK_TEST ${PYTEST_EXECUTABLE})
        set(PYFRMK_NAME "PyTest")
      else()
        message(WARNING "The Py.Test python test framework cannot be found")
        find_package(Nose QUIET)
        if(NOSE_FOUND)
          message(WARNING "Using Nose instead")
          set(PYFRMK_TEST ${NOSE_EXECUTABLE})
          set(PYFRMK_NAME "PythonNose")
        endif()
      endif()
    endif()
    if(PYFRMK_TEST)
      set(PYFRMK_TEST ${PYTHON_EXECUTABLE} ${PYFRMK_TEST})
    endif()
  endif()

  # compute the VCS version

  string(TIMESTAMP PROJECT_TIMESTAMP_VERSION "%Y%m%d%H%M%S" UTC)

  find_package(Git)

  set(CURRENT_GIT_TAG)

  if (GIT_FOUND)
    set(CURRENT_GIT_STAMP)
    execute_process(COMMAND git log -n1 --pretty='%H'
                    OUTPUT_VARIABLE CURRENT_GIT_STAMP
                    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (CURRENT_GIT_STAMP)
      string(REPLACE "'" "" CURRENT_GIT_STAMP ${CURRENT_GIT_STAMP})
    endif()

    if (CURRENT_GIT_STAMP)
      execute_process(COMMAND git describe --exact-match --tags ${CURRENT_GIT_STAMP}
                      OUTPUT_VARIABLE CURRENT_GIT_TAG
                      ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()
  endif()

  if(CURRENT_GIT_STAMP)
    if(CURRENT_GIT_TAG)
      set(PROJECT_VCS_VERSION ${CURRENT_GIT_TAG})
    else()
      set(PROJECT_VCS_VERSION ${PROJECT_TIMESTAMP_VERSION})
    endif()
  else()
    # we are not in git
    set(PROJECT_VCS_VERSION ${CMAKE_PROJECT_VERSION})
  endif()

  # Generate the version header for the project.
  string(TOUPPER ${project} _proj)

  if(versheader_cmd)
    execute_process(COMMAND
                    ${versheader_cmd} --quiet
                    ${project} ${CMAKE_PROJECT_VERSION} ${PROJECT_VCS_VERSION} ${CMAKE_BINARY_DIR}/${INCLUDE_INSTALL_SUFFIX}/${_proj}_VERSION.h)
    install(FILES ${CMAKE_BINARY_DIR}/include/${_proj}_VERSION.h DESTINATION ${INCLUDE_INSTALL_SUFFIX})
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
    set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${_proj}_VERSION.h)
  endif()

  if(instheader_cmd)
    set(so_version_option)
    if(USE_VERSIONED_LIBRARIES)
      set(so_version_option "-V")     
    endif()
    JOIN("${used_elements_projects}" ":" joined_used_projects)
    execute_process(COMMAND
                    ${instheader_cmd} --quiet ${so_version_option}
                    ${project} ${CMAKE_INSTALL_PREFIX} ${joined_used_projects} ${CMAKE_BINARY_DIR}/${INCLUDE_INSTALL_SUFFIX}/${_proj}_INSTALL.h)
    # special installation because the install location can be changed on the fly
    install(CODE "message\(STATUS \"Installing: ${_proj}_INSTALL.h in \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_SUFFIX}\"\)
execute_process\(COMMAND ${instheader_cmd} --quiet ${so_version_option} ${project} \${CMAKE_INSTALL_PREFIX} ${joined_used_projects} \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_SUFFIX}/${_proj}_INSTALL.h\)")
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
    set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${_proj}_INSTALL.h)
  endif()


  if(PROJECT_HIDE_SYMBOLS)
    if(expheader_cmd)
      execute_process(COMMAND
                      ${expheader_cmd} --quiet
                      ${project} ${CMAKE_BINARY_DIR}/${INCLUDE_INSTALL_SUFFIX}/${_proj}_EXPORT.h)
      install(FILES ${CMAKE_BINARY_DIR}/include/${_proj}_EXPORT.h DESTINATION ${INCLUDE_INSTALL_SUFFIX})
      add_definitions(-D${_proj}_HIDE_SYMBOLS)
      set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
      set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${_proj}_EXPORT.h)
    endif()
  endif()

  if(thisheader_cmd)
    execute_process(COMMAND
                    ${thisheader_cmd} --quiet
                    ${project} ${CMAKE_BINARY_DIR}/${INCLUDE_INSTALL_SUFFIX}/ThisProject.h)
    # This header is by design only local. It is then not installed
  endif()

  # Add generated headers to the include path.
  elements_include_directories(${CMAKE_BINARY_DIR}/${INCLUDE_INSTALL_SUFFIX})

  if(versmodule_cmd)
    execute_process(COMMAND
                    ${versmodule_cmd} --quiet
                    ${project} ${CMAKE_PROJECT_VERSION} ${PROJECT_VCS_VERSION} ${CMAKE_BINARY_DIR}/python/${_proj}_VERSION.py)
    install(FILES ${CMAKE_BINARY_DIR}/python/${_proj}_VERSION.py DESTINATION ${PYTHON_INSTALL_SUFFIX})
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
    set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_VERSION.py)
    if(NOT ("${SGS_HOST_OS}" STREQUAL "fc" AND (NOT SGS_HOST_OSVERS VERSION_LESS 30)))
      if(PYTHON_VERSION_MAJOR VERSION_LESS 3)
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_VERSION.pyo)
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_VERSION.pyc)
      endif()
    else()
      if(SQUEEZED_INSTALL AND (PYTHON_VERSION_MAJOR VERSION_LESS 3)
         AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_VERSION.pyo)
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_VERSION.pyc)
      endif()
    endif()
  endif()

  if(instmodule_cmd)
    set(so_version_option)
    if(USE_VERSIONED_LIBRARIES)
      set(so_version_option "-V")     
    endif()
    
    JOIN("${used_elements_projects}" ":" joined_used_projects)
    execute_process(COMMAND
                    ${instmodule_cmd} --quiet ${so_version_option}
                    ${project} ${CMAKE_INSTALL_PREFIX} ${joined_used_projects} ${CMAKE_BINARY_DIR}/python/${_proj}_INSTALL.py)
    # special install procedure because the install loction can be changed on the fly.
    install(CODE "message\(STATUS \"Installing: ${_proj}_INSTALL.py in \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_SUFFIX}\"\)
execute_process\(COMMAND ${instmodule_cmd} --quiet ${so_version_option} ${project} \${CMAKE_INSTALL_PREFIX} ${joined_used_projects} \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_SUFFIX}/${_proj}_INSTALL.py\)")
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
    set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_INSTALL.py)
    
    if(NOT ("${SGS_HOST_OS}" STREQUAL "fc" AND (NOT SGS_HOST_OSVERS VERSION_LESS 30)))
      if(PYTHON_VERSION_MAJOR VERSION_LESS 3)
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_INSTALL.pyo)
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_INSTALL.pyc)
      endif()
    else()
      if(SQUEEZED_INSTALL AND (PYTHON_VERSION_MAJOR VERSION_LESS 3)
         AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_INSTALL.pyo)
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${_proj}_INSTALL.pyc)
      endif()
    endif()
  endif()


  #--- Collect settings for subdirectories
  set(library_path)
  # Take into account the dependencies between local subdirectories before
  # adding them to the build.
  elements_collect_subdir_deps(${packages})
  # sort all known packages
  elements_sort_subdirectories(known_packages)
  # extract the local packages from the sorted list
  set(sorted_packages)
  foreach(var ${known_packages})
    list(FIND packages ${var} idx)
    if(NOT idx LESS 0)
      list(APPEND sorted_packages ${var})
    endif()
  endforeach()
  #message(STATUS "${known_packages}")
  #message(STATUS "${packages}")
  set(packages ${sorted_packages})
  #message(STATUS "${packages}")

  # Search standard libraries.
  set(std_library_path)
  if(CMAKE_HOST_UNIX)
    # Guess the LD_LIBRARY_PATH required by the compiler we use (only Unix).
    _elements_find_standard_lib(libstdc++.so std_library_path)
    if (CMAKE_CXX_COMPILER MATCHES "icpc")
      _elements_find_standard_lib(libimf.so icc_libdir)
      set(std_library_path ${std_library_path} ${icc_libdir})
    endif()
    # this ensures that the std libraries are in RPATH
    link_directories(${std_library_path})
  endif()

  file(WRITE ${CMAKE_BINARY_DIR}/subdirs_deps.dot "digraph subdirs_deps {\n")
  # Add all subdirectories to the project build.
  list(LENGTH packages packages_count)
  set(package_idx 0)
  foreach(package ${packages})
    math(EXPR package_idx "${package_idx} + 1")
    message(STATUS "Adding directory ${package} (${package_idx}/${packages_count})")
    add_subdirectory(${package})
  endforeach()
  file(APPEND ${CMAKE_BINARY_DIR}/subdirs_deps.dot "}\n")

  #--- Prepare environment configuration
  message(STATUS "Preparing environment configuration:")

  # - collect environment from externals
  elements_external_project_environment()

  # (so far, the build and the release environments are identical)
  set(project_build_environment ${project_environment})

  # - collect internal environment
  message(STATUS "  environment for the project")

  foreach(other_project ${used_elements_projects})
    set(project_environment ${project_environment}
        SEARCH_PATH ${${other_project}_DIR})
  endforeach()

  foreach(_pck ${data_packages})
    set(project_environment ${project_environment}
        SEARCH_PATH ${${_pck}_DIR})
  endforeach()

  foreach(_ds ${ELEMENTS_DEFAULT_SEARCH_PATH})  
    if(EXISTS ${_ds})
      set(project_environment ${project_environment}
          SEARCH_PATH ${_ds})
    endif()
  endforeach()

  if(EXISTS ${ELEMENTS_USR_SEARCH_PATH})
      set(project_environment ${project_environment}
        SEARCH_PATH ${ELEMENTS_USR_SEARCH_PATH})
  endif()

  foreach(other_project ${used_elements_projects})
    set(project_environment ${project_environment}
        INCLUDE ${other_project}Environment.xml)
  endforeach()

  foreach(_pck ${data_packages})
    set(project_environment ${project_environment}
        INCLUDE ${_pck}Environment.xml)
  endforeach()


  #   - installation dirs

  if(SQUEEZED_INSTALL)
    set(_inst_offset ../../../)
  else()
    set(_inst_offset)
  endif()

  if(NOT SQUEEZED_INSTALL)
    set(project_environment ${project_environment}
        PREPEND PATH LOCAL_ESCAPE_DOLLAR{.}/${_inst_offset}${SCRIPT_INSTALL_SUFFIX}
        PREPEND PATH LOCAL_ESCAPE_DOLLAR{.}/${_inst_offset}bin
        PREPEND LD_LIBRARY_PATH LOCAL_ESCAPE_DOLLAR{.}/${_inst_offset}${CMAKE_LIB_INSTALL_SUFFIX}
        PREPEND PYTHONPATH LOCAL_ESCAPE_DOLLAR{.}/${_inst_offset}${PYTHON_INSTALL_SUFFIX}
        PREPEND PYTHONPATH LOCAL_ESCAPE_DOLLAR{.}/${_inst_offset}${PYTHON_DYNLIB_INSTALL_SUFFIX})
  endif()

  set(project_environment ${project_environment}
      PREPEND ELEMENTS_CONF_PATH LOCAL_ESCAPE_DOLLAR{.}/${_inst_offset}${CONF_INSTALL_SUFFIX}
      PREPEND ELEMENTS_AUX_PATH LOCAL_ESCAPE_DOLLAR{.}/${_inst_offset}${AUX_INSTALL_SUFFIX})

  foreach(other_project ${used_elements_projects})
    if(EXISTS ${${other_project}_DIR})
      set(project_build_environment ${project_build_environment}
          SEARCH_PATH ${${other_project}_DIR})
    endif()
  endforeach()

  foreach(other_project ${used_elements_projects})
    set(project_build_environment ${project_build_environment}
        INCLUDE ${other_project}BuildEnvironment.xml)
  endforeach()



  foreach(_pck ${data_packages})
    if(EXISTS ${${_pck}_DIR})
      set(project_build_environment ${project_build_environment}
          SEARCH_PATH ${${_pck}_DIR})
    endif()
  endforeach()

  foreach(_pck ${data_packages})
    set(project_build_environment ${project_build_environment}
        INCLUDE ${_pck}Environment.xml)
  endforeach()

  if(NOT SQUEEZED_INSTALL)
    set(project_build_environment ${project_build_environment}
        PREPEND PATH ${CMAKE_INSTALL_PREFIX}/${SCRIPT_INSTALL_SUFFIX}
        PREPEND PATH ${CMAKE_INSTALL_PREFIX}/bin
        PREPEND LD_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_LIB_INSTALL_SUFFIX}
        PREPEND PYTHONPATH ${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_SUFFIX}
        PREPEND PYTHONPATH ${CMAKE_INSTALL_PREFIX}/${PYTHON_DYNLIB_INSTALL_SUFFIX})
  endif()
  set(project_build_environment ${project_build_environment}
      PREPEND ELEMENTS_CONF_PATH ${CMAKE_INSTALL_PREFIX}/${CONF_INSTALL_SUFFIX}
      PREPEND ELEMENTS_AUX_PATH ${CMAKE_INSTALL_PREFIX}/${AUX_INSTALL_SUFFIX})


  message(STATUS "  environment for local subdirectories")
  #   - project root (for relocatability)
  string(TOUPPER ${project} _proj)

  set(installed_project_environment "${project_environment}")

  set(project_environment ${project_environment} SET ${_proj}_PROJECT_ROOT "LOCAL_ESCAPE_DOLLAR{.}/../..")
  set(project_environment ${project_environment} SET THIS_PROJECT_ROOT "LOCAL_ESCAPE_DOLLAR{.}/../..")
  if(NOT SQUEEZED_INSTALL)
    set(installed_project_environment ${installed_project_environment} SET ${_proj}_PROJECT_ROOT "LOCAL_ESCAPE_DOLLAR{.}/../..")
    set(installed_project_environment ${installed_project_environment} SET THIS_PROJECT_ROOT "LOCAL_ESCAPE_DOLLAR{.}/../..")
  endif()

  set(installed_project_build_environment "${project_build_environment}")
  set(project_build_environment ${project_build_environment} SET ${_proj}_PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
  set(project_build_environment ${project_build_environment} SET THIS_PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

  if(NOT SQUEEZED_INSTALL)
    set(installed_project_build_environment ${installed_project_build_environment} SET ${_proj}_PROJECT_ROOT "${CMAKE_INSTALL_PREFIX}/../..")
    set(installed_project_build_environment ${installed_project_build_environment} SET THIS_PROJECT_ROOT "${CMAKE_INSTALL_PREFIX}/../..")
  endif()

  #   - 'packages':
  foreach(package ${packages})
    message(STATUS "    ${package}")
    get_filename_component(_pack ${package} NAME)
    string(TOUPPER ${_pack} _PACK)
    #     - roots (backward compatibility)
    set(project_environment ${project_environment} SET ${_PACK}ROOT \${${_proj}_PROJECT_ROOT}/${package})
    set(project_build_environment ${project_build_environment} SET ${_PACK}ROOT \${${_proj}_PROJECT_ROOT}/${package})

    #     - declared environments
    get_property(_pack_env DIRECTORY ${package} PROPERTY ENVIRONMENT)
    set(project_environment ${project_environment} ${_pack_env})
    set(project_build_environment ${project_build_environment} ${_pack_env})
    #       (build env only)
    get_property(_pack_env DIRECTORY ${package} PROPERTY BUILD_ENVIRONMENT)
    set(project_build_environment ${project_build_environment} ${_pack_env})

    # we need special handling of PYTHONPATH and PATH for the build-time environment
    set(_has_python NO)

    if(EXISTS ${CMAKE_SOURCE_DIR}/${package}/python)
      set(project_build_environment ${project_build_environment}
          PREPEND PYTHONPATH \${${_proj}_PROJECT_ROOT}/${package}/python)
      set(_has_python YES)
    endif()


    if(EXISTS ${CMAKE_SOURCE_DIR}/${package}/scripts)
      set(project_build_environment ${project_build_environment}
          PREPEND PATH \${${_proj}_PROJECT_ROOT}/${package}/scripts)
    endif()

    if(EXISTS ${CMAKE_SOURCE_DIR}/${package}/${CONF_DIR_NAME})
      set(project_build_environment ${project_build_environment}
          PREPEND ELEMENTS_CONF_PATH \${${_proj}_PROJECT_ROOT}/${package}/${CONF_DIR_NAME})
    endif()

    if(EXISTS ${CMAKE_SOURCE_DIR}/${package}/aux)
      set(project_build_environment ${project_build_environment}
          PREPEND ELEMENTS_AUX_PATH \${${_proj}_PROJECT_ROOT}/${package}/aux)
    endif()

    if(EXISTS ${CMAKE_SOURCE_DIR}/${package}/${AUX_DIR_NAME})
      set(project_build_environment ${project_build_environment}
          PREPEND ELEMENTS_AUX_PATH \${${_proj}_PROJECT_ROOT}/${package}/${AUX_DIR_NAME})
    endif()


  endforeach()

  #   - build dirs
  set(project_build_environment ${project_build_environment}
      PREPEND PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      PREPEND PATH ${CMAKE_BINARY_DIR}/scripts
      PREPEND LD_LIBRARY_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
      PREPEND PYTHONPATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
      PREPEND PYTHONPATH ${CMAKE_BINARY_DIR}/python
      PREPEND ELEMENTS_AUX_PATH ${CMAKE_BINARY_DIR}/${AUX_DIR_NAME}
      PREPEND ELEMENTS_CONF_PATH ${CMAKE_BINARY_DIR}/${CONF_DIR_NAME})

  # - produce environment XML description
  #   release version
  elements_generate_env_conf(${env_release_xml} ${project_environment})
  install(CODE "set\(ElementsProject_DIR ${ElementsProject_DIR}\)
  set\(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}\)
  find_package\(ElementsProject\)
  message\(STATUS \"Installing: ${installed_env_release_xml}\"\)
  set\(ELEMENTS_DEFAULT_SEARCH_PATH ${ELEMENTS_DEFAULT_SEARCH_PATH}\)
  set\(ELEMENTS_USR_SEARCH_PATH ${ELEMENTS_USR_SEARCH_PATH}\)
  set\(used_elements_projects ${used_elements_projects}\)
  foreach\(other_project ${used_elements_projects}\)
  set\(\${other_project}_DIR ${${other_project}_DIR}\)
  endforeach\(\)
  set\(data_packages ${data_packages}\)
  foreach\(_pck ${data_packages}\)
  set\(\${_pck}_DIR ${${_pck}_DIR}\)
  endforeach\(\)
  elements_generate_env_conf\(${installed_env_release_xml} ${installed_project_environment}\)")
  #   build-time version
  elements_generate_env_conf(${env_xml} ${project_build_environment})
  install(CODE "set\(ElementsProject_DIR ${ElementsProject_DIR}\)
  set\(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}\)
  find_package\(ElementsProject\)
message\(STATUS \"Installing: ${installed_env_xml}\"\)
elements_generate_env_conf\(${installed_env_xml} ${installed_project_build_environment}\)")
  #   add a small wrapper script in the build directory to easily run anything
  set(_env_cmd_line)
  foreach(t ${env_cmd}) # transform the env_cmd list in a space separated string
    set(_env_cmd_line "${_env_cmd_line} ${t}")
  endforeach()
  if(UNIX)
    file(WRITE ${CMAKE_BINARY_DIR}/run
         "#!/bin/sh\nexec ${_env_cmd_line} --xml ${env_xml} \"$@\"\n")
    execute_process(COMMAND chmod a+x ${CMAKE_BINARY_DIR}/run)
  endif() # ignore other systems

  if(ELEMENTS_BUILD_TESTS)

    add_custom_target(HTMLSummary)
    if(TEST_HTML_REPORT)
      add_custom_command(TARGET HTMLSummary
                         COMMAND echo "The HTMLSummary target is obsolete")
    endif()

    add_custom_target(JUnitSummary)
    if(TEST_JUNIT_REPORT)
      find_python_module(lxml)
      if(PY_LXML)
        find_file(ctest2junit_xsl_file
                  NAMES CTest2JUnit.xsl
                  PATHS ${CMAKE_MODULE_PATH}
                  PATH_SUFFIXES auxdir/test auxdir
                  NO_DEFAULT_PATH)
        add_custom_command(TARGET JUnitSummary
                           COMMAND ${env_cmd} --xml ${env_xml} 
                                   ${ctest2junit_cmd} ${PROJECT_BINARY_DIR} ${ctest2junit_xsl_file})
      else()
        add_custom_command(TARGET JUnitSummary
                           COMMAND echo "The JUnit reports cannot be produced because of a missing lxml python module.")
      endif()
    else()
      add_custom_command(TARGET JUnitSummary
                         COMMAND echo "The JUnit reports for the tests are not enabled.")
    endif()

  endif()


  #--- Generate config files to be imported by other projects.
  elements_generate_project_config_version_file()
  elements_generate_project_config_file()
  elements_generate_project_platform_config_file()
  elements_generate_exports(${packages})

  #--- Generate the manifest.xml file.
  if(NOT SQUEEZED_INSTALL)
    elements_generate_project_manifest(${CMAKE_BINARY_DIR}/manifest.xml ${ARGV})
    install(FILES ${CMAKE_BINARY_DIR}/manifest.xml DESTINATION ${CMAKE_INSTALL_PREFIX})
  endif()


  #--- CPack configuration
  # Please have a look at the general CPack documentation at
  # http://www.cmake.org/Wiki/CMake:CPackPackageGenerators
  # http://www.cmake.org/Wiki/CMake:CPackConfiguration

  set(CPACK_PACKAGE_NAME ${project})
  foreach(t MAJOR MINOR PATCH)
    set(CPACK_PACKAGE_VERSION_${t} ${CMAKE_PROJECT_VERSION_${t}})
  endforeach()
  set(CPACK_SYSTEM_NAME ${BINARY_TAG})
  set(CPACK_PACKAGE_RELOCATABLE TRUE)
  if(USE_LOCAL_INSTALLAREA)
    set(CPACK_PACKAGING_INSTALL_PREFIX ${ELEMENTS_BASE_DIR}/${CPACK_PACKAGE_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${BINARY_TAG})
  else()
    set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  endif()
  set(CPACK_GENERATOR RPM)
  set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})

  set(CPACK_PACKAGE_RELEASE 1 CACHE STRING "Release Number For the Packaging")
  set(CPACK_PACKAGE_VENDOR "Hubert Degaudenzi")

  set(CPACK_SOURCE_IGNORE_FILES "/InstallArea/;/${BUILD_PREFIX_NAME}\\\\..*/;/${BUILD_SUBDIR}/;/\\\\.svn/;/\\\\.settings/;\\\\..*project;\\\\.gitignore;/\\\\.git/")

  # RPM packaging specific stuff
  set(CPACK_RPM_PACKAGE_RELOCATABLE TRUE)

  if(NOT SQUEEZED_INSTALL)
    SET(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}")
    if(USE_TIMESTAMP_RPM_VERSION)
      SET(CPACK_RPM_PACKAGE_VERSION ${PROJECT_VCS_VERSION})
    else()
      SET(CPACK_RPM_PACKAGE_VERSION "1.0")
    endif()
  else()
    SET(CPACK_RPM_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
    SET(CPACK_RPM_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
  endif()
  SET(CPACK_RPM_PACKAGE_ARCHITECTURE ${SGS_ARCH})
  SET(CPACK_PACKAGE_FILE_NAME "${CPACK_RPM_PACKAGE_NAME}-${CPACK_RPM_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE}")
  SET(CPACK_RPM_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")
  SET(CPACK_RPM_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})


  set(CPACK_RPM_REGULAR_FILES "%defattr(-,root,root,-)")

  set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{xmldir}/${CPACK_PACKAGE_NAME}Environment.xml")

  if(NOT SQUEEZED_INSTALL)
    set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{_prefix}/manifest.xml")
  endif()


  set(CPACK_VERBOSE_LINE)
  set(CPACK_EXPORT_VERBOSE_LINE "unset VERBOSE")
  if(DEFINED ENV{VERBOSE})
    if($ENV{VERBOSE})
      set(CPACK_VERBOSE $ENV{VERBOSE})
    else()
      set(CPACK_VERBOSE 0)  
    endif()
    set(CPACK_VERBOSE_LINE "VERBOSE=${CPACK_VERBOSE}")
    set(CPACK_EXPORT_VERBOSE_LINE "export ${CPACK_VERBOSE_LINE}")
  endif()
  
#------------------------------------------------------------------------------
  get_property(regular_bin_objects GLOBAL PROPERTY REGULAR_BIN_OBJECTS)

  if(regular_bin_objects)

    list(SORT regular_bin_objects)
    list(REMOVE_DUPLICATES regular_bin_objects)
    foreach(_do ${regular_bin_objects})
      set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{_bindir}/${_do}")
    endforeach()

  endif()

#------------------------------------------------------------------------------
  get_property(cmake_extra_flags GLOBAL PROPERTY CMAKE_EXTRA_FLAGS)

  if(cmake_extra_flags)
    foreach(_do ${cmake_extra_flags})
      set(CPACK_EXTRA_CMAKEFLAGS "${CPACK_EXTRA_CMAKEFLAGS} ${_do}")
    endforeach()
  endif()
  
  if(CCACHE_FOUND AND CPACK_USE_CCACHE)
    set(CPACK_EXTRA_CMAKEFLAGS "${CPACK_EXTRA_CMAKEFLAGS} -DCMAKE_USE_CCACHE=ON")
  endif()

  if(DISTCC_FOUND AND CPACK_USE_DISTCC)
    set(CPACK_EXTRA_CMAKEFLAGS "${CPACK_EXTRA_CMAKEFLAGS} -DCMAKE_USE_DISTCC=ON")
  endif()

  option(RPM_FORWARD_PREFIX_PATH "Forward the CMAKE_PREFIX_PATH when using 'make rpm'" ON)

  if(NOT SQUEEZED_INSTALL)
    set(CPACK_CMAKE_PREFIX_PATH_LINE "export CMAKE_PREFIX_PATH=\$PWD/cmake:/usr/share/ElementsEnv/cmake:/usr/share/EuclidEnv/cmake")
  else()
    set(CPACK_CMAKE_PREFIX_PATH_LINE "#")
  endif()

  if(USE_RPM_CMAKE_MACRO)
    set(CPACK_CMAKE_MACRO "%cmake")
  else()
    set(CPACK_CMAKE_MACRO "%__cmake")
    if(SQUEEZED_INSTALL)
      set(CPACK_EXTRA_CMAKEFLAGS "${CPACK_EXTRA_CMAKEFLAGS} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
    endif()
  endif()


  if(RPM_FORWARD_PREFIX_PATH)
  
      file(TO_CMAKE_PATH "$ENV{CMAKE_PREFIX_PATH}" env_prefix_path)
      set(CPACK_PREFIX_LIST)
  
      if(NOT SQUEEZED_INSTALL)
          list(APPEND CPACK_PREFIX_LIST "\$PWD/cmake")
      endif()
  
      foreach(prefix_comp ${env_prefix_path})
          list(FIND CPACK_PREFIX_LIST "${prefix_comp}" _index)
          if(${_index} EQUAL -1)
              list(APPEND CPACK_PREFIX_LIST "${prefix_comp}")
          endif()
      endforeach()

      if(NOT SQUEEZED_INSTALL)
          list(FIND CPACK_PREFIX_LIST "/usr/share/ElementsEnv/cmake" _index)
          if(${_index} EQUAL -1)
              list(APPEND CPACK_PREFIX_LIST "/usr/share/ElementsEnv/cmake")
          endif()
          list(FIND CPACK_PREFIX_LIST "/usr/share/EuclidEnv/cmake" _index)
          if(${_index} EQUAL -1)
              list(APPEND CPACK_PREFIX_LIST "/usr/share/EuclidEnv/cmake")
          endif()
      endif()
  
      if(CPACK_PREFIX_LIST)
          JOIN("${CPACK_PREFIX_LIST}" ":" CPACK_PREFIX_PATH)
    
          set(CPACK_CMAKE_PREFIX_PATH_LINE "export CMAKE_PREFIX_PATH=${CPACK_PREFIX_PATH}")
      endif()
  
      message(STATUS "The CMAKE_PREFIX_PATH used in the spec file is: ${CPACK_PREFIX_PATH}")
  
  endif()

#------------------------------------------------------------------------------
  get_property(regular_lib_objects GLOBAL PROPERTY REGULAR_LIB_OBJECTS)

  set(VERSION_SUFFIX "")
  if(USE_VERSIONED_LIBRARIES)
    set(VERSION_SUFFIX ".${CMAKE_PROJECT_VERSION}")
  endif()
  

  if(regular_lib_objects)

    list(SORT regular_lib_objects)
    list(REMOVE_DUPLICATES regular_lib_objects)
    foreach(_do ${regular_lib_objects})
      set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{libdir}/${CMAKE_SHARED_LIBRARY_PREFIX}${_do}${CMAKE_SHARED_LIBRARY_SUFFIX}${VERSION_SUFFIX}")
    endforeach()
  endif()

#------------------------------------------------------------------------------

  get_property(proj_has_scripts GLOBAL PROPERTY PROJ_HAS_SCRIPTS)

  if(proj_has_scripts)

    get_property(regular_script_objects GLOBAL PROPERTY REGULAR_SCRIPT_OBJECTS)
    foreach(_do ${regular_script_objects})
      set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{scriptsdir}/${_do}")
    endforeach()

    #message(STATUS "The regular objects: ${CPACK_RPM_REGULAR_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_aux GLOBAL PROPERTY PROJ_HAS_AUX)

  if(proj_has_aux)

    get_property(regular_aux_objects GLOBAL PROPERTY REGULAR_AUX_OBJECTS)
    foreach(_do ${regular_aux_objects})
      set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{auxdir}/${_do}")
    endforeach()

    #message(STATUS "The regular objects: ${CPACK_RPM_REGULAR_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_conf GLOBAL PROPERTY PROJ_HAS_CONF)

  if(proj_has_conf)

    get_property(regular_conf_objects GLOBAL PROPERTY REGULAR_CONF_OBJECTS)
    foreach(_do ${regular_conf_objects})
      set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{confdir}/${_do}")
    endforeach()

    #message(STATUS "The regular objects: ${CPACK_RPM_REGULAR_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_python GLOBAL PROPERTY PROJ_HAS_PYTHON)

  if(proj_has_python)

    if(NOT ("${SGS_HOST_OS}" STREQUAL "fc" AND (NOT SGS_HOST_OSVERS VERSION_LESS 30)))
      if(NOT (PYTHON_VERSION_MAJOR VERSION_LESS 3))
        if(NOT (SQUEEZED_INSTALL AND NOT PYTHON_HAS_STD_LOCATION))
          set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS __pycache__)
        endif()
      endif()
    else()
      if(SQUEEZED_INSTALL AND NOT (PYTHON_VERSION_MAJOR VERSION_LESS 3)
         AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
        set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS __pycache__)      
      endif()
    endif()

    get_property(regular_python_objects GLOBAL PROPERTY REGULAR_PYTHON_OBJECTS)
    foreach(_do ${regular_python_objects})
      set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{pydir}/${_do}")
    endforeach()

    get_property(regular_python_dynlib_objects GLOBAL PROPERTY REGULAR_PYTHON_DYNLIB_OBJECTS)
    foreach(_do ${regular_python_dynlib_objects})
      set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{pydyndir}/${_do}")
    endforeach()

    #message(STATUS "The regular objects: ${CPACK_RPM_REGULAR_FILES}")
  endif()

#===============================================================================

  set(CPACK_RPM_DEVEL_FILES "%defattr(-,root,root,-)")

  set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{xmldir}/${CPACK_PACKAGE_NAME}BuildEnvironment.xml")

#------------------------------------------------------------------------------
  get_property(config_objects GLOBAL PROPERTY CONFIG_OBJECTS)

  if(config_objects)
    list(SORT config_objects)
    list(REMOVE_DUPLICATES config_objects)
    foreach(_do ${config_objects})
      if(NOT SQUEEZED_INSTALL)
        set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{_prefix}/${_do}")
      endif()
    endforeach()
    #message(STATUS "The devel objects: ${CPACK_RPM_DEVEL_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_include GLOBAL PROPERTY PROJ_HAS_INCLUDE)

  if(proj_has_include)

    get_property(regular_include_objects GLOBAL PROPERTY REGULAR_INCLUDE_OBJECTS)

    list(REMOVE_DUPLICATES regular_include_objects)
    if(regular_include_objects)
    if(NOT SQUEEZED_INSTALL)
      set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%dir %{_includedir}")
    endif()
    foreach(_do ${regular_include_objects})
      set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{_includedir}/${_do}")
    endforeach()
    endif()

    #message(STATUS "The devel objects: ${CPACK_RPM_DEVEL_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_cmake GLOBAL PROPERTY PROJ_HAS_CMAKE)

  if(proj_has_cmake)

    get_property(regular_cmake_objects GLOBAL PROPERTY REGULAR_CMAKE_OBJECTS)

    if(SQUEEZED_INSTALL)
      get_property(config_objects GLOBAL PROPERTY CONFIG_OBJECTS)
      foreach(_do ${config_objects})
        list(APPEND regular_cmake_objects ${_do})
      endforeach()
    endif()

    list(REMOVE_DUPLICATES regular_cmake_objects)

    if(regular_cmake_objects)
      set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%dir %{cmakedir}")
      foreach(_do ${regular_cmake_objects})
        set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{cmakedir}/${_do}")
      endforeach()
    endif()

    #message(STATUS "The devel objects: ${CPACK_RPM_DEVEL_FILES}")

  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_make GLOBAL PROPERTY PROJ_HAS_MAKE)

  if(proj_has_make)

   get_property(regular_make_objects GLOBAL PROPERTY REGULAR_MAKE_OBJECTS)

    list(REMOVE_DUPLICATES regular_make_objects)
    if(regular_make_objects)
      set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%dir %{makedir}")
      foreach(_do ${regular_make_objects})
        set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{makedir}/${_do}")
      endforeach()
    endif()
    
    #message(STATUS "The devel objects: ${CPACK_RPM_DEVEL_FILES}")

  endif()

#------------------------------------------------------------------------------
  if(USE_VERSIONED_LIBRARIES)
    get_property(regular_dev_lib_objects GLOBAL PROPERTY REGULAR_DEV_LIB_OBJECTS)
    if(regular_dev_lib_objects)

      list(SORT regular_dev_lib_objects)
      list(REMOVE_DUPLICATES regular_dev_lib_objects)
      foreach(_do ${regular_dev_lib_objects})
        set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{libdir}/${CMAKE_SHARED_LIBRARY_PREFIX}${_do}${CMAKE_SHARED_LIBRARY_SUFFIX}")
      endforeach()
    endif()
 
  endif()

#===============================================================================

#------------------------------------------------------------------------------
  get_property(debinfo_objects GLOBAL PROPERTY DEBINFO_OBJECTS)

  if(debinfo_objects)
    set(CPACK_RPM_DEBINFO_FILES "%defattr(-,root,root,-)")

    list(SORT debinfo_objects)
    foreach(_do ${debinfo_objects})
      set(CPACK_RPM_DEBINFO_FILES "${CPACK_RPM_DEBINFO_FILES}
${_do}")
    endforeach()
  endif()
  #message(STATUS "The debuginfo objects: ${CPACK_RPM_DEBINFO_FILES}")

#===============================================================================

  set(CPACK_RPM_DOC_FILES "%defattr(-,root,root,-)")
  set(CPACK_RPM_DOC_FILES "${CPACK_RPM_DOC_FILES}
%{docdir}")

#===============================================================================

  include(CPack)

  set(RPMBUILD_EXTRA_ARGS ""
      CACHE STRING "Pass extra argument to the rpmbuild command line")

  option(RPM_NO_CHECK "skip running rpmbuild check when using 'make rpm'" ON)

  find_package(Tar QUIET)
  if(TAR_FOUND)

    if (RPMBUILD_FOUND)
      option(USE_DEFAULT_RPMBUILD_DIR "Use default RPM build directory (the value of the %_topdir variable)" OFF)
      if(USE_DEFAULT_RPMBUILD_DIR)
        execute_process(COMMAND rpmbuild --eval %_topdir OUTPUT_VARIABLE PROJECT_RPM_TOPDIR OUTPUT_STRIP_TRAILING_WHITESPACE)
      else()
        set(PROJECT_RPM_TOPDIR "${PROJECT_BINARY_DIR}/Packages/RPM")
      endif()
      set(PROJECT_TARGZ_DIR "${PROJECT_RPM_TOPDIR}/SOURCES")
    else()
      set(PROJECT_TARGZ_DIR "${PROJECT_BINARY_DIR}/Packages")
    endif()

    add_custom_target(targz
                      COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_TARGZ_DIR}
                      COMMAND ${TAR_EXECUTABLE} zcf ${PROJECT_TARGZ_DIR}/${CPACK_RPM_PACKAGE_NAME}-${CPACK_RPM_PACKAGE_VERSION}.tar.gz --exclude-vcs --exclude "${BUILD_PREFIX_NAME}.*" --exclude "./${BUILD_SUBDIR}" --exclude "./.*" --exclude "./InstallArea" --exclude "__pycache__" --exclude "*.py[co]" --transform "s/./${CPACK_RPM_PACKAGE_NAME}-${CPACK_RPM_PACKAGE_VERSION}/"  .
                      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      COMMENT "Generating The Source TarBall ${PROJECT_TARGZ_DIR}/${CPACK_RPM_PACKAGE_NAME}-${CPACK_RPM_PACKAGE_VERSION}.tar.gz" VERBATIM
    )
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_TARGZ_DIR}/${CPACK_RPM_PACKAGE_NAME}-${CPACK_RPM_PACKAGE_VERSION}.tar.gz)


    if (RPMBUILD_FOUND)

      if(CPACK_REMOVE_SYSTEM_DEPS)
        set(CPACK_RPM_AUTOREQ_LINE "Autoreq: 0")
        set(RPM_DEVEL_BUILDDEP_SYS_LINES)
        set(RPM_DEP_SYS_LINES)
        set(RPM_DEVEL_DEP_SYS_LINES)
      else()
        set(CPACK_RPM_AUTOREQ_LINE)
        get_rpm_sys_dep_lines("gcc > 4.7;cmake >= 2.8.5" "BuildRequires" RPM_DEVEL_BUILDDEP_SYS_LINES)
        if(SQUEEZED_INSTALL)
          get_rpm_sys_dep_lines("python${PYTHON_EXPLICIT_VERSION}" "Requires" RPM_DEP_SYS_LINES)
        else()
          get_rpm_sys_dep_lines("python${PYTHON_EXPLICIT_VERSION}" "Requires" RPM_DEP_SYS_LINES)
        endif()
        get_rpm_sys_dep_lines("cmake >= 2.8.5" "Requires" RPM_DEVEL_DEP_SYS_LINES)
      endif()

      get_rpm_dep_list("${PROJECT_USE}" "debuginfo" "${SQUEEZED_INSTALL}" RPM_DEBUGINFO_DEP_LIST)
      get_rpm_dep_lines("${PROJECT_USE}" "debuginfo" "${SQUEEZED_INSTALL}" "Requires" RPM_DEBUGINFO_DEP_LINES)
      get_rpm_dep_lines("${PROJECT_USE}" "doc" "${SQUEEZED_INSTALL}" "Requires" RPM_DOC_DEP_LINES)
      
      

      get_rpm_dep_list("${PROJECT_USE}" "devel" "${SQUEEZED_INSTALL}" RPM_DEVEL_DEP_LIST)
      get_rpm_dep_lines("${PROJECT_USE}" "devel" "${SQUEEZED_INSTALL}" "BuildRequires" RPM_DEVEL_BUILDDEP_LINES)
      if(INSTALL_DOC)
        get_rpm_dep_lines("${PROJECT_USE}" "doc" "${SQUEEZED_INSTALL}" "BuildRequires" RPM_DOC_BUILDDEP_LINES)      
      endif()
      if( "${PROJECT_USE}" STREQUAL "")
        # if the project has no USE statement set a default build dependency
        if(SQUEEZED_INSTALL)
           if(NOT "${CPACK_PACKAGE_NAME}" STREQUAL "Elements")
             set(RPM_DEVEL_BUILDDEP_LINES "BuildRequires: Elements-devel")
             if(INSTALL_DOC)
               set(RPM_DOC_BUILDDEP_LINES "BuildRequires: Elements-doc")
             endif()
           endif()         
        endif()
      endif()
      
      get_rpm_dep_lines("${PROJECT_USE}" "devel" "${SQUEEZED_INSTALL}" "Requires" RPM_DEVEL_DEP_LINES)

      get_rpm_dep_list("${PROJECT_USE}" "" "${SQUEEZED_INSTALL}" RPM_DEP_LIST)
      get_rpm_dep_lines("${PROJECT_USE}" "" "${SQUEEZED_INSTALL}" "Requires" RPM_DEP_LINES)

      find_file(main_project_changelog_file
                NAMES ChangeLog
                PATHS ${CMAKE_SOURCE_DIR}
                PATH_SUFFIXES doc
                NO_DEFAULT_PATH)

      unset(CPACK_RPM_CHANGELOG)
      if(main_project_changelog_file)
        file(READ ${main_project_changelog_file} MAIN_PROJECT_CHANGELOG)
        set(CPACK_RPM_CHANGELOG "%changelog
${MAIN_PROJECT_CHANGELOG}
")
        message(STATUS "Using ${main_project_changelog_file} for the ChangeLog of the project")
      endif()

      set(RPM_DEBUG_PACKAGE_NIL "%define debug_package %{nil}")
      if(SQUEEZED_INSTALL)
        if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" OR "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
          set(RPM_DEBUG_PACKAGE_NIL)
        endif()
      endif()

      set(BYTECOMPILE_ERRORS_TERMINATE_BUILD 1)
      if(SQUEEZED_INSTALL AND NOT PYTHON_HAS_STD_LOCATION)
        set(BYTECOMPILE_ERRORS_TERMINATE_BUILD 0)
      endif()

      if(NOT SQUEEZED_INSTALL)
        find_file_to_configure(Elements.spec.in
                               FILETYPE "RPM SPEC"
                               PATH_SUFFIXES "auxdir/package"
                               OUTPUTDIR "${PROJECT_RPM_TOPDIR}/SPECS"
                               OUTPUTNAME "${project}.spec"
                               PATHS ${CMAKE_MODULE_PATH})
      else()
        find_file_to_configure(Elements-squeeze.spec.in
                               FILETYPE "RPM SPEC"
                               PATH_SUFFIXES "auxdir/package"
                               OUTPUTDIR "${PROJECT_RPM_TOPDIR}/SPECS"
                               OUTPUTNAME "${project}.spec"
                               PATHS ${CMAKE_MODULE_PATH})
      endif()
    
      file(MAKE_DIRECTORY ${PROJECT_RPM_TOPDIR}/BUILD)
      file(MAKE_DIRECTORY ${PROJECT_RPM_TOPDIR}/BUILDROOT)
      file(MAKE_DIRECTORY ${PROJECT_RPM_TOPDIR}/RPMS)
      file(MAKE_DIRECTORY ${PROJECT_RPM_TOPDIR}/SRPMS)

      set(RPMBUILD_ARGS "--define=\"_topdir ${PROJECT_RPM_TOPDIR}\"")
      
      if(RPM_NO_CHECK) 
         set(RPMBUILD_ARGS "${RPMBUILD_ARGS} --nocheck")
      endif()
            
      if(INSTALL_DOC)
         set(RPMBUILD_ARGS "${RPMBUILD_ARGS} --with doc")
      endif()

      if(debinfo_objects)
         set(RPMBUILD_ARGS "${RPMBUILD_ARGS} --with debinfo")      
      endif()

      
      message(STATUS "${rpmbuild_wrap_cmd} ${RPMBUILD_ARGS} ${RPMBUILD_EXTRA_ARGS} ${PROJECT_RPM_TOPDIR}/SPECS/${project}.spec")

      add_custom_target(rpm
                        COMMAND ${rpmbuild_wrap_cmd} ${RPMBUILD_ARGS} ${RPMBUILD_EXTRA_ARGS} ${PROJECT_RPM_TOPDIR}/SPECS/${project}.spec
                        COMMENT "Generating The RPM Files in ${PROJECT_RPM_TOPDIR}" VERBATIM
      )
      
      add_dependencies(rpm targz)

    endif()


  endif()

  include(ElementsDocumentation)
  include(ElementsCoverage)
  include(ElementsUninstall)
  include(ElementsInfo)

endmacro()

#-------------------------------------------------------------------------------
# _elements_use_other_projects([project version [project version]...])
#
# Internal macro implementing the handling of the "USE" option.
# (improve readability)
#-------------------------------------------------------------------------------
macro(_elements_use_other_projects)
  # Note: it works even if the env. var. is not set.
  file(TO_CMAKE_PATH "$ENV{CMAKE_PROJECT_PATH}" projects_search_path)

  foreach(_ds ${ELEMENTS_DEFAULT_SEARCH_PATH})
    if(EXISTS ${_ds})
      set(projects_search_path ${projects_search_path} ${_ds})
    endif()
  endforeach()


  if(EXISTS ${ELEMENTS_USR_SEARCH_PATH})
    set(projects_search_path ${projects_search_path} ${ELEMENTS_USR_SEARCH_PATH})
  endif()


  if(projects_search_path)
    list(REMOVE_DUPLICATES projects_search_path)
    message(STATUS "Looking for projects in ${projects_search_path}")
  else()
    message(STATUS "Looking for projects")
  endif()

  # this is needed because of the way variable expansion works in macros
  set(ARGN_ ${ARGN})

  # We initialize the other_dependee_list, which keeps who is the dependee of
  # each entry of the ARGN_ list
  list(LENGTH ARGN_ len)
  math(EXPR len "${len}/2")
  foreach(index RANGE 1 ${len})
    list(APPEND other_dependee_list ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION})
  endforeach()

  while(ARGN_)
    list(LENGTH ARGN_ len)
    if(len LESS 2)
      message(FATAL_ERROR "Wrong number of arguments to USE option")
    endif()
    list(GET ARGN_ 0 other_project)
    list(GET ARGN_ 1 other_project_version)
    list(GET other_dependee_list 0 other_dependee)
    list(GET other_dependee_list 1 other_dependee_version)
    list(REMOVE_AT ARGN_ 0 1)
    list(REMOVE_AT other_dependee_list 0 1)

    if(NOT other_project_version MATCHES "^HEAD.*")
      string(REGEX MATCH ${version_regex} _version ${other_project_version})

      set(other_project_cmake_version ${CMAKE_MATCH_1}.${CMAKE_MATCH_2})
      if(DEFINED CMAKE_MATCH_4)
        if(NOT "${CMAKE_MATCH_4}" STREQUAL "")
          set(other_project_cmake_version ${other_project_cmake_version}.${CMAKE_MATCH_4})
        endif()
      endif()
    else()
      # "HEAD" is a special version id (mapped to 999.999).
      set(other_project_cmake_version 999.999)
    endif()
    set(other_project_original_version ${other_project_version})

    # Manage the lists which contains the dependencies and the project which
    # introduced them
    if(${other_project}_FOUND)
      # If the dependency is already handled check that the version numbers
      # much, otherwise raise an error
      string(COMPARE NOTEQUAL "${${other_project}_VERSION}" "${other_project_original_version}" ver_mismatch)
      if(ver_mismatch)
        list(FIND dependency_list "${other_project}" dep_index)
        list(GET dependency_dependee_list ${dep_index} dep_name)
        math(EXPR dep_index "${dep_index}+1")
        list(GET dependency_dependee_list ${dep_index} dep_version)
        set(ver_mis_message "Dependency version mismatch:")
        set(ver_mis_message "${ver_mis_message} ${other_dependee} ${other_dependee_version}")
        set(ver_mis_message "${ver_mis_message} -> ${other_project} ${other_project_original_version}")
        set(ver_mis_message "${ver_mis_message} , ${dep_name} ${dep_version}")
        set(ver_mis_message "${ver_mis_message} -> ${other_project} ${${other_project}_VERSION}")
        message(FATAL_ERROR ${ver_mis_message})
      endif()
    else()
      # If the dependency is not handled yet populate the dependency lists
      # (the handling happens right after)
      list(APPEND dependency_list ${other_project} ${other_project_cmake_version})
      list(APPEND dependency_dependee_list ${other_dependee} ${other_dependee_version})

      set(suffixes)
      get_installed_project_suffixes(${other_project} ${other_project_version} ${BINARY_TAG} ${SGS_SYSTEM} suffixes)
      set(suffixes2)
      get_installed_versionless_project_suffixes(${other_project} ${BINARY_TAG} ${SGS_SYSTEM} suffixes2)
      foreach(pth ${projects_search_path})
        find_package(${other_project} ${other_project_cmake_version} QUIET
                     HINTS ${pth}
                     PATH_SUFFIXES ${suffixes}
                     NO_DEFAULT_PATH)
        if(${other_project}_FOUND)
          break()
        else()
          find_package(${other_project} ${other_project_cmake_version} QUIET
                       HINTS ${pth}
                       PATH_SUFFIXES ${suffixes2}
                       NO_DEFAULT_PATH)
          if(${other_project}_FOUND)
            break()
          endif()
        endif()
      endforeach()

      if(${other_project}_FOUND)
        message(STATUS "  found ${other_project} ${${other_project}_VERSION} ${${other_project}_DIR}")
        if(NOT "${SGS_SYSTEM}" STREQUAL "${${other_project}_astrotools_system}")
          message(FATAL_ERROR "Incompatible values of SGS_SYSTEM:
  ${CMAKE_PROJECT_NAME} -> ${SGS_SYSTEM}
  ${other_project} ${${other_project}_VERSION} -> ${${other_project}_astrotools_system}

  Check your configuration.
")
        endif()
        # include directories of other projects must be appended to the current
        # list to preserve the order of overriding
        elements_include_directories(AFTER ${${other_project}_INCLUDE_DIRS})
        # but in the INCLUDE_PATHS property the order gets reversed afterwards
        # so we need to prepend instead of append
        get_property(_inc_dirs GLOBAL PROPERTY INCLUDE_PATHS)
        set_property(GLOBAL PROPERTY INCLUDE_PATHS ${${other_project}_INCLUDE_DIRS} ${_inc_dirs})
        set(binary_paths ${${other_project}_BINARY_PATH} ${binary_paths})
        if(EXISTS ${${other_project}_DOC_PATH}/doxygen/${other_project}.tag)
          if(EXISTS ${${other_project}_DOC_PATH}/doxygen/html)
            set(DOXYGEN_TAGFILES
                "${DOXYGEN_TAGFILES} \"${${other_project}_DOC_PATH}/doxygen/${other_project}.tag=${${other_project}_DOC_PATH}/doxygen/html\"")
          endif()
        endif()
        foreach(exported ${${other_project}_EXPORTED_SUBDIRS})
          list(FIND known_packages ${exported} is_needed)
          if(is_needed LESS 0)
            list(APPEND known_packages ${exported})
            get_filename_component(expname ${exported} NAME)
            include(${expname}Export)
            message(STATUS "    imported module ${exported} ${${exported}_MODULE_VERSION}")
          endif()
        endforeach()
        list(APPEND known_packages ${${other_project}_OVERRIDDEN_SUBDIRS})
        # Note: we add them in reverse order so that they appear in the correct
        # inclusion order in the environment XML.
        set(used_elements_projects ${other_project} ${used_elements_projects})
        if(${other_project}_USES)
          # Add the project in the ARGN_ list
          list(INSERT ARGN_ 0 ${${other_project}_USES})
          # Update the other_dependee_list to declare that the ARGN_ entries
          # have been introduced for the other_project
          list(LENGTH ${other_project}_USES len)
          math(EXPR len "${len}/2")
          foreach(index RANGE 1 ${len})
            list(INSERT other_dependee_list 0 ${other_project} ${other_project_cmake_version})
          endforeach()
        endif()
      else()
        message(FATAL_ERROR "
Cannot find project ${other_project} ${other_project_version}
with the suffixes: ${suffixes}
or with the suffixes: ${suffixes2}
in the paths: ${projects_search_path}
")
      endif()
      #message(STATUS "know_packages (after ${other_project}) ${known_packages}")
    endif()

  endwhile()
  
endmacro()

#-------------------------------------------------------------------------------
# _elements_highest_version(output [version ...])
#
# Helper function to get the highest of a list of versions in the format vXrY[pZ]
# (actually it just compares the numbers).
#
# The highest version is stored in the 'output' variable.
#-------------------------------------------------------------------------------
function(_elements_highest_version output)
  if(ARGN)
    # use the first version as initial result
    list(GET ARGN 0 result)
    list(REMOVE_AT ARGN 0)
    # convert the version to a list of numbers
    #message(STATUS "_elements_highest_version: initial -> ${result}")
    string(REGEX MATCHALL "[0-9]+" result_digits ${result})
    list(LENGTH result_digits result_length)

    foreach(candidate ${ARGN})
      # convert the version to a list of numbers
      #message(STATUS "_elements_highest_version: candidate -> ${candidate}")
      string(REGEX MATCHALL "[0-9]+" candidate_digits ${candidate})
      list(LENGTH candidate_digits candidate_length)

      # get the upper limit of the loop over the elements
      # (note: in case of equality after the loop, the one with more elements
      #  wins, or 'result' in case of same length... so we preset the winner)
      if(result_length LESS candidate_length)
        math(EXPR limit "${result_length} - 1")
        set(candidate_higher TRUE)
      else()
        math(EXPR limit "${candidate_length} - 1")
        set(candidate_higher FALSE)
      endif()
      # loop on the elements of the two lists (result and candidate)
      foreach(idx RANGE ${limit})
        list(GET result_digits ${idx} r)
        list(GET candidate_digits ${idx} c)
        if(r LESS c)
          set(candidate_higher TRUE)
          break()
        elseif(c LESS r)
          set(candidate_higher FALSE)
          break()
        endif()
      endforeach()
      #message(STATUS "_elements_highest_version: candidate_higher -> ${candidate_higher}")
      # replace the result if the candidate is higher
      if(candidate_higher)
        set(result ${candidate})
        set(result_digits ${candidate_digits})
        set(result_length ${candidate_length})
      endif()
      #message(STATUS "_elements_highest_version: result -> ${result}")
    endforeach()
  else()
    # if we do not have arguments, return an empty variable
    set(result)
  endif()
  # pass back the result
  set(${output} ${result} PARENT_SCOPE)
endfunction()

#-------------------------------------------------------------------------------
# elements_find_data_package(name [version] [[PATH_SUFFIXES] suffixes...])
#
# Locate a CMT-style "data package", essentially a directory of the type:
#
#  <prefix>/<name>/<version>
#
# with a file called <name>Environment.xml inside.
#
# <name> can contain '/'s, but they are replaced by '_'s when looking for the
# XML file.
#
# <version> has to be a glob pattern (the default is '*').
#
# The package will be searched for in all the directories specified in the
# environment variable CMAKE_PROJECT_PATH and in CMAKE_PREFIX_PATH. If specified,
# the suffixes will be appended to each searched directory to look for the
# data packages.
#
# The root of the data package will be stored in <variable>.
#-------------------------------------------------------------------------------
function(elements_find_data_package name)
  if(NOT ${name}_FOUND)
    # Note: it works even if the env. var. is not set.
    file(TO_CMAKE_PATH "$ENV{CMAKE_PROJECT_PATH}" projects_search_path)
    file(TO_CMAKE_PATH "$ENV{CMAKE_PREFIX_PATH}" env_prefix_path)

    set(version *) # default version value
    if(ARGN AND NOT "${ARGV1}" STREQUAL "PATH_SUFFIXES")
      set(version ${ARGV1})
      list(REMOVE_AT ARGN 0)
    endif()

    if(ARGN)
      list(GET ARGN 0 arg)
      if("${arg}" STREQUAL "PATH_SUFFIXES")
        list(REMOVE_AT ARGN 0)
      endif()
    endif()
    # At this point, ARGN contains only the suffixes, if any.

    string(REPLACE / _ envname ${name}Environment.xml)

    set(candidate_version)
    set(candidate_path)
    foreach(prefix ${projects_search_path} ${CMAKE_PREFIX_PATH} ${env_prefix_path} ${DATA_MODULE_PATH})
      foreach(suffix "" ${ARGN})
        #message(STATUS "elements_find_data_package: check ${prefix}/${suffix}/${name}")
        if(IS_DIRECTORY ${prefix}/${suffix}/${name})
          #message(STATUS "elements_find_data_package: scanning ${prefix}/${suffix}/${name}")
          # Look for env files with the matching version.
          file(GLOB envfiles RELATIVE ${prefix}/${suffix}/${name} ${prefix}/${suffix}/${name}/${version}/${envname})
          # Translate the list of env files into the list of available versions
          set(versions)
          foreach(f ${envfiles})
            get_filename_component(f ${f} PATH)
            set(versions ${versions} ${f})
          endforeach()
          #message(STATUS "elements_find_data_package: found versions '${versions}'")
          if(versions)
            # find the highest version encountered so far
            _elements_highest_version(high ${candidate_version} ${versions})
            if(high AND NOT ("${high}" STREQUAL "${candidate_version}"))
              set(candidate_version ${high})
              set(candidate_path ${prefix}/${suffix}/${name}/${candidate_version})
            endif()
          endif()
        endif()
      endforeach()
    endforeach()
    if(candidate_version)
      set(${name}_FOUND TRUE CACHE INTERNAL "")
      set(${name}_DIR ${candidate_path} CACHE PATH "Location of ${name}")
      mark_as_advanced(${name}_FOUND ${name}_DIR)
      message(STATUS "Found ${name} ${candidate_version}: ${${name}_DIR}")
    else()
      message(FATAL_ERROR "Cannot find ${name} ${version} in ${projects_search_path} ${CMAKE_PREFIX_PATH} ${env_prefix_path} ${DATA_MODULE_PATH} with the ${ARGN} suffixes" )
    endif()
  endif()
endfunction()

#-------------------------------------------------------------------------------
# _elements_handle_data_packages([package [VERSION version] [project version [VERSION version]]...])
#
# Internal macro implementing the handling of the "USE" option.
# (improve readability)
#-------------------------------------------------------------------------------
macro(_elements_handle_data_packages)
  # this is needed because of the way variable expansion works in macros
  set(ARGN_ ${ARGN})
  if(ARGN_)
    message(STATUS "Looking for data packages")
  endif()
  while(ARGN_)
    list(LENGTH ARGN_ len)
    if(len LESS 2)
      message(FATAL_ERROR "Wrong number of arguments to DATA option")
    endif()  
    # extract data package name and (optional) version from the list
    list(GET ARGN_ 0 _data_package)
    list(GET ARGN_ 1 _data_pkg_vers)
    list(REMOVE_AT ARGN_ 0 1)
    if(NOT ${_data_package}_FOUND)
      elements_find_data_package(${_data_package} ${_data_pkg_vers} PATH_SUFFIXES ${ELEMENTS_DATA_SUFFIXES})
    else()
      message(STATUS "Using ${_data_package}: ${${_data_package}_DIR}")
    endif()
    if(${_data_package}_FOUND)
      set(data_packages ${_data_package} ${data_packages})
      string(REPLACE / _ _data_pkg_env ${_data_package}Environment.xml)
    endif()
  endwhile()
endmacro()

macro(_get_include_dir_from_package inc_dir pck)

  set(${inc_dir})
  if(TARGET ${pck})
    get_target_property(${inc_dir} ${pck} SOURCE_DIR)
  elseif(IS_ABSOLUTE ${pck} AND IS_DIRECTORY ${pck})
    set(${inc_dir} ${pck})
  elseif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${pck})
    set(${inc_dir} ${CMAKE_CURRENT_SOURCE_DIR}/${pck})
  elseif(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${pck}) # pck can be the name of a subdir
    set(${inc_dir} ${CMAKE_SOURCE_DIR}/${pck})
  else()
    # ensure that the current directory knows about the package
    find_package(${pck} QUIET)
    set(to_incl_var)
    string(TOUPPER ${pck} _pack_upper)
    if(${_pack_upper}_FOUND OR ${pck}_FOUND)
      # Handle some special cases first, then try for package uppercase (DIRS and DIR)
      # If the package is found, add INCLUDE_DIRS or (if not defined) INCLUDE_DIR.
      # If none of the two is defined, do not add anything.
      if("${pck}" STREQUAL "PythonLibs")
        set(to_incl_var PYTHON_INCLUDE_DIRS)
      elseif(${_pack_upper}_INCLUDE_DIRS)
        set(to_incl_var ${_pack_upper}_INCLUDE_DIRS)
      elseif(${_pack_upper}_INCLUDE_DIR)
        set(to_incl_var ${_pack_upper}_INCLUDE_DIR)
      elseif(${pck}_INCLUDE_DIRS)
        set(to_incl_var ${pck}_INCLUDE_DIRS)
      endif()
      # Include the directories
      set(${inc_dir} ${${to_incl_var}})
    endif()
  endif()
 
endmacro()


#-------------------------------------------------------------------------------
# include_package_directories(Package1 [Package2 ...]
#                             RECURSE_PATTERN pattern)
#
# Add the include directories of each package to the include directories. If the recurse
# pattern is present the subdirectories are also individually search for and the ones 
# containing files that respect the pattern are also included.
#-------------------------------------------------------------------------------
function(include_package_directories)

  CMAKE_PARSE_ARGUMENTS(ARG "" "RECURSE_PATTERN" "" ${ARGN})

  foreach(package ${ARG_UNPARSED_ARGUMENTS})
    # we need to ensure that the user can call this function also for directories
    _get_include_dir_from_package(to_incl ${package})

    if(to_incl)
      foreach(_i ${to_incl})
        starts_with_sys_include(_is_sys ${_i})
        if(${_is_sys} AND ${HIDE_SYSINC_WARNINGS})
          elements_include_directories(AFTER ${_i})
        else()
          # recursion applies only to non-system dirs
          if(ARG_RECURSE_PATTERN)
            elements_recurse(hsubdir ${_i} PATTERN ${ARG_RECURSE_PATTERN})
            if(hsubdir)
              list(REMOVE_DUPLICATES hsubdir)
              foreach(hs ${hsubdir})
                elements_include_directories(AFTER ${hs})
              endforeach()
            endif()
          else()
            elements_include_directories(AFTER ${_i})
          endif()    
        endif()
      endforeach()
    endif()
    
  endforeach()
endfunction()


#-------------------------------------------------------------------------------
# print_package_directories(Package1 [Package2 ...])
#
# print the include directories of each package.
#-------------------------------------------------------------------------------
function(print_package_directories)
  message(STATUS "print_package_directories(${ARGN})")
  foreach(package ${ARGN})
    # we need to ensure that the user can call this function also for directories
    if(TARGET ${package})
      get_target_property(to_incl ${package} SOURCE_DIR)
      if(to_incl)
        message(STATUS "print_package_directories1 include_directories(${to_incl})")
      endif()
    elseif(IS_ABSOLUTE ${package} AND IS_DIRECTORY ${package})
      message(STATUS "print_package_directories2 include_directories(${package})")
    elseif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${package})
      message(STATUS "print_package_directories3 include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${package})")
    elseif(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${package}) # package can be the name of a subdir
      message(STATUS "print_package_directories4 include_directories(${CMAKE_SOURCE_DIR}/${package})")
    else()
      # ensure that the current directory knows about the package
      find_package(${package} QUIET)
      set(to_incl)
      string(TOUPPER ${package} _pack_upper)
      if(${_pack_upper}_FOUND OR ${package}_FOUND)
        # Handle some special cases first, then try for package uppercase (DIRS and DIR)
        # If the package is found, add INCLUDE_DIRS or (if not defined) INCLUDE_DIR.
        # If none of the two is defined, do not add anything.
        if("${package}" STREQUAL "PythonLibs")
          set(to_incl PYTHON_INCLUDE_DIRS)
        elseif(${_pack_upper}_INCLUDE_DIRS)
          set(to_incl ${_pack_upper}_INCLUDE_DIRS)
        elseif(${_pack_upper}_INCLUDE_DIR)
          set(to_incl ${_pack_upper}_INCLUDE_DIR)
        elseif(${package}_INCLUDE_DIRS)
          set(to_incl ${package}_INCLUDE_DIRS)
        endif()
        # Include the directories
        message(STATUS "print_package_directories5 include_directories(${${to_incl}})")
      endif()
    endif()
  endforeach()
endfunction()

#-------------------------------------------------------------------------------
# elements_depends_on_subdirs(subdir1 [subdir2 ...])
#
# The presence of this function in a CMakeLists.txt is used by elements_sort_subdirectories
# to get the dependencies from the subdirectories before actually adding them.
#
# The function performs those operations that are not needed if there is no
# dependency declared.
#
# The arguments are actually ignored, so there is a check to execute it only once.
#-------------------------------------------------------------------------------
function(elements_depends_on_subdirs)
  # avoid multiple calls (note that the
  if(NOT elements_depends_on_subdirs_called)
    # get direct and indirect dependencies
    file(RELATIVE_PATH subdir_name ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    set(deps)
    elements_list_dependencies(deps ${subdir_name})

    # find the list of targets that generate headers in the packages we depend on.
    elements_get_genheader_targets(required_genheader_targets ${deps})
    #message(STATUS "required_genheader_targets: ${required_genheader_targets}")
    set(required_genheader_targets ${required_genheader_targets} PARENT_SCOPE)

    # add the the directories that provide headers to the include_directories
    foreach(subdir ${deps})
      if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${subdir})
        get_property(has_local_headers DIRECTORY ${CMAKE_SOURCE_DIR}/${subdir} PROPERTY INSTALLS_LOCAL_HEADERS SET)
        if(has_local_headers)
          elements_include_directories(${CMAKE_SOURCE_DIR}/${subdir})
        endif()
      endif()
    endforeach()

    # prevent multiple executions
    set(elements_depends_on_subdirs_called TRUE PARENT_SCOPE)
  endif()


  # add the dependencies lines to the DOT dependency graph
  foreach(d ${ARGN})
    file(APPEND ${CMAKE_BINARY_DIR}/subdirs_deps.dot "\"${subdir_name}\" -> \"${d}\";\n")
  endforeach()
endfunction()

#-------------------------------------------------------------------------------
# elements_collect_subdir_deps(subdirectories)
#
# look for dependencies declared in the subdirectories
#-------------------------------------------------------------------------------
macro(elements_collect_subdir_deps)
  foreach(_p ${ARGN})
    # initialize dependencies variable
    set(${_p}_DEPENDENCIES)
    # parse the CMakeLists.txt
    file(READ ${CMAKE_SOURCE_DIR}/${_p}/CMakeLists.txt file_contents)
    filter_comments(file_contents)
    string(REGEX MATCHALL "elements_depends_on_subdirs *\\(([^)]+)\\)" vars "${file_contents}")
    foreach(var ${vars})
      # extract the individual subdir names
      string(REGEX REPLACE "elements_depends_on_subdirs *\\(([^)]+)\\)" "\\1" __p ${var})
      # (replace space-type chars with spaces)
      string(REGEX REPLACE "[\t\r\n]+" " " __p "${__p}")
      separate_arguments(__p)
      foreach(___p ${__p})
        # check that the declared dependency refers to an existing (known) package
        list(FIND known_packages ${___p} idx)
        if(idx LESS 0)
          message(WARNING "Subdirectory '${_p}' declares dependency on unknown subdirectory '${___p}'")
        endif()
        list(APPEND ${_p}_DEPENDENCIES ${___p})
      endforeach()
    endforeach()
  endforeach()
endmacro()
# helper function used by elements_sort_subdirectories
macro(__visit__ _p)
  if(NOT __${_p}_visited__)
    set(__${_p}_visited__ TRUE)
    if(${_p}_DEPENDENCIES)
      foreach(___p ${${_p}_DEPENDENCIES})
        __visit__(${___p})
      endforeach()
    endif()
    list(APPEND out_packages ${_p})
  endif()
endmacro()


#-------------------------------------------------------------------------------
# elements_sort_subdirectories(var)
#
# Sort the list of subdirectories in the variable `var` according to the
# declared dependencies.
#-------------------------------------------------------------------------------
function(elements_sort_subdirectories var)
  set(out_packages)
  set(in_packages ${${var}})
  foreach(p ${in_packages})
    __visit__(${p})
  endforeach()
  set(${var} ${out_packages} PARENT_SCOPE)
endfunction()

#-------------------------------------------------------------------------------
# elements_list_dependencies(<variable> subdir)
#
# Add the subdirectories we depend on, (directly and indirectly) to the variable
# passed.
#-------------------------------------------------------------------------------
macro(elements_list_dependencies variable subdir)
  #message(STATUS "elements_list_dependencies(${subdir})")
  # recurse for the direct dependencies
  foreach(other ${${subdir}_DEPENDENCIES})
    list(FIND ${variable} ${other} other_idx)
    if(other_idx LESS 0) # recurse only if the entry is not yet in the list
      elements_list_dependencies(${variable} ${other})
      list(APPEND ${variable} ${other})
    endif()
  endforeach()
  #message(STATUS " --> ${${variable}}")
endmacro()

#-------------------------------------------------------------------------------
# elements_get_packages
#
# Find all the CMakeLists.txt files in the sub-directories and add their
# directories to the variable.
#-------------------------------------------------------------------------------
function(elements_get_packages var)
  # FIXME: trick to get the relative path to the build directory
  file(GLOB rel_build_dir RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
  set(packages)
  file(GLOB_RECURSE cmakelist_files RELATIVE ${CMAKE_SOURCE_DIR} CMakeLists.txt)
  foreach(file ${cmakelist_files})
    # ignore the source directory itself and files in the build directory
    if(NOT "${file}" STREQUAL "CMakeLists.txt" AND NOT (file MATCHES "^${rel_build_dir}" OR file MATCHES "^${BUILD_PREFIX_NAME}\\.|^build|/RPM/BUILD/"))
      get_filename_component(package ${file} PATH)
      list(APPEND packages ${package})
    endif()
  endforeach()
  list(SORT var)
  set(${var} ${packages} PARENT_SCOPE)
endfunction()


#-------------------------------------------------------------------------------
# elements_subdir(name version)
#
# Declare name and version of the subdirectory. The version now is optional.
# It is recommended not to set one.
#-------------------------------------------------------------------------------
macro(elements_subdir name)

  elements_get_package_name(_guessed_name)
  if (NOT "${_guessed_name}" STREQUAL "${name}")
    message(FATAL_ERROR "Declared subdir name (${name}) does not match the name of the directory (${_guessed_name})")
  endif()

  set (extra_macro_args ${ARGN})
  list(LENGTH extra_macro_args num_extra_args)
  if (${num_extra_args} GREATER 0)
    list(GET extra_macro_args 0 version)
  else()
    set(version 1.0)
  endif()

  # Set useful variables and properties
  set(subdir_name ${name})
  set(subdir_version ${version})
  set_directory_properties(PROPERTIES name ${name})
  set_directory_properties(PROPERTIES version ${version})
  
  set_property(GLOBAL APPEND PROPERTY PROJ_PACKAGE_LIST ${CMAKE_CURRENT_SOURCE_DIR})  

  # Generate the version header for the package.
  execute_process(COMMAND
                  ${versheader_cmd} --quiet
                  ${name} ${version} ${PROJECT_VCS_VERSION} ${CMAKE_CURRENT_BINARY_DIR}/${name}Version.h)

  execute_process(COMMAND
                  ${thismodheader_cmd} --quiet
                  ${name} ${CMAKE_CURRENT_BINARY_DIR}/ThisElementsModule.h)

endmacro()

#-------------------------------------------------------------------------------
# elements_get_package_name(VAR)
#
# Set the variable VAR to the current "package" (subdirectory) name.
#-------------------------------------------------------------------------------
macro(elements_get_package_name VAR)
  if (subdir_name)
    set(${VAR} ${subdir_name})
  else()
    # By convention, the package is the name of the source directory.
    get_filename_component(${VAR} ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  endif()
endmacro()

#-------------------------------------------------------------------------------
# _elements_strip_build_type_libs(VAR)
#
# Helper function to reduce the list of linked libraries.
#-------------------------------------------------------------------------------
function(_elements_strip_build_type_libs variable)
  set(collected ${${variable}})
  #message(STATUS "Stripping build type special libraries.")
  set(_coll)
  while(collected)
    # pop an element (library or qualifier)
    list(GET collected 0 entry)
    list(REMOVE_AT collected 0)
    if("${entry}" STREQUAL "debug" OR "${entry}" STREQUAL "optimized" OR "${entry}" STREQUAL "general")
      # it's a qualifier: pop another one (the library name)
      list(GET collected 0 lib)
      list(REMOVE_AT collected 0)
      # The possible values of CMAKE_BUILD_TYPE are Debug, Release,
      # RelWithDebInfo and MinSizeRel, plus the SGS/Elements special ones
      # Coverage and Profile. (treat an empty CMAKE_BUILD_TYPE as Release)
      if(("${entry}" STREQUAL "general") OR
         (CMAKE_BUILD_TYPE MATCHES "Debug|Coverage" AND "${entry}" STREQUAL "debug") OR
         ((NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "Rel|Profile") AND "${entry}" STREQUAL "optimized"))
        # we keep it only if corresponds to the build type
        set(_coll ${_coll} ${lib})
      endif()
    else()
      # it's not a qualifier: keep it
      set(_coll ${_coll} ${entry})
    endif()
  endwhile()
  set(collected ${_coll})
  if(collected)
    list(REMOVE_DUPLICATES collected)
  endif()
  set(${variable} ${collected} PARENT_SCOPE)
endfunction()

#-------------------------------------------------------------------------------
# elements_resolve_link_libraries(variable lib_or_package1 lib_or_package2 ...)
#
# Translate the package names in a list of link library options into the
# corresponding library options.
# Example:
#
#  find_package(Boost COMPONENTS filesystem regex)
#  find_package(ROOT COMPONENTS RIO)
#  elements_resolve_link_libraries(LIBS Boost ROOT)
#  ...
#  target_link_libraries(XYZ ${LIBS})
#
# Note: this function is more useful in wrappers to add_library etc, like
#       elements_add_library
#-------------------------------------------------------------------------------
function(elements_resolve_link_libraries variable)
  #message(STATUS "elements_resolve_link_libraries input: ${ARGN}")
  set(collected)
  set(to_be_resolved)
  foreach(package ${ARGN})
    # check if it is an actual library or a target first
    if(TARGET ${package})
      #message(STATUS "${package} is a TARGET")
      set(collected ${collected} ${package})
      get_target_property(libs ${package} REQUIRED_LIBRARIES)
      if(libs)
        set(to_be_resolved ${to_be_resolved} ${libs})
      endif()
    elseif(EXISTS ${package}) # it's a real file
      #message(STATUS "${package} is a FILE")
      set(collected ${collected} ${package})
    else()
      # it must be an available package
      string(TOUPPER ${package} _pack_upper)
      # The case of CMAKE_DL_LIBS is more special than others
      if(${_pack_upper}_FOUND OR ${package}_FOUND)
        # Handle some special cases first, then try for PACKAGE_LIBRARIES
        # otherwise fall back on Package_LIBRARIES.
        if("${package}" STREQUAL "PythonLibs")
          set(collected ${collected} ${PYTHON_LIBRARIES})
        elseif(${_pack_upper}_LIBRARIES)
          set(collected ${collected} ${${_pack_upper}_LIBRARIES})
        else()
          set(collected ${collected} ${${package}_LIBRARIES})
        endif()
      else()
        # if it's not a package, we just add it as it is... there are a lot of special cases
        set(collected ${collected} ${package})
      endif()
    endif()
  endforeach()
  _elements_strip_build_type_libs(to_be_resolved)
  if(to_be_resolved)
    elements_resolve_link_libraries(to_be_resolved ${to_be_resolved})
    set(collected ${collected} ${to_be_resolved})
  endif()
  #message(STATUS "elements_resolve_link_libraries collected: ${collected}")
  _elements_strip_build_type_libs(collected)
  #message(STATUS "elements_resolve_link_libraries output: ${collected}")
  set(${variable} ${collected} PARENT_SCOPE)
endfunction()

#-------------------------------------------------------------------------------
# elements_global_target_append(global_target local_target file1 [file2 ...])
# (macro)
#
# Adds local files as sources for the global target 'global_target' making it
# depend on the local target 'local_target'.
#-------------------------------------------------------------------------------
macro(elements_global_target_append global_target local_target)
  set_property(GLOBAL APPEND PROPERTY ${global_target}_SOURCES ${ARGN})
  set_property(GLOBAL APPEND PROPERTY ${global_target}_DEPENDS ${local_target})
endmacro()

#-------------------------------------------------------------------------------
# elements_global_target_get_info(global_target local_targets_var files_var)
# (macro)
#
# Put the information to configure the global target 'global_target' in the
# two variables local_targets_var and files_var.
#-------------------------------------------------------------------------------
macro(elements_global_target_get_info global_target local_targets_var files_var)
  get_property(${files_var} GLOBAL PROPERTY ${global_target}_SOURCES)
  get_property(${local_targets_var} GLOBAL PROPERTY ${global_target}_DEPENDS)
endmacro()


#-------------------------------------------------------------------------------
# elements_merge_files_append(merge_tgt local_target file1 [file2 ...])
#
# Add files to be included in the merge target 'merge_tgt', using 'local_target'
# as dependency trigger.
#-------------------------------------------------------------------------------
function(elements_merge_files_append merge_tgt local_target)
  elements_global_target_append(Merged${merge_tgt} ${local_target} ${ARGN})
endfunction()

#-------------------------------------------------------------------------------
# elements_merge_files(merge_tgt dest filename)
#
# Create a global target Merged${merge_tgt} that takes input files and dependencies
# from the packages (declared with elements_merge_files_append).
#-------------------------------------------------------------------------------
function(elements_merge_files merge_tgt dest filename)
  elements_global_target_get_info(Merged${merge_tgt} deps parts)
  if(parts)
    # create the targets
    set(output ${CMAKE_BINARY_DIR}/${dest}/${filename})
    add_custom_command(OUTPUT ${output}
                       COMMAND ${merge_cmd} ${parts} ${output}
                       DEPENDS ${parts})
    add_custom_target(Merged${merge_tgt} ALL DEPENDS ${output})
    # prepare the high level dependencies
    add_dependencies(Merged${merge_tgt} ${deps})

    # target to generate a partial merged file
    add_custom_command(OUTPUT ${output}_force
                       COMMAND ${merge_cmd} --ignore-missing ${parts} ${output})
    add_custom_target(Merged${merge_tgt}_force DEPENDS ${output}_force)
    # ensure that we merge what we have before installing if the output was not
    # produced
    install(CODE "if(NOT EXISTS ${output})
                  message(WARNING \"creating partial ${output}\")
                  execute_process(COMMAND ${merge_cmd} --ignore-missing ${parts} ${output})
                  endif()")

    # install rule for the merged DB
    install(FILES ${output} DESTINATION ${dest} OPTIONAL)
  endif()
endfunction()



#-------------------------------------------------------------------------------
# elements_get_required_include_dirs(<output> <libraries>)
#
# Get the include directories required by the linker libraries specified
# and prepend them to the output variable.
#-------------------------------------------------------------------------------
function(elements_get_required_include_dirs output)
  set(collected)
  foreach(lib ${ARGN})
    set(req)
    if(TARGET ${lib})
      list(APPEND collected ${lib})
      get_property(req TARGET ${lib} PROPERTY REQUIRED_INCLUDE_DIRS)
      if(req)
        list(APPEND collected ${req})
      endif()
    endif()
  endforeach()
  if(collected)
    set(collected ${collected} ${${output}})
    list(REMOVE_DUPLICATES collected)
    set(${output} ${collected} PARENT_SCOPE)
  endif()
endfunction()

#-------------------------------------------------------------------------------
# elements_get_required_library_dirs(<output> <libraries>)
#
# Get the library directories required by the linker libraries specified
# and prepend them to the output variable.
#-------------------------------------------------------------------------------
function(elements_get_required_library_dirs output)
  set(collected)
  foreach(lib ${ARGN})
    set(req)
    # Note: adding a directory to the library path make sense only to find
    # shared libraries (and not static ones).
    if(EXISTS ${lib} AND lib MATCHES "${CMAKE_SHARED_LIBRARY_PREFIX}[^/]*${CMAKE_SHARED_LIBRARY_SUFFIX}\$")
      get_filename_component(req ${lib} PATH)
      if(req)
        list(APPEND collected ${req})
      endif()
      # FIXME: we should handle the inherited targets
      # (but it's not mandatory because they where already handled)
    #else()
    #  message(STATUS "Ignoring ${lib}")
    endif()
  endforeach()
  if(collected)
    set(${output} ${collected} ${${output}} PARENT_SCOPE)
  endif()
endfunction()


#-------------------------------------------------------------------------------
# elements_get_genheader_targets(<variable> [subdir1 ...])
#
# Collect the targets that are used to generate the headers in the
# subdirectories specified in the arguments and store the list in the variable.
#-------------------------------------------------------------------------------
function(elements_get_genheader_targets variable)
  set(targets)
  foreach(subdir ${ARGN})
    if(EXISTS ${CMAKE_SOURCE_DIR}/${subdir})
      get_property(tmp DIRECTORY ${CMAKE_SOURCE_DIR}/${subdir} PROPERTY GENERATED_HEADERS_TARGETS)
      set(targets ${targets} ${tmp})
    endif()
  endforeach()
  if(targets)
    list(REMOVE_DUPLICATES targets)
  endif()
  set(${variable} ${targets} PARENT_SCOPE)
endfunction()

#-------------------------------------------------------------------------------
# elements_common_add_build(sources...
#                 LINK_LIBRARIES library1 package2 ...
#                 INCLUDE_DIRS dir1 package2 ...)
#
# Internal. Helper macro to factor out the common code to configure a buildable
# target (library, module, dictionary...)
#-------------------------------------------------------------------------------
macro(elements_common_add_build)
  CMAKE_PARSE_ARGUMENTS(ARG "" "" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS" ${ARGN})
  # obsolete option
  if(ARG_LIBRARIES)
    message(WARNING "Deprecated option 'LIBRARIES', use 'LINK_LIBRARIES' instead")
    set(ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES} ${ARG_LIBRARIES})
  endif()

  elements_resolve_link_libraries(ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES})

  # find the sources
  elements_expand_sources(srcs ${ARG_UNPARSED_ARGUMENTS})

  # get the inherited include directories
  elements_get_required_include_dirs(ARG_INCLUDE_DIRS ${ARG_LINK_LIBRARIES})

  # add the package includes to the current list
  include_package_directories(${ARG_INCLUDE_DIRS})

  # get the library dirs required to get the libraries we use
  elements_get_required_library_dirs(lib_path ${ARG_LINK_LIBRARIES})
  set_property(GLOBAL APPEND PROPERTY LIBRARY_PATH ${lib_path})

endmacro()

#-------------------------------------------------------------------------------
# elements_add_genheader_dependencies(target)
#
# Add the dependencies declared in the variables required_genheader_targets and
# required_local_genheader_targets to the specified target.
#
# The special variable required_genheader_targets is filled from the property
# GENERATED_HEADERS_TARGETS of the subdirectories we depend on.
#
# The variable required_local_genheader_targets should be used within a
# subdirectory if the generated headers are needed locally.
#-------------------------------------------------------------------------------
macro(elements_add_genheader_dependencies target)
  if(required_genheader_targets)
    add_dependencies(${target} ${required_genheader_targets})
  endif()
  if(required_local_genheader_targets)
    add_dependencies(${target} ${required_local_genheader_targets})
  endif()
endmacro()

#-------------------------------------------------------------------------------
# _elements_detach_debinfo(<target>)
#
# Helper macro to detach the debug information from the target.
#
# The debug info of the given target are extracted and saved on a different file
# with the extension '.dbg', that is installed alongside the binary.
#-------------------------------------------------------------------------------
macro(_elements_detach_debinfo target)

  if(NOT SQUEEZED_INSTALL)
  if((CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo OR CMAKE_BUILD_TYPE STREQUAL Debug ) AND ELEMENTS_DETACHED_DEBINFO)
    # get the type of the target (MODULE_LIBRARY, SHARED_LIBRARY, EXECUTABLE)
    get_property(_type TARGET ${target} PROPERTY TYPE)
    # message(STATUS "_elements_detach_debinfo(${target}): target type -> ${_type}")
    if(NOT _type STREQUAL STATIC_LIBRARY) # we ignore static libraries
      # guess the target file name
      if(_type MATCHES "MODULE|LIBRARY")
        #message(STATUS "_elements_detach_debinfo(${target}): library sub-type -> ${CMAKE_MATCH_0}")
        # TODO: the library name may be different from the default.
        #       see OUTPUT_NAME and LIBRARY_OUPUT_NAME
        set(_tn ${CMAKE_SHARED_${CMAKE_MATCH_0}_PREFIX}${target}${CMAKE_SHARED_${CMAKE_MATCH_0}_SUFFIX})
        set(_builddir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
        set(_dest ${CMAKE_LIB_INSTALL_SUFFIX})
        set(spec_prefix "%{libdir}")
        get_property(_prefix TARGET ${target} PROPERTY PREFIX)
        get_property(_suffix TARGET ${target} PROPERTY SUFFIX)
        # python module
        if(_prefix STREQUAL "_")
          set(_tn ${_prefix}${target}${_suffix})
          set(_builddir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
          set(_dest ${PYTHON_DYNLIB_INSTALL_SUFFIX})
          set(spec_prefix "%{pydyndir}")
        elseif(_prefix STREQUAL "")
          set(_tn ${_prefix}${target}${_suffix})
          set(_builddir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
          set(_dest ${PYTHON_DYNLIB_INSTALL_SUFFIX})        
          set(spec_prefix "%{pydyndir}")
        endif()
      else()
        set(_tn ${target})
        if(ELEMENTS_USE_EXE_SUFFIX)
          set(_tn ${_tn}.exe)
        endif()
        set(_builddir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        set(_dest bin)
        set(spec_prefix "%{_bindir}")
      endif()
    endif()
    # message(STATUS "_elements_detach_debinfo(${target}): target name -> ${_tn}")
    # From 'man objcopy':
    #   objcopy --only-keep-debug foo foo.dbg
    #   objcopy --strip-debug foo
    #   objcopy --add-gnu-debuglink=foo.dbg foo
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --only-keep-debug ${_tn} ${_tn}.dbg
        COMMAND ${CMAKE_OBJCOPY} --strip-debug ${_tn}
        COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink=${_tn}.dbg ${_tn}
        WORKING_DIRECTORY ${_builddir}
        COMMENT "Detaching debug infos for ${_tn} (${target}).")
    # ensure that the debug file is installed on 'make install'...
    install(FILES ${_builddir}/${_tn}.dbg DESTINATION ${_dest} OPTIONAL)
    # ... and removed on 'make clean'.
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${_builddir}/${_tn}.dbg)
    set_property(GLOBAL APPEND PROPERTY DEBINFO_OBJECTS ${spec_prefix}/${_tn}.dbg)
  endif()
  endif()
endmacro()

#---------------------------------------------------------------------------------------------------
# elements_add_library(<name>
#                      source1 source2 ...
#                      LINK_LIBRARIES library1 library2 ...
#                      INCLUDE_DIRS dir1 package2 ...
#                      [LINKER_LANGUAGE C|CXX]
#                      [NO_PUBLIC_HEADERS | PUBLIC_HEADERS dir1 dir2 ...])
#
# Extension of standard CMake 'add_library' command.
# Create a library from the specified sources (glob patterns are allowed), linking
# it with the libraries specified and adding the include directories to the search path.
#---------------------------------------------------------------------------------------------------
function(elements_add_library library)
  # this function uses an extra option: 'PUBLIC_HEADERS'
  CMAKE_PARSE_ARGUMENTS(ARG "NO_PUBLIC_HEADERS;NO_INSTALL;NO_EXIST_CHECK" "LINKER_LANGUAGE" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS;PUBLIC_HEADERS" ${ARGN})

  elements_get_package_name(package)
  if(NOT ARG_NO_PUBLIC_HEADERS AND NOT ARG_PUBLIC_HEADERS)
    message(WARNING "Library ${library} (in ${package}) does not declare PUBLIC_HEADERS. Use the option NO_PUBLIC_HEADERS if it is intended.")
  endif()

  if(NOT ARG_UNPARSED_ARGUMENTS)
    if(NOT ARG_PUBLIC_HEADERS)
      message(WARNING "Library ${library} (in ${package}) does not declare PUBLIC_HEADERS and does not declare any source files.
Provide source files and the NO_PUBLIC_HEADERS option for a plugin/module library
     or PUBLIC_HEADERS directory without source files for a header files-only library
     or PUBLIC_HEADERS directory and source files for a regular linker library.\n")
    else()
      message(STATUS "Library ${library} (in ${package}) declares PUBLIC_HEADERS and does not declare any source files: creating a header files-only library.")
    endif()
  endif()


  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS} LIBRARIES ${ARG_LIBRARIES} LINK_LIBRARIES ${ARG_LINK_LIBRARIES} INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  # find the header files
  elements_expand_source_dirs(h_srcs ${ARG_PUBLIC_HEADERS})

  add_library(${library} ${srcs} ${h_srcs})
  if(ELEMENTS_HIDE_SYMBOLS)
    if(NOT ARG_NO_INSTALL)
      include(GenerateExportHeader)
      generate_export_header(${library} BASE_NAME ${library} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/${INCLUDE_INSTALL_SUFFIX}/${library}_export.h)
      install(FILES ${CMAKE_BINARY_DIR}/${INCLUDE_INSTALL_SUFFIX}/${library}_export.h DESTINATION ${INCLUDE_INSTALL_SUFFIX})
      set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
      set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${library}_export.h)
    endif()
  else()
    set_target_properties(${library} PROPERTIES DEFINE_SYMBOL "")
  endif()

  set_target_properties(${library} PROPERTIES BASENAME "${CMAKE_SHARED_LIBRARY_PREFIX}${library}${CMAKE_SHARED_LIBRARY_SUFFIX}")

  if(ARG_LINKER_LANGUAGE)
    set_target_properties(${library} PROPERTIES LINKER_LANGUAGE ${ARG_LINKER_LANGUAGE})
  endif()

  set_target_properties(${library} PROPERTIES COMPILE_DEFINITIONS ELEMENTS_LINKER_LIBRARY)
  target_link_libraries(${library} PRIVATE ${ARG_LINK_LIBRARIES})
  _elements_detach_debinfo(${library})

  # Declare that the used headers are needed by the libraries linked against this one
  set_target_properties(${library} PROPERTIES
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    REQUIRED_INCLUDE_DIRS "${ARG_INCLUDE_DIRS}"
    REQUIRED_LIBRARIES "${ARG_LINK_LIBRARIES}")
  set_property(GLOBAL APPEND PROPERTY LINKER_LIBRARIES ${library})

  if(USE_VERSIONED_LIBRARIES)
    set_target_properties(${library} PROPERTIES SOVERSION ${CMAKE_PROJECT_VERSION})
  endif()

  #----Installation details-------------------------------------------------------
  if(NOT ARG_NO_INSTALL)
    install(TARGETS ${library} EXPORT ${CMAKE_PROJECT_NAME}Exports DESTINATION ${CMAKE_LIB_INSTALL_SUFFIX} OPTIONAL)
    elements_export(LIBRARY ${library})
    if (ARG_NO_EXIST_CHECK)
      elements_install_headers(${ARG_PUBLIC_HEADERS} NO_EXIST_CHECK)
    else()
      elements_install_headers(${ARG_PUBLIC_HEADERS})
    endif()
    install(EXPORT ${CMAKE_PROJECT_NAME}Exports DESTINATION ${CMAKE_INSTALL_SUFFIX} OPTIONAL)
    if(USE_VERSIONED_LIBRARIES)
      set_property(GLOBAL APPEND PROPERTY REGULAR_DEV_LIB_OBJECTS ${library})  
    endif()
    set_property(GLOBAL APPEND PROPERTY REGULAR_LIB_OBJECTS ${library})
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
    set_property(GLOBAL APPEND PROPERTY REGULAR_CMAKE_OBJECTS ${CMAKE_PROJECT_NAME}Exports.cmake)
    string(TOLOWER ${CMAKE_BUILD_TYPE} lower_cmake_build_type)
    set_property(GLOBAL APPEND PROPERTY REGULAR_CMAKE_OBJECTS ${CMAKE_PROJECT_NAME}Exports-${lower_cmake_build_type}.cmake)
  endif()

endfunction()

# Backward compatibility macro
macro(elements_linker_library)
  message(WARNING "Deprecated function 'elements_linker_library', use 'elements_add_library' instead")
  elements_add_library(${ARGN})
endmacro()

#---------------------------------------------------------------------------------------------------
#---elements_add_module(<name> source1 source2 ... LINK_LIBRARIES library1 library2 ...)
#---------------------------------------------------------------------------------------------------
function(elements_add_module library)
  elements_common_add_build(${ARGN})

  add_library(${library} MODULE ${srcs})
  target_link_libraries(${library} PRIVATE ${ROOT_Reflex_LIBRARY} ${ARG_LINK_LIBRARIES})
  _elements_detach_debinfo(${library})

  set_property(GLOBAL APPEND PROPERTY COMPONENT_LIBRARIES ${library})

  #----Installation details-------------------------------------------------------
  install(TARGETS ${library} LIBRARY DESTINATION ${CMAKE_LIB_INSTALL_SUFFIX} OPTIONAL)
  elements_export(MODULE ${library})
  if(USE_VERSIONED_LIBRARIES)
    set_property(GLOBAL APPEND PROPERTY REGULAR_DEV_LIB_OBJECTS ${library})  
  endif()
  set_property(GLOBAL APPEND PROPERTY REGULAR_LIB_OBJECTS ${library})
endfunction()

# Backward compatibility macro
macro(elements_component_library)
  message(WARNING "Deprecated function 'elements_component_library', use 'elements_add_module' instead")
  elements_add_module(${ARGN})
endmacro()

#-------------------------------------------------------------------------------
# elements_add_dictionary(dictionary header selection
#                      LINK_LIBRARIES ...
#                      INCLUDE_DIRS ...
#                      OPTIONS ...
#                      [SPLIT_CLASSDEF])
#
# Find all the CMakeLists.txt files in the sub-directories and add their
# directories to the variable.
#-------------------------------------------------------------------------------
function(elements_add_dictionary dictionary header selection)
  # ensure that we can produce dictionaries
  find_package(ROOT QUIET)
  if(NOT ROOT_REFLEX_DICT_ENABLED)
    message(FATAL_ERROR "ROOT cannot produce dictionaries with genreflex.")
  endif()
  # this function uses an extra option: 'OPTIONS'
  CMAKE_PARSE_ARGUMENTS(ARG "SPLIT_CLASSDEF" "" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS;OPTIONS" ${ARGN})
  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS} LIBRARIES ${ARG_LIBRARIES} LINK_LIBRARIES ${ARG_LINK_LIBRARIES} INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  # FIXME: With ROOT 6 the '_Instantiations' dummy class used in the
  #        dictionaries must have a different name in each dictionary.
  set(ARG_OPTIONS ${ARG_OPTIONS}
      -U_Instantiations
      -D_Instantiations=${dictionary}_Instantiations)

  # override the genreflex call to wrap it in the right environment
  set(ROOT_genreflex_CMD ${env_cmd} --xml ${env_xml} ${ROOT_genreflex_CMD})

  # we need to forward the SPLIT_CLASSDEF option to reflex_dictionary()
  if(ARG_SPLIT_CLASSDEF)
    set(ARG_SPLIT_CLASSDEF SPLIT_CLASSDEF)
  else()
    set(ARG_SPLIT_CLASSDEF)
  endif()
  reflex_dictionary(${dictionary} ${header} ${selection} LINK_LIBRARIES ${ARG_LINK_LIBRARIES} OPTIONS ${ARG_OPTIONS} ${ARG_SPLIT_CLASSDEF})
  set_property(TARGET ${dictionary}Dict
               APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-suggest-override")
  _elements_detach_debinfo(${dictionary}Dict)

  if(TARGET ${dictionary}GenDeps)
    elements_add_genheader_dependencies(${dictionary}GenDeps)
  else()
    elements_add_genheader_dependencies(${dictionary}Gen)
  endif()

  # Notify the project level target
  get_property(rootmapname TARGET ${dictionary}Gen PROPERTY ROOTMAPFILE)
  elements_merge_files_append(DictRootmap ${dictionary}Gen ${CMAKE_CURRENT_BINARY_DIR}/${rootmapname})

  if(ROOT_HAS_PCMS)
    get_property(pcmname TARGET ${dictionary}Gen PROPERTY PCMFILE)
    add_custom_command(OUTPUT ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${pcmname}
                       COMMAND ${CMAKE_COMMAND} -E copy ${pcmname} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${pcmname}
                       DEPENDS ${dictionary}Gen)
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${pcmname})
    add_custom_target(${dictionary}PCM ALL
                      DEPENDS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${pcmname})
  endif()

  #----Installation details-------------------------------------------------------
  install(TARGETS ${dictionary}Dict LIBRARY DESTINATION ${CMAKE_LIB_INSTALL_SUFFIX} OPTIONAL)
  if(ROOT_HAS_PCMS)
    install(FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${pcmname} DESTINATION ${CMAKE_LIB_INSTALL_SUFFIX} OPTIONAL)
  endif()
endfunction()

#---------------------------------------------------------------------------------------------------
# elements_add_python_module(name
#                           sources ...
#                           PLAIN_MODULE
#                           LINK_LIBRARIES ...
#                           INCLUDE_DIRS ...
#                           LINKER_LANGUAGE C|CXX)
#
# Build a binary python module from the given sources.
#---------------------------------------------------------------------------------------------------
function(elements_add_python_module module)

  # this function uses an extra option: 'PLAIN_MODULE'
  CMAKE_PARSE_ARGUMENTS(ARG "PLAIN_MODULE" "LINKER_LANGUAGE" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS;PUBLIC_HEADERS" ${ARGN})

  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS}
                            LIBRARIES ${ARG_LIBRARIES}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  # require Python libraries
  find_package(PythonLibs ${PYTHON_EXPLICIT_VERSION} QUIET REQUIRED)

  elements_include_directories(AFTER ${PYTHON_INCLUDE_DIRS})
  add_library(${module} MODULE ${srcs})
  
  if(ARG_LINKER_LANGUAGE)
    set_target_properties(${module} PROPERTIES LINKER_LANGUAGE ${ARG_LINKER_LANGUAGE})
  endif()

  if(NOT ${ARG_PLAIN_MODULE})
    set_target_properties(${module} PROPERTIES SUFFIX .so PREFIX "_")
    set_target_properties(${module} PROPERTIES BASENAME "_${module}.so")
  else()
    set_target_properties(${module} PROPERTIES SUFFIX .so PREFIX "")
    set_target_properties(${module} PROPERTIES BASENAME "${module}.so")
  endif()
  target_link_libraries(${module} PRIVATE ${PYTHON_LIBRARIES} ${ARG_LINK_LIBRARIES})
  _elements_detach_debinfo(${module})

  #----Installation details-------------------------------------------------------

  install(TARGETS ${module} LIBRARY DESTINATION ${PYTHON_DYNLIB_INSTALL_SUFFIX} OPTIONAL)
  set_target_properties(${module} PROPERTIES INSTALL_RPATH "$ORIGIN/../../${CMAKE_LIB_INSTALL_SUFFIX}")

  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
  if(NOT ${ARG_PLAIN_MODULE})
    set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_DYNLIB_OBJECTS _${module}.so)
  else()
    set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_DYNLIB_OBJECTS ${module}.so)
  endif()

endfunction()


#---------------------------------------------------------------------------------------------------
# _generate_swig_files(<swig_module>
#                      i_src1 i_src2 ...
#                      OUTFILE out.cxx
#                      LINK_LIBRARIES library1 library2 ...
#                      INCLUDE_DIRS dir1 package2 ...)
#
# generate the SWIG python and C++ files
#---------------------------------------------------------------------------------------------------
function(_generate_swig_files swig_module)

  find_package(SWIG QUIET REQUIRED)
  CMAKE_PARSE_ARGUMENTS(ARG "" "OUTFILE" "INCLUDE_DIRS;LINK_LIBRARIES" ${ARGN})
  
  if("${ARG_OUTFILE}" STREQUAL "")
    message(FATAL_ERROR "_generate_swig_files: No OUTFILE defined")
  endif()

  set(i_srcs ${ARG_UNPARSED_ARGUMENTS})


  # locate and set include directories
  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS})


  get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  list(REMOVE_DUPLICATES dirs)
  set(SWIG_MOD_INCLUDE_DIRS)
  foreach(dir ${dirs})
    set(SWIG_MOD_INCLUDE_DIRS -I${dir} ${SWIG_MOD_INCLUDE_DIRS})
  endforeach()
  
  set(PY_MODULE_DIR ${CMAKE_BINARY_DIR}/python)
  set(PY_MODULE ${swig_module})
  set(PY_MODULE_SWIG_SRC ${ARG_OUTFILE})

  execute_process(
    COMMAND ${SWIG_EXECUTABLE} -MM -python -keyword -module ${PY_MODULE} -Wextra -outdir ${PY_MODULE_DIR} -c++ ${SWIG_MOD_INCLUDE_DIRS} ${i_srcs}
    OUTPUT_VARIABLE swmm_dependencies
    RESULT_VARIABLE swmm_return_value
  )

  if(swmm_dependencies)
    string(REGEX MATCHALL "\n  [^ ]+" temp ${swmm_dependencies})
    set(swig_deps)
    foreach(t ${temp})
      string(STRIP "${t}" t)
      set(swig_deps ${swig_deps} "${t}")
    endforeach()
  endif()

  #SWIG command
  add_custom_command(
    OUTPUT
        ${PY_MODULE_DIR}/${PY_MODULE}.py
        ${PY_MODULE_SWIG_SRC}
    COMMAND
        ${env_cmd} --xml ${env_xml} ${SWIG_EXECUTABLE}
        -python
        -keyword
        -module ${PY_MODULE}
        -Wextra
        -outdir ${PY_MODULE_DIR}
        -c++
        ${SWIG_MOD_INCLUDE_DIRS}
        -o ${PY_MODULE_SWIG_SRC}
        ${i_srcs}
    DEPENDS
        ${i_srcs} ${swig_deps}
    COMMENT "Generating SWIG binding: ${SWIG_EXECUTABLE} -python -keyword -module ${PY_MODULE} -Wextra -outdir ${PY_MODULE_DIR} -c++ ${SWIG_MOD_INCLUDE_DIRS} -o ${PY_MODULE_SWIG_SRC} ${i_srcs}"
  )

  set_source_files_properties(${PY_MODULE_SWIG_SRC} PROPERTIES GENERATED TRUE)

  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${i_srcs} ${swig_deps})
 
  if(CXX_HAS_SUGGEST_OVERRIDE)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-suggest-override")
  endif()
  
  if(CXX_HAS_CAST_FUNCTION_TYPE)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-cast-function-type")
  endif()
  
  if(CXX_HAS_SHADOW)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-shadow")
  endif()

  if(CXX_HAS_NULL_DEREFERENCE)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-null-dereference")
  endif()



  install(FILES ${PY_MODULE_DIR}/${PY_MODULE}.py DESTINATION ${PYTHON_INSTALL_SUFFIX})
  
endfunction()

#---------------------------------------------------------------------------------------------------
# elements_add_swig_binding(<name>
#                           [interface] source1 source2 ...
#                           LINK_LIBRARIES library1 library2 ...
#                           INCLUDE_DIRS dir1 package2 ...
#                           [NO_PUBLIC_HEADERS | PUBLIC_HEADERS dir1 dir2 ...])
#
# Create a SWIG binary python module from the specified sources (glob patterns are allowed), linking
# it with the libraries specified and adding the include directories to the search path. The sources
# can be either *.i or *.cpp files. Their location is relative to the base of the Elements package
# (module).
#---------------------------------------------------------------------------------------------------
function(elements_add_swig_binding binding)

  CMAKE_PARSE_ARGUMENTS(ARG "NO_PUBLIC_HEADERS" "" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS;PUBLIC_HEADERS" ${ARGN})

  if(NOT ARG_NO_PUBLIC_HEADERS AND NOT ARG_PUBLIC_HEADERS)
    elements_get_package_name(package)
    message(WARNING "Swig binding ${binding} (in ${package}) does not declare PUBLIC_HEADERS. Use the option NO_PUBLIC_HEADERS if it is intended.")
  endif()

  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS}
                            LIBRARIES ${ARG_LIBRARIES}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  elements_expand_sources(srcs ${ARG_UNPARSED_ARGUMENTS})
  set(cpp_srcs)
  set(i_srcs)
  foreach(s ${srcs})
    if(s MATCHES "(.*).i")
      list(APPEND i_srcs ${s})
    else()
      list(APPEND cpp_srcs ${s})
    endif()
    set_property(SOURCE ${s} PROPERTY CPLUSPLUS ON)
  endforeach()
  
  set(PY_MODULE_SWIG_SRC ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${binding}PYTHON_wrap.cxx)
  
  _generate_swig_files(${binding}
                       ${i_srcs}
                       OUTFILE ${PY_MODULE_SWIG_SRC}
                       INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  if(CXX_HAS_MISSING_FIELD_INITIALIZERS)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-missing-field-initializers")
  endif()

  if(CXX_HAS_CAST_FUNCTION_TYPE)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-cast-function-type")
  endif() 

  if(CXX_HAS_NO_SELF_ASSIGN)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-self-assign")    
  endif()

  if(CXX_HAS_NO_PARENTHESES_EQUALITY)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-parentheses-equality")
  endif()

  if(CXX_HAS_CONVERSION)
    set_property(SOURCE ${PY_MODULE_SWIG_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-conversion")
  endif()


  elements_add_python_module(${binding}
                             ${PY_MODULE_SWIG_SRC} ${cpp_srcs}
                             LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                             INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
  set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${binding}.py)
  
  if(NOT ("${SGS_HOST_OS}" STREQUAL "fc" AND (NOT SGS_HOST_OSVERS VERSION_LESS 30)))
    if(PYTHON_VERSION_MAJOR VERSION_LESS 3)
      set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${binding}.pyo)
      set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${binding}.pyc)
    endif()
  else()
    if(SQUEEZED_INSTALL AND (PYTHON_VERSION_MAJOR VERSION_LESS 3)
       AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
      set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${binding}.pyo)
      set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${binding}.pyc)
    endif()
  endif()

  elements_install_headers(${ARG_PUBLIC_HEADERS})

endfunction()

#---------------------------------------------------------------------------------------------------
# _generate_cython_cpp(interface
#                      OUTFILE out.cxx
#                      LINK_LIBRARIES library1 library2 ...
#                      INCLUDE_DIRS dir1 package2 ...
#                      LINKER_LANGUAGE C|CXX)
#
# Generate the C++ source file from the .pyx file using the INCLUDE_DIRS
#---------------------------------------------------------------------------------------------------
function(_generate_cython_cpp)

  find_package(Cython QUIET REQUIRED)
  CMAKE_PARSE_ARGUMENTS(ARG "" "OUTFILE;LINKER_LANGUAGE" "INCLUDE_DIRS;LINK_LIBRARIES" ${ARGN})
  
  if("${ARG_OUTFILE}" STREQUAL "")
    message(FATAL_ERROR "_generate_cython_cpp: No OUTFILE defined")
  endif()

  set(src ${ARG_UNPARSED_ARGUMENTS})

  # locate and set include directories
  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS})



  # get the source file directory
  get_source_file_property(src_location ${src} LOCATION)
  get_filename_component(pyx_dir ${src_location} DIRECTORY)

  elements_include_directories(${pyx_dir})

  get_property(cy_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  if(cy_dirs)
    list(REMOVE_DUPLICATES cy_dirs)
  endif()
  
  # pattern enabled recursion in non-system directories
  set(all_cy_dirs)
  foreach(_i ${cy_dirs})
    set(starts_with_sys FALSE)
    starts_with_sys_include(starts_with_sys ${_i})
    if(NOT ${starts_with_sys})
      elements_recurse(hsubdir ${_i} PATTERN "*.px[di]")
      if(hsubdir)
        list(REMOVE_DUPLICATES hsubdir)
        set(all_cy_dirs ${all_cy_dirs} ${hsubdir})    
      endif()
    endif()
    set(all_cy_dirs ${all_cy_dirs} ${_i})    
  endforeach()
  list(REMOVE_DUPLICATES all_cy_dirs)
  
  
  set(CYTHON_MOD_INCLUDE_DIRS)
  foreach(dir ${all_cy_dirs})
    set(CYTHON_MOD_INCLUDE_DIRS ${CYTHON_MOD_INCLUDE_DIRS} -I${dir})
  endforeach()  
  
  
  if ("${ARG_LINKER_LANGUAGE}" STREQUAL "CXX")
    set(linker_arg "--cplus")
  else()
    set(linker_arg "")
  endif()

  # Set additional flags.
  set(annotate_arg)
  if(CYTHON_ANNOTATE)
    set(annotate_arg "--annotate")
  endif()

  set(no_docstrings_arg)
  if( CYTHON_NO_DOCSTRINGS )
    set(no_docstrings_arg "--no-docstrings")
  endif()

  if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" OR
        "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
      set(cython_debug_arg "--gdb")
  endif()
  if("${PYTHONLIBS_VERSION_STRING}" MATCHES "^2.")
    set(version_arg "-2")
  elseif("${PYTHONLIBS_VERSION_STRING}" MATCHES "^3.")
    set(version_arg "-3")
  else()
    set(version_arg)
  endif()  
    
  add_custom_command(
    OUTPUT 
        ${ARG_OUTFILE}
    COMMAND 
        ${env_cmd} --xml ${env_xml} ${CYTHON_EXECUTABLE}
        ${linker_arg}
        ${CYTHON_MOD_INCLUDE_DIRS}
        ${version_arg}
        ${annotate_arg} 
        ${no_docstrings_arg} 
        ${cython_debug_arg}
        ${CYTHON_FLAGS}
        --output-file ${ARG_OUTFILE}
        ${src}
    DEPENDS 
        ${src}
    COMMENT "Generating Cython module: ${CYTHON_EXECUTABLE} ${linker_arg} ${CYTHON_MOD_INCLUDE_DIRS} ${version_arg} ${annotate_arg} ${no_docstrings_arg} ${cython_debug_arg} ${CYTHON_FLAGS} --output-file ${ARG_OUTFILE}  ${srcs}"
    )
  
  set_source_files_properties(${ARG_OUTFILE} PROPERTIES GENERATED TRUE)
  set_property(SOURCE ${ARG_OUTFILE} APPEND_STRING 
               PROPERTY COMPILE_FLAGS " -fvisibility=default -UELEMENTS_HIDE_SYMBOLS")  

  
  if(CXX_HAS_CAST_FUNCTION_TYPE)
    set_property(SOURCE ${ARG_OUTFILE} APPEND_STRING 
                 PROPERTY COMPILE_FLAGS " -Wno-cast-function-type")  
  endif()

endfunction()


#---------------------------------------------------------------------------------------------------
# elements_add_cython_module([interface] source1 source2 ...
#                            LINK_LIBRARIES library1 library2 ...
#                            INCLUDE_DIRS dir1 package2 ...
#                            [NO_PUBLIC_HEADERS | PUBLIC_HEADERS dir1 dir2 ...]
#                            LINKER_LANGUAGE C|CXX)
#
# Create a Cython binary python module from the specified sources (glob patterns are allowed), linking
# it with the libraries specified and adding the include directories to the search path. The sources
# can be either *.i or *.cpp files. Their location is relative to the base of the Elements package
# (module).
#---------------------------------------------------------------------------------------------------
function(elements_add_cython_module)

  CMAKE_PARSE_ARGUMENTS(ARG "NO_PUBLIC_HEADERS" "LINKER_LANGUAGE" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS;PUBLIC_HEADERS" ${ARGN})

  if(NOT ARG_LINKER_LANGUAGE)
    set(ARG_LINKER_LANGUAGE "CXX")
  endif()

  elements_expand_sources(srcs ${ARG_UNPARSED_ARGUMENTS})
  set(pyx_module_sources)
  set(other_module_sources)
  foreach( _file ${srcs})
    if( ${_file} MATCHES ".*\\.py[x]?$" )
      list( APPEND pyx_module_sources ${_file} )
    else()
      list( APPEND other_module_sources ${_file} )
    endif()
  endforeach()

  list(LENGTH pyx_module_sources nb_pyx)
  
  if(${nb_pyx} GREATER 1)
    message(FATAL_ERROR "To many pyx files for the Cython module: ${pyx_module_sources}")
  endif()

  get_filename_component(mod_name ${pyx_module_sources} NAME_WE)

  if(NOT ARG_NO_PUBLIC_HEADERS AND NOT ARG_PUBLIC_HEADERS)
    elements_get_package_name(package)
    message(WARNING "Cython module ${mod_name} (in ${package}) does not declare PUBLIC_HEADERS. Use the option NO_PUBLIC_HEADERS if it is intended.")
  endif()

  if("${ARG_LINKER_LANGUAGE}" STREQUAL "CXX")
    set(PY_MODULE_CYTHON_SRC ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${mod_name}CYTHON_wrap.cxx)
  else()
    set(PY_MODULE_CYTHON_SRC ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${mod_name}CYTHON_wrap.c)
  endif()

  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS}
                            LIBRARIES ${ARG_LIBRARIES}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  _generate_cython_cpp(${pyx_module_sources}
                       OUTFILE ${PY_MODULE_CYTHON_SRC}
                       LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                       INCLUDE_DIRS ${ARG_INCLUDE_DIRS}
                       LINKER_LANGUAGE ${ARG_LINKER_LANGUAGE})

  if(CXX_HAS_MISSING_FIELD_INITIALIZERS)
    set_property(SOURCE ${PY_MODULE_CYTHON_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-missing-field-initializers")
  endif()

  if(CXX_HAS_UNUSED_FUNCTION)
    set_property(SOURCE ${PY_MODULE_CYTHON_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-unused-function")
  endif()

  if(CXX_HAS_NO_UNNEEDED_INTERNAL_DECLARATION)
    set_property(SOURCE ${PY_MODULE_CYTHON_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-unneeded-internal-declaration")
  endif()

  if(CXX_HAS_NO_CXX17_EXTENSIONS)
    set_property(SOURCE ${PY_MODULE_CYTHON_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-c++17-extensions")  
  endif()

  if(CXX_HAS_NO_PARENTHESES_EQUALITY)
    set_property(SOURCE ${PY_MODULE_CYTHON_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-parentheses-equality")  
  endif()

  if(CXX_HAS_NO_CONSTANT_LOGICAL_OPERAND)
    set_property(SOURCE ${PY_MODULE_CYTHON_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-constant-logical-operand")    
  endif()

  if(CXX_HAS_SHADOW)
    set_property(SOURCE ${PY_MODULE_CYTHON_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-shadow")    
  endif()

  if(CXX_HAS_CONVERSION)
    set_property(SOURCE ${PY_MODULE_CYTHON_SRC} APPEND_STRING
                 PROPERTY COMPILE_FLAGS " -Wno-conversion")
  endif()


  elements_add_python_module(${mod_name}
                             PLAIN_MODULE
                             ${PY_MODULE_CYTHON_SRC} ${other_module_sources}
                             LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                             INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  set_target_properties(${mod_name} PROPERTIES PLAIN_MODULE TRUE)
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
  
  elements_install_headers(${ARG_PUBLIC_HEADERS})

endfunction()

#---------------------------------------------------------------------------------------------------
# elements_add_pybind11_module(module source1 source2 ...
#                              LINK_LIBRARIES library1 library2 ...
#                              INCLUDE_DIRS dir1 package2 ...
#                              LINKER_LANGUAGE C|CXX)
#
# Create a pybind11 binary python module from the specified sources (glob patterns are allowed), linking
# it with the libraries specified and adding the include directories to the search path. The sources
# can be either *.i or *.cpp files. Their location is relative to the base of the Elements package
# (module).
#---------------------------------------------------------------------------------------------------
function(elements_add_pybind11_module module)

  CMAKE_PARSE_ARGUMENTS(ARG "" "LINKER_LANGUAGE" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS" ${ARGN})

  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS}
                            LIBRARIES ${ARG_LIBRARIES}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  # require Python libraries
  find_package(PythonLibs ${PYTHON_EXPLICIT_VERSION} QUIET REQUIRED)

  elements_include_directories(AFTER ${PYTHON_INCLUDE_DIRS})
  
  find_package(pybind11)

  pybind11_add_module(${module} ${srcs})

  if(ARG_LINKER_LANGUAGE)
    set_target_properties(${module} PROPERTIES LINKER_LANGUAGE ${ARG_LINKER_LANGUAGE})
  endif()

  target_link_libraries(${module} PRIVATE ${PYTHON_LIBRARIES} ${ARG_LINK_LIBRARIES})

  _elements_detach_debinfo(${module})

  #----Installation details-------------------------------------------------------
  install(TARGETS ${module} LIBRARY DESTINATION ${PYTHON_DYNLIB_INSTALL_SUFFIX} OPTIONAL)
  set_target_properties(${module} PROPERTIES INSTALL_RPATH "$ORIGIN/../../${CMAKE_LIB_INSTALL_SUFFIX}")

  get_property(_prefix TARGET ${module} PROPERTY PREFIX)
  get_property(_suffix TARGET ${module} PROPERTY SUFFIX)

  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
  set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_DYNLIB_OBJECTS ${_prefix}${module}${_suffix})


endfunction()
#---------------------------------------------------------------------------------------------------
# elements_add_executable(<name>
#                      source1 source2 ...
#                      LINK_LIBRARIES library1 library2 ...
#                      INCLUDE_DIRS dir1 package2 ...)
#
# Extension of standard CMake 'add_executable' command.
# Create a library from the specified sources (glob patterns are allowed), linking
# it with the libraries specified and adding the include directories to the search path.
#---------------------------------------------------------------------------------------------------
function(elements_add_executable executable)

  CMAKE_PARSE_ARGUMENTS(ARG "NO_INSTALL" "" "" ${ARGN})

  elements_common_add_build(${ARGN})

  add_executable(${executable} ${srcs})
  target_link_libraries(${executable} PRIVATE ${ARG_LINK_LIBRARIES})
  _elements_detach_debinfo(${executable})

  if (ELEMENTS_USE_EXE_SUFFIX)
    set_target_properties(${executable} PROPERTIES SUFFIX .exe)
    set_target_properties(${executable} PROPERTIES BASENAME ${executable}.exe)
  else()
    set_target_properties(${executable} PROPERTIES BASENAME ${executable}.exe)
  endif()

  #----Installation details-------------------------------------------------------
  
  if (NOT ARG_NO_INSTALL)
    install(TARGETS ${executable} EXPORT ${CMAKE_PROJECT_NAME}Exports RUNTIME DESTINATION bin OPTIONAL)
    install(EXPORT ${CMAKE_PROJECT_NAME}Exports DESTINATION ${CMAKE_INSTALL_SUFFIX} OPTIONAL)
    elements_export(EXECUTABLE ${executable})
    set_property(GLOBAL APPEND PROPERTY REGULAR_BIN_OBJECTS ${executable})
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
    set_property(GLOBAL APPEND PROPERTY REGULAR_CMAKE_OBJECTS ${CMAKE_PROJECT_NAME}Exports.cmake)
    string(TOLOWER ${CMAKE_BUILD_TYPE} lower_cmake_build_type)
    set_property(GLOBAL APPEND PROPERTY REGULAR_CMAKE_OBJECTS ${CMAKE_PROJECT_NAME}Exports-${lower_cmake_build_type}.cmake)
  endif()

endfunction()

#---------------------------------------------------------------------------------------------------
# elements_add_test_executable(<name>
#                              source1 source2 ...
#                              LINK_LIBRARIES library1 library2 ...
#                              INCLUDE_DIRS dir1 package2 ...
#                              [TYPE Boost|CppUnit])
#
# Special version of elements_add_executable which automatically adds the dependency
# on CppUnit or Boost
#---------------------------------------------------------------------------------------------------
function(elements_add_test_executable name)

  CMAKE_PARSE_ARGUMENTS(${name}_TEST_EXEC "" "TYPE" "" ${ARGN})

  elements_common_add_build(${${name}_TEST_EXEC_UNPARSED_ARGUMENTS})

  if(NOT ${name}_TEST_EXEC_TYPE)
    set(${name}_TEST_EXEC_TYPE None)
  endif()

  set(${name}_TEST_EXEC_EXECUTABLE ${name})
  set(executable ${${name}_TEST_EXEC_EXECUTABLE})

  elements_get_package_name(package)


  if(NOT "${${name}_TEST_EXEC_TYPE}" STREQUAL "None")
  
    if ("${${name}_TEST_EXEC_TYPE}" STREQUAL "Boost")
      find_package(Boost COMPONENTS unit_test_framework QUIET REQUIRED)
    else()
      find_package(${${name}_TEST_EXEC_TYPE} QUIET REQUIRED)
    endif()
    
    if (NOT TARGET ${package}_tests_dir)
      add_custom_target(${package}_tests_dir
                        COMMAND  ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/tests
                        COMMENT "Generating The ${package}/tests directory" VERBATIM)
    endif()
    
    set(testmain_file ${CMAKE_CURRENT_BINARY_DIR}/tests/${${name}_TEST_EXEC_TYPE}TestMain.cpp)
    set_source_files_properties(${testmain_file} PROPERTIES GENERATED TRUE)
    if ("${${name}_TEST_EXEC_TYPE}" STREQUAL "Boost")
      set_source_files_properties(${testmain_file} PROPERTIES COMPILE_DEFINITIONS_DEBUG "BOOST_TEST_TOOLS_UNDER_DEBUGGER")
      set_source_files_properties(${srcs} PROPERTIES COMPILE_DEFINITIONS_DEBUG "BOOST_TEST_TOOLS_UNDER_DEBUGGER")
    endif()
    if(NOT TARGET ${package}_${${name}_TEST_EXEC_TYPE}TestMain)
      add_custom_target(${package}_${${name}_TEST_EXEC_TYPE}TestMain
                        COMMAND ${${${name}_TEST_EXEC_TYPE}_testmain_cmd} --quiet ${package} ${testmain_file}
                        DEPENDS ${package}_tests_dir
                        COMMENT "Generating the ${package} ${${name}_TEST_EXEC_TYPE}TestMain.cpp" VERBATIM)
      set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${testmain_file})
    endif()
      
    set(INSTALL_OPT)
      
    if(NOT INSTALL_TESTS)
      set(INSTALL_OPT "NO_INSTALL")
    endif()
      
    if (NOT TARGET ${package}${${name}_TEST_EXEC_TYPE}Test)
      elements_add_library(${package}${${name}_TEST_EXEC_TYPE}Test ${testmain_file}
                           LINK_LIBRARIES ${${name}_TEST_EXEC_TYPE}
                           INCLUDE_DIRS ${${name}_TEST_EXEC_TYPE}
                           NO_PUBLIC_HEADERS ${INSTALL_OPT}
                           )
      add_dependencies(${package}${${name}_TEST_EXEC_TYPE}Test ${package}_${${name}_TEST_EXEC_TYPE}TestMain)
    endif()
      
    elements_add_executable(${executable} ${srcs}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES} ${${name}_TEST_EXEC_TYPE} ${package}${${name}_TEST_EXEC_TYPE}Test
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS} ${${name}_TEST_EXEC_TYPE}
                            ${INSTALL_OPT})
    add_dependencies(${executable} ${package}_${${name}_TEST_EXEC_TYPE}TestMain)

  else()
    elements_add_executable(${executable} ${srcs}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS}
                            ${INSTALL_OPT})
  endif()


  get_target_property(exec_suffix ${executable} SUFFIX)
  if(NOT exec_suffix)
    set(exec_suffix)
  endif()

endfunction()

#---------------------------------------------------------------------------------------------------
# elements_add_unit_test(<name>
#                     source1 source2 ...
#                     [EXECUTABLE exename]
#                     LINK_LIBRARIES library1 library2 ...
#                     INCLUDE_DIRS dir1 package2 ...
#                     [WORKING_DIRECTORY dir]
#                     [ENVIRONMENT variable[+]=value ...]
#                     [TIMEOUT seconds]
#                     [TYPE Boost|CppUnit])
#
# Special version of elements_add_executable which automatically adds the dependency
# on CppUnit and declares the test to CTest (add_test).
# The WORKING_DIRECTORY option can be passed if the command needs to be run from
# a fixed directory.
# If special environment settings are needed, they can be specified in the
# section ENVIRONMENT as <var>=<value> or <var>+=<value>, where the second format
# prepends the value to the PATH-like variable.
# The default TYPE is CppUnit and Boost can also be specified.
#---------------------------------------------------------------------------------------------------
function(elements_add_unit_test name)
  if(ELEMENTS_BUILD_TESTS)

    CMAKE_PARSE_ARGUMENTS(${name}_UNIT_TEST "" "EXECUTABLE;TYPE;TIMEOUT;WORKING_DIRECTORY" "ENVIRONMENT;LABELS" ${ARGN})

    elements_common_add_build(${${name}_UNIT_TEST_UNPARSED_ARGUMENTS})

    if(NOT ${name}_UNIT_TEST_TYPE)
      set(${name}_UNIT_TEST_TYPE None)
    endif()

    if(NOT ${name}_UNIT_TEST_WORKING_DIRECTORY)
      set(${name}_UNIT_TEST_WORKING_DIRECTORY .)
    endif()

    if(NOT ${name}_UNIT_TEST_EXECUTABLE)
      set(${name}_UNIT_TEST_EXECUTABLE ${name})
    endif()

    set(executable ${${name}_UNIT_TEST_EXECUTABLE})

    elements_get_package_name(package)


    elements_add_test_executable(${executable} ${srcs}
                                 LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                                 INCLUDE_DIRS ${ARG_INCLUDE_DIRS}
                                 TYPE ${${name}_UNIT_TEST_TYPE})


    foreach(var ${${name}_UNIT_TEST_ENVIRONMENT})
      string(FIND ${var} "+=" is_prepend)
      if(NOT is_prepend LESS 0)
        # the argument contains +=
        string(REPLACE "+=" "=" var ${var})
        set(extra_env ${extra_env} -p ${var})
      else()
        set(extra_env ${extra_env} -s ${var})
      endif()
    endforeach()
    
    set(exec_argument)
    
    if(TEST_XML_REPORT)
      if ("${${name}_UNIT_TEST_TYPE}" STREQUAL "Boost")
        if(TEST_JUNIT_REPORT AND NOT (Boost_VERSION_STRING VERSION_LESS 1.63.0))
          set(exec_argument --log_format=JUNIT --log_sink=${PROJECT_BINARY_DIR}/Testing/Temporary/${executable}.${${name}_UNIT_TEST_TYPE}.JUnit.xml --log_level=all)
        else()
          set(exec_argument --log_format=XML --log_sink=${PROJECT_BINARY_DIR}/Testing/Temporary/${executable}.${${name}_UNIT_TEST_TYPE}.xml --log_level=all)      
        endif()
      endif()
    endif()

    add_test(NAME ${package}.${name}
             WORKING_DIRECTORY ${${name}_UNIT_TEST_WORKING_DIRECTORY}
             COMMAND ${env_cmd} ${extra_env} --xml ${env_xml}
             ${executable}${exec_suffix} ${exec_argument})

    set_property(GLOBAL APPEND PROPERTY TEST_LIST ${package}.${name}:${executable}${exec_suffix})

    set_property(TEST ${package}.${name} PROPERTY CMDLINE "${executable}${exec_suffix}")
    set_property(TEST ${package}.${name} APPEND PROPERTY LABELS UnitTest ${package} Binary)
    
    if(NOT "${${name}_UNIT_TEST_TYPE}" STREQUAL "None")
      set_property(TEST ${package}.${name} APPEND PROPERTY LABELS ${${name}_UNIT_TEST_TYPE})
    endif()

    foreach(t ${${name}_UNIT_TEST_LABELS})
      set_property(TEST ${package}.${name} APPEND PROPERTY LABELS ${t})
    endforeach()

    if(${name}_UNIT_TEST_TIMEOUT)
      set_property(TEST ${package}.${name} PROPERTY TIMEOUT ${${name}_UNIT_TEST_TIMEOUT})
    endif()

  endif()
endfunction()


#-------------------------------------------------------------------------------
# elements_add_test(<name>
#                   [FRAMEWORK options1 options2 ...|COMMAND cmd args ...]
#                   [WORKING_DIRECTORY dir]
#                   [ENVIRONMENT variable[+]=value ...]
#                   [DEPENDS other_test ...]
#                   [FAILS] [PASSREGEX regex] [FAILREGEX regex]
#                   [LABELS label1 label2]
#                   [TIMEOUT seconds])
#
# Declare a run-time test in the subdirectory.
# The test can be of the types:
#  FRAMEWORK - run a job with the specified options
#  COMMAND - execute a command
# If special environment settings are needed, they can be specified in the
# section ENVIRONMENT as <var>=<value> or <var>+=<value>, where the second format
# prepends the value to the PATH-like variable.
# The WORKING_DIRECTORY option can be passed if the command needs to be run from
# a fixed directory (ignored by QMTEST tests).
# Great flexibility is given by the following options:
#  FAILS - the tests succeds if the command fails (return code !=0)
#  DEPENDS - ensures an order of execution of tests (e.g. do not run a read
#            test if the write one failed)
#  PASSREGEX - Specify a regexp; if matched in the output the test is successful
#  FAILREGEX - Specify a regexp; if matched in the output the test is failed
#
#-------------------------------------------------------------------------------
function(elements_add_test name)
  CMAKE_PARSE_ARGUMENTS(ARG "FAILS" "TIMEOUT;WORKING_DIRECTORY" "ENVIRONMENT;FRAMEWORK;COMMAND;DEPENDS;PASSREGEX;FAILREGEX;LABELS" ${ARGN})
  elements_get_package_name(package)

  if(ARG_FRAMEWORK)
    foreach(optfile  ${ARG_FRAMEWORK})
      if(IS_ABSOLUTE ${optfile})
        set(optfiles ${optfiles} ${optfile})
      else()
        set(optfiles ${optfiles} ${CMAKE_CURRENT_SOURCE_DIR}/${optfile})
      endif()
    endforeach()
    set(cmdline ${elementsrun_cmd} ${optfiles})

  elseif(ARG_COMMAND)
    set(cmdline ${ARG_COMMAND})

  else()
    message(FATAL_ERROR "Type of test '${name}' not declared")
  endif()

  if(NOT ARG_WORKING_DIRECTORY)
    set(ARG_WORKING_DIRECTORY .)
  endif()


  foreach(var ${ARG_ENVIRONMENT})
    string(FIND ${var} "+=" is_prepend)
    if(NOT is_prepend LESS 0)
      # the argument contains +=
      string(REPLACE "+=" "=" var ${var})
      set(extra_env ${extra_env} -p ${var})
    else()
      set(extra_env ${extra_env} -s ${var})
    endif()
  endforeach()

  add_test(NAME ${package}.${name}
           WORKING_DIRECTORY ${ARG_WORKING_DIRECTORY}
           COMMAND ${env_cmd} ${extra_env} --xml ${env_xml}
           ${cmdline})

  set_property(TEST ${package}.${name} APPEND PROPERTY LABELS ${package})
  set_property(TEST ${package}.${name} PROPERTY CMDLINE "${cmdline}")

  foreach(t ${ARG_LABELS})
    set_property(TEST ${package}.${name} APPEND PROPERTY LABELS ${t})
  endforeach()


  if(ARG_DEPENDS)
    foreach(t ${ARG_DEPENDS})
      list(APPEND depends ${package}.${t})
    endforeach()
    set_property(TEST ${package}.${name} PROPERTY DEPENDS ${depends})
  endif()

  if(ARG_FAILS)
    set_property(TEST ${package}.${name} PROPERTY WILL_FAIL TRUE)
  endif()

  if(ARG_PASSREGEX)
    set_property(TEST ${package}.${name} PROPERTY PASS_REGULAR_EXPRESSION ${ARG_PASSREGEX})
  endif()

  if(ARG_FAILREGEX)
    set_property(TEST ${package}.${name} PROPERTY FAIL_REGULAR_EXPRESSION ${ARG_FAILREGEX})
  endif()

  if(ARG_TIMEOUT)
    set_property(TEST ${package}.${name} PROPERTY TIMEOUT ${ARG_TIMEOUT})
  endif()

endfunction()

#---------------------------------------------------------------------------------------------------
# elements_install_headers(dir1 dir2 ...)
#
# Install the declared directories in the 'include' directory.
# To be used in case the header files do not have a library.
#---------------------------------------------------------------------------------------------------
function(elements_install_headers)

  CMAKE_PARSE_ARGUMENTS(ARG "NO_EXIST_CHECK" "" "" ${ARGN})

  set(has_local_headers FALSE)
  foreach(hdr_dir ${ARG_UNPARSED_ARGUMENTS})
    if(NOT IS_ABSOLUTE ${hdr_dir})
      set(full_hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/${hdr_dir})
    else()
      set(full_hdr_dir ${hdr_dir})
    endif()
    if(ARG_NO_EXIST_CHECK)
      install(DIRECTORY ${hdr_dir}
              DESTINATION ${INCLUDE_INSTALL_SUFFIX}
              FILES_MATCHING
              PATTERN "*.h"
              PATTERN "*.icpp"
              PATTERN "*.tpp"
              PATTERN "*.hpp"
              PATTERN "*.hxx"
              PATTERN "*.i"
              PATTERN "*.pxd"
              PATTERN "CVS" EXCLUDE
              PATTERN ".svn" EXCLUDE)
      if(NOT IS_ABSOLUTE ${hdr_dir})
        set(has_local_headers TRUE)
        set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${hdr_dir})
      endif()
    else()
      if(IS_DIRECTORY ${full_hdr_dir})
        install(DIRECTORY ${hdr_dir}
                DESTINATION ${INCLUDE_INSTALL_SUFFIX}
                FILES_MATCHING
                PATTERN "*.h"
                PATTERN "*.icpp"
                PATTERN "*.tpp"
                PATTERN "*.hpp"
                PATTERN "*.hxx"
                PATTERN "*.i"
                PATTERN "*.pxd"
                PATTERN "CVS" EXCLUDE
                PATTERN ".svn" EXCLUDE)
        if(NOT IS_ABSOLUTE ${hdr_dir})
          set(has_local_headers TRUE)
          set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${hdr_dir})
        endif()
      else()
        message(FATAL_ERROR "No ${hdr_dir} directory for header files in the ${CMAKE_CURRENT_SOURCE_DIR} location")
      endif()
    endif()
  endforeach()
  # flag the current directory as one that installs headers
  #   the property is used when collecting the include directories for the
  #   dependent subdirs
  if(has_local_headers)
    set_property(DIRECTORY PROPERTY INSTALLS_LOCAL_HEADERS TRUE)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
  endif()
endfunction()

#---------------------------------------------------------------------------------------------------
# add_python_test_dir(dir1
#                     PREFIX ""
#                     NAME ""
#                     PATTERN *.py
#                     TIMEOUT ""
#                     EXCLUDE ""
#                     ENVIRONMENT ""
#                     )
#
# Add the python files in the directory as test. It collects the python test files
# and add a test for the python test framework (py.test, nose or unittest)
#---------------------------------------------------------------------------------------------------
function(add_python_test_dir)

  CMAKE_PARSE_ARGUMENTS(PYTEST_ARG "" "PREFIX;PATTERN;NAME;TIMEOUT" "EXCLUDE;ENVIRONMENT" ${ARGN})

  if(NOT PYTEST_ARG_UNPARSED_ARGUMENTS)
      set(PYTEST_ARG_UNPARSED_ARGUMENTS "tests/python")
  endif()

  if(NOT PYTEST_ARG_PATTERN)
    set(PYTEST_ARG_PATTERN "*.py")
  endif()

  if(PYFRMK_TEST)
    set(pytest_type ${PYFRMK_NAME})
  else()
    set(pytest_type Python)
  endif()

  if(PYTEST_ARG_NAME)
      set(pytest_name ${PYTEST_ARG_NAME})
  else()
    if(PYTEST_ARG_PREFIX)
      set(pytest_name ${PYTEST_ARG_PREFIX}.${pytest_type})
    else()
     set(pytest_name ${pytest_type})
    endif()
  endif()


  foreach(pytestsubdir ${PYTEST_ARG_UNPARSED_ARGUMENTS})
    elements_expand_sources(tmp_pysrcs ${CMAKE_CURRENT_SOURCE_DIR}/${pytestsubdir}/${PYTEST_ARG_PATTERN})
    set(pysrcs ${pysrcs} ${tmp_pysrcs})
  endforeach()

  if(PYTEST_ARG_EXCLUDE)
    foreach(i_arg_ex ${PYTEST_ARG_EXCLUDE})
      list(REMOVE_ITEM pysrcs ${CMAKE_CURRENT_SOURCE_DIR}/${i_arg_ex})
    endforeach()
  endif()

  elements_get_package_name(package)

  if(PYFRMK_TEST)
    set(PYFRMK_JUNIT_FILE_OPT)
    set(PYFRMK_COVERAGE_OPT)
    if("${PYFRMK_NAME}" STREQUAL "PyTest")
      if(TEST_JUNIT_REPORT)
        set(PYFRMK_JUNIT_FILE_OPT --junit-xml=${PROJECT_BINARY_DIR}/Testing/Temporary/${package}.${pytest_name}.JUnit.xml)
      endif()
      if("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage")
        find_python_module(pytest_cov)
        if(PY_PYTEST_COV)
          set(PYFRMK_COVERAGE_OPT  --cov-report=xml:${PROJECT_BINARY_DIR}/Testing/Temporary/${PYFRMK_NAME}_coverage.xml)
          set(PYFRMK_COVERAGE_OPT ${PYFRMK_COVERAGE_OPT} --cov-append)
        endif()
        get_property(proj_python_package_list GLOBAL PROPERTY PROJ_PYTHON_PACKAGE_LIST)
        foreach(p ${proj_python_package_list})
          set(PYFRMK_COVERAGE_OPT ${PYFRMK_COVERAGE_OPT} --cov ${p})
        endforeach()
      endif()
    endif()
    elements_add_test(${pytest_name}
                      COMMAND  ${PYFRMK_TEST} ${PYFRMK_JUNIT_FILE_OPT} ${PYFRMK_COVERAGE_OPT} ${pysrcs}
                      ENVIRONMENT ${PYTEST_ARG_ENVIRONMENT})
    set_property(TEST ${package}.${pytest_name} APPEND PROPERTY LABELS Python UnitTest ${PYFRMK_NAME})
    if(PYTEST_ARG_TIMEOUT)
      set_property(TEST ${package}.${pytest_name} PROPERTY TIMEOUT ${PYTEST_ARG_TIMEOUT})
    endif()
  else()
    if(NOT PYTHON_VERSION_STRING VERSION_LESS "2.7")
      foreach(pytestsubdir ${PYTEST_ARG_UNPARSED_ARGUMENTS})
        set(pytest_name "${pytest_name}:${pytestsubdir}")
        elements_add_test(${pytest_name}
                          COMMAND ${PYTHON_EXECUTABLE} -m unittest discover -s ${CMAKE_CURRENT_SOURCE_DIR}/${pytestsubdir} -p "${PYTEST_ARG_PATTERN}" 
                          ENVIRONMENT ${PYTEST_ARG_ENVIRONMENT})
        set_property(TEST ${package}.${pytest_name} APPEND PROPERTY LABELS Python UnitTest)
        if(PYTEST_ARG_TIMEOUT)
          set_property(TEST ${package}.${pytest_name} PROPERTY TIMEOUT ${PYTEST_ARG_TIMEOUT})
        endif()
      endforeach()
    endif()
  endif()


  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON_TEST TRUE)

endfunction()



#-------------------------------------------------------------------------------
# elements_install_python_modules([subdir1 [subdir2 ...]])
#
# Declare that the subdirectory needs to install python modules.
# The hierarchy of directories and  files in the python directory will be
# installed.  If the first level of directories do not contain __init__.py, a
# warning is issued and an empty one will be installed.
#
# If no argument is given the default subdirectory is python.
#
# Note: We need to avoid conflicts with the automatic generated __init__.py for
#       configurables (elements_generate_configurables)
#       There are 2 cases:
#       * install_python called before genconf
#         we fill the list of modules to tell genconf not to install its dummy
#         version
#       * genconf called before install_python
#         we install on top of the one installed by genconf
# FIXME: it should be cleaner
#-------------------------------------------------------------------------------
function(elements_install_python_modules)

  CMAKE_PARSE_ARGUMENTS(INSTALL_PY_MOD "" "TEST_TIMEOUT" "TEST_EXCLUDE;TEST_ENVIRONMENT" ${ARGN})

  if(NOT INSTALL_PY_MOD_UNPARSED_ARGUMENTS)
      set(INSTALL_PY_MOD_UNPARSED_ARGUMENTS "python")
  endif()

  foreach(pysubdir ${INSTALL_PY_MOD_UNPARSED_ARGUMENTS})

    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${pysubdir})
      install(DIRECTORY ${pysubdir}/
              DESTINATION ${PYTHON_INSTALL_SUFFIX}
              FILES_MATCHING
              PATTERN "*.py"
              PATTERN "CVS" EXCLUDE
              PATTERN ".svn" EXCLUDE)
    # check for the presence of the __init__.py's and install them if needed
      file(GLOB sub-dir RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${pysubdir}/*)
      foreach(dir ${sub-dir})
        if(NOT "${dir}" STREQUAL "${pysubdir}/.svn"
           AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir}
           AND NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/__init__.py)
          set(pyfile ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/__init__.py)
          file(RELATIVE_PATH pyfile ${CMAKE_BINARY_DIR} ${pyfile})
          message(WARNING "The file ${pyfile} is missing. I shall install an empty one.")
          if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)
            file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/__init__.py "# Empty file generated automatically\n")
          endif()
          install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
                  DESTINATION ${CMAKE_INSTALL_PREFIX}/${dir})
        endif()
        # Add the Python module name to the list of provided ones.
        get_filename_component(modname ${dir} NAME)
        set_property(DIRECTORY APPEND PROPERTY has_python_modules ${modname})
        set_property(DIRECTORY PROPERTY module_has_python_dir yes)
        if(NOT "${dir}" STREQUAL "${pysubdir}/.svn")
          set_property(GLOBAL APPEND PROPERTY PROJ_PYTHON_PACKAGE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
          set_property(GLOBAL APPEND PROPERTY REGULAR_PYTHON_OBJECTS ${modname})
        endif()
      endforeach()
      set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
      if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/python)
        if(INSTALL_PY_MOD_TEST_TIMEOUT)
          add_python_test_dir(tests/python 
                              TIMEOUT ${INSTALL_PY_MOD_TEST_TIMEOUT} 
                              EXCLUDE ${INSTALL_PY_MOD_TEST_EXCLUDE}
                              ENVIRONMENT ${INSTALL_PY_MOD_TEST_ENVIRONMENT})
        else()
          add_python_test_dir(tests/python 
                              EXCLUDE ${INSTALL_PY_MOD_TEST_EXCLUDE}
                              ENVIRONMENT ${INSTALL_PY_MOD_TEST_ENVIRONMENT})
        endif()
      endif()
    else()
      message(FATAL_ERROR "No python directory ${pysubdir} in the ${CMAKE_CURRENT_SOURCE_DIR} location")
    endif()

  endforeach()

endfunction()

#---------------------------------------------------------------------------------------------------
# elements_install_scripts()
#
# Declare that the package needs to install the content of the 'scripts' directory.
#---------------------------------------------------------------------------------------------------
function(elements_install_scripts)

  CMAKE_PARSE_ARGUMENTS(INSTALL_SCR "" "" "" ${ARGN})

  if(NOT INSTALL_SCR_UNPARSED_ARGUMENTS)
      set(INSTALL_SCR_UNPARSED_ARGUMENTS "scripts")
  endif()

  foreach(scrsubdir ${INSTALL_SCR_UNPARSED_ARGUMENTS})

    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${scrsubdir})
      install(DIRECTORY ${scrsubdir}/ DESTINATION ${SCRIPT_INSTALL_SUFFIX}
              FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                               GROUP_EXECUTE GROUP_READ
                               WORLD_EXECUTE WORLD_READ
              PATTERN "CVS" EXCLUDE
              PATTERN ".svn" EXCLUDE
              PATTERN "*~" EXCLUDE
              PATTERN "*.pyc" EXCLUDE
              PATTERN "__pycache__" EXCLUDE
              PATTERN "*.pyo" EXCLUDE)
      set_property(GLOBAL APPEND PROPERTY PROJ_HAS_SCRIPTS TRUE)
      file(GLOB scr_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${scrsubdir} ${CMAKE_CURRENT_SOURCE_DIR}/${scrsubdir}/*)
      foreach(scr ${scr_list})
          set_property(GLOBAL APPEND PROPERTY REGULAR_SCRIPT_OBJECTS ${scr})
      endforeach()
    else()
      message(FATAL_ERROR "No \"${scrsubdir}\" scripts directory in the ${CMAKE_CURRENT_SOURCE_DIR} location")
    endif()

  endforeach()

endfunction()

#---------------------------------------------------------------------------------------------------
# elements_install_aux_files()
#
# Declare that the package needs to install the content of the 'aux' directory.
#---------------------------------------------------------------------------------------------------
function(elements_install_aux_files)

  # early check at configure time for the existence of the directory
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/aux OR IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${AUX_DIR_NAME} OR IS_DIRECTORY ${CMAKE_BINARY_DIR}/${AUX_DIR_NAME})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/aux)
      message(WARNING "The aux directory name in the ${CMAKE_CURRENT_SOURCE_DIR} location is dangerous. Please rename it to ${AUX_DIR_NAME}")
    endif()
    foreach(ad ${CMAKE_CURRENT_SOURCE_DIR}/aux ${CMAKE_CURRENT_SOURCE_DIR}/${AUX_DIR_NAME} ${CMAKE_BINARY_DIR}/${AUX_DIR_NAME})
      if(IS_DIRECTORY ${ad})
        install(DIRECTORY ${ad}/
               DESTINATION ${AUX_INSTALL_SUFFIX}
               PATTERN "CVS" EXCLUDE
               PATTERN ".svn" EXCLUDE
               PATTERN "*~" EXCLUDE)
        file(GLOB aux_list RELATIVE ${ad} ${ad}/*)
        foreach(af ${aux_list})
          set_property(GLOBAL APPEND PROPERTY REGULAR_AUX_OBJECTS ${af})
        endforeach()
        set_property(GLOBAL APPEND PROPERTY PROJ_HAS_AUX TRUE)
      endif()
    endforeach()
  else()
    message(FATAL_ERROR "No ${AUX_DIR_NAME} directory in the ${CMAKE_CURRENT_SOURCE_DIR} location")
  endif()
endfunction()

#---------------------------------------------------------------------------------------------------
# elements_install_conf_files()
#
# Declare that the package needs to install the content of the 'conf' directory.
#---------------------------------------------------------------------------------------------------
function(elements_install_conf_files)

  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/conf)
    install(DIRECTORY ${CONF_DIR_NAME}/
            DESTINATION ${CONF_INSTALL_SUFFIX}
            PATTERN "CVS" EXCLUDE
            PATTERN ".svn" EXCLUDE
            PATTERN "*~" EXCLUDE)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CONF TRUE)
    file(GLOB conf_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/conf ${CMAKE_CURRENT_SOURCE_DIR}/conf/*)
    foreach(cf ${conf_list})
      set_property(GLOBAL APPEND PROPERTY REGULAR_CONF_OBJECTS ${cf})
    endforeach()
  else()
    message(FATAL_ERROR "No ${CONF_DIR_NAME} directory in the ${CMAKE_CURRENT_SOURCE_DIR} location")
  endif()

endfunction()


#---------------------------------------------------------------------------------------------------
# elements_alias(name command...)
#
# Create a shell script that wraps the call to the specified command, as in
# usual Unix shell aliases.
#---------------------------------------------------------------------------------------------------
function(elements_alias name)
  if(NOT ARGN)
    message(FATAL_ERROR "No command specified for wrapper (alias) ${name}")
  endif()
  # prepare actual command line
  set(cmd)
  foreach(arg ${ARGN})
    set(cmd "${cmd} \"${arg}\"")
  endforeach()
  # create wrapper
  file(WRITE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${name}
       "#!/bin/sh
exec ${cmd} \"\$@\"
")
  # make it executable
  execute_process(COMMAND chmod 755 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${name})
  # install
  install(PROGRAMS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${name} DESTINATION ${SCRIPT_INSTALL_SUFFIX})

  set_property(GLOBAL APPEND PROPERTY REGULAR_SCRIPT_OBJECTS ${name})
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_SCRIPTS TRUE)
endfunction()

#-------------------------------------------------------------------------------
# elements_install_cmake_modules()
#
# Install the content of the cmake directory.
#-------------------------------------------------------------------------------
macro(elements_install_cmake_modules)
  install(DIRECTORY cmake/
          DESTINATION ${CMAKE_INSTALL_SUFFIX}
          FILES_MATCHING
            PATTERN "*.cmake"
            PATTERN "*.in"
            PATTERN "CVS" EXCLUDE
            PATTERN ".svn" EXCLUDE)
  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
  set_property(DIRECTORY PROPERTY ELEMENTS_EXPORTED_CMAKE ON)
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
  file(GLOB cm_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake/*)
  foreach(cm ${cm_list})
    set_property(GLOBAL APPEND PROPERTY REGULAR_CMAKE_OBJECTS ${cm})
  endforeach()
endmacro()

#-------------------------------------------------------------------------------
# elements_generate_project_config_version_file()
#
# Create the file used by CMake to check if the found version of a package
# matches the requested one.
#-------------------------------------------------------------------------------
macro(elements_generate_project_config_version_file)
  message(STATUS "Generating ${CMAKE_PROJECT_NAME}ConfigVersion.cmake")

  if(NOT "${CMAKE_PROJECT_VERSION_PATCH}" STREQUAL "")
    set(vers_id ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH})
  else()
    set(vers_id ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR})
  endif()

  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)
  file(WRITE ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
"set(PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(PACKAGE_VERSION ${vers_id})
if(\"\${PACKAGE_NAME}\" STREQUAL \"\${PACKAGE_FIND_NAME}\")
  if(\"\${PACKAGE_VERSION}\" STREQUAL \"\${PACKAGE_FIND_VERSION}\")
    set(PACKAGE_VERSION_EXACT 1)
    set(PACKAGE_VERSION_COMPATIBLE 1)
    set(PACKAGE_VERSION_UNSUITABLE 0)
  elseif(\"\${PACKAGE_FIND_VERSION}\" STREQUAL \"\") # No explicit version requested.
    set(PACKAGE_VERSION_EXACT 0)
    set(PACKAGE_VERSION_COMPATIBLE 1)
    set(PACKAGE_VERSION_UNSUITABLE 0)
  else()
    set(PACKAGE_VERSION_EXACT 0)
    set(PACKAGE_VERSION_COMPATIBLE 0)
    set(PACKAGE_VERSION_UNSUITABLE 1)
  endif()
endif()
")
  install(FILES ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}ConfigVersion.cmake DESTINATION ${CMAKE_CONFIG_INSTALL_PREFIX})
  set_property(GLOBAL APPEND PROPERTY CONFIG_OBJECTS ${CMAKE_PROJECT_NAME}ConfigVersion.cmake)
endmacro()

#-------------------------------------------------------------------------------
# elements_generate_project_config_file()
#
# Generate the config file used by the other projects using this one.
#-------------------------------------------------------------------------------
macro(elements_generate_project_config_file)
  message(STATUS "Generating ${CMAKE_PROJECT_NAME}Config.cmake")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)
  file(WRITE ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}Config.cmake
"# File automatically generated: DO NOT EDIT.
set(${CMAKE_PROJECT_NAME}_astrotools_version ${astrotools_version})
set(${CMAKE_PROJECT_NAME}_astrotools_system ${SGS_SYSTEM})

set(${CMAKE_PROJECT_NAME}_PLATFORM ${BINARY_TAG})

set(${CMAKE_PROJECT_NAME}_VERSION ${CMAKE_PROJECT_VERSION})
set(${CMAKE_PROJECT_NAME}_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(${CMAKE_PROJECT_NAME}_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
set(${CMAKE_PROJECT_NAME}_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})

set(${CMAKE_PROJECT_NAME}_USES ${PROJECT_USE})

")

  if(NOT SQUEEZED_INSTALL)
    file(APPEND ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}Config.cmake
"#
list(INSERT CMAKE_MODULE_PATH 0 \${${CMAKE_PROJECT_NAME}_DIR}/cmake/modules)
list(INSERT CMAKE_MODULE_PATH 0 \${${CMAKE_PROJECT_NAME}_DIR}/cmake)
")
  endif()

  file(APPEND ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}Config.cmake
"#
include(${CMAKE_PROJECT_NAME}PlatformConfig)
")

  install(FILES ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}Config.cmake DESTINATION ${CMAKE_CONFIG_INSTALL_PREFIX})
  set_property(GLOBAL APPEND PROPERTY CONFIG_OBJECTS ${CMAKE_PROJECT_NAME}Config.cmake)
endmacro()

#-------------------------------------------------------------------------------
# elements_generate_project_platform_config_file()
#
# Generate the platform(build)-specific config file included by the other
# projects using this one.
#-------------------------------------------------------------------------------
macro(elements_generate_project_platform_config_file)
  message(STATUS "Generating ${CMAKE_PROJECT_NAME}PlatformConfig.cmake")

  # collecting infos
  get_property(linker_libraries GLOBAL PROPERTY LINKER_LIBRARIES)
  get_property(component_libraries GLOBAL PROPERTY COMPONENT_LIBRARIES)

  string(REPLACE "\$" "\\\$" project_environment_string "${installed_project_environment}")

  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)
  set(filename ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}PlatformConfig.cmake)
  file(WRITE ${filename}
"# File automatically generated: DO NOT EDIT.

# Get the exported informations about the targets
get_filename_component(_dir " \${CMAKE_CURRENT_LIST_FILE} " PATH)
get_filename_component(_p_dir " \${_dir} " PATH)
")

if(SQUEEZED_INSTALL)
  file(APPEND ${filename} "
get_filename_component(_pp_dir " \${_p_dir} " PATH)
get_filename_component(_ppp_dir " \${_pp_dir} " PATH)
# Set useful properties
set(_pref_dir \${_ppp_dir})
")
else()
  file(APPEND ${filename} "
# Set useful properties
set(_pref_dir \${_p_dir})
")
endif()


file(APPEND ${filename} "
set(${CMAKE_PROJECT_NAME}_INCLUDE_DIRS \${_pref_dir}/${INCLUDE_INSTALL_SUFFIX})
set(${CMAKE_PROJECT_NAME}_LIBRARY_DIRS \${_pref_dir}/${CMAKE_LIB_INSTALL_SUFFIX})
")


if(SQUEEZED_INSTALL)
  file(APPEND ${filename} "
set(${CMAKE_PROJECT_NAME}_BINARY_PATH \${_pref_dir}/bin)
")
else()
  file(APPEND ${filename} "
set(${CMAKE_PROJECT_NAME}_BINARY_PATH \${_pref_dir}/bin \${_pref_dir}/scripts)
")
endif()

file(APPEND ${filename} "
set(${CMAKE_PROJECT_NAME}_CONF_PATH \${_pref_dir}/${CONF_INSTALL_SUFFIX})
set(${CMAKE_PROJECT_NAME}_AUX_PATH \${_pref_dir}/${AUX_INSTALL_SUFFIX})
set(${CMAKE_PROJECT_NAME}_DOC_PATH \${_pref_dir}/${DOC_INSTALL_SUFFIX})


set(${CMAKE_PROJECT_NAME}_PYTHON_PATH \${_pref_dir}/${PYTHON_INSTALL_SUFFIX})
set(${CMAKE_PROJECT_NAME}_COMPONENT_LIBRARIES ${component_libraries})
set(${CMAKE_PROJECT_NAME}_LINKER_LIBRARIES ${linker_libraries})

set(${CMAKE_PROJECT_NAME}_ENVIRONMENT ${project_environment_string})

set(${CMAKE_PROJECT_NAME}_EXPORTED_SUBDIRS)
foreach(p ${packages})
  get_filename_component(pn \${p} NAME)
  if(EXISTS \${_dir}/\${pn}Export.cmake)
    set(${CMAKE_PROJECT_NAME}_EXPORTED_SUBDIRS \${${CMAKE_PROJECT_NAME}_EXPORTED_SUBDIRS} \${p})
  endif()
endforeach()

set(${CMAKE_PROJECT_NAME}_OVERRIDDEN_SUBDIRS ${override_subdirs})
")

  install(FILES ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}PlatformConfig.cmake DESTINATION ${CMAKE_INSTALL_SUFFIX})
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
  set_property(GLOBAL APPEND PROPERTY REGULAR_CMAKE_OBJECTS ${CMAKE_PROJECT_NAME}PlatformConfig.cmake)
endmacro()

#-------------------------------------------------------------------------------
# elements_env(<SET|PREPEND|APPEND|REMOVE|UNSET|INCLUDE> <var> <value> [...repeat...])
#
# Declare environment variables to be modified.
# Note: this is just a wrapper around set_property, the actual logic is in
# elements_project() and elements_generate_env_conf().
#-------------------------------------------------------------------------------
function(elements_env)
  #message(STATUS "elements_env(): ARGN -> ${ARGN}")
  # ensure that the variables in the value are not expanded when passing the arguments
  #string(REPLACE "\$" "\\\$" _argn "${ARGN}")
  #message(STATUS "_argn -> ${_argn}")
  set_property(DIRECTORY APPEND PROPERTY ENVIRONMENT "${ARGN}")
endfunction()

#-------------------------------------------------------------------------------
# elements_build_env(<SET|PREPEND|APPEND|REMOVE|UNSET|INCLUDE> <var> <value> [...repeat...])
#
# Same as elements_env(), but the environment is set only for building.
#-------------------------------------------------------------------------------
function(elements_build_env)
  #message(STATUS "elements_build_env(): ARGN -> ${ARGN}")
  # ensure that the variables in the value are not expanded when passing the arguments
  #string(REPLACE "\$" "\\\$" _argn "${ARGN}")
  #message(STATUS "_argn -> ${_argn}")
  set_property(DIRECTORY APPEND PROPERTY BUILD_ENVIRONMENT "${ARGN}")
endfunction()

#-------------------------------------------------------------------------------
# _env_conf_pop_instruction(...)
#
# helper macro used by elements_generate_env_conf.
#-------------------------------------------------------------------------------
macro(_env_conf_pop_instruction instr lst)
#  message(STATUS "_env_conf_pop_instruction ${lst} => ${${lst}}")
  list(GET ${lst} 0 tmp_instr)
  if(("${tmp_instr}" STREQUAL "INCLUDE") 
      OR ("${tmp_instr}" STREQUAL "UNSET") 
      OR ("${tmp_instr}" STREQUAL "SEARCH_PATH"))
    list(GET ${lst} 0 1 ${instr})
    list(REMOVE_AT ${lst} 0 1)
    # even if the command expects only one argument, ${instr} must have 3 elements
    # because of the way it must be passed to _env_line()
    set(${instr} ${${instr}} _dummy_)
  else()
    list(GET ${lst} 0 1 2 ${instr})
    list(REMOVE_AT ${lst} 0 1 2)
  endif()
endmacro()

#-------------------------------------------------------------------------------
# _env_line(...)
#
# helper macro used by elements_generate_env_conf.
#-------------------------------------------------------------------------------
macro(_env_line cmd var val output)
  set(val_ ${val})
  foreach(root_var ${root_vars})
    if(${root_var})
      if(val_ MATCHES "^${${root_var}}")
        file(RELATIVE_PATH val_ ${${root_var}} ${val_})
        set(val_ \${${root_var}}/${val_})
      endif()
    endif()
  endforeach()

  if("${cmd}" STREQUAL "SET")
    set(${output} "<env:set variable=\"${var}\">${val_}</env:set>")
  elseif("${cmd}" STREQUAL "UNSET")
    set(${output} "<env:unset variable=\"${var}\"></env:unset>")
  elseif("${cmd}" STREQUAL "PREPEND")
    set(${output} "<env:prepend variable=\"${var}\">${val_}</env:prepend>")
  elseif("${cmd}" STREQUAL "APPEND")
    set(${output} "<env:append variable=\"${var}\">${val_}</env:append>")
  elseif("${cmd}" STREQUAL "REMOVE")
    set(${output} "<env:remove variable=\"${var}\">${val_}</env:remove>")
  elseif("${cmd}" STREQUAL "INCLUDE")
    get_filename_component(inc_name ${var} NAME)
    get_filename_component(inc_path ${var} PATH)
    if(${inc_path})
      set(${output} "<env:include hints=\"${inc_path}\">${inc_name}</env:include>")
    else()
      set(${output} "<env:include>${inc_name}</env:include>")
    endif()
  elseif("${cmd}" STREQUAL "SEARCH_PATH")
    set(${output} "<env:search_path>${var}</env:search_path>")
  else()
    message(FATAL_ERROR "Unknown environment command ${cmd}")
  endif()
endmacro()

#-------------------------------------------------------------------------------
# elements_generate_env_conf(filename <env description>)
#
# Generate the XML file describing the changes to the environment required by
# this project.
#-------------------------------------------------------------------------------
function(elements_generate_env_conf filename)

  set(data "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<env:config xmlns:env=\"EnvSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"EnvSchema EnvSchema.xsd \">\n")

  # variables that need to be used to make the environment relative
  set(root_vars SGS_releases SGS_external)
  foreach(root_var ${root_vars})
    set(data "${data}  <env:default variable=\"${root_var}\">${${root_var}}</env:default>\n")
  endforeach()

  # include inherited environments
  # (note: it's important that the full search path is ready before we start including)

  foreach(other_project ${used_elements_projects})
    if(${${other_project}_DIR})
      set(data "${data}  <env:search_path>${${other_project}_DIR}</env:search_path>\n")
    endif()
  endforeach()

  foreach(other_project ${used_elements_projects})
    if(${${other_project}_DIR})
      set(data "${data}  <env:include>${other_project}Environment.xml</env:include>\n")
    endif()
  endforeach()

  foreach(_pck ${data_packages})
    if(${${_pck}_DIR})
      set(data "${data}  <env:search_path>${${_pck}_DIR}</env:search_path>\n")
    endif()
  endforeach()

  foreach(_pck ${data_packages})
    if(${${_pck}_DIR})
      set(data "${data}  <env:include>${_pck}Environment.xml</env:include>\n")
    endif()
  endforeach()

  set(commands ${ARGN})

#  message(STATUS "start - ${commands}")
  while(commands)
#    message(STATUS "iter - ${commands}")
    _env_conf_pop_instruction(instr commands)
    # ensure that the variables in the value are not expanded when passing the arguments
    string(REPLACE "\$" "\\\$" instr "${instr}")
    string(REPLACE "LOCAL_ESCAPE_DOLLAR" "\\\$" instr "${instr}")
    _env_line(${instr} ln)
    set(data "${data}  ${ln}\n")
  endwhile()
  set(data "${data}</env:config>\n")

  get_filename_component(fn ${filename} NAME)
  message(STATUS "Generating ${fn}")  
  
  file(WRITE ${filename} "${data}")
endfunction()

#-------------------------------------------------------------------------------
# _elements_find_standard_libdir(libname var)
#
# Find the location of a standard library asking the compiler.
#-------------------------------------------------------------------------------
function(_elements_find_standard_lib libname var)
  #message(STATUS "find ${libname} -> ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -print-file-name=${libname}")
  set(_cmd "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -print-file-name=${libname}")
  separate_arguments(_cmd)
  execute_process(COMMAND ${_cmd} OUTPUT_VARIABLE cpplib)
  get_filename_component(cpplib ${cpplib} REALPATH)
  get_filename_component(cpplib ${cpplib} PATH)
  #message(STATUS "${libname} lib dir -> ${cpplib}")
  set(${var} ${cpplib} PARENT_SCOPE)
endfunction()

#-------------------------------------------------------------------------------
# elements_external_project_environment()
#
# Collect the environment details from the found packages and add them to the
# variable project_environment.
#-------------------------------------------------------------------------------
macro(elements_external_project_environment)
  message(STATUS "  environment for external packages")
  # collecting environment infos
  set(python_path)
  set(binary_path)
  set(conf_path)
  set(aux_path)
  set(environment)
  set(library_path2)

  # add path to standard libraries to LD_LIBRARY_PATH
  set(library_path2 ${std_library_path})

  get_property(packages_found GLOBAL PROPERTY PACKAGES_FOUND)
  #message("${packages_found}")
  foreach(pack ${packages_found})
    # Check that it is not a "Elements project" (the environment is included in a
    # different way in elements_generate_env_conf).
    list(FIND used_elements_projects ${pack} elements_project_idx)
    if((NOT "${pack}" STREQUAL "ElementsProject") AND (elements_project_idx EQUAL -1) AND (NOT "${pack}" STREQUAL "PythonModules"))
      message(STATUS "    ${pack}")
      # this is needed to get the non-cache variables for the packages
      
      find_package(${pack} QUIET)

      if("${pack}" STREQUAL "PythonInterp" OR "${pack}" STREQUAL "PythonLibs")
        set(pack Python)
      endif()
      string(TOUPPER ${pack} _pack_upper)

      if(${_pack_upper}_EXECUTABLE)
        get_filename_component(bin_path ${${_pack_upper}_EXECUTABLE} PATH)
        list(APPEND binary_path ${bin_path})
      endif()

      list(APPEND binary_path   ${${pack}_BINARY_PATH})
      list(APPEND python_path   ${${pack}_PYTHON_PATH})
      list(APPEND conf_path     ${${pack}_CONF_PATH})
      list(APPEND aux_path      ${${pack}_AUX_PATH})
      list(APPEND doc_path      ${${pack}_DOC_PATH})
      list(APPEND environment   ${${pack}_ENVIRONMENT})
      list(APPEND library_path2 ${${pack}_LIBRARY_DIR} ${${pack}_LIBRARY_DIRS})
      # Try the version with the name of the package uppercase (unless the
      # package name is already uppercase).
      if(NOT "${pack}" STREQUAL "${_pack_upper}")
        list(APPEND binary_path   ${${_pack_upper}_BINARY_PATH})
        list(APPEND python_path   ${${_pack_upper}_PYTHON_PATH})
        list(APPEND conf_path     ${${_pack_upper}_CONF_PATH})
        list(APPEND aux_path      ${${_pack_upper}_AUX_PATH})
        list(APPEND doc_path      ${${_pack_upper}_DOC_PATH})
        list(APPEND environment   ${${_pack_upper}_ENVIRONMENT})
        list(APPEND library_path2 ${${_pack_upper}_LIBRARY_DIR} ${${_pack_upper}_LIBRARY_DIRS})
      endif()
    endif()
  endforeach()

  get_property(library_path GLOBAL PROPERTY LIBRARY_PATH)
  set(library_path ${library_path} ${library_path2})
  # Remove system libraries from the library_path
  #list(REMOVE_ITEM library_path /usr/lib /lib /usr/lib64 /lib64 /usr/lib32 /lib32)
  set(old_library_path ${library_path})
  set(library_path)
  set(lib_match_str "^(/usr|/usr/local)?/lib(32|64)?")
  if(APPLE)
    if($ENV{MACPORT_LOCATION})
      set(lib_match_str "^(/usr|/usr/local|$ENV{MACPORT_LOCATION})?/lib(32|64)?")
    else()
      set(lib_match_str "^(/usr|/usr/local|/opt/local)?/lib(32|64)?")
    endif()
  endif()
  foreach(d ${old_library_path})
    if(NOT d MATCHES ${lib_match_str})
      set(library_path ${library_path} ${d})
    endif()
  endforeach()

  # Remove system paths from the binary_path
  #list(REMOVE_ITEM binary_path /usr/bin /bin)
  set(old_binary_path ${binary_path})
  set(binary_path)
  set(bin_match_str "^(/usr|/usr/local)?/bin")
  if(APPLE)
    if($ENV{MACPORT_LOCATION})
      set(bin_match_str "^(/usr|/usr/local|$ENV{MACPORT_LOCATION})?/bin")
    else()
      set(bin_match_str "^(/usr|/usr/local|/opt/local)?/bin")
    endif()
  endif()
  foreach(d ${old_binary_path})
    if(NOT d MATCHES ${bin_match_str})
      set(binary_path ${binary_path} ${d})
    endif()
  endforeach()


  foreach(var library_path python_path binary_path conf_path aux_path)
    if(${var})
      list(REMOVE_DUPLICATES ${var})
    endif()
  endforeach()

  foreach(val ${python_path})
    set(project_environment ${project_environment} PREPEND PYTHONPATH ${val})
  endforeach()

  foreach(val ${binary_path})
    set(project_environment ${project_environment} PREPEND PATH ${val})
  endforeach()

  foreach(val ${library_path})
    set(project_environment ${project_environment} PREPEND LD_LIBRARY_PATH ${val})
  endforeach()


  foreach(val ${conf_path})
    set(project_environment ${project_environment} PREPEND ELEMENTS_CONF_PATH ${val})
  endforeach()

  foreach(val ${aux_path})
    set(project_environment ${project_environment} PREPEND ELEMENTS_AUX_PATH ${val})
  endforeach()


  set(project_environment ${project_environment} ${environment})

endmacro()

#-------------------------------------------------------------------------------
# elements_export( (LIBRARY | EXECUTABLE | MODULE)
#               <name> )
#
# Internal function used to export targets.
#-------------------------------------------------------------------------------
function(elements_export type name)
  set_property(DIRECTORY APPEND PROPERTY ELEMENTS_EXPORTED_${type} ${name})
endfunction()

#-------------------------------------------------------------------------------
# elements_generate_exports(subdirs...)
#
# Internal function that generate the export data.
#-------------------------------------------------------------------------------
macro(elements_generate_exports)
  foreach(package ${ARGN})
    # we do not use the "Hat" for the export names
    get_filename_component(pkgname ${package} NAME)
    get_property(exported_libs  DIRECTORY ${package} PROPERTY ELEMENTS_EXPORTED_LIBRARY)
    get_property(exported_execs DIRECTORY ${package} PROPERTY ELEMENTS_EXPORTED_EXECUTABLE)
    get_property(exported_mods  DIRECTORY ${package} PROPERTY ELEMENTS_EXPORTED_MODULE)
    get_property(exported_cmake DIRECTORY ${package} PROPERTY ELEMENTS_EXPORTED_CMAKE SET)
    get_property(subdir_version DIRECTORY ${package} PROPERTY version)

    if (exported_libs OR exported_execs OR exported_mods
        OR exported_cmake OR ${package}_DEPENDENCIES OR subdir_version)
      set(pkg_exp_file ${pkgname}Export.cmake)

      message(STATUS "Generating ${pkg_exp_file}")
      set(pkg_exp_file ${CMAKE_CURRENT_BINARY_DIR}/${pkg_exp_file})

      if (NOT SQUEEZED_INSTALL)
        file(WRITE ${pkg_exp_file}
"# File automatically generated: DO NOT EDIT.

# Compute the installation prefix relative to this file.
get_filename_component(_IMPORT_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)
get_filename_component(_IMPORT_PREFIX \"\${_IMPORT_PREFIX}\" PATH)

elements_include_directories(AFTER \${_IMPORT_PREFIX}/include)
link_directories(AFTER \${_IMPORT_PREFIX}/${lib_install_suff})

")
      else()
        file(WRITE ${pkg_exp_file}
"# File automatically generated: DO NOT EDIT.

set(_IMPORT_PREFIX \"${CMAKE_INSTALL_PREFIX}\")

elements_include_directories(AFTER \${_IMPORT_PREFIX}/include)
link_directories(AFTER \${_IMPORT_PREFIX}/${lib_install_suff})

")      
      endif()


      foreach(library ${exported_libs})
        file(APPEND ${pkg_exp_file} "add_library(${library} SHARED IMPORTED)\n")
        file(APPEND ${pkg_exp_file} "set_target_properties(${library} PROPERTIES\n")

        foreach(pn REQUIRED_INCLUDE_DIRS REQUIRED_LIBRARIES)
          get_property(prop TARGET ${library} PROPERTY ${pn})
          if (prop)
            file(APPEND ${pkg_exp_file} "  ${pn} \"${prop}\"\n")
          endif()
        endforeach()

        if(NOT CMAKE_VERSION VERSION_LESS 3.9.0)
#          set(prop $<TARGET_FILE:${library}>)
          get_property(prop TARGET ${library} PROPERTY BASENAME)        
        else()
          get_property(prop TARGET ${library} PROPERTY LOCATION)        
          get_filename_component(prop ${prop} NAME)
        endif()
        file(APPEND ${pkg_exp_file} "  IMPORTED_SONAME \"${prop}\"\n")
        file(APPEND ${pkg_exp_file} "  IMPORTED_LOCATION \"\${_IMPORT_PREFIX}/${CMAKE_LIB_INSTALL_SUFFIX}/${prop}\"\n")

        file(APPEND ${pkg_exp_file} "  )\n")
      endforeach()

      foreach(executable ${exported_execs})

        file(APPEND ${pkg_exp_file} "add_executable(${executable} IMPORTED)\n")
        file(APPEND ${pkg_exp_file} "set_target_properties(${executable} PROPERTIES\n")

        if(NOT CMAKE_VERSION VERSION_LESS 3.9.0)
          get_property(prop TARGET ${executable} PROPERTY BASENAME)        
        else()
          get_property(prop TARGET ${executable} PROPERTY LOCATION)        
          get_filename_component(prop ${prop} NAME)
        endif()
        file(APPEND ${pkg_exp_file} "  IMPORTED_LOCATION \"\${_IMPORT_PREFIX}/bin/${prop}\"\n")

        file(APPEND ${pkg_exp_file} "  )\n")
      endforeach()

      foreach(module ${exported_mods})
        file(APPEND ${pkg_exp_file} "add_library(${module} MODULE IMPORTED)\n")
      endforeach()

      if(${package}_DEPENDENCIES)
        file(APPEND ${pkg_exp_file} "set(${package}_DEPENDENCIES ${${package}_DEPENDENCIES})\n")
      endif()

      if(subdir_version)
        file(APPEND ${pkg_exp_file} "set(${package}_MODULE_VERSION ${subdir_version})\n")
      endif()
    endif()
    install(FILES ${pkg_exp_file} DESTINATION ${CMAKE_INSTALL_SUFFIX})
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
    set_property(GLOBAL APPEND PROPERTY REGULAR_CMAKE_OBJECTS ${pkgname}Export.cmake)
  endforeach()
endmacro()

#-------------------------------------------------------------------------------
# elements_generate_project_manifest()
#
# Internal function to generate project metadata like dependencies on other
# projects and on external software libraries.
#-------------------------------------------------------------------------------
function(elements_generate_project_manifest filename project version)
  # FIXME: partial replication of function argument parsing done in elements_project()
  CMAKE_PARSE_ARGUMENTS(PROJECT "" "DESCRIPTION" "USE;DATA" ${ARGN})
  # Non need to check consistency because it's already done in elements_project().

  #header
  set(data "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<manifest>\n")

  # Project name and version
  set(data "${data}  <project name=\"${project}\" version=\"${version}\" description=\"${PROJECT_DESCRIPTION}\" />\n")

  include(GetGitRevisionDescription)

  get_git_head_revision(ref_value hash_value)
  if(hash_value)
    set(data "${data}  <git hash=\"${hash_value}\"")
    if(ref_value)
      set(data "${data}  ref=\"${ref_value}\"")
    endif()
    git_get_exact_tag(tag_value --tags)
    if(tag_value)
      set(data "${data} tag=\"${tag_value}\"")
    endif()
    set(data "${data} />\n")
  endif()

  # Astro toolchain infos
  if(astrotools_version)
    set(data "${data}  <astrotools>\n")
    # version
    set(data "${data}    <version>${astrotools_version}</version>\n")
    # platform specifications
    set(data "${data}    <binary_tag>${BINARY_TAG}</binary_tag>\n")
    set(data "${data}    <sgs_system>${SGS_SYSTEM}</sgs_system>\n")
    set(data "${data}  </astrotools>\n")
  endif()

  # Build options
  # FIXME: I need an explicit list of options to store

  # Used projects
  if(PROJECT_USE)
    set(data "${data}  <used_projects>\n")
    while(PROJECT_USE)
      list(GET PROJECT_USE 0 n)
      list(GET PROJECT_USE 1 v)
      list(REMOVE_AT PROJECT_USE 0 1)
      set(data "${data}    <project name=\"${n}\" version=\"${v}\" />\n")
    endwhile()
    set(data "${data}  </used_projects>\n")
  endif()

  # Used data packages
  if(PROJECT_DATA)
    set(data "${data}  <used_data_pkgs>\n")
    while(PROJECT_DATA)
      list(GET PROJECT_DATA 0 n)
      list(REMOVE_AT PROJECT_DATA 0)
      set(v *)
      if(PROJECT_DATA)
        list(GET PROJECT_DATA 0 next)
        if("${next}" STREQUAL "VERSION")
          list(GET PROJECT_DATA 1 v)
          list(REMOVE_AT PROJECT_DATA 0 1)
        endif()
      endif()
      set(data "${data}    <package name=\"${n}\" version=\"${v}\" />\n")
    endwhile()
    set(data "${data}  </used_data_pkgs>\n")
  endif()

  # trailer
  set(data "${data}</manifest>\n")

  get_filename_component(fn ${filename} NAME)
  message(STATUS "Generating ${fn}")
  file(WRITE ${filename} "${data}")
endfunction()


function(elements_add_python_program executable module)

  cmake_parse_arguments(PYPROG NO_CONFIG_FILE "" "" ${ARGN})

  get_property(has_python_dir DIRECTORY PROPERTY module_has_python_dir)

  if (NOT has_python_dir)
    message(FATAL_ERROR "There is not python module defined. Please call elements_install_python_modules() first.")
  endif()

  # Make the scripts directory in the build directory if it does not exist
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/scripts)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/scripts)
  endif()
  # Get the name of the file to create
  set(executable_file ${CMAKE_BINARY_DIR}/scripts/${executable})

  string(REPLACE "." "/" program_file ${module})
  set(program_file python/${program_file}.py)

  # for the local bootstrapping the python_path pointing to
  # the sources has to be passed
  get_property(python_pkg_list GLOBAL PROPERTY PROJ_PYTHON_PACKAGE_LIST)

  get_directory_property(elements_module_name name)
  get_directory_property(elements_module_version version)

  set(PYTHON_SCRIPT_VERSION ${PYTHON_VERSION_MAJOR})
  if(PYTHON_EXPLICIT_VERSION)
    set(PYTHON_SCRIPT_VERSION ${PYTHON_EXPLICIT_VERSION})  
  endif()

  if(PYPROG_NO_CONFIG_FILE)
    add_custom_command(OUTPUT ${executable_file}
                       COMMAND ${pythonprogramscript_cmd} --python-explicit-version="${PYTHON_SCRIPT_VERSION}" 
                               --module ${module} --outdir ${CMAKE_BINARY_DIR}/scripts --execname ${executable} 
                               --project-name ${CMAKE_PROJECT_NAME} --elements-module-name ${elements_module_name}
                               --elements-module-version ${elements_module_version} 
                               --elements-default-loglevel=${ELEMENTS_DEFAULT_LOGLEVEL} --no-config-file
                       DEPENDS ${program_file})  
  else()
    add_custom_command(OUTPUT ${executable_file}
                       COMMAND ${pythonprogramscript_cmd} --python-explicit-version="${PYTHON_SCRIPT_VERSION}"
                               --module ${module} --outdir ${CMAKE_BINARY_DIR}/scripts --execname ${executable}
                               --project-name ${CMAKE_PROJECT_NAME} --elements-module-name ${elements_module_name}
                               --elements-module-version ${elements_module_version} 
                               --elements-default-loglevel=${ELEMENTS_DEFAULT_LOGLEVEL}
                       DEPENDS ${program_file})
  endif()

  string(REPLACE "." "_" python_program_target ${module})
  add_custom_target(${python_program_target} ALL DEPENDS ${executable_file})
  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${executable_file})

  install(PROGRAMS ${executable_file} DESTINATION ${SCRIPT_INSTALL_SUFFIX})

  set_property(GLOBAL APPEND PROPERTY REGULAR_SCRIPT_OBJECTS ${executable})
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_SCRIPTS TRUE)
endfunction()
