if (NOT NOSE_FOUND)

	find_program(NOSE_EXECUTABLE nosetests)
	set(NOSE_EXECUTABLE ${NOSE_EXECUTABLE} CACHE STRING "")

	if (NOSE_EXECUTABLE)
		if (VERBOSE)
			message(STATUS "Path: ${NOSE_EXECUTABLE}")
		endif (VERBOSE)
	else (NOSE_EXECUTABLE)
		message(FATAL_ERROR "nose not found")
	endif (NOSE_EXECUTABLE)

# handle the QUIETLY and REQUIRED arguments and set NOSE_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(NOSE DEFAULT_MSG NOSE_EXECUTABLE)

    mark_as_advanced(NOSE_FOUND NOSE_EXECUTABLE)


endif (NOT NOSE_FOUND)

