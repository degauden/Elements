# Special toolchain file that looks for the projects used and choose the version
# of the Astrotools toolchain from them.

cmake_minimum_required(VERSION 2.8.5)

include(CMakeParseArguments)

# detect the required Astrotools version
function(guess_astrotools_version var)

  # Note: it works even if the env. var. is not set.
  file(TO_CMAKE_PATH "$ENV{CMAKE_PREFIX_PATH}" sp1)
  file(TO_CMAKE_PATH "$ENV{CMAKE_PROJECT_PATH}" sp2)
  set(projects_search_path ${CMAKE_PREFIX_PATH} ${sp1} ${sp2})

  # extract the list of projects we depend on
  file(READ CMakeLists.txt config_file)
  string(REGEX MATCH "elements_project *\\(([^)]+)\\)" args ${config_file})
  set(args ${CMAKE_MATCH_1})
  # (replace space-type chars with spaces)
  string(REGEX REPLACE "[ \t\r\n]+" " " args "${args}")
  separate_arguments(args)
  CMAKE_PARSE_ARGUMENTS(PROJECT "" "" "USE;DATA" ${args})
  #message(STATUS "${PROJECT_USE}")

  while(PROJECT_USE)
    list(LENGTH PROJECT_USE len)
    if(len LESS 2)
      return() # ignore wrong arguments (for the moment)
    endif()
    list(GET PROJECT_USE 0 other_project)
    list(GET PROJECT_USE 1 other_project_version)
    list(REMOVE_AT PROJECT_USE 0 1)

    string(TOUPPER ${other_project} other_project_upcase)
    set(suffixes ${other_project}
                 ${other_project_upcase}/${other_project_upcase}_${other_project_version}
                 ${other_project}/${other_project_version}
                 ${other_project_upcase})

    #message(STATUS "projects_search_path -> ${projects_search_path}")
    #message(STATUS "suffixes -> ${suffixes}")
    foreach(base ${projects_search_path})
      foreach(suffix ${suffixes})
        file(GLOB configs ${base}/${suffix}/InstallArea/*/${other_project}Config.cmake)
        #message(STATUS "config -> ${configs}")
        foreach(config ${configs})
          file(READ ${config} config_file)
          if(config_file MATCHES "set *\\( *${other_project}_VERSION *${other_project_version} *\\)")
            string(REGEX MATCH "set *\\( *${other_project}_astrotools_version *([^ ]*) *\\)" _ ${config_file})
            set(${var} ${CMAKE_MATCH_1} PARENT_SCOPE)
            message(STATUS "Detected astrotools version ${CMAKE_MATCH_1} in ${base}/${suffix}")
            return()
          endif()
        endforeach()
      endforeach()
    endforeach()
  endwhile()

endfunction()

include(${CMAKE_CURRENT_LIST_DIR}/UseAstroTools.cmake)

macro(inherit_astrotools)
  guess_astrotools_version(astrotools_version)
  if(astrotools_version)
    use_astrotools(${astrotools_version})
  endif()
endmacro()
