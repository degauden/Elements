if (NOT HAS_ELEMENTS_TOOLCHAIN)

message(STATUS "-----------------------------------------------------------------------")
message(STATUS "                  --- ElementsToolChain ---                            ")
message(STATUS "-----------------------------------------------------------------------")


# Initial bootstrap to locate the Toolchain macros
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_MODULE_PATH})

# Loading the ToolChain library macros and functions
include(ElementsToolChainMacros)


init()
#find_projects(projects ${CMAKE_SOURCE_DIR}/CMakeLists.txt)
#debug_print_var(projects)
#set_paths_from_projects(${projects})
#
#debug_print_var(CMAKE_MODULE_PATH)


message(STATUS "-----------------------------------------------------------------------")

find_projects2(projects2 collected2 ${CMAKE_SOURCE_DIR}/CMakeLists.txt)
debug_print_var(projects2)
debug_print_var(collected2)

debug_print_var(CMAKE_MODULE_PATH)


set_paths_from_projects2(${projects2})

debug_print_var(CMAKE_MODULE_PATH)


# include(tests/UtilsTests)


reset_include_guards()

message(STATUS "-----------------------------------------------------------------------")
message(STATUS "                  --- End of ElementsToolChain ---                     ")
message(STATUS "-----------------------------------------------------------------------")

set(HAS_ELEMENTS_TOOLCHAIN ON)

endif()