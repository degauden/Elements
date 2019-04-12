if (NOT RPM_FOUND)

    find_program(RPM_EXECUTABLE rpm
                 HINTS ENV RPM_INSTALL_DIR
                 PATH_SUFFIXES bin)
    set(RPM_EXECUTABLE ${RPM_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set RPM_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Tar DEFAULT_MSG RPM_EXECUTABLE)

    mark_as_advanced(RPM_FOUND RPM_EXECUTABLE)


endif (NOT RPM_FOUND)

