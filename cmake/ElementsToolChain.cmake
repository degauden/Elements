if (NOT HAS_ELEMENTS_TOOLCHAIN)

message(STATUS "-----------------------------------------------------------------------")
message(STATUS "                  --- ElementsToolChain ---                            ")
message(STATUS "-----------------------------------------------------------------------")

if (CMAKE_GENERATOR MATCHES "Ninja")
    file(
        WRITE "${CMAKE_BINARY_DIR}/GNUMakeRulesOverwrite.cmake" 
            "STRING(REPLACE \"-MD\" \"-MMD\" CMAKE_DEPFILE_FLAGS_C \"\${CMAKE_DEPFILE_FLAGS_C}\")\n"
            "STRING(REPLACE \"-MD\" \"-MMD\" CMAKE_DEPFILE_FLAGS_CXX \"\${CMAKE_DEPFILE_FLAGS_CXX}\")\n"
    )
    set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_BINARY_DIR}/GNUMakeRulesOverwrite.cmake" CACHE INTERNAL "")
endif()


# Initial bootstrap to locate the Toolchain macros
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_MODULE_PATH})

# Loading the ToolChain library macros and functions
include(ElementsToolChainMacros)


init()
find_projects(projects collected ${CMAKE_SOURCE_DIR}/CMakeLists.txt)


set_paths_from_projects(${projects})

debug_print_var(CMAKE_MODULE_PATH)

set(CMAKE_PREFIX_PATH ${CMAKE_MODULE_PATH} CACHE STRING "The internal prefix path")

reset_include_guards()
reset_message_guards()

set(USED_CMAKE_GENERATOR "${CMAKE_GENERATOR}" CACHE STRING "Expose CMAKE_GENERATOR" FORCE)
message(STATUS "Generator: ${USED_CMAKE_GENERATOR}")

set(CMAKE_CROSSCOMPILING FALSE)
message(STATUS "-----------------------------------------------------------------------")
message(STATUS "                  --- End of ElementsToolChain ---                     ")
message(STATUS "-----------------------------------------------------------------------")

set(HAS_ELEMENTS_TOOLCHAIN ON)

endif()