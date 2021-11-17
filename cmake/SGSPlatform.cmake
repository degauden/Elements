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
#                E.g.: if BINARY_TAG is x86_64-ub12-gcc46-opt, SGS_SYSTEM
#                      should be set to x86_64-slc6-gcc46.
################################################################################

include_guard(GLOBAL)

################################################################################
# Helper function for the platform build type
################################################################################

function(getShortBuildType short_type long_type)

  # Convert CMAKE_BUILD_TYPE to SGS_BUILD_TYPE

  string(TOLOWER "${long_type}" lower_long_type)
  
  if("${lower_long_type}" STREQUAL "release")
    set(${short_type} opt PARENT_SCOPE)
  elseif("${lower_long_type}" STREQUAL "debug")
    set(${short_type} dbg PARENT_SCOPE)
  elseif("${lower_long_type}" STREQUAL "coverage")
    set(${short_type} cov PARENT_SCOPE)
  elseif("${lower_long_type}" STREQUAL "profile")
    set(${short_type} pro PARENT_SCOPE)
  elseif("${lower_long_type}" STREQUAL "relwithdebinfo")
    set(${short_type} o2g PARENT_SCOPE)
  elseif("${lower_long_type}" STREQUAL "minsizerel")
    set(${short_type} min PARENT_SCOPE)
  else()
    message(FATAL_ERROR "Build type ${lower_long_type} not supported.")
  endif()

  
endfunction()

################################################################################
# Helper function to extract the compiler major and minor version numbers
################################################################################

function(getCompVersionNumbers compiler_name comp_major_var comp_minor_var)

  if (CMAKE_C_COMPILER)
    set(compiler_exe ${CMAKE_C_COMPILER})
  else()
    find_program(compiler_exe NAMES ${compiler_name} 
               DOC "Host C compiler")
  endif()
               
  if("${compiler_name}" STREQUAL "gcc")
    
    execute_process(COMMAND ${compiler_exe} -dumpversion OUTPUT_VARIABLE compiler_version)
    string(REGEX MATCHALL "[0-9]+" compiler_version_components ${compiler_version})
    list(LENGTH compiler_version_components compiler_version_components_nb)
    if(compiler_version_components_nb LESS "2")
      execute_process(COMMAND ${compiler_exe} --version OUTPUT_VARIABLE compiler_version)
      string(REGEX MATCHALL "[0-9]+" compiler_version_components ${compiler_version})  
    endif()
    list(GET compiler_version_components 0 compiler_major)
    list(GET compiler_version_components 1 compiler_minor)
    
  elseif("${compiler_name}" STREQUAL "icc")
    
    execute_process(COMMAND ${compiler_exe} -dumpversion OUTPUT_VARIABLE compiler_version)
    string(REGEX MATCHALL "[0-9]+" compiler_version_components ${compiler_version})
    list(GET compiler_version_components 0 compiler_major)
    list(GET compiler_version_components 1 compiler_minor)

  elseif("${compiler_name}" STREQUAL "clang")

    execute_process(COMMAND ${compiler_exe} --version OUTPUT_VARIABLE compiler_version)
    if(APPLE)
      string(REGEX MATCH "LLVM[ \t]+([0-9]+)[.]([0-9]+)" compiler_version_components ${compiler_version})
      set(compiler_major ${CMAKE_MATCH_1})
      set(compiler_minor ${CMAKE_MATCH_2})
      if(NOT compiler_major)
        string(REGEX MATCHALL "[0-9]+" compiler_version_components ${compiler_version})
        list(GET compiler_version_components 0 compiler_major)
        list(GET compiler_version_components 1 compiler_minor)
        set(compiler "llvm")
      endif()
    else()
      string(REGEX MATCHALL "[0-9]+" compiler_version_components ${compiler_version})
      list(GET compiler_version_components 0 compiler_major)
      list(GET compiler_version_components 1 compiler_minor)
    endif()
    
  else()
  
    message(WARNING "Unknown host C compiler ${compiler_name}")
    set(compiler_major)
    set(compiler_minor)
  
  endif()

  set(${comp_major_var} ${compiler_major} PARENT_SCOPE)
  set(${comp_minor_var} ${compiler_minor} PARENT_SCOPE)
  

