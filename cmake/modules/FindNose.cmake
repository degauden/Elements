if (NOT NOSE_FOUND)

    find_package(PythonInterp ${PYTHON_EXPLICIT_VERSION})
    
    set(explicit_nose)
    if(PYTHON_EXPLICIT_VERSION)
      set(explicit_nose nosetests-${PYTHON_EXPLICIT_VERSION})
    endif()
    
    
    if(PYTHONINTERP_FOUND)
        get_filename_component(_python_path ${PYTHON_EXECUTABLE} PATH)
        find_program(NOSE_EXECUTABLE
                     NAMES ${explicit_nose} nosetests
                     HINTS ${_python_path})
    else()
	    find_program(NOSE_EXECUTABLE NAMES ${explicit_nose} nosetests
	                 HINTS ENV NOSE_INSTALL_DIR
                     PATH_SUFFIXES bin)
    endif()

	set(NOSE_EXECUTABLE ${NOSE_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set NOSE_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Nose DEFAULT_MSG NOSE_EXECUTABLE)

    mark_as_advanced(NOSE_FOUND NOSE_EXECUTABLE)


endif (NOT NOSE_FOUND)

