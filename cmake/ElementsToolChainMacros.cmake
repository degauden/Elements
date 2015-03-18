CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

include(ElementsUtils)


macro(preload_local_module_path)

  # Preset the CMAKE_MODULE_PATH from the environment, if not already defined.
  if(NOT CMAKE_MODULE_PATH)
    # Note: this works even if the envirnoment variable is not set.
    file(TO_CMAKE_PATH "$ENV{CMAKE_MODULE_PATH}" CMAKE_MODULE_PATH)
  endif()

  # Automatically add the modules directory provided by the project.
  if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/modules)
      set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
    endif()
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
  endif()

  # Remove duplicates
  list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)

endmacro()


## Initialize common variables.
macro(init)
  preload_local_module_path()
  if(NOT BINARY_TAG)
    include(SGSPlatform)
    sgs_get_target_platform()
  endif()
endmacro()



include(CMakeParseArguments)


## Helper for recursion and ordering of found projects.
function(_internal_find_projects projects_var config_file)
    debug_message(STATUS "processing ${config_file}")
    set(collected_config ${collected_config} ${config_file})
    debug_print_var(collected_config)
    # Extract information from configuration file
    file(READ ${config_file} config_file_data)
    filter_comments(config_file_data)
    # Warning: this regular expression implies that 'gaudi_project' is not on the
    # first line of the file (but it is needed to ensure that it is not commented out)
    string(REGEX MATCH "[ \t]*(elements_project)[ \t]*\\(([^)]+)\\)" match ${config_file_data})
    set(match ${CMAKE_MATCH_2})
#    if(NOT match)
#        message(FATAL_ERROR "${config_file} does not contain elements_project")
#    endif()
    #message(STATUS "find_used_projects: match -> ${match}")
    # (replace space-type chars with spaces)
    string(REGEX REPLACE "[ \t\r\n]+" " " args "${match}")
    separate_arguments(args)
    CMAKE_PARSE_ARGUMENTS(PROJECT "" "" "USE;DATA;DESCRIPTION" ${args})

    # get the project name and add it to the list of used projects
    list(GET PROJECT_UNPARSED_ARGUMENTS 0 proj_name)
    string(TOUPPER ${proj_name} proj_name)
    debug_print_var(proj_name)
    set(projects ${proj_name} ${${projects_var}})

    # define cache variables for this project
    if(NOT ${proj_name}_CONFIG_FILE)
        get_filename_component(${proj_name}_CONFIG_FILE ${config_file} ABSOLUTE CACHE)
    endif()
    get_filename_component(${proj_name}_ROOT_DIR ${${proj_name}_CONFIG_FILE} PATH CACHE)

    # then we look for the used projects
    debug_print_var(PROJECT_USE)
    while(PROJECT_USE)
        # PROJECT_USE format is "<proj1> <vers1> <proj2> <vers2>..."
        # we extract two entries per iteration
        list(GET PROJECT_USE 0 name)
           list(GET PROJECT_USE 1 version)
           list(REMOVE_AT PROJECT_USE 0 1)
           string(TOUPPER ${name} name_upper)
           # look for the configuration file of the project
           find_file(${name_upper}_CONFIG_FILE NAMES CMakeLists.txt
                     PATH_SUFFIXES ${name}/${version}
                                   ${name_upper}/${name_upper}_${version}
                                   ${name}_${version}
                                   ${name})
        # recursion
        if(${name_upper}_CONFIG_FILE)
            # protect against infinit recursion
            list(FIND collected_config ${${name_upper}_CONFIG_FILE} conf_pos)
            if(NOT conf_pos EQUAL -1)
              message(FATAL_ERROR "Infinite recursion detected at project ${name}")
            endif()
            _internal_find_projects(projects ${${name_upper}_CONFIG_FILE} ${name_upper})
        endif()
    endwhile()

    # propagate the full list of projects to the caller
    set(${projects_var} ${projects} PARENT_SCOPE)
endfunction()


## Look for used projects
function(find_projects projects_var config_file)
  set(projects)
  set(collected_config)
  _internal_find_projects(projects ${config_file})
  if(projects)
    list(REMOVE_DUPLICATES projects)
    list(REVERSE projects)
  endif()
  debug_print_var(projects)
  set(${projects_var} ${projects} PARENT_SCOPE)
endfunction()


## Extend search paths with projects and tools
macro(set_paths_from_projects)
    # we need to reverse the list of arguments because we will prepend to the
    # search paths in a loop
    set(_entries ${ARGN})
    if(_entries)
        list(REVERSE _entries)
    endif()

    # prepare the helper variable
    set(_path)
    # and the variable for the extension of the toolchain (e.g. override externals)
    set(_extra_toolchains)
    foreach(_entry ${_entries})
        string(TOUPPER "${_entry}" _entry)
        set(_root ${${_entry}_ROOT_DIR})
        # we add the tool/project directory ...
        set(_path ${_root} ${_path}) # FIXME: is it needed?
        # ... and some optional extra entries
        foreach(_root ${_root}/cmake ${_root}/InstallArea/${BINARY_TAG}/cmake)
            if(EXISTS ${_root})
                set(_path ${_root} ${_path})
            endif()
        endforeach()
        if(EXISTS ${_root}/InstallArea/${BINARY_TAG}/cmake/extra-toolchain.cmake)
          list(APPEND _extra_toolchains ${_root}/InstallArea/${BINARY_TAG}/cmake/extra-toolchain.cmake)
        endif()
    endforeach()

    # set the real search paths variables
    set(CMAKE_MODULE_PATH ${_path} ${CMAKE_MODULE_PATH})
    # include all the toolchain extensions (they should be prepending entries to CMAKE_MODULE_PATH)
    foreach(_extra_toolchain ${_extra_toolchains})
      include(${_extra_toolchain})
    endforeach()
endmacro()

## How to use these functions in a toolchain:
#
# init()
# find_projects(projects ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt)
#
# set_paths_from_projects(${projects})



