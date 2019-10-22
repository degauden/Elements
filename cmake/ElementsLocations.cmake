include_guard()

include(SGSPlatform)
include(ElementsBuildFlags)

if(HIDE_SYSINC_WARNINGS)
  set(CMAKE_NO_SYSTEM_FROM_IMPORTED FALSE)  
else()
  set(CMAKE_NO_SYSTEM_FROM_IMPORTED TRUE)
endif()

if(NOT DEFINED SQUEEZED_INSTALL)
    set(SQUEEZED_INSTALL ON
        CACHE STRING "Enable the squizzing of the installation into a prefix directory"
        FORCE)
    message(STATUS "Sets the default value for SQUEEZED_INSTALL to ${SQUEEZED_INSTALL}")     
endif()

# Install Area business
if(USE_LOCAL_INSTALLAREA)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/InstallArea/${BINARY_TAG} CACHE PATH
        "Install path prefix, prepended onto install directories." FORCE )
    set(SQUEEZED_INSTALL OFF
        CACHE STRING "Enable the squizzing of the installation into a prefix directory"
        FORCE)
endif()



if(NOT SQUEEZED_INSTALL)

  if(DEFINED ENV{SOFTWARE_BASE_VAR})
    set(ELEMENTS_BASE_VAR "$ENV{SOFTWARE_BASE_VAR}" CACHE STRING "Elements Base Install Variable")
    message(STATUS "SOFTWARE_BASE_VAR is in the environment: ${ELEMENTS_BASE_VAR}")
  else()
    set(ELEMENTS_BASE_VAR "EUCLID_BASE" CACHE STRING "Elements Base Install Variable")
    message(STATUS "SOFTWARE_BASE_VAR is not in the environment: falling back to ${ELEMENTS_BASE_VAR}")
  endif()


  if(DEFINED ENV{${ELEMENTS_BASE_VAR}})
    set(ELEMENTS_BASE_DIR "$ENV{${ELEMENTS_BASE_VAR}}" CACHE STRING "Elements Base Install Directory from the ${ELEMENTS_BASE_VAR} env variable")
    message(STATUS "${ELEMENTS_BASE_VAR} is in the environment: ${ELEMENTS_BASE_DIR}")
  else()
    set(ELEMENTS_BASE_DIR "/opt/euclid" CACHE STRING "Euclid Base Install Directory")
    message(STATUS "${ELEMENTS_BASE_VAR} is not in the environment: using default ${ELEMENTS_BASE_DIR}")
  endif()
  
endif()


set(ELEMENTS_DATA_SUFFIXES DBASE;PARAM;EXTRAPACKAGES CACHE STRING
    "List of (suffix) directories where to look for data packages.")

if(NOT USE_LOCAL_INSTALLAREA)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(NOT SQUEEZED_INSTALL)
       set(CMAKE_INSTALL_PREFIX ${ELEMENTS_BASE_DIR}/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${BINARY_TAG} CACHE PATH
           "Install path prefix, prepended onto install directories." FORCE )
    endif()
  endif()
endif()

message(STATUS "The installation location is ${CMAKE_INSTALL_PREFIX}")
message(STATUS "The squeezing of the installation is ${SQUEEZED_INSTALL}")

set(lib_install_suff lib)

if(SQUEEZED_INSTALL)
  set(lib_install_suff lib64)
  if(SGS_ARCH STREQUAL x86_64)
    if(EXISTS /usr/lib64)
      set(lib_install_suff lib64)
    else()
      set(lib_install_suff lib)
    endif()
  else()
    if(EXISTS /usr/lib32)
      set(lib_install_suff lib32)
    else()
      set(lib_install_suff lib)
    endif()
  endif()
endif()

set(CMAKE_LIB_INSTALL_SUFFIX ${lib_install_suff} CACHE STRING "Suffix for the install directory of the libraries")
set(CMAKE_BIN_INSTALL_SUFFIX bin CACHE STRING "Suffix for the install directory of the binaries")



if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE STRING
	   "Single build output directory for all executables" FORCE)
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_LIB_INSTALL_SUFFIX} CACHE STRING
	   "Single build output directory for all libraries" FORCE)
endif()


set(CONF_DIR_NAME "conf" CACHE STRING "Name of the configuration files directory")
set(AUX_DIR_NAME "auxdir" CACHE STRING "Name of the auxiliary files directory")
set(MAKE_DIR_NAME "make" CACHE STRING "Name of the make files directory")
set(DOC_DIR_NAME "doc" CACHE STRING "Name of the documentation directory")

set(INCLUDE_INSTALL_SUFFIX include)
if(SQUEEZED_INSTALL)
  set(BIN_INSTALL_SUFFIX bin)
  set(SCRIPT_INSTALL_SUFFIX bin)
  set(CONF_INSTALL_SUFFIX share/${CONF_DIR_NAME})
  set(AUX_INSTALL_SUFFIX share/${AUX_DIR_NAME})
  set(CMAKE_INSTALL_SUFFIX ${CMAKE_LIB_INSTALL_SUFFIX}/cmake/ElementsProject)
  set(CMAKE_CONFIG_INSTALL_SUFFIX ${CMAKE_INSTALL_SUFFIX})
  set(CMAKE_CONFIG_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SUFFIX})
  set(XML_INSTALL_SUFFIX ${CMAKE_INSTALL_SUFFIX})
  set(MAKE_INSTALL_SUFFIX share/Elements/${MAKE_DIR_NAME})
  set(DOC_INSTALL_SUFFIX share/${DOC_DIR_NAME}/${CMAKE_PROJECT_NAME})
