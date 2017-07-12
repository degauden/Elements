include_guard()

include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

include(SGSPlatform)

macro(check_and_use_cxx_option opt var)
    check_cxx_compiler_flag(${opt} ${var})
    if(${var})
      message(STATUS "   C++ uses \"${opt}\"")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${opt}"
          CACHE STRING "Flags used by the compiler during all build types."
          FORCE)
    endif()
endmacro()


macro(check_and_use_c_option opt var)
    check_c_compiler_flag(${opt} ${var})
    if(${var})
      message(STATUS "   C uses \"${opt}\"")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${opt}"
          CACHE STRING "Flags used by the compiler during all build types."
          FORCE)
    endif()
endmacro()


if((SGS_COMP STREQUAL "clang") OR (SGS_COMP STREQUAL "llvm"))
  find_package(Clang)
  SET (CMAKE_C_COMPILER    "${CLANG_C_COMPILER}")
  SET (CMAKE_CXX_COMPILER  "${CLANG_CXX_COMPILER}")
  SET (CMAKE_AR            "${LLVM_AR}")
  SET (CMAKE_LINKER        "${LLVM_LINKER}")
  SET (CMAKE_NM            "${LLVM_NM}")
  SET (CMAKE_OBJDUMP       "${LLVM_OBJDUMP}")
  SET (CMAKE_RANLIB        "${LLVM_RANLIB}")
endif()


if(NOT BUILD_PREFIX_NAME)
  set(BUILD_PREFIX_NAME "build" CACHE STRING "Prefix name for the build directory" FORCE)
endif()
message(STATUS "The build prefix is set to ${BUILD_PREFIX_NAME}")

if(NOT PYTHON_EXPLICIT_VERSION)
  set(PYTHON_EXPLICIT_VERSION "" CACHE STRING "Set the explicit python version to be used" FORCE)
endif()

if(NOT "${PYTHON_EXPLICIT_VERSION}" STREQUAL "")
  set_property(GLOBAL APPEND PROPERTY CMAKE_EXTRA_FLAGS "-DPYTHON_EXPLICIT_VERSION=${PYTHON_EXPLICIT_VERSION}")
endif()

