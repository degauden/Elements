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
set(USE_SPHINX_APIDOC OFF
    CACHE STRING "Generates the sphinx API documentation"
    FORCE)

set(USE_SPHINX_BREATHE OFF
    CACHE STRING "Use sphinx C++ API Breathe plugin to bind to doxygen"
    FORCE)

set(USE_PYTHON_DOXYGEN OFF
    CACHE STRING "Use Doxygen for the Python Documentation"
    FORCE)

