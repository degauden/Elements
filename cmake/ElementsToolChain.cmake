if (NOT HAS_ELEMENTS_TOOLCHAIN)



message(STATUS "-----------------------------------------------------------------------")
message(STATUS "                  --- ElementsToolChain ---                            ")
message(STATUS "-----------------------------------------------------------------------")

# Bootstrap the directory containing this very file
if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules)
  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/modules ${CMAKE_MODULE_PATH})
endif()
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_MODULE_PATH})

# Remove duplicates
list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)

# Loading the ToolChain library macros and functions

INCLUDE(ElementsToolChainMacros)

init()
find_projects(projects ${CMAKE_SOURCE_DIR}/CMakeLists.txt)
set_paths_from_projects(${projects})

debug_print_var(CMAKE_CURRENT_LIST_DIR)
debug_print_var(CMAKE_MODULE_PATH)
debug_print_var(CMAKE_SOURCE_DIR)


debug_message(STATUS "-----------------------------------------------------------------------")

set(HAS_ELEMENTS_TOOLCHAIN ON)

endif()