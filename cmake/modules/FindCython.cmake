if (NOT CYTHON_FOUND)

    find_package(PythonInterp ${PYTHON_EXPLICIT_VERSION})
    
    set(explicit_cython)
    if(PYTHON_EXPLICIT_VERSION)
      set(explicit_cython cython${PYTHON_EXPLICIT_VERSION})
    endif()
    
    
    if(PYTHONINTERP_FOUND)
        get_filename_component(_python_path ${PYTHON_EXECUTABLE} PATH)
        find_program(CYTHON_EXECUTABLE
                     NAMES ${explicit_cython} cython cython3
                     HINTS ${_python_path})
    else()
	    find_program(CYTHON_EXECUTABLE NAMES ${explicit_cython} cython cython3)
    endif()

	set(CYTHON_EXECUTABLE ${CYTHON_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set CYTHON_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Cython DEFAULT_MSG CYTHON_EXECUTABLE)

    mark_as_advanced(CYTHON_FOUND CYTHON_EXECUTABLE)


endif (NOT CYTHON_FOUND)

