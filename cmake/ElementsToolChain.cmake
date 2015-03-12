if (NOT HAS_ELEMENTS_TOOLCHAIN)



message(STATUS "-----------------------------------------------------------------------")
message(STATUS "                  --- ElementsToolChain ---                            ")
message(STATUS "-----------------------------------------------------------------------")

# Bootstrap the directory containing this very file
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_MODULE_PATH})

# Remove duplicates
list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)


# Loading the ToolChain library macros and functions

INCLUDE(ElementsToolChainMacros)

debug_print_var(CMAKE_CURRENT_LIST_DIR)
debug_print_var(CMAKE_MODULE_PATH)

debug_message(STATUS "This is the CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
debug_message(STATUS "This is the CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")


debug_message(STATUS "-----------------------------------------------------------------------")

set(HAS_ELEMENTS_TOOLCHAIN ON)

endif()