endfunction()


function(getLongBuildType long_type short_type)
  
  # Convert SGS_BUILD_TYPE to CMAKE_BUILD_TYPE

  if("${short_type}" STREQUAL "opt")
    set(${long_type} Release PARENT_SCOPE)
  elseif("${short_type}" STREQUAL "dbg")
    set(${long_type} Debug PARENT_SCOPE)
  elseif("${short_type}" STREQUAL "cov")
    set(${long_type} Coverage PARENT_SCOPE)
  elseif("${short_type}" STREQUAL "pro")
    set(${long_type} Profile PARENT_SCOPE)
  elseif("${short_type}" STREQUAL "o2g")
    set(${long_type} RelWithDebInfo PARENT_SCOPE)
  elseif("${short_type}" STREQUAL "min")
    set(${long_type} MinSizeRel PARENT_SCOPE)
  else()
    message(FATAL_ERROR "Build type ${short_type} not supported.")
  endif()

endfunction()

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

  set(osvers "")

  if(NOT SGS_HOST_OS OR NOT SGS_HOST_OSVERS)
    if(APPLE)
      set(os osx)
      execute_process(COMMAND sw_vers "-productVersion"
                      COMMAND cut -d . -f 1-2
                      OUTPUT_VARIABLE osvers OUTPUT_STRIP_TRAILING_WHITESPACE)
      string(REPLACE "." "" osvers ${osvers})
    else()
      set(issue_file_list /etc/redhat-release /etc/system-release /etc/SuSE-release /etc/issue /etc/issue.net)
      foreach(issue_file ${issue_file_list})
        if(EXISTS ${issue_file})
          execute_process(COMMAND cat ${issue_file} OUTPUT_VARIABLE issue OUTPUT_STRIP_TRAILING_WHITESPACE)
          if(issue MATCHES Ubuntu)
            set(os ub)
            if(issue MATCHES ".*Ubuntu ([0-9]+)[.]([0-9]+).*")
              string(REGEX REPLACE ".*Ubuntu ([0-9]+)[.]([0-9]+).*" "\\1" osvers "${issue}")
            endif()
            break()
          elseif(issue MATCHES "Scientific Linux|SLC|Fedora|CentOS Linux|CentOS") # RedHat-like distributions
            string(TOLOWER "${CMAKE_MATCH_0}" os)
            if("${os}" STREQUAL fedora)
              set(os fc) # we use an abbreviation for Fedora
            endif()
            if("${os}" STREQUAL "scientific linux")
              set(os sl) # we use an abbreviation for Scientific Linux
            endif()
            if(("${os}" STREQUAL "centos linux") OR ("${os}" STREQUAL "centos"))
              set(os co) # we use an abbreviation for Scientific Linux
            endif()
            if(issue MATCHES ".*release ([0-9]+)[. ].*")
              string(REGEX REPLACE ".*release ([0-9]+)[. ].*" "\\1" osvers "${issue}")
            endif()
            break()
          else()
            set(os linux)
            set(osvers)
          endif()
        endif()
      endforeach()
      if("${os}" STREQUAL "linux")
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
  if(NOT SGS_HOST_COMP OR NOT SGS_HOST_COMPVERS)
    if(APPLE)
      find_program(SGS_HOST_C_COMPILER   NAMES clang gcc cc clang icc bcc xlc
                   DOC "Host C compiler")
      find_program(SGS_HOST_CXX_COMPILER NAMES clang++ c++ g++ clang++ icpc CC aCC bcc xlC
                   DOC "Host C++ compiler")
    else()
      find_program(SGS_HOST_C_COMPILER   NAMES gcc cc clang icc bcc xlc
                   DOC "Host C compiler")
      find_program(SGS_HOST_CXX_COMPILER NAMES c++ g++ clang++ icpc CC aCC bcc xlC
                   DOC "Host C++ compiler")
    endif()
    mark_as_advanced(SGS_HOST_C_COMPILER SGS_HOST_CXX_COMPILER)
    if(SGS_HOST_C_COMPILER MATCHES /gcc)
      if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(compiler clang)
      else()
        set(compiler gcc)
        execute_process(COMMAND ${SGS_HOST_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
        list(LENGTH GCC_VERSION_COMPONENTS GCC_VERSION_COMPONENTS_NB)
        if(GCC_VERSION_COMPONENTS_NB LESS "2")
          execute_process(COMMAND ${SGS_HOST_C_COMPILER} --version OUTPUT_VARIABLE GCC_VERSION)
          string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})  
        endif()
        list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
        list(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)
        set(cvers ${GCC_MAJOR}${GCC_MINOR})
      endif()
    elseif(SGS_HOST_C_COMPILER MATCHES /icc)
      set(compiler icc)
      execute_process(COMMAND ${SGS_HOST_C_COMPILER} -dumpversion OUTPUT_VARIABLE ICC_VERSION)
      string(REGEX MATCHALL "[0-9]+" ICC_VERSION_COMPONENTS ${ICC_VERSION})
      list(GET ICC_VERSION_COMPONENTS 0 ICC_MAJOR)
      list(GET ICC_VERSION_COMPONENTS 1 ICC_MINOR)
      set(cvers ${ICC_MAJOR})
    elseif(SGS_HOST_C_COMPILER MATCHES /clang)
      set(compiler clang)
    else()
      message(WARNING "Unknown host C compiler ${SGS_HOST_C_COMPILER}")
      set(compiler)
      set(cvers)
    endif()

    if("${compiler}" STREQUAL "clang")
      execute_process(COMMAND ${SGS_HOST_C_COMPILER} --version OUTPUT_VARIABLE CLANG_VERSION)
      if(APPLE)
        string(REGEX MATCH "LLVM[ \t]+([0-9]+)[.]([0-9]+)" CLANG_VERSION_COMPONENTS ${CLANG_VERSION})
        set(CLANG_MAJOR ${CMAKE_MATCH_1})
        set(CLANG_MINOR ${CMAKE_MATCH_2})
        if(NOT CLANG_MAJOR)
          string(REGEX MATCHALL "[0-9]+" CLANG_VERSION_COMPONENTS ${CLANG_VERSION})
          list(GET CLANG_VERSION_COMPONENTS 0 CLANG_MAJOR)
          list(GET CLANG_VERSION_COMPONENTS 1 CLANG_MINOR)
          set(compiler "llvm")
        endif()
      else()
        string(REGEX MATCHALL "[0-9]+" CLANG_VERSION_COMPONENTS ${CLANG_VERSION})
        list(GET CLANG_VERSION_COMPONENTS 0 CLANG_MAJOR)
        list(GET CLANG_VERSION_COMPONENTS 1 CLANG_MINOR)
      endif()
      set(cvers ${CLANG_MAJOR}${CLANG_MINOR})
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
      if(CMAKE_BUILD_TYPE)
        getShortBuildType(b_type ${CMAKE_BUILD_TYPE})
        set(tag_source BUILD_TYPE)
      else()
        set(b_type o2g)
        set(tag_source default)
      endif()
      set(tag ${SGS_HOST_SYSTEM}-${b_type})
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
    set(SGS_OS     "${CMAKE_MATCH_1}")
    set(SGS_OSVERS "${CMAKE_MATCH_2}")
  else()
    set(SGS_OS     ${os})
    set(SGS_OSVERS "")
  endif()

  set(SGS_SUBOS)
  if(SGS_OS MATCHES "^conda_(.*)")
    set(SGS_SUBOS ${CMAKE_MATCH_1})
  endif()

  if(SGS_SUBOS)
    set(SGS_COREOS ${SGS_SUBOS})
  else()
    set(SGS_COREOS ${SGS_OS})  
  endif()

  if (comp MATCHES "([^0-9.]+)([0-9.]+|max)")
    set(SGS_COMP     ${CMAKE_MATCH_1})
    set(SGS_COMPVERS ${CMAKE_MATCH_2})
  else()
    set(SGS_COMP     ${comp})
    set(SGS_COMPVERS "")
  endif()

  getLongBuildType(type ${SGS_BUILD_TYPE})
  
  set(CMAKE_BUILD_TYPE ${type} CACHE STRING
      "Choose the type of build, options are: empty, Debug, Release, Coverage, Profile, RelWithDebInfo, MinSizeRel." FORCE)

  set(SGS_BUILD_TYPE_SHORT_NAMES o2g opt dbg pro cov min PARENT_SCOPE)

  # architecture
  set(CMAKE_SYSTEM_PROCESSOR ${SGS_ARCH} PARENT_SCOPE)

  # system name
  if("${SGS_COREOS}" STREQUAL "winxp")
    set(CMAKE_SYSTEM_NAME Windows PARENT_SCOPE)
  elseif("${SGS_COREOS}" STREQUAL "mac" OR "${SGS_COREOS}" STREQUAL "osx")
    set(CMAKE_SYSTEM_NAME Darwin PARENT_SCOPE)
  elseif("${SGS_COREOS}" STREQUAL "slc" OR "${SGS_COREOS}" STREQUAL "sl" OR "${SGS_COREOS}" STREQUAL "ub" 
         OR "${SGS_COREOS}" STREQUAL "fc" OR "${SGS_COREOS}" STREQUAL "co" OR "${SGS_COREOS}" STREQUAL "cos" 
         OR "${SGS_COREOS}" STREQUAL "linux")
    set(CMAKE_SYSTEM_NAME Linux PARENT_SCOPE)
  else()
    set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
    message(WARNING "OS ${SGS_COREOS} is not a known platform, assuming it's a ${CMAKE_SYSTEM_NAME}.")
  endif()

  # set default platform ids
  set(SGS_platform ${SGS_SYSTEM}-${SGS_BUILD_TYPE} CACHE STRING "Platform ID for the AA project binaries.")
  set(SGS_system   ${SGS_SYSTEM}-opt               CACHE STRING "Platform ID for the external libraries.")

  mark_as_advanced(SGS_platform SGS_system)

  # Report the platform ids.
  message(STATUS "Target system: ${SGS_TARGET}")
  message(STATUS "Build type: ${SGS_BUILD_TYPE}")

  if(NOT "${SGS_HOST_SYSTEM}" STREQUAL "${SGS_TARGET}")
    message(STATUS "Host system: ${SGS_HOST_SYSTEM}")
  endif()

  if(NOT "${SGS_TARGET}" STREQUAL "${SGS_SYSTEM}")
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

