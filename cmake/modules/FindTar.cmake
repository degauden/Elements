if (NOT TAR_FOUND)

	find_program(TAR_EXECUTABLE tar)
	set(TAR_EXECUTABLE ${TAR_EXECUTABLE} CACHE STRING "")

	if (TAR_EXECUTABLE)
		if (VERBOSE)
			message(STATUS "Path: ${TAR_EXECUTABLE}")
		endif (VERBOSE)
	else (TAR_EXECUTABLE)
		message(FATAL_ERROR "tar not found")
	endif (TAR_EXECUTABLE)

# handle the QUIETLY and REQUIRED arguments and set TAR_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(TAR DEFAULT_MSG TAR_EXECUTABLE)

    mark_as_advanced(TAR_FOUND TAR_EXECUTABLE)


endif (NOT TAR_FOUND)

