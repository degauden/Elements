################################################################################
# Astro CMake toolchain
#-------------------------------------------------------------------------------
# The Astro CMake toolchain is required to build a project using the libraries and
# tools provided by SPI/SFT (a.k.a. SGSCMT).
#
# The variables used to tune the toolchain behavior are:
#
#  - BINARY_TAG: inferred from the system or from the environment (CMAKECONFIG,
#                CMTCONFIG), defines the target platform (by default is the same
#                as the host)
#  - SGS_SYSTEM: by default it is derived from BINARY_TAG, but it can be set
#                explicitly to a compatible supported platform if the default
#                one is not supported.
#                E.g.: if BINARY_TAG is x86_64-ubuntu12.04-gcc46-opt, SGS_SYSTEM
#                      should be set to x86_64-slc6-gcc46.
################################################################################

if(SGSPlatform_included)
  return()
endif(SGSPlatform_included)
set(SGSPlatform_included true)

################################################################################
# Functions to get system informations for the SGS configuration.
################################################################################
# Get the host architecture.
function(sgs_find_host_arch)
  if(NOT SGS_HOST_ARCH)
    if(CMAKE_HOST_SYSTEM_PROCESSOR)
      # Fix to get the right value for MacOSX
      if( (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin") AND (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386") )
        execute_process(COMMAND uname -m OUTPUT_VARIABLE arch OUTPUT_STRIP_TRAILING_WHITESPACE)
      else()
        set(arch ${CMAKE_HOST_SYSTEM_PROCESSOR})
      endif()
    else()
      if(UNIX)
        execute_process(COMMAND uname -p OUTPUT_VARIABLE arch OUTPUT_STRIP_TRAILING_WHITESPACE)
      else()
        set(arch $ENV{PROCESSOR_ARCHITECTURE})
      endif()
    endif()

    set(SGS_HOST_ARCH ${arch} CACHE STRING "Architecture of the host (same as CMAKE_HOST_SYSTEM_PROCESSOR).")
    mark_as_advanced(SGS_HOST_ARCH)
  endif()
