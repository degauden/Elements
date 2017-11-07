if (NOT XSD_FOUND)

    find_program(XSDCXX_EXECUTABLE xsdcxx
                 HINTS ENV XSD_ROOT_DIR XSD_INSTALL_DIR
                 PATH_SUFFIXES bin)
    message(STATUS "This is XSDCXX executable: ${XSDCXX_EXECUTABLE}")

    find_path(XSD_INCLUDE_DIR xsd/cxx/config.hxx
              HINTS ENV XSD_ROOT_DIR XSD_INSTALL_DIR
              PATH_SUFFIXES include)

    set(XSD_INCLUDE_DIRS ${XSD_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set XSD_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xsd DEFAULT_MSG XSDCXX_EXECUTABLE XSD_INCLUDE_DIRS)

    mark_as_advanced(XSD_FOUND XSDCXX_EXECUTABLE XSD_INCLUDE_DIRS)


endif (NOT XSD_FOUND)