else()
  set(BIN_INSTALL_SUFFIX bin)
  set(SCRIPT_INSTALL_SUFFIX scripts)
  set(CONF_INSTALL_SUFFIX ${CONF_DIR_NAME})
  set(AUX_INSTALL_SUFFIX ${AUX_DIR_NAME})
  set(CMAKE_INSTALL_SUFFIX cmake)
  set(CMAKE_CONFIG_INSTALL_SUFFIX .)
  set(CMAKE_CONFIG_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(XML_INSTALL_SUFFIX .)
  set(MAKE_INSTALL_SUFFIX ${MAKE_DIR_NAME})
  set(DOC_INSTALL_SUFFIX ${DOC_DIR_NAME})
endif()

#------------------------------------------------------------------------------------------------
# RPATH business

if(ELEMENTS_USE_RPATH)

  if (CMAKE_SYSTEM_NAME MATCHES Linux)
    SET(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_LIB_INSTALL_SUFFIX}")

    # add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


    # the RPATH to be used when installing, but only if it's not a system directory
    LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "$ORIGIN/../${CMAKE_LIB_INSTALL_SUFFIX}" isSystemDir)
    IF("${isSystemDir}" STREQUAL "-1")
      SET(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_LIB_INSTALL_SUFFIX}")
    ENDIF()
  endif()

  if(APPLE)
    set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_LIB_INSTALL_SUFFIX} CACHE PATH
        "Install RPath." FORCE )
    set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_LIB_INSTALL_SUFFIX} CACHE PATH
        "Install Name Dir." FORCE )
  endif()


endif()


#python business

set(PYTHON_INSTALL_SUFFIX python)
set(PYTHON_DYNLIB_INSTALL_SUFFIX python/lib-dynload)

if(SQUEEZED_INSTALL)

  find_package(PythonInterp ${PYTHON_EXPLICIT_VERSION})

  execute_process(COMMAND "${PYTHON_EXECUTABLE}" "-c"
                  "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True, prefix='${CMAKE_INSTALL_PREFIX}').replace('${CMAKE_INSTALL_PREFIX}/',''))"
                  OUTPUT_VARIABLE PYTHON_INSTALL_SUFFIX
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)


  get_filename_component(python_install_suffix_parent ${PYTHON_INSTALL_SUFFIX} DIRECTORY)

  set(PYTHON_DYNLIB_INSTALL_SUFFIX ${python_install_suffix_parent}/lib-dynload)

endif()

get_arch_lib_dir(that_arch)

file(TO_CMAKE_PATH "$ENV{CMAKE_PREFIX_PATH}" current_cmake_prefix_path)

set(ELEMENTS_DEFAULT_SEARCH_PATH)
foreach(_ds ${current_cmake_prefix_path})  
  list(APPEND ELEMENTS_DEFAULT_SEARCH_PATH ${_ds}/${that_arch}/cmake/ElementsProject)
endforeach()
set(ELEMENTS_USR_SEARCH_PATH /usr/${that_arch}/cmake/ElementsProject)

file(TO_CMAKE_PATH "$ENV{XDG_DATA_DIRS}" data_dirs)
if(data_dirs)
  list(INSERT data_paths 0 ${data_dirs})
endif()

file(TO_CMAKE_PATH "$ENV{XDG_DATA_HOME}" data_home)
if(data_home)
  list(INSERT data_paths 0 ${data_home})
endif()

list(APPEND data_paths /usr/share)
list(REMOVE_DUPLICATES data_paths)

set(DATA_MODULE_PATH ${data_paths} CACHE STRING "List of base directories where to look for data packages.")

set(SPEC_LIBDIR "%{_prefix}/${CMAKE_LIB_INSTALL_SUFFIX}")
set(SPEC_PYDIR "%{_prefix}/${PYTHON_INSTALL_SUFFIX}")
set(SPEC_PYDYNDIR "%{_prefix}/${PYTHON_DYNLIB_INSTALL_SUFFIX}")
set(SPEC_SCRIPTSDIR "%{_prefix}/${SCRIPT_INSTALL_SUFFIX}")
set(SPEC_CMAKEDIR "%{_prefix}/${CMAKE_INSTALL_SUFFIX}")
set(SPEC_MAKEDIR "%{_prefix}/${MAKE_INSTALL_SUFFIX}")
set(SPEC_CONFDIR "%{_prefix}/${CONF_INSTALL_SUFFIX}")
set(SPEC_AUXDIR "%{_prefix}/${AUX_INSTALL_SUFFIX}")
set(SPEC_DOCDIR "%{_prefix}/${DOC_INSTALL_SUFFIX}")
set(SPEC_XMLDIR "%{_prefix}/${XML_INSTALL_SUFFIX}")

# remove the trailing / or .
foreach(_do LIB PY PYDYN SCRIPTS CMAKE MAKE CONF AUX DOC XML)
  dir_strip_end(SPEC_${_do}DIR)
endforeach()

