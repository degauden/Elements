if (NOT HAS_ELEMENTS_TOOLCHAIN)

message(STATUS "-----------------------------------------------------------------------")
message(STATUS "                  --- ElementsToolChain ---                            ")
message(STATUS "-----------------------------------------------------------------------")


# Initial bootstrap to locate the Toolchain macros
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_MODULE_PATH})

# Loading the ToolChain library macros and functions
include(ElementsToolChainMacros)


init()
find_projects(projects collected2 ${CMAKE_SOURCE_DIR}/CMakeLists.txt)


set_paths_from_projects(${projects})

debug_print_var(CMAKE_MODULE_PATH)

set(CMAKE_PREFIX_PATH ${CMAKE_MODULE_PATH} CACHE STRING "The internal prefix path")

reset_include_guards()
reset_message_guards()

message(STATUS "-----------------------------------------------------------------------")
message(STATUS "                  --- End of ElementsToolChain ---                     ")
message(STATUS "-----------------------------------------------------------------------")

set(HAS_ELEMENTS_TOOLCHAIN ON)

endif()