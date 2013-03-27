cmake_minimum_required(VERSION 2.8.5)

# Declare the version of Astro Tools we use
# (must be done before including astrotools-common to allow evolution of the
# structure)
set(astrotools_version 1.0)

include(${CMAKE_CURRENT_LIST_DIR}/astrotools-common.cmake)

# please keep alphabetic order and the structure (tabbing).
# it makes it much easier to edit/read this file!


## Application Area Projects
SGS_AA_project(ROOT  5.34.05)
#
## Compilers
SGS_compiler(gcc43 gcc 4.3.6)
SGS_compiler(gcc46 gcc 4.6.2)
SGS_compiler(gcc47 gcc 4.7.2)
SGS_compiler(clang30 clang 3.0)
SGS_compiler(gccmax gcc 4.7.2)
#
## Externals
if(${SGS_COMP}${SGS_COMPVERS} STREQUAL "gccmax" OR SGS_SYSTEM MATCHES "-slc6-")
SGS_external_package(Boost             1.48.0p1                                 )
else()
SGS_external_package(Boost             1.48.0                                   )
endif()
SGS_external_package(cmake             2.8.6                                    )
SGS_external_package(coverage          3.5.2                                    )
SGS_external_package(CppUnit           1.12.1_p1                                )
SGS_external_package(doxygen           1.7.6                                    )
if(NOT APPLE)
SGS_external_package(fftw              3.1.2                     fftw3          )
else()
SGS_external_package(fftw              3.2.2                     fftw3          )
endif()
if(NOT SGS_COMP STREQUAL "gccmax")
SGS_external_package(GCCXML            0.9.0_20110825            gccxml         )
else()
SGS_external_package(GCCXML            0.9.0_20120309            gccxml         )
endif()
SGS_external_package(graphviz          2.28.0                                   )
SGS_external_package(ipython           0.12.1                                   )
if(NOT SGS_SYSTEM MATCHES "-slc6-") # uuid is not distributed with SLC6
SGS_external_package(uuid              1.42                                     )
endif()

## Prepare the search paths according to the versions above
SGS_prepare_paths()