debug_print_var(CMAKE_C_COMPILER)
debug_print_var(CMAKE_CXX_COMPILER)

getCompVersionNumbers(${SGS_COMP} SGS_COMP_MAJOR SGS_COMP_MINOR)
set(SGS_COMP_VERSION "${SGS_COMP_MAJOR}.${SGS_COMP_MINOR}")


## Debug messages.
# foreach(p SGS_HOST_ SGS_)
#  foreach(v ARCH OS OSVERS COMP COMPVERS)
#    message(STATUS "toolchain: ${p}${v} -> ${${p}${v}}")
#  endforeach()
#endforeach()
#message(STATUS "toolchain: SGS_BUILD_TYPE -> ${SGS_BUILD_TYPE}")
#
#message(STATUS "toolchain: CMAKE_HOST_SYSTEM_PROCESSOR -> ${CMAKE_HOST_SYSTEM_PROCESSOR}")
#message(STATUS "toolchain: CMAKE_HOST_SYSTEM_NAME      -> ${CMAKE_HOST_SYSTEM_NAME}")
#message(STATUS "toolchain: CMAKE_HOST_SYSTEM_VERSION   -> ${CMAKE_HOST_SYSTEM_VERSION}")

if("${SGS_COMP}" STREQUAL "icc")
    find_program(CMAKE_C_COMPILER
                 NAMES icc
                 DOC "C compiler")
    find_program(CMAKE_CXX_COMPILER
                 NAMES icpc
                 DOC "C++ compiler")
    find_program(CMAKE_Fortran_COMPILER
                 NAMES ifort
                 DOC "Fortran compiler")
endif()

