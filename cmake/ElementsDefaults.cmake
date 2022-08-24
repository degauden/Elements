#[=======================================================================[.rst:
ElementsDefaults
-----------------

This file set the default values for various CMake cache variables used by Elements itself. The default 
values for a new project are stored in the `ElementsBuildFlags` module.

.. cmake:variable:: ELEMENTS_HIDE_SYMBOLS
   
   Enable explicit symbol visibility on gcc-4. Set to ``ON``

.. cmake:variable:: CXX_SUGGEST_OVERRIDE
   
   Enable the -Wsuggest-override warning. Set to ``ON``


#]=======================================================================]


set(ELEMENTS_HIDE_SYMBOLS ON
    CACHE STRING "Enable explicit symbol visibility on gcc-4"
    FORCE)

set(CXX_SUGGEST_OVERRIDE ON
    CACHE STRING "Enable the -Wsuggest-override warning"
    FORCE)

set(FLOAT_EQUAL_WARNING ON
    CACHE STRING "Enable the -Wfloat-equal warning"
    FORCE)

set(CONVERSION_WARNING ON
    CACHE STRING "Enable the -Wconversion warning"
    FORCE)

set(HIDE_SYSINC_WARNINGS ON
    CACHE STRING "Hide System includes warnings by using -isystem instead of -I"
    FORCE)

# Use only the Doxygen API documentation generation
set(USE_SPHINX_APIDOC ON
    CACHE STRING "Generates the sphinx API documentation"
    FORCE)

set(USE_SPHINX_BREATHE OFF
    CACHE STRING "Use sphinx C++ API Breathe plugin to bind to doxygen"
    FORCE)

set(USE_PYTHON_DOXYGEN OFF
    CACHE STRING "Use Doxygen for the Python Documentation"
    FORCE)

set(EXTRA_SPHINX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md
    CACHE STRING "List of extra doc files for the sphinx generation"
    FORCE)