endfunction()
################################################################################
# Detect the OS name and version
function(sgs_find_host_os)
  if(NOT SGS_HOST_OS OR NOT SGS_HOST_OSVERS)
    if(APPLE)
      set(os mac)
      execute_process(COMMAND sw_vers "-productVersion"
                      COMMAND cut -d . -f 1-2
                      OUTPUT_VARIABLE osvers OUTPUT_STRIP_TRAILING_WHITESPACE)
      string(REPLACE "." "" osvers ${osvers})
    else()
      set(issue_file_list /etc/redhat-release /etc/system-release /etc/SuSE-release /etc/issue /etc/issue.net)
      foreach(issue_file ${issue_file_list})
        execute_process(COMMAND cat ${issue_file} OUTPUT_VARIABLE issue OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(issue MATCHES Ubuntu)
          set(os ubuntu)
          string(REGEX REPLACE ".*Ubuntu ([0-9]+)[.]([0-9]+).*" "\\1.\\2" osvers "${issue}")
          break()
        elseif(issue MATCHES "Scientific Linux|SLC|Fedora|CentOS Linux") # RedHat-like distributions
          string(TOLOWER "${CMAKE_MATCH_0}" os)
          if(os STREQUAL fedora)
            set(os fc) # we use an abbreviation for Fedora
          endif()
          if(os STREQUAL "scientific linux")
            set(os sl) # we use an abbreviation for Scientific Linux
          endif()
          if(os STREQUAL "centos linux")
            set(os co) # we use an abbreviation for Scientific Linux
          endif()
          string(REGEX REPLACE ".*release ([0-9]+)[. ].*" "\\1" osvers "${issue}")
          break()
        else()
          set(os linux)
          set(osvers)
        endif()
      endforeach()
      if(os STREQUAL "linux")
        message(WARNING "Unkown OS, assuming 'linux'")
      endif()
    endif()

    set(SGS_HOST_OS ${os} CACHE STRING "Name of the operating system (or Linux distribution)." FORCE)
    set(SGS_HOST_OSVERS ${osvers} CACHE STRING "Version of the operating system (or Linux distribution)." FORCE)
    mark_as_advanced(SGS_HOST_OS SGS_HOST_OSVERS)
  endif()
endfunction()
################################################################################
# Get system compiler.
function(sgs_find_host_compiler)
  message(STATUS "--------------------------------------------------->> This is the COMPILER_ID ${CMAKE_CXX_COMPILER_ID}")
  if(NOT SGS_HOST_COMP OR NOT SGS_HOST_COMPVERS)
    find_program(SGS_HOST_C_COMPILER   NAMES gcc cc cl clang icc bcc xlc
                 DOC "Host C compiler")
    find_program(SGS_HOST_CXX_COMPILER NAMES c++ g++ cl clang++ icpc CC aCC bcc xlC
                 DOC "Host C++ compiler")
    mark_as_advanced(SGS_HOST_C_COMPILER SGS_HOST_CXX_COMPILER)
    if(SGS_HOST_C_COMPILER MATCHES /cl)
      set(compiler vc)
      execute_process(COMMAND ${SGS_HOST_C_COMPILER} ERROR_VARIABLE versioninfo OUTPUT_VARIABLE out)
      string(REGEX REPLACE ".*Version ([0-9]+)[.].*" "\\1" cvers "${versioninfo}")
      math(EXPR cvers "${cvers} - 6")
    elseif(SGS_HOST_C_COMPILER MATCHES /gcc)
      set(compiler gcc)
      execute_process(COMMAND ${SGS_HOST_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
      string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
      list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
      list(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)
      set(cvers ${GCC_MAJOR}${GCC_MINOR})
    elseif(SGS_HOST_C_COMPILER MATCHES /icc)
      set(compiler icc)
      execute_process(COMMAND ${SGS_HOST_C_COMPILER} -dumpversion OUTPUT_VARIABLE ICC_VERSION)
      string(REGEX MATCHALL "[0-9]+" ICC_VERSION_COMPONENTS ${ICC_VERSION})
      list(GET ICC_VERSION_COMPONENTS 0 ICC_MAJOR)
      list(GET ICC_VERSION_COMPONENTS 1 ICC_MINOR)
      set(cvers ${ICC_MAJOR})
    elseif(SGS_HOST_C_COMPILER MATCHES /clang)
      set(compiler clang)
      execute_process(COMMAND ${SGS_HOST_C_COMPILER} --version OUTPUT_VARIABLE CLANG_VERSION)
      string(REGEX MATCHALL "[0-9]+" CLANG_VERSION_COMPONENTS ${CLANG_VERSION})
      list(GET CLANG_VERSION_COMPONENTS 0 CLANG_MAJOR)
      list(GET CLANG_VERSION_COMPONENTS 1 CLANG_MINOR)
      set(cvers ${CLANG_MAJOR}${CLANG_MINOR})
    else()
      message(WARNING "Unknown host C compiler ${SGS_HOST_C_COMPILER}")
      set(compiler)
      set(cvers)
    endif()

    set(SGS_HOST_COMP ${compiler} CACHE STRING "Name of the host default compiler." FORCE)
    set(SGS_HOST_COMPVERS ${cvers} CACHE STRING "Version of the host default compiler." FORCE)
    mark_as_advanced(SGS_HOST_COMP SGS_HOST_COMPVERS)
  endif()
endfunction()
################################################################################
# Detect host system
function(sgs_detect_host_platform)
  sgs_find_host_arch()
  sgs_find_host_os()
  sgs_find_host_compiler()
  set(SGS_HOST_SYSTEM ${SGS_HOST_ARCH}-${SGS_HOST_OS}${SGS_HOST_OSVERS}-${SGS_HOST_COMP}${SGS_HOST_COMPVERS}
      CACHE STRING "Platform id of the system.")
  mark_as_advanced(SGS_HOST_SYSTEM)
endfunction()
################################################################################
# Get the target system platform (arch., OS, compiler)
function(sgs_get_target_platform)
  if(NOT BINARY_TAG)
    # Take the target system id from the environment
    if(NOT "$ENV{BINARY_TAG}" STREQUAL "")
      set(tag $ENV{BINARY_TAG})
      set(tag_source BINARY_TAG)
    elseif(NOT "$ENV{CMAKECONFIG}" STREQUAL "")
      set(tag $ENV{CMAKECONFIG})
      set(tag_source CMAKECONFIG)
    elseif(NOT "$ENV{CMTCONFIG}" STREQUAL "")
      set(tag $ENV{CMTCONFIG})
      set(tag_source CMTCONFIG)
    else()
      set(tag ${SGS_HOST_SYSTEM}-o2g)
      set(tag_source default)
    endif()
    message(STATUS "Target binary tag from ${tag_source}: ${tag}")
    set(BINARY_TAG ${tag} CACHE STRING "Platform id for the produced binaries.")
  endif()
  # Split the target binary tag
  string(REGEX MATCHALL "[^-]+" out ${BINARY_TAG})
  list(GET out 0 arch)
  list(GET out 1 os)
  list(GET out 2 comp)
  list(GET out 3 type)

  set(SGS_BUILD_TYPE ${type} CACHE STRING "Type of build (SGS id).")

  set(SGS_TARGET ${arch}-${os}-${comp})
  if(NOT SGS_SYSTEM)
    set(SGS_SYSTEM ${arch}-${os}-${comp} CACHE STRING "Platform id of the target system or a compatible one.")
  endif()

  # Convert the components of the tag in the equivalents of SGS_HOST_*,
  # but transient
  set(SGS_ARCH  ${arch})

  if (os MATCHES "([^0-9.]+)([0-9.]+)")
    set(SGS_OS     ${CMAKE_MATCH_1})
    set(SGS_OSVERS ${CMAKE_MATCH_2})
  else()
    set(SGS_OS     ${os})
    set(SGS_OSVERS "")
  endif()

  if (comp MATCHES "([^0-9.]+)([0-9.]+|max)")
    set(SGS_COMP     ${CMAKE_MATCH_1})
    set(SGS_COMPVERS ${CMAKE_MATCH_2})
  else()
    set(SGS_COMP     ${comp})
    set(SGS_COMPVERS "")
  endif()

  # Convert SGS_BUILD_TYPE to CMAKE_BUILD_TYPE
  if(SGS_BUILD_TYPE STREQUAL "opt")
    set(type Release)
  elseif(SGS_BUILD_TYPE STREQUAL "dbg")
    set(type Debug)
  elseif(SGS_BUILD_TYPE STREQUAL "cov")
    set(type Coverage)
  elseif(SGS_BUILD_TYPE STREQUAL "pro")
    set(type Profile)
  elseif(SGS_BUILD_TYPE STREQUAL "o2g")
    set(type RelWithDebInfo)
  elseif(SGS_BUILD_TYPE STREQUAL "min")
    set(type MinSizeRel)
  else()
    message(FATAL_ERROR "SGS build type ${type} not supported.")
  endif()
  set(CMAKE_BUILD_TYPE ${type} CACHE STRING
      "Choose the type of build, options are: empty, Debug, Release, Coverage, Profile, RelWithDebInfo, MinSizeRel." FORCE)

  set(SGS_BUILD_TYPE_SHORT_NAMES o2g opt dbg pro cov min PARENT_SCOPE)

  # architecture
  set(CMAKE_SYSTEM_PROCESSOR ${SGS_ARCH} PARENT_SCOPE)

  # system name
  if(SGS_OS STREQUAL "winxp")
    set(CMAKE_SYSTEM_NAME Windows PARENT_SCOPE)
  elseif(SGS_OS STREQUAL "mac" OR SGS_OS STREQUAL "osx")
    set(CMAKE_SYSTEM_NAME Darwin PARENT_SCOPE)
  elseif(SGS_OS STREQUAL "slc" OR SGS_OS STREQUAL "sl" OR SGS_OS STREQUAL "ubuntu" OR SGS_OS STREQUAL "fc" OR SGS_OS STREQUAL "co" OR SGS_OS STREQUAL "linux")
    set(CMAKE_SYSTEM_NAME Linux PARENT_SCOPE)
  else()
    set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
    message(WARNING "OS ${SGS_OS} is not a known platform, assuming it's a ${CMAKE_SYSTEM_NAME}.")
  endif()

  # set default platform ids
  set(SGS_platform ${SGS_SYSTEM}-${SGS_BUILD_TYPE} CACHE STRING "Platform ID for the AA project binaries.")
  set(SGS_system   ${SGS_SYSTEM}-opt               CACHE STRING "Platform ID for the external libraries.")

  mark_as_advanced(SGS_platform SGS_system)

  # Report the platform ids.
  message(STATUS "Target system: ${SGS_TARGET}")
  message(STATUS "Build type: ${SGS_BUILD_TYPE}")

  if(NOT SGS_HOST_SYSTEM STREQUAL SGS_TARGET)
    message(STATUS "Host system: ${SGS_HOST_SYSTEM}")
  endif()

  if(NOT SGS_TARGET STREQUAL SGS_SYSTEM)
    message(STATUS "Use SGS system: ${SGS_SYSTEM}")
  endif()

  # copy variables to parent scope
  foreach(v ARCH OS OSVERS COMP COMPVERS TARGET)
    set(SGS_${v} ${SGS_${v}} PARENT_SCOPE)
  endforeach()
endfunction()


################################################################################
# Run platform detection (system and target).
################################################################################
# Deduce the SGS configuration tag from the system
sgs_detect_host_platform()
sgs_get_target_platform()

## Debug messages.
#foreach(p SGS_HOST_ SGS_)
#  foreach(v ARCH OS OSVERS COMP COMPVERS)
#    message(STATUS "toolchain: ${p}${v} -> ${${p}${v}}")
#  endforeach()
#endforeach()
#message(STATUS "toolchain: SGS_BUILD_TYPE -> ${SGS_BUILD_TYPE}")
#
#message(STATUS "toolchain: CMAKE_HOST_SYSTEM_PROCESSOR -> ${CMAKE_HOST_SYSTEM_PROCESSOR}")
#message(STATUS "toolchain: CMAKE_HOST_SYSTEM_NAME      -> ${CMAKE_HOST_SYSTEM_NAME}")
#message(STATUS "toolchain: CMAKE_HOST_SYSTEM_VERSION   -> ${CMAKE_HOST_SYSTEM_VERSION}")



