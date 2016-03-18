if (NOT XSD_FOUND)


    find_program(XSDCXX_EXECUTABLE xsdcxx)

	message(STATUS "This is XSDCXX executable: ${XSDCXX_EXECUTABLE}")

# handle the QUIETLY and REQUIRED arguments and set XSD_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xsd DEFAULT_MSG XSDCXX_EXECUTABLE)

    mark_as_advanced(XSD_FOUND XSDCXX_EXECUTABLE)


endif (NOT XSD_FOUND)