if(NOT BUILD_SUBDIR)
  file(RELATIVE_PATH build_subdir_name ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
  set(BUILD_SUBDIR ${build_subdir_name} CACHE STRING "Relative location for the build" FORCE)
endif()

message(STATUS "The path to the sources is set to ${CMAKE_SOURCE_DIR}")
message(STATUS "The path to the build is set to ${CMAKE_BINARY_DIR}")
message(STATUS "The relative location for the build is set to ${BUILD_SUBDIR}")

set(CYTHON_ANNOTATE OFF
  CACHE BOOL "Create an annotated .html file when compiling *.pyx.")
set(CYTHON_NO_DOCSTRINGS OFF
  CACHE BOOL "Strip docstrings from the compiled module.")
set(CYTHON_FLAGS "" CACHE STRING
  "Extra flags to the cython compiler.")

# Special defaults
if ( (SGS_COMP STREQUAL gcc AND ( (NOT SGS_COMPVERS VERSION_LESS "47") OR (SGS_COMPVERS MATCHES "max") ))
    OR (SGS_COMP STREQUAL clang AND (NOT SGS_COMPVERS VERSION_LESS "30") )
    OR (SGS_COMP STREQUAL llvm))

  # C++11 is enable by default on gcc47 and gcc48
  set(ELEMENTS_CPP11_DEFAULT ON)
elseif(SGS_COMP STREQUAL icc)
  set(ELEMENTS_CPP11_DEFAULT ON)
else()
  set(ELEMENTS_CPP11_DEFAULT OFF)
endif()

set(ELEMENTS_PARALLEL_DEFAULT OFF)

set(ELEMENTS_FORTIFY_DEFAULT ON)


#--- Elements Build Options -------------------------------------------------------
# Build options that map to compile time features
#

option(ELEMENTS_HIDE_SYMBOLS
       "Enable explicit symbol visibility on gcc-4"
       OFF)


option(ELEMENTS_CPP11
       "enable C++11 compilation"
       ${ELEMENTS_CPP11_DEFAULT})

option(ELEMENTS_CPP14
       "Enable C++14 compilation"
       OFF)


option(ELEMENTS_PARALLEL
       "Enable C++11 parallel support with OpenMP"
       ${ELEMENTS_PARALLEL_DEFAULT})

option(ELEMENTS_FORTIFY
       "Enable g++ fortify option"
       ${ELEMENTS_FORTIFY_DEFAULT})

option(USE_LOCAL_INSTALLAREA
       "Use local InstallArea for the Developers"
       OFF)

option(OPT_DEBUG
       "Enable optimisation for the Debug version"
       ON)

option(ELEMENTS_LINKOPT
       "Enable Link Time Optimisation"
       OFF)

option(USE_PYTHON_NOSE
       "Use nose as python test runner"
       OFF)

option(USE_SPHINX
       "Use sphinx documentation generation"
       ON)

option(USE_DOXYGEN
       "Use doxygen documentation generation"
       ON)

option(USE_PYTHON_DOXYGEN
       "Use doxygen documentation generation"
       ON)

option(USE_SPHINX_APIDOC
       "Use sphinx API documentation generation"
       ON)

option(USE_SPHINX_BREATHE
       "Use sphinx C++ API Breathe plugin to bind to doxygen"
       OFF)

option(ELEMENTS_USE_RPATH
       "Use full RPATH for both build and installation"
       ON)

option(HIDE_SYSINC_WARNINGS
       "Hide System includes warnings by using -isystem instead of -I"
       OFF)

option(CXX_SUGGEST_OVERRIDE
       "Enable the -Wsuggest-override warning"
       OFF)

option(FLOAT_EQUAL_WARNING
       "Enable the -Wfloat-equal warning"
       OFF)

option(SQUEEZED_INSTALL
       "Enable the squeezing of the installation into a prefix directory"
       ON)

option(SANITIZE_OPTIONS
       "Activate the Sanitizing options"
       OFF)
       
if(NOT SANITIZE_STYLE)
  set(SANITIZE_STYLE "undefined" CACHE STRING "Style used for the -fsanitize= option" FORCE)
endif()

#--- Compilation Flags ---------------------------------------------------------
if(NOT ELEMENTS_FLAGS_SET)
  #message(STATUS "Setting cached build flags")

    # Common compilation flags
  set(CMAKE_CXX_FLAGS
      "-fmessage-length=0 -pipe -ansi -Wall -Wextra -Werror=return-type -pthread -pedantic -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -Wno-long-long -Wno-unknown-pragmas -fPIC"
      CACHE STRING "Flags used by the compiler during all build types."
      FORCE)
      
  set(CMAKE_C_FLAGS
      "-fmessage-length=0 -pipe -ansi -Wall -Wextra -Werror=return-type -pthread -pedantic -Wwrite-strings -Wpointer-arith -Wno-long-long -Wno-unknown-pragmas -Wno-unused-parameter -fPIC"
      CACHE STRING "Flags used by the compiler during all build types."
      FORCE)


  if(SANITIZE_OPTIONS AND (SGS_COMP STREQUAL gcc))
    check_and_use_cxx_option(-fsanitize=${SANITIZE_STYLE} CXX_HAS_SANITIZE)
    check_and_use_c_option(-fsanitize=${SANITIZE_STYLE} C_HAS_SANITIZE)
  endif()

  if(FLOAT_EQUAL_WARNING)
    check_and_use_cxx_option(-Wfloat-equal CXX_HAS_FLOAT_EQUAL)
    check_and_use_c_option(-Wfloat-equal C_HAS_FLOAT_EQUAL)
  endif()

  if(CXX_SUGGEST_OVERRIDE AND (SGS_COMP STREQUAL gcc))
    check_and_use_cxx_option(-Wsuggest-override CXX_HAS_SUGGEST_OVERRIDE)
  endif()

  # Build type compilation flags (if different from default or unknown to CMake)
  set(CMAKE_CXX_FLAGS_RELEASE "-O2"
      CACHE STRING "Flags used by the compiler during release builds."
      FORCE)
  set(CMAKE_C_FLAGS_RELEASE "-O2"
      CACHE STRING "Flags used by the compiler during release builds."
      FORCE)

  if (ELEMENTS_LINKOPT AND SGS_COMPVERS VERSION_GREATER "47")
    set(CMAKE_CXX_FLAGS_RELEASE "-flto ${CMAKE_CXX_FLAGS_RELEASE}"
        CACHE STRING "Flags used by the compiler during release builds."
        FORCE)
    set(CMAKE_C_FLAGS_RELEASE "-flto ${CMAKE_C_FLAGS_RELEASE}"
        CACHE STRING "Flags used by the compiler during release builds."
        FORCE)
  endif()



  if (SGS_COMPVERS VERSION_GREATER "47")
    # Use -Og with Debug builds in gcc >= 4.8
    set(CMAKE_CXX_FLAGS_DEBUG "-g"
      CACHE STRING "Flags used by the compiler during Debug builds."
      FORCE)
    set(CMAKE_C_FLAGS_DEBUG "-g"
        CACHE STRING "Flags used by the compiler during Debug builds."
        FORCE)
    if(OPT_DEBUG)
      check_cxx_compiler_flag(-Og CXX_HAS_MINUS_OG)
      if(CXX_HAS_MINUS_OG)
        set(CMAKE_CXX_FLAGS_DEBUG "-Og ${CMAKE_CXX_FLAGS_DEBUG}"
            CACHE STRING "Flags used by the compiler during Debug builds."
            FORCE)
      endif()
      check_c_compiler_flag(-Og C_HAS_MINUS_OG)
      if(C_HAS_MINUS_OG)
        set(CMAKE_C_FLAGS_DEBUG "-Og ${CMAKE_C_FLAGS_DEBUG}"
            CACHE STRING "Flags used by the compiler during Debug builds."
            FORCE)
      endif()
    endif()
  endif()


  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g"
      CACHE STRING "Flags used by the compiler during Release with Debug Info builds."
      FORCE)
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g"
      CACHE STRING "Flags used by the compiler during Release with Debug Info builds."
      FORCE)
  if (ELEMENTS_LINKOPT AND SGS_COMPVERS VERSION_GREATER "47")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-flto ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}"
        CACHE STRING "Flags used by the compiler during release builds."
        FORCE)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-flto ${CMAKE_C_FLAGS_RELWITHDEBINFO}"
        CACHE STRING "Flags used by the compiler during release builds."
        FORCE)
  endif()



  if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR CMAKE_BUILD_TYPE STREQUAL "Release")
      add_definitions(-DNDEBUG)
  endif()



  set(CMAKE_CXX_FLAGS_COVERAGE "--coverage"
      CACHE STRING "Flags used by the compiler during coverage builds."
      FORCE)
  set(CMAKE_C_FLAGS_COVERAGE "--coverage"
      CACHE STRING "Flags used by the compiler during coverage builds."
      FORCE)

  # @todo Check why the -D_GLIBCXX_PROFILE cannot be used with Boost.
  set(CMAKE_CXX_FLAGS_PROFILE "-pg"
      CACHE STRING "Flags used by the compiler during profile builds."
      FORCE)
  set(CMAKE_C_FLAGS_PROFILE "-pg"
      CACHE STRING "Flags used by the compiler during profile builds."
      FORCE)

  # The others are already marked as 'advanced' by CMake, these are custom.
  mark_as_advanced(CMAKE_C_FLAGS_COVERAGE CMAKE_CXX_FLAGS_COVERAGE
                   CMAKE_C_FLAGS_PROFILE CMAKE_CXX_FLAGS_PROFILE)


  #--- Link shared flags -------------------------------------------------------
  if (CMAKE_SYSTEM_NAME MATCHES Linux)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--enable-new-dtags -Wl,--as-needed -Wl,--no-undefined  -Wl,-z,max-page-size=0x1000"
        CACHE STRING "Flags used by the linker during the creation of dll's."
        FORCE)
    set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--enable-new-dtags -Wl,--as-needed -Wl,--no-undefined  -Wl,-z,max-page-size=0x1000"
        CACHE STRING "Flags used by the linker during the creation of modules."
        FORCE)
    if(CMAKE_BUILD_TYPE STREQUAL "Profile" AND SGS_COMPVERS VERSION_LESS "50")
      set(CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-new-dtags -Wl,--as-needed ${CMAKE_EXE_LINKER_FLAGS}"
          CACHE STRING "Flags used by the linker during the creation of exe's."
          FORCE)
    else()
      set(CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-new-dtags -Wl,--as-needed -pie ${CMAKE_EXE_LINKER_FLAGS}"
          CACHE STRING "Flags used by the linker during the creation of exe's."
          FORCE)    
    endif()
  endif()

  if(APPLE)
    # special link options for MacOSX
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flat_namespace -undefined dynamic_lookup"
        CACHE STRING "Flags used by the linker during the creation of dll's."
        FORCE)
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -flat_namespace -undefined dynamic_lookup"
        CACHE STRING "Flags used by the linker during the creation of modules."
        FORCE)
  endif()

  # prevent resetting of the flags
  set(ELEMENTS_FLAGS_SET ON
      CACHE INTERNAL "flag to check if the compilation flags have already been set")
