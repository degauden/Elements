# - ElementsProject
# Define the macros used by Elements-based projects.
#
# Authors: Pere Mato, Marco Clemencic, Hubert Degaudenzi
#
# Commit Id: $Format:%H$

cmake_minimum_required(VERSION 2.8.5)

# FIXME: use of LOCATION property is deprecated and should be replaced with the
#        generator expression $<TARGET_FILE>, but the way we use it requires
#        CMake >= 2.8.12, so we must keep the old behavior until we bump the
#        cmake_minimum_required version. (policy added in CMake 3.0)
if(NOT CMAKE_VERSION VERSION_LESS 3.0) # i.e CMAKE_VERSION >= 3.0
  cmake_policy(SET CMP0026 OLD)
  if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    cmake_policy(SET CMP0042 OLD)
  endif()
endif()

# this policy is related to the symbol visibility
# please run "cmake --help-policy CMP0063" for more details
if(NOT CMAKE_VERSION VERSION_LESS 3.3) # i.e CMAKE_VERSION >= 3.3
  cmake_policy(SET CMP0063 NEW)
else()
  if(CMAKE_VERSION VERSION_GREATER 3.0.2)
    cmake_policy(SET CMP0063 OLD)
  endif()
endif()

if (NOT HAS_ELEMENTS_TOOLCHAIN)
  # this is the call to the preload_local_module_path is the toolchain has not been called
  # Preset the CMAKE_MODULE_PATH from the environment, if not already defined.
  if(NOT CMAKE_MODULE_PATH)
    # Note: this works even if the envirnoment variable is not set.
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
set(CMAKE_VERBOSE_MAKEFILES OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Ensure that the include directories added are always taken first.
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

if (ELEMENTS_BUILD_PREFIX_CMD)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${ELEMENTS_BUILD_PREFIX_CMD}")
  message(STATUS "Prefix build commands with '${ELEMENTS_BUILD_PREFIX_CMD}'")
else()

  find_package(CCache QUIET)

  if(CCACHE_FOUND)
    option(CMAKE_USE_CCACHE "Use ccache to speed up compilation." OFF)
    if(CMAKE_USE_CCACHE)
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_EXECUTABLE})
      message(STATUS "Using ccache for building")
    endif()
  endif()

  find_package(DistCC QUIET)

  if(DISTCC_FOUND)
    option(CMAKE_USE_DISTCC "Use distcc to speed up compilation." OFF)
    if(CMAKE_USE_DISTCC)
      if(CMAKE_USE_CCACHE AND CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "CCACHE_PREFIX=${DISTCC_EXECUTABLE} ${CCACHE_EXECUTABLE}")
        message(STATUS "Enabling distcc builds in ccache")
      else()
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${DISTCC_EXECUTABLE})
        message(STATUS "Using distcc for building")
      endif()
    endif()
  endif()

endif()

# This option make sense only if we have 'objcopy'
if(CMAKE_OBJCOPY)
  option(ELEMENTS_DETACHED_DEBINFO
         "When CMAKE_BUILD_TYPE is RelWithDebInfo, save the debug information on a different file."
         ON)
else()
  set(ELEMENTS_DETACHED_DEBINFO OFF)
endif()

option(USE_ODB "Use the ODB libraries" OFF)
option(ELEMENTS_USE_STRICT_BINARY_DEP "Flag to force the strict binary dependencies" OFF)
option(ELEMENTS_USE_CASE_SENSITIVE_PROJECTS "No uppercase projects allowed" ON)

if(DEFINED ENV{EUCLID_BASE})
  set(EUCLID_BASE_DIR "$ENV{EUCLID_BASE}" CACHE STRING "Euclid Base Install Directory")
  message(STATUS "EUCLID_BASE is in the environment: ${EUCLID_BASE_DIR}")
else()
  set(EUCLID_BASE_DIR "/opt/euclid" CACHE STRING "Euclid Base Install Directory")
  message(STATUS "EUCLID_BASE is not in the environment: using default ${EUCLID_BASE_DIR}")
endif()


#---------------------------------------------------------------------------------------------------
# Programs and utilities needed for the build
#---------------------------------------------------------------------------------------------------
include(CMakeParseArguments)

find_package(PythonInterp QUIET)

