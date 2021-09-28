include(ElementsUtils)

include_guard(GLOBAL)

find_file_to_configure(cmake_uninstall.cmake.in
                       FILETYPE "CMake"
                       PATH_SUFFIXES "auxdir/command"
                       OUTPUTDIR "${CMAKE_CURRENT_BINARY_DIR}"
                       OUTPUTNAME cmake_uninstall.cmake
                       PATHS ${CMAKE_MODULE_PATH})
                       
                       
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)