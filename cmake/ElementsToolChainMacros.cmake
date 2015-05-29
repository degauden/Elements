CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

include(ElementsUtils)

include_guard()

macro(preset_module_path_from_env)

  # Preset the CMAKE_MODULE_PATH from the environment, if not already defined.
  if(NOT CMAKE_MODULE_PATH)
    # Note: this works even if the envirnoment variable is not set.
    file(TO_CMAKE_PATH "$ENV{CMAKE_MODULE_PATH}" CMAKE_MODULE_PATH)
  endif()

endmacro()



macro(preload_toolchain_module_path)

  preset_module_path_from_env()

  # Automatically add the modules directory provided by the project.
  if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules)
      set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/modules ${CMAKE_MODULE_PATH})
    endif()
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_MODULE_PATH})
  endif()

  # Remove duplicates
  list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)

endmacro()



macro(preload_local_module_path)

  preset_module_path_from_env()

  # Automatically add the modules directory provided by the project.
  if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/modules)
      set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
    endif()
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
  endif()

  # Remove duplicates
  list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)
  message(STATUS "------------------============>>>>>>>>>>> This is the CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

endmacro()


## Initialize common variables.
macro(init)
  preload_toolchain_module_path()
#  preload_local_module_path()
  if(NOT BINARY_TAG)
    include(SGSPlatform)
    sgs_get_target_platform()
  endif()
endmacro()



include(CMakeParseArguments)

function(_internal_find_local_project projects_var project_uses_var config_file)

  file(READ ${config_file} config_file_data)
  filter_comments(config_file_data)
  string(REGEX MATCH "[ \t]*(elements_project)[ \t]*\\(([^)]+)\\)" match_use ${config_file_data})
  set(match_use ${CMAKE_MATCH_2})

  if(match_use STREQUAL "")
    message(FATAL_ERROR "${config_file} does not contain elements_project")
  endif()

  string(REGEX REPLACE "[ \t\r\n]+" " " args "${match_use}")
  separate_arguments(args)
  CMAKE_PARSE_ARGUMENTS(PROJECT "" "" "USE;DATA;DESCRIPTION" ${args})

  list(GET PROJECT_UNPARSED_ARGUMENTS 0 proj_name)

  string(TOUPPER ${proj_name} proj_upper_name)

  set(projects ${proj_name} ${${projects_var}})

  if(NOT ${proj_upper_name}_CONFIG_FILE)
    get_filename_component(${proj_upper_name}_CONFIG_FILE ${config_file} ABSOLUTE CACHE)
  endif()
  get_filename_component(${proj_upper_name}_ROOT_DIR ${${proj_upper_name}_CONFIG_FILE} PATH CACHE)

  set(${projects_uses_var} ${PROJECT_USE} PARENT_SCOPE)
  set(${projects_var} ${projects} PARENT_SCOPE)

endfunction()


function(_internal_find_installed_project projects_var project_uses_var config_file)

  file(READ ${config_file} config_file_data)
  filter_comments(config_file_data)
  string(REGEX MATCH "[ \t]*(set[ \t]*\\([ \t]*)([^_])_USES[ \t]+([^)]+)\\)" match_use ${config_file_data})
  set(match_use ${CMAKE_MATCH_3})

  if(match_use STREQUAL "")
    message(FATAL_ERROR "${config_file} does not contain elements_project")
  endif()

  string(REGEX REPLACE "[ \t\r\n;]+" " " args "${match_use}")
  separate_arguments(args)

  set(proj_name ${CMAKE_MATCH_2})
  string(TOUPPER ${proj_name} proj_upper_name)

  set(projects ${proj_name} ${${projects_var}})

  if(NOT ${proj_upper_name}_CONFIG_FILE)
    get_filename_component(${proj_upper_name}_CONFIG_FILE ${config_file} ABSOLUTE CACHE)
  endif()

  get_filename_component(tmp_dir_name ${${proj_upper_name}_CONFIG_FILE} PATH)

  get_filename_component(tmp_dir_name ${tmp_dir_name} PATH)

  get_filename_component(${proj_upper_name}_ROOT_DIR ${tmp_dir_name} PATH CACHE)


  set(${projects_uses_var} ${PROJECT_USE} PARENT_SCOPE)
  set(${projects_var} ${projects} PARENT_SCOPE)
endfunction()


## Helper for recursion and ordering of found projects.
function(_internal_find_projects projects_var config_file)
    debug_message(STATUS "processing ${config_file}")
    set(collected_config ${collected_config} ${config_file})
    debug_print_var(collected_config)
    # Extract information from configuration file
    file(READ ${config_file} config_file_data)
    filter_comments(config_file_data)
    # Warning: this regular expression implies that 'elements_project' is not on the
    # first line of the file (but it is needed to ensure that it is not commented out)
    string(REGEX MATCH "[ \t]*(elements_project)[ \t]*\\(([^)]+)\\)" match_use ${config_file_data})
    set(match_use ${CMAKE_MATCH_2})
    debug_message(STATUS "find_used_projects: match_use -> ${match_use}")
    if(match_use STREQUAL "")
        message(FATAL_ERROR "${config_file} does not contain elements_project")
    endif()
    # (replace space-type chars with spaces)
    string(REGEX REPLACE "[ \t\r\n]+" " " args "${match_use}")
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


## Helper for recursion and ordering of found projects.
function(_internal_find_projects2 projects_var config_file)

    debug_message(STATUS "///////////////////////////////////////////////////////////////////")
    debug_message(STATUS "processing ${config_file}")

#    set(collected_config ${collected_config} ${config_file})

    set(collected_config2 ${collected_config2})


    debug_message(STATUS "Collected Configs 2 ${collected_config2}")


    get_project_from_file(${config_file} project_name version_name project_dep_list)
    debug_print_var(project_name)
    debug_print_var(version_name)
    debug_print_var(project_dep_list)

    set(projects2 ${${projects_var}} ${project_name})

    string(TOUPPER ${project_name} upper_proj_name)
    debug_print_var(upper_proj_name)

    # define cache variables for this project
    if(NOT ${upper_proj_name}_CONFIG_FILE)
      get_filename_component(${upper_proj_name}_CONFIG_FILE ${config_file} ABSOLUTE CACHE)
    endif()

    list(FIND collected_config2 ${${upper_proj_name}_CONFIG_FILE} conf_pos)
    if(conf_pos EQUAL -1)
      list(APPEND collected_config2 ${${upper_proj_name}_CONFIG_FILE})
    endif()

    debug_message(STATUS "Collected Configs 2 ${collected_config2}")



    get_filename_component(cfg_file ${upper_proj_name}_CONFIG_FILE NAME)

    if(cfg_file STREQUAL "CMakeLists.txt")
      get_filename_component(${upper_proj_name}_ROOT_DIR ${${upper_proj_name}_CONFIG_FILE} PATH CACHE)
    else()
      get_filename_component(root_dir1 ${${upper_proj_name}_CONFIG_FILE} PATH)
      get_filename_component(root_dir2 ${root_dir1} PATH)
      get_filename_component(${upper_proj_name}_ROOT_DIR ${root_dir2} PATH CACHE)
    endif()

    debug_print_var(${upper_proj_name}_CONFIG_FILE)
    debug_print_var(${upper_proj_name}_ROOT_DIR)



    # then we look for the used projects
    debug_print_var(project_dep_list)
    while(project_dep_list)
        # project_dep_list format is "<proj1> <vers1> <proj2> <vers2>..."
        # we extract two entries per iteration
        list(GET project_dep_list 0 name)
           list(GET project_dep_list 1 version)
           list(REMOVE_AT project_dep_list 0 1)
           string(TOUPPER ${name} name_upper)
           # look for the configuration file of the project
           find_file(${name_upper}_CONFIG_FILE NAMES ${name}Config.cmake
                     PATH_SUFFIXES ${name}/${version}/InstallArea/${BINARY_TAG}
                                   ${name}/InstallArea/${BINARY_TAG}
                     PATHS ENV CMAKE_PROJECT_PATH
                     NO_DEFAULT_PATH)

        # recursion
        if(${name_upper}_CONFIG_FILE)
            # protect against infinite recursion
            list(FIND collected_config2 ${${name_upper}_CONFIG_FILE} conf_pos)
#            if(NOT conf_pos EQUAL -1)
              # message(FATAL_ERROR "Infinite recursion detected at project ${name}")
#            endif()
            _internal_find_projects2(projects2 ${${name_upper}_CONFIG_FILE})
        endif()
    endwhile()

    debug_message(STATUS "-------->>> Collected Configs 2 ${collected_config2} <<< -------")


    # propagate the full list of projects to the caller
    set(${projects_var} ${projects2} PARENT_SCOPE)
    set(collected_config2 ${collected_config2} PARENT_SCOPE)

    debug_message(STATUS "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")


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


function(find_projects2 projects_var collected_var config_file)
  set(projects2)
  set(collected_config2)
  _internal_find_projects2(projects2 ${config_file})
  if(projects2)
    list(REMOVE_DUPLICATES projects2)
    list(REVERSE projects2)
  endif()
  set(${projects_var} ${projects2} PARENT_SCOPE)
  set(${collected_var} ${collected_config2} PARENT_SCOPE)
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

macro(set_paths_from_projects2)

    # we need to reverse the list of arguments because we will prepend to the
    # search paths in a loop
    set(_entries ${ARGN})

    # prepare the helper variable
    set(_path)
    # and the variable for the extension of the toolchain (e.g. override externals)
    set(_extra_toolchains)
    foreach(_entry ${_entries})
        string(TOUPPER "${_entry}" _entry)
        set(_root ${${_entry}_ROOT_DIR})
        set(_conf ${${_entry}_CONFIG_FILE})

        get_filename_component(cfg_file ${_conf} NAME)
        get_filename_component(_root ${_conf} PATH)

        # we add the tool/project directory ...
        # ... and some optional extra entries
        foreach(_root ${_root}/cmake ${_root}/cmake/modules)
          if(EXISTS ${_root})
            debug_message(STATUS "The ${_root} path will be prepended to the CMAKE_MODULE_PATH")
            set(_path ${_root} ${_path})
          else()
            debug_message(STATUS "The ${_root} path doesn't exist")
          endif()
        endforeach()
        if(EXISTS ${_root}/cmake/extra-toolchain.cmake)
          list(APPEND _extra_toolchains ${_root}/cmake/extra-toolchain.cmake)
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



