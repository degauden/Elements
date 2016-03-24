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

endmacro()


## Initialize common variables.
macro(init)
  preload_toolchain_module_path()
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

    set(collected_config2 ${collected_config2})

    get_project_from_file(${config_file} project_name version_name project_dep_list)

    set(projects ${${projects_var}} ${project_name})

    string(TOUPPER ${project_name} upper_proj_name)

    # define cache variables for this project
    if(NOT ${upper_proj_name}_CONFIG_FILE)
      get_filename_component(${upper_proj_name}_CONFIG_FILE ${config_file} ABSOLUTE CACHE)
    endif()

    list(FIND collected_config2 ${${upper_proj_name}_CONFIG_FILE} conf_pos)
    if(conf_pos EQUAL -1)
      list(APPEND collected_config2 ${${upper_proj_name}_CONFIG_FILE})
    endif()


    get_filename_component(cfg_file ${upper_proj_name}_CONFIG_FILE NAME)

    if(cfg_file STREQUAL "CMakeLists.txt")
      get_filename_component(${upper_proj_name}_ROOT_DIR ${${upper_proj_name}_CONFIG_FILE} PATH CACHE)
    else()
      get_filename_component(root_dir1 ${${upper_proj_name}_CONFIG_FILE} PATH)
      get_filename_component(root_dir2 ${root_dir1} PATH)
      get_filename_component(${upper_proj_name}_ROOT_DIR ${root_dir2} PATH CACHE)
    endif()


    # then we look for the used projects
    while(project_dep_list)
        # project_dep_list format is "<proj1> <vers1> <proj2> <vers2>..."
        # we extract two entries per iteration
        list(GET project_dep_list 0 name)
        list(GET project_dep_list 1 version)
        list(REMOVE_AT project_dep_list 0 1)
        string(TOUPPER ${name} name_upper)
        # look for the configuration file of the project
        FILE(TO_CMAKE_PATH "$ENV{CMAKE_PROJECT_PATH}" path_list)
        set(suffixes)
        get_installed_project_suffixes(${name} ${version} ${BINARY_TAG} ${SGS_SYSTEM} suffixes)
        foreach(pth ${path_list})
          find_file(${name_upper}_CONFIG_FILE ${name}Config.cmake
                    PATHS ${pth}
                    PATH_SUFFIXES ${suffixes}
                    NO_DEFAULT_PATH)
          if(NOT ${name_upper}_CONFIG_FILE)
            # lookup a project without a version subdir
            set(suffixes2)
            get_installed_versionless_project_suffixes(${name} ${BINARY_TAG} ${SGS_SYSTEM} suffixes2)
            find_file(${name_upper}_CONFIG_FILE NAMES ${name}Config.cmake
                      PATH_SUFFIXES ${suffixes2}
                      PATHS ${pth}
                      NO_DEFAULT_PATH)
            # check the internal version
            set(match_found FALSE)
            if(EXISTS ${${name_upper}_CONFIG_FILE})
              check_project_version_from_file(${${name_upper}_CONFIG_FILE} ${name} ${version} match_found)
            endif()


            if(NOT ${match_found})
              unset(${name_upper}_CONFIG_FILE CACHE)
            endif()


          endif()
          if(${name_upper}_CONFIG_FILE)
            # file found, lets get out of the foreach
            break()
          endif()
        endforeach()

        # recursion
        if(${name_upper}_CONFIG_FILE)
            list(FIND collected_config2 ${${name_upper}_CONFIG_FILE} conf_pos)

            if(conf_pos EQUAL -1)
              check_project_version_from_file(${${name_upper}_CONFIG_FILE} ${name} ${version} match_found)
              if(${match_found})
                _internal_find_projects(projects ${${name_upper}_CONFIG_FILE})
              else()
                unset(${name_upper}_CONFIG_FILE)
              endif()
            endif()
        endif()
    endwhile()

    # propagate the full list of projects to the caller
    set(${projects_var} ${projects} PARENT_SCOPE)
    set(collected_config2 ${collected_config2} PARENT_SCOPE)


endfunction()


## Look for used projects
function(find_projects projects_var collected_var config_file)
  set(projects)
  set(collected_config)
  _internal_find_projects(projects ${config_file})
  if(projects)
    list(REMOVE_DUPLICATES projects)
    list(REVERSE projects)
  endif()
  set(${projects_var} ${projects} PARENT_SCOPE)
  set(${collected_var} ${collected_config} PARENT_SCOPE)
endfunction()


## Extend search paths with projects and tools
macro(set_paths_from_projects)

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
            set(_path ${_root} ${_path})
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

    list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)


endmacro()

## How to use these functions in a toolchain:
#
# init()
# find_projects(projects ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt)
#
# set_paths_from_projects(${projects})



