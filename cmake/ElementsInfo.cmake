include(ElementsUtils)

include_guard()

get_property(module_list GLOBAL PROPERTY PROJ_PACKAGE_LIST)

get_property(include_list GLOBAL PROPERTY PROJ_INCLUDE_LIST)
list(REMOVE_DUPLICATES include_list)

get_property(test_list GLOBAL PROPERTY TEST_LIST)


find_file_to_configure(cmake_info.cmake.in
                       FILETYPE "CMake"
                       PATH_SUFFIXES "auxdir/command"
                       OUTPUTDIR "${CMAKE_CURRENT_BINARY_DIR}"
                       OUTPUTNAME cmake_info.cmake
                       PATHS ${CMAKE_MODULE_PATH})
                       
                       
add_custom_target(info
                  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_info.cmake)