endif()


if(UNIX)
  add_definitions(-D_GNU_SOURCE -Df2cFortran)
endif()

if(APPLE)
  # by default, CMake uses the option -bundle for modules, but we need -dynamiclib for them too
  string(REPLACE "-bundle" "-dynamiclib" CMAKE_SHARED_MODULE_CREATE_C_FLAGS "${CMAKE_SHARED_MODULE_CREATE_C_FLAGS}")
  string(REPLACE "-bundle" "-dynamiclib" CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS "${CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS}")
  include_directories(BEFORE SYSTEM /opt/local/include/${SGS_COMP}${SGS_COMPVERS}/c++)
endif()

#--- Special build flags -------------------------------------------------------
if ((ELEMENTS_HIDE_SYMBOLS) AND (SGS_COMP STREQUAL gcc AND ( (NOT SGS_COMPVERS VERSION_LESS "40") OR (SGS_COMPVERS MATCHES "max") )))
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
  add_definitions(-DELEMENTS_HIDE_SYMBOLS)
endif()

if(USE_ODB)
  set(ODB_CXX_EXTRA_FLAGS ""
      CACHE STRING "flag passed to the ODB compiler"
      FORCE)
endif()


if ( ELEMENTS_CPP11 AND ELEMENTS_CPP14)
  message(WARNING "Both -std=c++11 and -std=c++14 are active. Please remove one by setting/unsetting the right option")
