if (NOT PYTEST_FOUND)

    find_package(PythonInterp ${PYTHON_EXPLICIT_VERSION})
    
    set(explicit_pytest)
    set(implicit_pytest py.test)

    if(PYTHON_EXPLICIT_VERSION)
      set(explicit_pytest py.test-${PYTHON_EXPLICIT_VERSION})
    endif()
    
    
    if(PYTHONINTERP_FOUND)
        get_filename_component(_python_path ${PYTHON_EXECUTABLE} PATH)
        find_program(PYTEST_EXECUTABLE
                     NAMES ${explicit_pytest} ${implicit_pytest} 
                     HINTS ${_python_path})
    else()
        find_program(PYTEST_EXECUTABLE NAMES ${explicit_pytest} ${implicit_pytest}
                     HINTS ENV PYTEST_INSTALL_DIR
                     PATH_SUFFIXES bin)
    endif()

    set(PYTEST_EXECUTABLE ${PYTEST_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set PYTEST_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(PyTest DEFAULT_MSG PYTEST_EXECUTABLE)

    mark_as_advanced(PYTEST_FOUND PYTEST_EXECUTABLE)


endif (NOT PYTEST_FOUND)

