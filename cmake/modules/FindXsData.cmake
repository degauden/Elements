if (NOT XSDATA_FOUND)

    find_program(XSDATA_EXECUTABLE xsdata
                 HINTS ENV XSDATA_INSTALL_DIR
                 PATH_SUFFIXES bin)
    set(XSDATA_EXECUTABLE ${XSDATA_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set XSDATA_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(XsData DEFAULT_MSG XSDATA_EXECUTABLE)

    mark_as_advanced(XSDATA_FOUND XSDATA_EXECUTABLE)


endif (NOT XSDATA_FOUND)

