if (NOT DISTCC_FOUND)

    find_program(DISTCC_EXECUTABLE distcc
                 HINTS ENV DISTCC_INSTALL_DIR
                 PATH_SUFFIXES bin
                 )
    set(DISTCC_EXECUTABLE ${DISTCC_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set DISTCC_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(DistCC DEFAULT_MSG DISTCC_EXECUTABLE)

    mark_as_advanced(DISTCC_FOUND DISTCC_EXECUTABLE)


endif (NOT DISTCC_FOUND)