endif()


if ( ELEMENTS_CPP11 )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
  if ( APPLE AND ((SGS_COMP STREQUAL "clang") OR (SGS_COMP STREQUAL "llvm") ) )
    check_and_use_cxx_option(-stdlib=libc++ CXX_HAS_MINUS_STDLIB)
  endif()
  if(USE_ODB)
    set(ODB_CXX_EXTRA_FLAGS --std c++11)
  endif()
endif()

if ( ELEMENTS_CPP14 )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11") # this is the latest C standard available
  if ( APPLE AND ((SGS_COMP STREQUAL "clang") OR (SGS_COMP STREQUAL "llvm") ) )
    check_and_use_cxx_option(-stdlib=libc++ CXX_HAS_MINUS_STDLIB)
  endif()
endif()


if ( APPLE AND ( (SGS_COMP STREQUAL "clang") OR (SGS_COMP STREQUAL "llvm")))
  if(DEFINED ENV{MACPORT_LOCATION})
    set(macport_inc "$ENV{MACPORT_LOCATION}/include")
  else()
    set(macport_inc "/opt/local/include")
  endif()
  if(EXISTS ${macport_inc})
    include_directories(SYSTEM ${macport_inc})
    if(SGS_COMP STREQUAL "clang")
      if (SGS_COMPVERS VERSION_GREATER "34")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${macport_inc}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${macport_inc}")
      else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -isystem-prefix -Xclang ${macport_inc}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xclang -isystem-prefix -Xclang  ${macport_inc}")
      endif()
    endif()
    if(SGS_COMP STREQUAL "llvm")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${macport_inc}")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${macport_inc}")
    endif()
  endif()
