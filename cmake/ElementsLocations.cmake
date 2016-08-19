include_guard()

include(SGSPlatform)
include(ElementsBuildFlags)

set(ELEMENTS_DATA_SUFFIXES DBASE;PARAM;EXTRAPACKAGES CACHE STRING
    "List of (suffix) directories where to look for data packages.")

# Install Area business
if(USE_LOCAL_INSTALLAREA)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/InstallArea/${BINARY_TAG} CACHE PATH
        "Install path prefix, prepended onto install directories." FORCE )
    set(SQUEEZED_INSTALL OFF
        CACHE STRING "Enable the squizzing of the installation into a prefix directory"
        FORCE)
else()
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(NOT SQUEEZED_INSTALL)
       set(CMAKE_INSTALL_PREFIX ${EUCLID_BASE_DIR}/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${BINARY_TAG} CACHE PATH
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

set(CMAKE_LIB_INSTALL_SUFFIX ${lib_install_suff} CACHE STRING "Suffix for the install directory of the libraries" FORCE)



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

set(INCLUDE_INSTALL_SUFFIX include)
if(SQUEEZED_INSTALL)
  set(BIN_INSTALL_SUFFIX bin)
  set(SCRIPT_INSTALL_SUFFIX bin)
  set(CONF_INSTALL_SUFFIX share/${CONF_DIR_NAME})
  set(AUX_INSTALL_SUFFIX share/${AUX_DIR_NAME})
  set(CMAKE_INSTALL_SUFFIX ${CMAKE_LIB_INSTALL_SUFFIX}/cmake/Elements)
  set(CMAKE_CONFIG_INSTALL_SUFFIX ${CMAKE_INSTALL_SUFFIX})
  set(XML_INSTALL_SUFFIX share/Elements)
else()
  set(BIN_INSTALL_SUFFIX bin)
  set(SCRIPT_INSTALL_SUFFIX scripts)
  set(CONF_INSTALL_SUFFIX ${CONF_DIR_NAME})
  set(AUX_INSTALL_SUFFIX ${AUX_DIR_NAME})
  set(CMAKE_INSTALL_SUFFIX cmake)
  set(CMAKE_CONFIG_INSTALL_SUFFIX .)
  set(XML_INSTALL_SUFFIX .)
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

#execute_process(COMMAND "python -c \"from distutils.sysconfig import get_python_lib; print(get_python_lib(prefix='${CMAKE_INSTALL_PREFIX}'))\"" OUTPUT_VARIABLE py_lib_fullpath)
#execute_process(COMMAND "python -c \"from distutils.sysconfig import get_python_lib; print(get_python_lib())\"" OUTPUT_VARIABLE py_lib_fullpath)

set(PYTHON_INSTALL_SUFFIX python)
set(PYTHON_DYNLIB_INSTALL_SUFFIX python/lib-dynload)

if(SQUEEZED_INSTALL)
  find_package(PythonInterp)

  execute_process(COMMAND "${PYTHON_EXECUTABLE}" "-c"
                  "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True, prefix='${CMAKE_INSTALL_PREFIX}').replace('${CMAKE_INSTALL_PREFIX}/',''))"
                  OUTPUT_VARIABLE PYTHON_INSTALL_SUFFIX
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)


  get_filename_component(python_install_suffix_parent ${PYTHON_INSTALL_SUFFIX} DIRECTORY)

  set(PYTHON_DYNLIB_INSTALL_SUFFIX ${python_install_suffix_parent}/lib-dynload)

endif()

get_arch_lib_dir(that_arch)

set(ELEMENTS_DEFAULT_SEARCH_PATH ${CMAKE_INSTALL_PREFIX}/${that_arch}/cmake/Elements)
set(ELEMENTS_USR_SEARCH_PATH /usr/${that_arch}/cmake/Elements)