#-------------------------------------------------------------------------------
# elements_project(project version
#               [USE proj1 vers1 [proj2 vers2 ...]]
#               [DATA package [VERSION vers] [package [VERSION vers] ...]])
#
# Main macro for a Elements-based project.
# Each project must call this macro once in the top-level CMakeLists.txt,
# stating the project name and the version in the regular format (X.Y[.Z]).
#
# The USE list can be used to declare which Elements-based projects are required by
# the broject being compiled.
#
# The DATA list can be used to declare the data packages requried by the project
# runtime.
#-------------------------------------------------------------------------------
macro(elements_project project version)
  include(SGSPlatform)
  project(${project})
  #----For some reason this is not set by calling 'project()'
  set(CMAKE_PROJECT_NAME ${project})

  #--- Define the version of the project - can be used to generate sources,
  set(CMAKE_PROJECT_VERSION ${version} CACHE STRING "Version of the project")

  #--- Parse the other arguments on the
  CMAKE_PARSE_ARGUMENTS(PROJECT "" "" "USE;DATA;DESCRIPTION" ${ARGN})
  if (PROJECT_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Wrong arguments.")
  endif()

  if(NOT CMAKE_PROJECT_VERSION MATCHES "^HEAD.*")
    string(REGEX MATCH "v?([0-9]+)[r.]([0-9]+)([p.]([0-9]+))?" _version ${CMAKE_PROJECT_VERSION})
    set(CMAKE_PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1} CACHE INTERNAL "Major version of project")
    set(CMAKE_PROJECT_VERSION_MINOR ${CMAKE_MATCH_2} CACHE INTERNAL "Minor version of project")
    set(CMAKE_PROJECT_VERSION_PATCH ${CMAKE_MATCH_4} CACHE INTERNAL "Patch version of project")
  else()
    # 'HEAD' version is special
    set(CMAKE_PROJECT_VERSION_MAJOR 999)
    set(CMAKE_PROJECT_VERSION_MINOR 999)
    set(CMAKE_PROJECT_VERSION_PATCH 0)
  endif()

  #--- Project Options and Global settings----------------------------------------------------------
  option(BUILD_SHARED_LIBS "Set to OFF to build static libraries." ON)
  option(ELEMENTS_BUILD_TESTS "Set to OFF to disable the build of the tests (libraries and executables)." ON)
  option(ELEMENTS_HIDE_WARNINGS "Turn on or off options that are used to hide warning messages." ON)
  option(ELEMENTS_USE_EXE_SUFFIX "Add the .exe suffix to executables on Unix systems (like CMT does)." OFF)
  #-------------------------------------------------------------------------------------------------
  set(ELEMENTS_DATA_SUFFIXES DBASE;PARAM;EXTRAPACKAGES CACHE STRING
      "List of (suffix) directories where to look for data packages.")

  # Install Area business
  if(USE_LOCAL_INSTALLAREA)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
      set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/InstallArea/${BINARY_TAG} CACHE PATH
          "Install path prefix, prepended onto install directories." FORCE )
    endif()
  else()
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
      set(CMAKE_INSTALL_PREFIX ${EUCLID_BASE_DIR}/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${BINARY_TAG} CACHE PATH
          "Install path prefix, prepended onto install directories." FORCE )
    endif()
  endif()

  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE STRING
	   "Single build output directory for all executables" FORCE)
  endif()
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING
	   "Single build output directory for all libraries" FORCE)
  endif()


  set(env_xml ${CMAKE_BINARY_DIR}/${project}BuildEnvironment.xml
      CACHE STRING "path to the XML file for the environment to be used in building and testing")

  set(installed_env_xml \${CMAKE_INSTALL_PREFIX}/${project}BuildEnvironment.xml
      CACHE STRING "path to the XML file for the environment to be used for installation")


  set(env_release_xml ${CMAKE_BINARY_DIR}/${project}Environment.xml
      CACHE STRING "path to the XML file for the environment to be used once the project is installed")

  set(installed_env_release_xml \${CMAKE_INSTALL_PREFIX}/${project}Environment.xml
      CACHE STRING "path to the XML file for the environment to be used once the project is installed")


  mark_as_advanced(CMAKE_RUNTIME_OUTPUT_DIRECTORY CMAKE_LIBRARY_OUTPUT_DIRECTORY
                   env_xml env_release_xml
                   installed_env_xml installed_env_release_xml)

  set(CONF_DIR_NAME "conf" CACHE STRING "Name of the configuration files directory")
  set(AUX_DIR_NAME "auxdir" CACHE STRING "Name of the auxiliary files directory")


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
  _elements_handle_data_packages(${PROJECT_DATA})

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


  find_program(Boost_testmain_cmd createBoostTestMain.py HINTS ${binary_paths})
  if(Boost_testmain_cmd)
    set(Boost_testmain_cmd ${PYTHON_EXECUTABLE} ${Boost_testmain_cmd})
  endif()

  find_program(CppUnit_testmain_cmd createCppUnitTestMain.py HINTS ${binary_paths})
  if(CppUnit_testmain_cmd)
    set(CppUnit_testmain_cmd ${PYTHON_EXECUTABLE} ${CppUnit_testmain_cmd})
  endif()



  find_program(zippythondir_cmd ZipPythonDir.py HINTS ${binary_paths})
  if(zippythondir_cmd)
    set(zippythondir_cmd ${PYTHON_EXECUTABLE} ${zippythondir_cmd})
  endif()

  find_program(elementsrun_cmd elementsrun.py HINTS ${binary_paths})
  set(elementsrun_cmd ${PYTHON_EXECUTABLE} ${elementsrun_cmd})

  find_program(rpmbuild_wrap_cmd rpmbuild_wrap.py HINTS ${binary_paths})
  set(rpmbuild_wrap_cmd ${PYTHON_EXECUTABLE} ${rpmbuild_wrap_cmd})

  find_program(pythonprogramscript_cmd createPythonProgramScript.py HINTS ${binary_paths})
  set(pythonprogramscript_cmd ${PYTHON_EXECUTABLE} ${pythonprogramscript_cmd})

  mark_as_advanced(env_cmd merge_cmd versheader_cmd instheader_cmd versmodule_cmd instmodule_cmd
                   thisheader_cmd thismodule_cmd
                   Boost_testmain_cmd CppUnit_testmain_cmd
                   zippythondir_cmd elementsrun_cmd
                   rpmbuild_wrap_cmd)

  #--- Project Installations------------------------------------------------------------------------
  install(DIRECTORY cmake/ DESTINATION cmake
                           FILES_MATCHING
                             PATTERN "*.cmake"
                             PATTERN "*.in"
                             PATTERN "*.dox"
                             PATTERN "*.py"
                             PATTERN "*.sh"
                             PATTERN "*.csh"
                             PATTERN "*.bat"
                             PATTERN ".svn" EXCLUDE)

  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)


  #--- Global actions for the project
  #message(STATUS "CMAKE_MODULE_PATH -> ${CMAKE_MODULE_PATH}")
  include(ElementsBuildFlags)


  #------------------------------------------------------------------------------------------------
  # RPATH business

  if(ELEMENTS_USE_RPATH)

    if (CMAKE_SYSTEM_NAME MATCHES Linux)
      SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

      # add the automatically determined parts of the RPATH
      # which point to directories outside the build tree to the install RPATH
      SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


      # the RPATH to be used when installing, but only if it's not a system directory
      LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "$ORIGIN/../lib" isSystemDir)
      IF("${isSystemDir}" STREQUAL "-1")
        SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
      ENDIF()
    endif()
    
    if(APPLE)
      set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH
          "Install RPath." FORCE )  
      set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH
          "Install Name Dir." FORCE )  
    endif()
    

  endif()

  #------------------------------------------------------------------------------------------------

  # get the python test framework

  if(PYTHONINTERP_FOUND)
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
  endif()


  # Generate the version header for the project.
  string(TOUPPER ${project} _proj)

  if(versheader_cmd)
    execute_process(COMMAND
                    ${versheader_cmd} --quiet
                    ${project} ${CMAKE_PROJECT_VERSION} ${CMAKE_BINARY_DIR}/include/${_proj}_VERSION.h)
    install(FILES ${CMAKE_BINARY_DIR}/include/${_proj}_VERSION.h DESTINATION include)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
  endif()

  if(instheader_cmd)
    JOIN("${used_elements_projects}" ":" joined_used_projects)
    execute_process(COMMAND
                    ${instheader_cmd} --quiet
                    ${project} ${CMAKE_INSTALL_PREFIX} ${joined_used_projects} ${CMAKE_BINARY_DIR}/include/${_proj}_INSTALL.h)
    # special installation because the install location can be changed on the fly
    install(CODE "message\(STATUS \"Installing: ${_proj}_INSTALL.h in \${CMAKE_INSTALL_PREFIX}/include\"\)
execute_process\(COMMAND ${instheader_cmd} --quiet ${project} \${CMAKE_INSTALL_PREFIX} ${joined_used_projects} \${CMAKE_INSTALL_PREFIX}/include/${_proj}_INSTALL.h\)")
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
  endif()


  if(thisheader_cmd)
    execute_process(COMMAND
                    ${thisheader_cmd} --quiet
                    ${project} ${CMAKE_BINARY_DIR}/include/ThisProject.h)
    # This header is by design only local. It is then not installed
  endif()

  # Add generated headers to the include path.
  include_directories(${CMAKE_BINARY_DIR}/include)

  if(versmodule_cmd)
    execute_process(COMMAND
                    ${versmodule_cmd} --quiet
                    ${project} ${CMAKE_PROJECT_VERSION} ${CMAKE_BINARY_DIR}/python/${_proj}_VERSION.py)
    install(FILES ${CMAKE_BINARY_DIR}/python/${_proj}_VERSION.py DESTINATION python)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
  endif()

  if(instmodule_cmd)
    JOIN("${used_elements_projects}" ":" joined_used_projects)
    execute_process(COMMAND
                    ${instmodule_cmd} --quiet
                    ${project} ${CMAKE_INSTALL_PREFIX} ${joined_used_projects} ${CMAKE_BINARY_DIR}/python/${_proj}_INSTALL.py)
    # install(FILES ${CMAKE_BINARY_DIR}/python/${_proj}_INSTALL.py DESTINATION python)
    # special install procedure because the install loction can be changed on the fly.
    install(CODE "message\(STATUS \"Installing: ${_proj}_INSTALL.py in \${CMAKE_INSTALL_PREFIX}/python\"\)
execute_process\(COMMAND ${instmodule_cmd} --quiet ${project} \${CMAKE_INSTALL_PREFIX} ${joined_used_projects} \${CMAKE_INSTALL_PREFIX}/python/${_proj}_INSTALL.py\)")
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
  endif()


  if(thismodule_cmd)
    execute_process(COMMAND
                    ${thismodule_cmd} --quiet
                    ${project} ${CMAKE_BINARY_DIR}/python/ThisProject.py)
    install(FILES ${CMAKE_BINARY_DIR}/python/ThisProject.py DESTINATION python)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
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

  # FIXME: it is not possible to produce the file python.zip at installation time
  # because the install scripts of the subdirectories are executed after those
  # of the parent project and we cannot have a post-install target because of
  # http://public.kitware.com/Bug/view.php?id=8438
  # install(CODE "execute_process(COMMAND  ${zippythondir_cmd} ${CMAKE_INSTALL_PREFIX}/python)")
  if(zippythondir_cmd)
    add_custom_target(python.zip
                      COMMAND ${zippythondir_cmd} ${CMAKE_INSTALL_PREFIX}/python
                      COMMENT "Zipping Python modules")
  endif()
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

  foreach(other_project ${used_elements_projects})
    set(project_environment ${project_environment}
        INCLUDE ${other_project}Environment.xml)
  endforeach()


  #   - installation dirs
  set(project_environment ${project_environment}
        PREPEND PATH LOCAL_ESCAPE_DOLLAR{.}/scripts
        PREPEND PATH LOCAL_ESCAPE_DOLLAR{.}/bin
        PREPEND LD_LIBRARY_PATH LOCAL_ESCAPE_DOLLAR{.}/lib
        PREPEND PYTHONPATH LOCAL_ESCAPE_DOLLAR{.}/python
        PREPEND PYTHONPATH LOCAL_ESCAPE_DOLLAR{.}/python/lib-dynload
        PREPEND ELEMENTS_CONF_PATH LOCAL_ESCAPE_DOLLAR{.}/${CONF_DIR_NAME}
        PREPEND ELEMENTS_AUX_PATH LOCAL_ESCAPE_DOLLAR{.}/${AUX_DIR_NAME})

  foreach(other_project ${used_elements_projects})
    set(project_build_environment ${project_build_environment}
        SEARCH_PATH ${${other_project}_DIR})
  endforeach()

  foreach(other_project ${used_elements_projects})
    set(project_build_environment ${project_build_environment}
        INCLUDE ${other_project}BuildEnvironment.xml)
  endforeach()


  #     (installation dirs added to build env to be able to test pre-built bins)
  set(project_build_environment ${project_build_environment}
        PREPEND PATH ${CMAKE_INSTALL_PREFIX}/scripts
        PREPEND PATH ${CMAKE_INSTALL_PREFIX}/bin
        PREPEND LD_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib
        PREPEND PYTHONPATH ${CMAKE_INSTALL_PREFIX}/python
        PREPEND PYTHONPATH ${CMAKE_INSTALL_PREFIX}/python/lib-dynload
        PREPEND ELEMENTS_CONF_PATH ${CMAKE_INSTALL_PREFIX}/${CONF_DIR_NAME}
        PREPEND ELEMENTS_AUX_PATH ${CMAKE_INSTALL_PREFIX}/${AUX_DIR_NAME})

  message(STATUS "  environment for local subdirectories")
  #   - project root (for relocatability)
  string(TOUPPER ${project} _proj)
  #set(project_environment ${project_environment} SET ${_proj}_PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
  file(RELATIVE_PATH _PROJECT_ROOT ${CMAKE_INSTALL_PREFIX} ${CMAKE_SOURCE_DIR})
  #message(STATUS "_PROJECT_ROOT -> ${_PROJECT_ROOT}")
  set(project_environment ${project_environment} SET ${_proj}_PROJECT_ROOT "LOCAL_ESCAPE_DOLLAR{.}/../..")
  set(installed_project_environment "${project_environment}")
  set(installed_project_build_environment "${project_build_environment}")
  set(project_build_environment ${project_build_environment} SET ${_proj}_PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
  set(installed_project_build_environment ${installed_project_build_environment} SET ${_proj}_PROJECT_ROOT "${CMAKE_INSTALL_PREFIX}/../..")
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
      PREPEND ELEMENTS_AUX_PATH ${CMAKE_BINARY_DIR}/${AUX_DIR_NAME})

  # - produce environment XML description
  #   release version
  elements_generate_env_conf(${env_release_xml} ${project_environment})
  install(CODE "find_package\(ElementsProject\)
message\(STATUS \"Installing: ${installed_env_release_xml}\"\)
set\(used_elements_projects ${used_elements_projects}\)
foreach\(other_project ${used_elements_projects}\)
set\(\${other_project}_DIR \${\${other_project}_DIR}\)
endforeach\(\)
elements_generate_env_conf\(${installed_env_release_xml} ${installed_project_environment}\)")
#  install(FILES ${env_release_xml} DESTINATION ${CMAKE_INSTALL_PREFIX})
  #   build-time version
  elements_generate_env_conf(${env_xml} ${project_build_environment})
  install(CODE "find_package\(ElementsProject\)
message\(STATUS \"Installing: ${installed_env_xml}\"\)
elements_generate_env_conf\(${installed_env_xml} ${installed_project_build_environment}\)")
#  install(FILES ${env_xml} DESTINATION ${CMAKE_INSTALL_PREFIX})
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



  #--- Generate config files to be imported by other projects.
  elements_generate_project_config_version_file()
  elements_generate_project_config_file()
  elements_generate_project_platform_config_file()
  elements_generate_exports(${packages})

  #--- Generate the manifest.xml file.
  elements_generate_project_manifest(${CMAKE_BINARY_DIR}/manifest.xml ${ARGV})
  install(FILES ${CMAKE_BINARY_DIR}/manifest.xml DESTINATION .)

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
  # set(CPACK_PACKAGE_INSTALL_DIRECTORY /opt/euclid)
  # set(CPACK_RPM_PACKAGE_PREFIX /opt/euclid)
  set(CPACK_PACKAGING_INSTALL_PREFIX ${EUCLID_BASE_DIR}/${CPACK_PACKAGE_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${BINARY_TAG})
  set(CPACK_GENERATOR RPM)
  set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})

  set(CPACK_PACKAGE_RELEASE 1 CACHE STRING "Release Number For the Packaging")
  set(CPACK_PACKAGE_VENDOR "The Euclid Consortium")

  set(CPACK_SOURCE_IGNORE_FILES "/InstallArea/;/${BUILD_PREFIX_NAME}\\\\..*/;/${BUILD_SUBDIR}/;/\\\\.svn/;/\\\\.settings/;\\\\..*project;\\\\.gitignore")

  # RPM packaging specific stuff
  set(CPACK_RPM_PACKAGE_RELOCATABLE TRUE)

  SET(CPACK_RPM_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
  SET(CPACK_RPM_PACKAGE_ARCHITECTURE ${SGS_ARCH})
  SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE}")
  SET(CPACK_RPM_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")
  SET(CPACK_RPM_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})


  set(CPACK_RPM_REGULAR_FILES "%files")
  set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%defattr(-,root,root,-)
%dir %{_prefix}")
  set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{_prefix}/${CPACK_PACKAGE_NAME}Environment.xml")
  set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{_prefix}/manifest.xml")

#------------------------------------------------------------------------------
  get_property(regular_bin_objects GLOBAL PROPERTY REGULAR_BIN_OBJECTS)

  if(regular_bin_objects)
    set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%dir %{_bindir}")
    list(SORT regular_bin_objects)
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


#------------------------------------------------------------------------------
  get_property(regular_lib_objects GLOBAL PROPERTY REGULAR_LIB_OBJECTS)

  if(regular_lib_objects)
    set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%dir %{libdir}")
    list(SORT regular_lib_objects)
    foreach(_do ${regular_lib_objects})
      set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{libdir}/${CMAKE_SHARED_LIBRARY_PREFIX}${_do}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    endforeach()
  endif()


#------------------------------------------------------------------------------
  get_property(proj_has_scripts GLOBAL PROPERTY PROJ_HAS_SCRIPTS)

  if(proj_has_scripts)
        set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{scriptsdir}")
    #message(STATUS "The regular objects: ${CPACK_RPM_REGULAR_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_aux GLOBAL PROPERTY PROJ_HAS_AUX)

  if(proj_has_aux)
        set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{auxdir}")
    #message(STATUS "The regular objects: ${CPACK_RPM_REGULAR_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_conf GLOBAL PROPERTY PROJ_HAS_CONF)

  if(proj_has_conf)
        set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{confdir}")
    #message(STATUS "The regular objects: ${CPACK_RPM_REGULAR_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_python GLOBAL PROPERTY PROJ_HAS_PYTHON)

  if(proj_has_python)
        set(CPACK_RPM_REGULAR_FILES "${CPACK_RPM_REGULAR_FILES}
%{pydir}")
    #message(STATUS "The regular objects: ${CPACK_RPM_REGULAR_FILES}")
  endif()

#===============================================================================

  set(CPACK_RPM_DEVEL_FILES "%files devel")
  set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%defattr(-,root,root,-)")
  set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{_prefix}/${CPACK_PACKAGE_NAME}BuildEnvironment.xml")
#------------------------------------------------------------------------------
  get_property(config_objects GLOBAL PROPERTY CONFIG_OBJECTS)

  if(config_objects)
    list(SORT config_objects)
    foreach(_do ${config_objects})
      set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{_prefix}/${_do}")
    endforeach()
    #message(STATUS "The devel objects: ${CPACK_RPM_DEVEL_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_include GLOBAL PROPERTY PROJ_HAS_INCLUDE)

  if(proj_has_include)
        set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{_includedir}")
    #message(STATUS "The devel objects: ${CPACK_RPM_DEVEL_FILES}")
  endif()

#------------------------------------------------------------------------------
  get_property(proj_has_cmake GLOBAL PROPERTY PROJ_HAS_CMAKE)

  if(proj_has_cmake)
        set(CPACK_RPM_DEVEL_FILES "${CPACK_RPM_DEVEL_FILES}
%{cmakedir}")
    #message(STATUS "The devel objects: ${CPACK_RPM_DEVEL_FILES}")
  endif()



#===============================================================================

#------------------------------------------------------------------------------
  get_property(debinfo_objects GLOBAL PROPERTY DEBINFO_OBJECTS)

  if(debinfo_objects)
    set(CPACK_RPM_DEBINFO_FILES "%files debuginfo")
    set(CPACK_RPM_DEBINFO_FILES "${CPACK_RPM_DEBINFO_FILES}
%defattr(-,root,root,-)")

    list(SORT debinfo_objects)
    foreach(_do ${debinfo_objects})
      if("${_do}" MATCHES "^lib")
        set(CPACK_RPM_DEBINFO_FILES "${CPACK_RPM_DEBINFO_FILES}
%{libdir}/${_do}")
      else()
        set(CPACK_RPM_DEBINFO_FILES "${CPACK_RPM_DEBINFO_FILES}
%{_bindir}/${_do}")
      endif()
    endforeach()
    #message(STATUS "The debuginfo objects: ${CPACK_RPM_DEBINFO_FILES}")
  endif()

#===============================================================================



  include(CPack)

  find_package(Tar QUIET)
  if(TAR_FOUND)

    find_package(RPMBuild QUIET)

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
                      COMMAND ${TAR_EXECUTABLE} zcf ${PROJECT_TARGZ_DIR}/${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz --exclude "${BUILD_PREFIX_NAME}.*" --exclude "./${BUILD_SUBDIR}" --exclude "./.*" --exclude "./InstallArea" --transform "s/./${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}/"  .
                      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      COMMENT "Generating The Source TarBall ${PROJECT_TARGZ_DIR}/${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz" VERBATIM
    )

    if (RPMBUILD_FOUND)


      get_rpm_dep_list("${PROJECT_USE}" "debuginfo" RPM_DEBUGINFO_DEP_LIST)

      get_rpm_dep_list("${PROJECT_USE}" "devel" RPM_DEVEL_DEP_LIST)

      get_rpm_dep_list("${PROJECT_USE}" "" RPM_DEP_LIST)

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



     find_file_to_configure(Elements.spec.in
                            FILETYPE "RPM SPEC"
                            OUTPUTDIR "${PROJECT_RPM_TOPDIR}/SPECS"
                            OUTPUTNAME "${project}.spec"
                            PATHS ${CMAKE_MODULE_PATH})


      add_custom_target(rpmbuilddir
                        COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_RPM_TOPDIR}/BUILD
                        COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_RPM_TOPDIR}/BUILDROOT
                        COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_RPM_TOPDIR}/RPMS
                        COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_RPM_TOPDIR}/SRPMS
                        COMMENT "Generating ${PROJECT_RPM_TOPDIR} as rpmbuild directory" VERBATIM
      )


      add_custom_target(rpm
                        COMMAND ${rpmbuild_wrap_cmd} ${PROJECT_RPM_TOPDIR}/SPECS/${project}.spec
                        COMMENT "Generating The RPM Files in ${PROJECT_RPM_TOPDIR}" VERBATIM
      )

      add_dependencies(rpm targz)
      add_dependencies(rpm rpmbuilddir)

    endif()


  endif()

  include(ElementsDocumentation)

endmacro()

#-------------------------------------------------------------------------------
# _elements_use_other_projects([project version [project version]...])
#
# Internal macro implementing the handline of the "USE" option.
# (improve readability)
#-------------------------------------------------------------------------------
macro(_elements_use_other_projects)
  # Note: it works even if the env. var. is not set.
  file(TO_CMAKE_PATH "$ENV{CMAKE_PROJECT_PATH}" projects_search_path)

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
      string(REGEX MATCH "v?([0-9]+)[r.]([0-9]+)([p.]([0-9]+))?" _version ${other_project_version})

      set(other_project_cmake_version ${CMAKE_MATCH_1}.${CMAKE_MATCH_2})
      if(DEFINED CMAKE_MATCH_4)
        if(NOT CMAKE_MATCH_4 STREQUAL "")
          set(other_project_cmake_version ${other_project_cmake_version}.${CMAKE_MATCH_4})
        endif()
      endif()
    else()
      # "HEAD" is a special version id (mapped to v999r999).
      set(other_project_cmake_version 999.999)
    endif()

    # Manage the lists which contains the dependencies and the project which
    # introduced them
    if(${other_project}_FOUND)
      # If the dependency is already handled check that the version numbers
      # much, otherwise raise an error
      string(COMPARE NOTEQUAL "${${other_project}_VERSION}" "${other_project_cmake_version}" ver_mismatch)
      if(ver_mismatch)
        list(FIND dependency_list "${other_project}" dep_index)
        list(GET dependency_dependee_list ${dep_index} dep_name)
        math(EXPR dep_index "${dep_index}+1")
        list(GET dependency_dependee_list ${dep_index} dep_version)
        set(ver_mis_message "Dependency version mismatch:")
        set(ver_mis_message "${ver_mis_message} ${other_dependee} ${other_dependee_version}")
        set(ver_mis_message "${ver_mis_message} -> ${other_project} ${other_project_cmake_version}")
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
      foreach(pth ${projects_search_path})
        find_package(${other_project} ${other_project_cmake_version} QUIET
                     HINTS ${pth}
                     PATH_SUFFIXES ${suffixes})
        if(${other_project}_FOUND)
          break()
        else()
          set(suffixes2)
          get_installed_versionless_project_suffixes(${other_project} ${BINARY_TAG} ${SGS_SYSTEM} suffixes2)
          find_package(${other_project} ${other_project_cmake_version} QUIET
                       HINTS ${pth}
                       PATH_SUFFIXES ${suffixes2})
          if(${other_project}_FOUND)
            break()
          endif()
        endif()
      endforeach()
      if(${other_project}_FOUND)
        message(STATUS "  found ${other_project} ${${other_project}_VERSION} ${${other_project}_DIR}")
        if(NOT SGS_SYSTEM STREQUAL ${other_project}_astrotools_system)
          message(FATAL_ERROR "Incompatible values of SGS_SYSTEM:
  ${CMAKE_PROJECT_NAME} -> ${SGS_SYSTEM}
  ${other_project} ${${other_project}_VERSION} -> ${${other_project}_astrotools_system}

  Check your configuration.
")
        endif()
        # include directories of other projects must be appended to the current
        # list to preserve the order of overriding
        include_directories(AFTER ${${other_project}_INCLUDE_DIRS})
        # but in the INCLUDE_PATHS property the order gets reversed afterwards
        # so we need to prepend instead of append
        get_property(_inc_dirs GLOBAL PROPERTY INCLUDE_PATHS)
        set_property(GLOBAL PROPERTY INCLUDE_PATHS ${${other_project}_INCLUDE_DIRS} ${_inc_dirs})
        set(binary_paths ${${other_project}_BINARY_PATH} ${binary_paths})
        foreach(exported ${${other_project}_EXPORTED_SUBDIRS})
          list(FIND known_packages ${exported} is_needed)
          if(is_needed LESS 0)
            list(APPEND known_packages ${exported})
            get_filename_component(expname ${exported} NAME)
            include(${expname}Export)
            message(STATUS "    imported ${exported} ${${exported}_VERSION}")
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
# the suffixes willbe appended to eache searched directory to look for the
# data packages.
#
# The root of the data package will be stored in <variable>.
#-------------------------------------------------------------------------------
function(elements_find_data_package name)
  #message(STATUS "elements_find_data_package(${ARGV})")
  if(NOT ${name}_FOUND)
    # Note: it works even if the env. var. is not set.
    file(TO_CMAKE_PATH "$ENV{CMAKE_PROJECT_PATH}" projects_search_path)
    file(TO_CMAKE_PATH "$ENV{CMAKE_PREFIX_PATH}" env_prefix_path)

    set(version *) # default version value
    if(ARGN AND NOT ARGV1 STREQUAL PATH_SUFFIXES)
      set(version ${ARGV1})
      list(REMOVE_AT ARGN 0)
    endif()

    if(ARGN)
      list(GET ARGN 0 arg)
      if(arg STREQUAL PATH_SUFFIXES)
        list(REMOVE_AT ARGN 0)
      endif()
    endif()
    # At this point, ARGN contains only the suffixes, if any.

    string(REPLACE / _ envname ${name}Environment.xml)

    set(candidate_version)
    set(candidate_path)
    foreach(prefix ${projects_search_path} ${CMAKE_PREFIX_PATH} ${env_prefix_path})
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
            if(high AND NOT (high STREQUAL candidate_version))
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
      message(FATAL_ERROR "Cannot find ${name} ${version}")
    endif()
  endif()
endfunction()

#-------------------------------------------------------------------------------
# _elements_handle_data_pacakges([package [VERSION version] [project version [VERSION version]]...])
#
# Internal macro implementing the handline of the "USE" option.
# (improve readability)
#-------------------------------------------------------------------------------
macro(_elements_handle_data_packages)
  # this is neede because of the way variable expansion works in macros
  set(ARGN_ ${ARGN})
  if(ARGN_)
    message(STATUS "Looking for data packages")
  endif()
  while(ARGN_)
    # extract data package name and (optional) version from the list
    list(GET ARGN_ 0 _data_package)
    list(REMOVE_AT ARGN_ 0)
    if(ARGN_) # we can look for the version only if we still have data)
      list(GET ARGN_ 0 _data_pkg_vers)
      if(_data_pkg_vers STREQUAL VERSION)
        list(GET ARGN_ 1 _data_pkg_vers)
        list(REMOVE_AT ARGN_ 0 1)
      else()
        set(_data_pkg_vers *) # default version value
      endif()
    else()
      set(_data_pkg_vers *) # default version value
    endif()
    if(NOT ${_data_package}_FOUND)
      elements_find_data_package(${_data_package} ${_data_pkg_vers} PATH_SUFFIXES ${ELEMENTS_DATA_SUFFIXES})
    else()
      message(STATUS "Using ${_data_package}: ${${_data_package}_DIR}")
    endif()
    if(${_data_package}_FOUND)
      string(REPLACE / _ _data_pkg_env ${_data_package}Environment.xml)
      set(project_environment ${project_environment} INCLUDE ${${_data_package}_DIR}/${_data_pkg_env})
    endif()
  endwhile()
endmacro()

#-------------------------------------------------------------------------------
# include_package_directories(Package1 [Package2 ...])
#
# Add the include directories of each package to the include directories.
#-------------------------------------------------------------------------------
function(include_package_directories)
  #message(STATUS "include_package_directories(${ARGN})")
  foreach(package ${ARGN})
    # we need to ensure that the user can call this function also for directories
    if(TARGET ${package})
      get_target_property(to_incl ${package} SOURCE_DIR)
      if(to_incl)
        #message(STATUS "include_package_directories1 include_directories(${to_incl})")
        include_directories(${to_incl})
      endif()
    elseif(IS_ABSOLUTE ${package} AND IS_DIRECTORY ${package})
      #message(STATUS "include_package_directories2 include_directories(${package})")
      include_directories(${package})
    elseif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${package})
      #message(STATUS "include_package_directories3 include_directories(${package})")
      include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${package})
    elseif(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${package}) # package can be the name of a subdir
      #message(STATUS "include_package_directories4 include_directories(${package})")
      include_directories(${CMAKE_SOURCE_DIR}/${package})
    else()
      # ensure that the current directory knows about the package
      find_package(${package} QUIET)
      set(to_incl)
      string(TOUPPER ${package} _pack_upper)
      if(${_pack_upper}_FOUND OR ${package}_FOUND)
        # Handle some special cases first, then try for package uppercase (DIRS and DIR)
        # If the package is found, add INCLUDE_DIRS or (if not defined) INCLUDE_DIR.
        # If none of the two is defined, do not add anything.
        if(${package} STREQUAL PythonLibs)
          set(to_incl PYTHON_INCLUDE_DIRS)
        elseif(${_pack_upper}_INCLUDE_DIRS)
          set(to_incl ${_pack_upper}_INCLUDE_DIRS)
        elseif(${_pack_upper}_INCLUDE_DIR)
          set(to_incl ${_pack_upper}_INCLUDE_DIR)
        elseif(${package}_INCLUDE_DIRS)
          set(to_incl ${package}_INCLUDE_DIRS)
        endif()
        # Include the directories
        #message(STATUS "include_package_directories5 include_directories(${${to_incl}})")
        include_directories(${${to_incl}})
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
# The fuction performs those operations that are not needed if there is no
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
          include_directories(${CMAKE_SOURCE_DIR}/${subdir})
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
    if(NOT file STREQUAL CMakeLists.txt AND NOT (file MATCHES "^${rel_build_dir}" OR file MATCHES "^${BUILD_PREFIX_NAME}\\."))
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
  if (NOT _guessed_name STREQUAL "${name}")
    message(WARNING "Declared subdir name (${name}) does not match the name of the directory (${_guessed_name})")
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

  # Generate the version header for the package.
  execute_process(COMMAND
                  ${versheader_cmd} --quiet
                  ${name} ${version} ${CMAKE_CURRENT_BINARY_DIR}/${name}Version.h)
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
    if(entry STREQUAL debug OR entry STREQUAL optimized OR entry STREQUAL general)
      # it's a qualifier: pop another one (the library name)
      list(GET collected 0 lib)
      list(REMOVE_AT collected 0)
      # The possible values of CMAKE_BUILD_TYPE are Debug, Release,
      # RelWithDebInfo and MinSizeRel, plus the SGS/Elements special ones
      # Coverage and Profile. (treat an empty CMAKE_BUILD_TYPE as Release)
      if((entry STREQUAL general) OR
         (CMAKE_BUILD_TYPE MATCHES "Debug|Coverage" AND entry STREQUAL debug) OR
         ((NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "Rel|Profile") AND entry STREQUAL optimized))
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
        if(${package} STREQUAL PythonLibs)
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
  if(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo AND ELEMENTS_DETACHED_DEBINFO)
    # get the type of the target (MODULE_LIBRARY, SHARED_LIBRARY, EXECUTABLE)
    get_property(_type TARGET ${target} PROPERTY TYPE)
    #message(STATUS "_elements_detach_debinfo(${target}): target type -> ${_type}")
    if(NOT _type STREQUAL STATIC_LIBRARY) # we ignore static libraries
      # guess the target file name
      if(_type MATCHES "MODULE|LIBRARY")
        #message(STATUS "_elements_detach_debinfo(${target}): library sub-type -> ${CMAKE_MATCH_0}")
        # TODO: the library name may be different from the default.
        #       see OUTPUT_NAME and LIBRARY_OUPUT_NAME
        set(_tn ${CMAKE_SHARED_${CMAKE_MATCH_0}_PREFIX}${target}${CMAKE_SHARED_${CMAKE_MATCH_0}_SUFFIX})
        set(_builddir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
        set(_dest lib)
      else()
        set(_tn ${target})
        if(ELEMENTS_USE_EXE_SUFFIX)
          set(_tn ${_tn}.exe)
        endif()
        set(_builddir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        set(_dest bin)
      endif()
    endif()
    #message(STATUS "_elements_detach_debinfo(${target}): target name -> ${_tn}")
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
    set_property(GLOBAL APPEND PROPERTY DEBINFO_OBJECTS ${_tn}.dbg)
  endif()
endmacro()

#---------------------------------------------------------------------------------------------------
# elements_add_library(<name>
#                   source1 source2 ...
#                   LINK_LIBRARIES library1 library2 ...
#                   INCLUDE_DIRS dir1 package2 ...
#                   [LINKER_LANGUAGE C|CXX]
#                   [NO_PUBLIC_HEADERS | PUBLIC_HEADERS dir1 dir2 ...])
#
# Extension of standard CMake 'add_library' command.
# Create a library from the specified sources (glob patterns are allowed), linking
# it with the libraries specified and adding the include directories to the search path.
#---------------------------------------------------------------------------------------------------
function(elements_add_library library)
  # this function uses an extra option: 'PUBLIC_HEADERS'
  CMAKE_PARSE_ARGUMENTS(ARG "NO_PUBLIC_HEADERS" "LINKER_LANGUAGE" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS;PUBLIC_HEADERS" ${ARGN})

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

  if(ARG_LINKER_LANGUAGE)
    set_target_properties(${library} PROPERTIES LINKER_LANGUAGE ${ARG_LINKER_LANGUAGE})
  endif()

  set_target_properties(${library} PROPERTIES COMPILE_DEFINITIONS ELEMENTS_LINKER_LIBRARY)
  target_link_libraries(${library} ${ARG_LINK_LIBRARIES})
  _elements_detach_debinfo(${library})

  # Declare that the used headers are needed by the libraries linked against this one
  set_target_properties(${library} PROPERTIES
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    REQUIRED_INCLUDE_DIRS "${ARG_INCLUDE_DIRS}"
    REQUIRED_LIBRARIES "${ARG_LINK_LIBRARIES}")
  set_property(GLOBAL APPEND PROPERTY LINKER_LIBRARIES ${library})

  #----Installation details-------------------------------------------------------
  install(TARGETS ${library} EXPORT ${CMAKE_PROJECT_NAME}Exports DESTINATION lib OPTIONAL)
  elements_export(LIBRARY ${library})
  elements_install_headers(${ARG_PUBLIC_HEADERS})
  install(EXPORT ${CMAKE_PROJECT_NAME}Exports DESTINATION cmake OPTIONAL)
  set_property(GLOBAL APPEND PROPERTY REGULAR_LIB_OBJECTS ${library})
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
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
  target_link_libraries(${library} ${ROOT_Reflex_LIBRARY} ${ARG_LINK_LIBRARIES})
  _elements_detach_debinfo(${library})

  set_property(GLOBAL APPEND PROPERTY COMPONENT_LIBRARIES ${library})

  #----Installation details-------------------------------------------------------
  install(TARGETS ${library} LIBRARY DESTINATION lib OPTIONAL)
  elements_export(MODULE ${library})
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
  set_target_properties(${dictionary}Dict PROPERTIES COMPILE_FLAGS "-Wno-overloaded-virtual")
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
    add_custom_target(${dictionary}PCM ALL
                      DEPENDS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${pcmname})
  endif()

  #----Installation details-------------------------------------------------------
  install(TARGETS ${dictionary}Dict LIBRARY DESTINATION lib OPTIONAL)
  if(ROOT_HAS_PCMS)
    install(FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${pcmname} DESTINATION lib OPTIONAL)
  endif()
endfunction()

#---------------------------------------------------------------------------------------------------
# elements_add_python_module(name
#                         sources ...
#                         LINK_LIBRARIES ...
#                         INCLUDE_DIRS ...)
#
# Build a binary python module from the given sources.
#---------------------------------------------------------------------------------------------------
function(elements_add_python_module module)
  elements_common_add_build(${ARGN})

  # require Python libraries
  find_package(PythonLibs QUIET REQUIRED)

  include_directories(${PYTHON_INCLUDE_DIRS})
  add_library(${module} MODULE ${srcs})
  set_target_properties(${module} PROPERTIES SUFFIX .so PREFIX "_")
  target_link_libraries(${module} ${PYTHON_LIBRARIES} ${ARG_LINK_LIBRARIES})
#  _elements_detach_debinfo(${module})

  #----Installation details-------------------------------------------------------
  install(TARGETS ${module} LIBRARY DESTINATION python/lib-dynload OPTIONAL)
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
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

  find_package(SWIG QUIET REQUIRED)
  find_package(PythonLibs QUIET REQUIRED)

  # this function uses an extra option: 'PUBLIC_HEADERS'
  CMAKE_PARSE_ARGUMENTS(ARG "NO_PUBLIC_HEADERS" "" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS;PUBLIC_HEADERS" ${ARGN})

  set(MODULE_ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES})
  set(MODULE_ARG_INCLUDE_DIRS ${ARG_INCLUDE_DIRS})


  elements_common_add_build(${ARG_UNPARSED_ARGUMENTS}
                            LIBRARIES ${ARG_LIBRARIES}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS})

  get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  list(REMOVE_DUPLICATES dirs)
  set(SWIG_MOD_INCLUDE_DIRS)
  foreach(dir ${dirs})
    set(SWIG_MOD_INCLUDE_DIRS ${SWIG_MOD_INCLUDE_DIRS} -I${dir})
  endforeach()

  if(NOT ARG_NO_PUBLIC_HEADERS AND NOT ARG_PUBLIC_HEADERS)
    elements_get_package_name(package)
    message(WARNING "Binding ${binding} (in ${package}) does not declare PUBLIC_HEADERS. Use the option NO_PUBLIC_HEADERS if it is intended.")
  endif()

  # find the sources
  elements_expand_sources(srcs ${ARG_UNPARSED_ARGUMENTS})

  set(cpp_srcs)
  set(i_srcs)
  foreach(s ${srcs})
    if(s MATCHES "(.*).i")
      list(APPEND i_srcs ${s})
    else()
      list(APPEND cpp_srcs ${s})
    endif()
  endforeach()

  set(PY_MODULE_DIR ${CMAKE_BINARY_DIR}/python)
  set(PY_MODULE ${binding})
  set(PY_MODULE_SWIG_SRC ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PY_MODULE}PYTHON_wrap.cxx)


  #SWIG command
  add_custom_command(
	OUTPUT
		${PY_MODULE_DIR}/${PY_MODULE}.py
		${PY_MODULE_SWIG_SRC}
	COMMAND
		${SWIG_EXECUTABLE}
		-python
		-module ${binding}
		-Wextra
		-outdir ${PY_MODULE_DIR}
		-c++
		${SWIG_MOD_INCLUDE_DIRS}
		-o ${PY_MODULE_SWIG_SRC}
		${i_srcs}
	DEPENDS
		${i_srcs}
	COMMENT "Generating SWIG binding"
  )

  elements_add_python_module(${binding}
                             ${PY_MODULE_SWIG_SRC} ${cpp_srcs}
                             LINK_LIBRARIES ${MODULE_ARG_LINK_LIBRARIES}
                             INCLUDE_DIRS ${MODULE_ARG_INCLUDE_DIRS})

  install(FILES ${PY_MODULE_DIR}/${binding}.py DESTINATION python)
  elements_install_headers(${ARG_PUBLIC_HEADERS})

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
  elements_common_add_build(${ARGN})

  add_executable(${executable} ${srcs})
  target_link_libraries(${executable} ${ARG_LINK_LIBRARIES})
  _elements_detach_debinfo(${executable})

  if (ELEMENTS_USE_EXE_SUFFIX)
    set_target_properties(${executable} PROPERTIES SUFFIX .exe)
  endif()

  #----Installation details-------------------------------------------------------
  install(TARGETS ${executable} EXPORT ${CMAKE_PROJECT_NAME}Exports RUNTIME DESTINATION bin OPTIONAL)
  install(EXPORT ${CMAKE_PROJECT_NAME}Exports DESTINATION cmake OPTIONAL)
  elements_export(EXECUTABLE ${executable})
  set_property(GLOBAL APPEND PROPERTY REGULAR_BIN_OBJECTS ${executable})
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
endfunction()

#---------------------------------------------------------------------------------------------------
# elements_add_unit_test(<name>
#                     source1 source2 ...
#                      [EXECUTABLE exename]
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


    if(NOT ${${name}_UNIT_TEST_TYPE} STREQUAL "None")
      if (${${name}_UNIT_TEST_TYPE} STREQUAL "Boost")
        find_package(Boost COMPONENTS unit_test_framework QUIET REQUIRED)
      else()
        find_package(${${name}_UNIT_TEST_TYPE} QUIET REQUIRED)
      endif()
      if (NOT TARGET ${package}_tests_dir)
        add_custom_target(${package}_tests_dir
                          COMMAND  ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/tests
                          COMMENT "Generating The ${package}/tests directory" VERBATIM)
      endif()
      set(testmain_file ${CMAKE_CURRENT_BINARY_DIR}/tests/${${name}_UNIT_TEST_TYPE}TestMain.cpp)
      set_source_files_properties(${testmain_file} PROPERTIES GENERATED TRUE)
      if(NOT TARGET ${package}_${${name}_UNIT_TEST_TYPE}TestMain)
        add_custom_target(${package}_${${name}_UNIT_TEST_TYPE}TestMain
                          COMMAND ${${${name}_UNIT_TEST_TYPE}_testmain_cmd} --quiet ${package} ${testmain_file}
                          DEPENDS ${package}_tests_dir
                          COMMENT "Generating the ${package} ${${name}_UNIT_TEST_TYPE}TestMain.cpp" VERBATIM)
      endif()
      set(srcs ${srcs} ${testmain_file})
      elements_add_executable(${executable} ${srcs}
                              LINK_LIBRARIES ${ARG_LINK_LIBRARIES} ${${name}_UNIT_TEST_TYPE}
                              INCLUDE_DIRS ${ARG_INCLUDE_DIRS} ${${name}_UNIT_TEST_TYPE})
      add_dependencies(${executable} ${package}_${${name}_UNIT_TEST_TYPE}TestMain)

    else()
      elements_add_executable(${executable} ${srcs}
                              LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                              INCLUDE_DIRS ${ARG_INCLUDE_DIRS})
    endif()


    get_target_property(exec_suffix ${executable} SUFFIX)
    if(NOT exec_suffix)
      set(exec_suffix)
    endif()

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

    add_test(NAME ${package}.${name}
             WORKING_DIRECTORY ${${name}_UNIT_TEST_WORKING_DIRECTORY}
             COMMAND ${env_cmd} ${extra_env} --xml ${env_xml}
             ${executable}${exec_suffix})

    set_property(TEST ${package}.${name} APPEND PROPERTY LABELS UnitTest ${package} Binary)
    if(NOT ${${name}_UNIT_TEST_TYPE} STREQUAL "None")
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
  set(has_local_headers FALSE)
  foreach(hdr_dir ${ARGN})
    if(NOT IS_ABSOLUTE ${hdr_dir})
      set(full_hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/${hdr_dir})
    else()
      set(full_hdr_dir ${hdr_dir})
    endif()
    if(IS_DIRECTORY ${full_hdr_dir})
      install(DIRECTORY ${hdr_dir}
              DESTINATION include
              FILES_MATCHING
              PATTERN "*.h"
              PATTERN "*.icpp"
              PATTERN "*.hpp"
              PATTERN "*.hxx"
              PATTERN "*.i"
              PATTERN "CVS" EXCLUDE
              PATTERN ".svn" EXCLUDE)
      if(NOT IS_ABSOLUTE ${hdr_dir})
        set(has_local_headers TRUE)
      endif()
    else()
      message(FATAL_ERROR "No ${hdr_dir} directory for header files in the ${CMAKE_CURRENT_SOURCE_DIR} location")
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
#                     )
#
# Add the python files in the directory as test. It collects the python test files
# and add a test for the python test framework (py.test, nose or unittest)
#---------------------------------------------------------------------------------------------------
function(add_python_test_dir subdir)

  CMAKE_PARSE_ARGUMENTS(PYTEST_ARG "" "PREFIX;PATTERN;NAME;TIMEOUT" "" ${ARGN})

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

  elements_expand_sources(pysrcs ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/${PYTEST_ARG_PATTERN})

  elements_get_package_name(package)

  if(PYFRMK_TEST)
    elements_add_test(${pytest_name}
                      COMMAND ${PYFRMK_TEST} ${pysrcs})
    set_property(TEST ${package}.${pytest_name} APPEND PROPERTY LABELS Python UnitTest ${PYFRMK_NAME})
  else()
    if(NOT PYTHON_VERSION_STRING VERSION_LESS "2.7")
      elements_add_test(${pytest_name}
                        COMMAND ${PYTHON_EXECUTABLE} -m unittest discover -s ${CMAKE_CURRENT_SOURCE_DIR}/${subdir} -p "${PYTEST_ARG_PATTERN}" )
      set_property(TEST ${package}.${pytest_name} APPEND PROPERTY LABELS Python UnitTest)
    endif()
  endif()

  if(PYTEST_ARG_TIMEOUT)
    set_property(TEST ${package}.${pytest_name} PROPERTY TIMEOUT ${PYTEST_ARG_TIMEOUT})
  endif()


endfunction()



#-------------------------------------------------------------------------------
# elements_install_python_modules()
#
# Declare that the subdirectory needs to install python modules.
# The hierarchy of directories and  files in the python directory will be
# installed.  If the first level of directories do not contain __init__.py, a
# warning is issued and an empty one will be installed.
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

  CMAKE_PARSE_ARGUMENTS(INSTALL_PY_MOD "" "TEST_TIMEOUT" "" ${ARGN})

  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python)
    install(DIRECTORY python/
            DESTINATION python
            FILES_MATCHING
            PATTERN "*.py"
            PATTERN "CVS" EXCLUDE
            PATTERN ".svn" EXCLUDE)
    # check for the presence of the __init__.py's and install them if needed
    file(GLOB sub-dir RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} python/*)
    foreach(dir ${sub-dir})
      if(NOT dir STREQUAL python/.svn
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
      if(NOT dir STREQUAL python/.svn)
        set_property(GLOBAL APPEND PROPERTY PROJ_PYTHON_PACKAGE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
      endif()
    endforeach()
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_PYTHON TRUE)
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/python)
      if(INSTALL_PY_MOD_TEST_TIMEOUT)
        add_python_test_dir(tests/python TIMEOUT ${INSTALL_PY_MOD_TEST_TIMEOUT})
      else()
        add_python_test_dir(tests/python)
      endif()
    endif()
  else()
    message(FATAL_ERROR "No python directory in the ${CMAKE_CURRENT_SOURCE_DIR} location")
  endif()
endfunction()

#---------------------------------------------------------------------------------------------------
# elements_install_scripts()
#
# Declare that the package needs to install the content of the 'scripts' directory.
#---------------------------------------------------------------------------------------------------
function(elements_install_scripts)
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
    install(DIRECTORY scripts/ DESTINATION scripts
            FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                             GROUP_EXECUTE GROUP_READ
                             WORLD_EXECUTE WORLD_READ
            PATTERN "CVS" EXCLUDE
            PATTERN ".svn" EXCLUDE
            PATTERN "*~" EXCLUDE
            PATTERN "*.pyc" EXCLUDE)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_SCRIPTS TRUE)
  else()
    message(FATAL_ERROR "No scripts directory in the ${CMAKE_CURRENT_SOURCE_DIR} location")
  endif()
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
      install(DIRECTORY aux/
              DESTINATION ${AUX_DIR_NAME}
              PATTERN "CVS" EXCLUDE
              PATTERN ".svn" EXCLUDE
              PATTERN "*~" EXCLUDE)
    endif()
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${AUX_DIR_NAME})
      install(DIRECTORY ${AUX_DIR_NAME}/
              DESTINATION ${AUX_DIR_NAME}
              PATTERN "CVS" EXCLUDE
              PATTERN ".svn" EXCLUDE
              PATTERN "*~" EXCLUDE)
    endif()
    if(IS_DIRECTORY ${CMAKE_BINARY_DIR}/${AUX_DIR_NAME})
      install(DIRECTORY ${CMAKE_BINARY_DIR}/${AUX_DIR_NAME}/
              DESTINATION ${AUX_DIR_NAME}
              PATTERN "CVS" EXCLUDE
              PATTERN ".svn" EXCLUDE
              PATTERN "*~" EXCLUDE)
    endif()
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_AUX TRUE)
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
            DESTINATION ${CONF_DIR_NAME}
            PATTERN "CVS" EXCLUDE
            PATTERN ".svn" EXCLUDE
            PATTERN "*~" EXCLUDE)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CONF TRUE)
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
  install(PROGRAMS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${name} DESTINATION scripts)
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_SCRIPTS TRUE)
endfunction()

#---------------------------------------------------------------------------------------------------
# elements_install_joboptions(<files...>)
#
# Install the specified options files in the directory 'jobOptions/<package>'.
#---------------------------------------------------------------------------------------------------
function(elements_install_joboptions)
  elements_get_package_name(package)
  install(FILES ${ARGN} DESTINATION jobOptions/${package})
endfunction()

#---------------------------------------------------------------------------------------------------
# elements_install_resources(<data files...> [DESTINATION subdir])
#
# Install the specified options files in the directory 'data/<package>[/subdir]'.
#---------------------------------------------------------------------------------------------------
function(elements_install_resources)
  CMAKE_PARSE_ARGUMENTS(ARG "" "DESTINATION" "" ${ARGN})

  elements_get_package_name(package)
  install(FILES ${ARG_UNPARSED_ARGUMENTS} DESTINATION data/${package}/${ARG_DESTINATION})
endfunction()

#-------------------------------------------------------------------------------
# elements_install_cmake_modules()
#
# Install the content of the cmake directory.
#-------------------------------------------------------------------------------
macro(elements_install_cmake_modules)
  install(DIRECTORY cmake/
          DESTINATION cmake
          FILES_MATCHING
            PATTERN "*.cmake"
            PATTERN "*.in"
            PATTERN "CVS" EXCLUDE
            PATTERN ".svn" EXCLUDE)
  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
  set_property(DIRECTORY PROPERTY ELEMENTS_EXPORTED_CMAKE ON)
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
endmacro()

#-------------------------------------------------------------------------------
# elements_generate_project_config_version_file()
#
# Create the file used by CMake to check if the found version of a package
# matches the requested one.
#-------------------------------------------------------------------------------
macro(elements_generate_project_config_version_file)
  message(STATUS "Generating ${CMAKE_PROJECT_NAME}ConfigVersion.cmake")

  if(CMAKE_PROJECT_VERSION_PATCH)
    set(vers_id ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH})
  else()
    set(vers_id ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR})
  endif()

  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)
  file(WRITE ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
"set(PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(PACKAGE_VERSION ${vers_id})
if(PACKAGE_NAME STREQUAL PACKAGE_FIND_NAME)
  if(PACKAGE_VERSION STREQUAL PACKAGE_FIND_VERSION)
    set(PACKAGE_VERSION_EXACT 1)
    set(PACKAGE_VERSION_COMPATIBLE 1)
    set(PACKAGE_VERSION_UNSUITABLE 0)
  elseif(PACKAGE_FIND_VERSION STREQUAL \"\") # No explicit version requested.
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
  install(FILES ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}ConfigVersion.cmake DESTINATION .)
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

list(INSERT CMAKE_MODULE_PATH 0 \${${CMAKE_PROJECT_NAME}_DIR}/cmake/modules)
list(INSERT CMAKE_MODULE_PATH 0 \${${CMAKE_PROJECT_NAME}_DIR}/cmake)
include(${CMAKE_PROJECT_NAME}PlatformConfig)
")
  install(FILES ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}Config.cmake DESTINATION .)
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

  string(REPLACE "\$" "\\\$" project_environment_string "${project_environment}")

  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)
  set(filename ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}PlatformConfig.cmake)
  file(WRITE ${filename}
"# File automatically generated: DO NOT EDIT.

# Get the exported informations about the targets
get_filename_component(_dir " \${CMAKE_CURRENT_LIST_FILE} " PATH)
#include(\${_dir}/${CMAKE_PROJECT_NAME}Exports.cmake)

# Set useful properties
get_filename_component(_dir " \${_dir} " PATH)
set(${CMAKE_PROJECT_NAME}_INCLUDE_DIRS \${_dir}/include)
set(${CMAKE_PROJECT_NAME}_LIBRARY_DIRS \${_dir}/lib)

set(${CMAKE_PROJECT_NAME}_BINARY_PATH \${_dir}/bin \${_dir}/scripts)
set(${CMAKE_PROJECT_NAME}_PYTHON_PATH \${_dir}/python)
set(${CMAKE_PROJECT_NAME}_CONF_PATH \${_dir}/${CONF_DIR_NAME})
set(${CMAKE_PROJECT_NAME}_AUX_PATH \${_dir}/${AUX_DIR_NAME})

set(${CMAKE_PROJECT_NAME}_COMPONENT_LIBRARIES ${component_libraries})
set(${CMAKE_PROJECT_NAME}_LINKER_LIBRARIES ${linker_libraries})

set(${CMAKE_PROJECT_NAME}_ENVIRONMENT ${project_environment_string})

set(${CMAKE_PROJECT_NAME}_EXPORTED_SUBDIRS)
foreach(p ${packages})
  get_filename_component(pn \${p} NAME)
  if(EXISTS \${_dir}/cmake/\${pn}Export.cmake)
    set(${CMAKE_PROJECT_NAME}_EXPORTED_SUBDIRS \${${CMAKE_PROJECT_NAME}_EXPORTED_SUBDIRS} \${p})
  endif()
endforeach()

set(${CMAKE_PROJECT_NAME}_OVERRIDDEN_SUBDIRS ${override_subdirs})
")

  install(FILES ${CMAKE_BINARY_DIR}/config/${CMAKE_PROJECT_NAME}PlatformConfig.cmake DESTINATION cmake)
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
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
  #message(STATUS "_env_conf_pop_instruction ${lst} => ${${lst}}")
  list(GET ${lst} 0 ${instr})
  if(${instr} STREQUAL INCLUDE OR ${instr} STREQUAL UNSET OR ${instr} STREQUAL SEARCH_PATH)
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

  if(${cmd} STREQUAL "SET")
    set(${output} "<env:set variable=\"${var}\">${val_}</env:set>")
  elseif(${cmd} STREQUAL "UNSET")
    set(${output} "<env:unset variable=\"${var}\"></env:unset>")
  elseif(${cmd} STREQUAL "PREPEND")
    set(${output} "<env:prepend variable=\"${var}\">${val_}</env:prepend>")
  elseif(${cmd} STREQUAL "APPEND")
    set(${output} "<env:append variable=\"${var}\">${val_}</env:append>")
  elseif(${cmd} STREQUAL "REMOVE")
    set(${output} "<env:remove variable=\"${var}\">${val_}</env:remove>")
  elseif(${cmd} STREQUAL "INCLUDE")
    get_filename_component(inc_name ${var} NAME)
    get_filename_component(inc_path ${var} PATH)
    if(${inc_path})
      set(${output} "<env:include hints=\"${inc_path}\">${inc_name}</env:include>")
    else()
      set(${output} "<env:include>${inc_name}</env:include>")
    endif()
  elseif(${cmd} STREQUAL "SEARCH_PATH")
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

  set(commands ${ARGN})

  #message(STATUS "start - ${commands}")
  while(commands)
    #message(STATUS "iter - ${commands}")
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
    if((NOT pack STREQUAL ElementsProject) AND (elements_project_idx EQUAL -1))
      message(STATUS "    ${pack}")
      # this is needed to get the non-cache variables for the packages
      find_package(${pack} QUIET)

      if(pack STREQUAL PythonInterp OR pack STREQUAL PythonLibs)
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
      list(APPEND environment   ${${pack}_ENVIRONMENT})
      list(APPEND library_path2 ${${pack}_LIBRARY_DIR} ${${pack}_LIBRARY_DIRS})
      # Try the version with the name of the package uppercase (unless the
      # package name is already uppercase).
      if(NOT pack STREQUAL _pack_upper)
        list(APPEND binary_path   ${${_pack_upper}_BINARY_PATH})
        list(APPEND python_path   ${${_pack_upper}_PYTHON_PATH})
        list(APPEND conf_path     ${${_pack_upper}_CONF_PATH})
        list(APPEND aux_path      ${${_pack_upper}_AUX_PATH})
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

      file(WRITE ${pkg_exp_file}
"# File automatically generated: DO NOT EDIT.

# Compute the installation prefix relative to this file.
get_filename_component(_IMPORT_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)
get_filename_component(_IMPORT_PREFIX \"\${_IMPORT_PREFIX}\" PATH)

")

      foreach(library ${exported_libs})
        file(APPEND ${pkg_exp_file} "add_library(${library} SHARED IMPORTED)\n")
        file(APPEND ${pkg_exp_file} "set_target_properties(${library} PROPERTIES\n")

        foreach(pn REQUIRED_INCLUDE_DIRS REQUIRED_LIBRARIES)
          get_property(prop TARGET ${library} PROPERTY ${pn})
          if (prop)
            file(APPEND ${pkg_exp_file} "  ${pn} \"${prop}\"\n")
          endif()
        endforeach()

        get_property(prop TARGET ${library} PROPERTY LOCATION)
        get_filename_component(prop ${prop} NAME)
        file(APPEND ${pkg_exp_file} "  IMPORTED_SONAME \"${prop}\"\n")
        file(APPEND ${pkg_exp_file} "  IMPORTED_LOCATION \"\${_IMPORT_PREFIX}/lib/${prop}\"\n")

        file(APPEND ${pkg_exp_file} "  )\n")
      endforeach()

      foreach(executable ${exported_execs})

        file(APPEND ${pkg_exp_file} "add_executable(${executable} IMPORTED)\n")
        file(APPEND ${pkg_exp_file} "set_target_properties(${executable} PROPERTIES\n")

        get_property(prop TARGET ${executable} PROPERTY LOCATION)
        get_filename_component(prop ${prop} NAME)
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
        file(APPEND ${pkg_exp_file} "set(${package}_VERSION ${subdir_version})\n")
      endif()
    endif()
    install(FILES ${pkg_exp_file} DESTINATION cmake)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
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
  CMAKE_PARSE_ARGUMENTS(PROJECT "" "" "USE;DATA" ${ARGN})
  # Non need to check consistency because it's already done in elements_project().

  #header
  set(data "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<manifest>\n")

  # Project name and version
  set(data "${data}  <project name=\"${project}\" version=\"${version}\" />\n")

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
        if(next STREQUAL VERSION)
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
  # Make the scripts directory in the build directory if it does not exist
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/scripts)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/scripts)
  endif()
  # Get the name of the file to create
  set(executable_file ${CMAKE_BINARY_DIR}/scripts/${executable})

  string(REPLACE "." "/" program_file ${module})
  set(program_file python/${program_file}.py)

  add_custom_command(OUTPUT ${executable_file}
                     COMMAND ${pythonprogramscript_cmd} --module ${module} --outdir ${CMAKE_BINARY_DIR}/scripts --execname ${executable}
                     DEPENDS ${program_file})

  string(REPLACE "." "_" python_program_target ${module})
  add_custom_target(${python_program_target} ALL DEPENDS ${executable_file})

  install(PROGRAMS ${executable_file} DESTINATION scripts)
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_SCRIPTS TRUE)
endfunction()