endif()

if ( ELEMENTS_PARALLEL AND (SGS_COMP STREQUAL gcc AND ( (SGS_COMPVERS VERSION_GREATER "41") OR (SGS_COMPVERS MATCHES "max") )) )
  add_definitions(-D_GLIBCXX_PARALLEL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

if ( ELEMENTS_FORTIFY AND (SGS_COMP STREQUAL gcc AND ( (SGS_COMPVERS VERSION_GREATER "40") OR (SGS_COMPVERS MATCHES "max") )) )
  if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND SGS_COMPVERS VERSION_GREATER "47" AND OPT_DEBUG)
    add_definitions(-D_FORTIFY_SOURCE=2)
  endif()
  if ( (CMAKE_BUILD_TYPE STREQUAL "Release") OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo") OR (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel"))
    add_definitions(-D_FORTIFY_SOURCE=2)
  endif()
endif()


# special case
if(ELEMENTS_HIDE_SYMBOLS AND (comp MATCHES gcc4))
  add_definitions(-DELEMENTS_HIDE_SYMBOLS)
endif()
#

if (SGS_HOST_ARCH AND SGS_ARCH)
  if (SGS_HOST_ARCH STREQUAL x86_64 AND SGS_ARCH STREQUAL i686)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    set(GCCXML_CXX_FLAGS "${GCCXML_CXX_FLAGS} -m32")
  elseif(NOT SGS_HOST_ARCH STREQUAL SGS_ARCH)
    message(FATAL_ERROR "Cannot build for ${SGS_ARCH} on ${SGS_HOST_ARCH}.")
  endif()
endif()

#--- Tuning of warnings --------------------------------------------------------
if(ELEMENTS_HIDE_WARNINGS)
  if( (SGS_COMP MATCHES clang) OR (SGS_COMP MATCHES llvm) )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-overloaded-virtual -Wno-char-subscripts -Wno-unused-parameter")
  elseif(SGS_COMP STREQUAL gcc AND ( (SGS_COMPVERS VERSION_GREATER "42") OR (SGS_COMPVERS MATCHES "max") ))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-empty-body")
  endif()
endif()

if( (SGS_COMP STREQUAL "clang") OR (SGS_COMP STREQUAL "llvm") )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
endif()

#--- Special flags -------------------------------------------------------------
add_definitions(-DBOOST_FILESYSTEM_VERSION=3)

if((SGS_COMP STREQUAL gcc AND ( (SGS_COMPVERS VERSION_GREATER "46") OR (SGS_COMPVERS MATCHES "max") )) OR ELEMENTS_CPP11)
  set(GCCXML_CXX_FLAGS "${GCCXML_CXX_FLAGS} -D__STRICT_ANSI__")
endif()

if(SGS_COMP STREQUAL gcc)
  if(SGS_COMPVERS STREQUAL 43)
    # The -pedantic flag gives problems on GCC 4.3.
    string(REPLACE "-pedantic" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-pedantic" "" CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}")
  elseif(SGS_COMPVERS VERSION_GREATER "47")
    string(REPLACE "-pedantic" "-Wpedantic" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-pedantic" "-Wpedantic" CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}")
  endif()
endif()
