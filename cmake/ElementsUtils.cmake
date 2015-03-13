if (NOT HAS_ELEMENTS_UTILS)


# Options
option(USE_DEBUG_PRINT
       "make the debug_print_var macro talkative"
       ON)


macro(debug_print_var var)
  if(USE_DEBUG_PRINT)
    message(STATUS "${var} -> ${${var}}")
  endif()
endmacro()


macro(debug_message)
  if(USE_DEBUG_PRINT)
    message(${ARGN})
  endif()
ENDMACRO()

#-------------------------------------------------------------------------------
# elements_expand_sources(<variable> source_pattern1 source_pattern2 ...)
#
# Expand glob patterns for input files to a list of files, first searching in
# ``src`` then in the current directory.
#-------------------------------------------------------------------------------
macro(elements_expand_sources VAR)
  set(${VAR})
  foreach(fp ${ARGN})
    file(GLOB files src/${fp})
    if(files)
      set(${VAR} ${${VAR}} ${files})
    else()
      file(GLOB files ${fp})
      if(files)
        set(${VAR} ${${VAR}} ${files})
      else()
        set(${VAR} ${${VAR}} ${fp})
      endif()
    endif()
  endforeach()
endmacro()

function(filter_comments var)
  # Convert file contents into a CMake list (where each element in the list
  # is one line of the file)
  #
  STRING(REGEX REPLACE ";" "\\\\;" contents2 "${${var}}")
  STRING(REGEX REPLACE "\n" ";" contents2 "${contents2}")
  foreach(__t ${contents2})
    if (NOT "${__t}" MATCHES "^ *#+")
      LIST(APPEND contents3 ${__t})
    endif()
  endforeach()
  STRING(REGEX REPLACE ";" "\n" contents3 "${contents3}")
  STRING(REGEX REPLACE "\\\\;" ";" contents3 "${contents3}")
  set(${var} ${contents3} PARENT_SCOPE)
endfunction()



set(HAS_ELEMENTS_UTILS ON)
endif()
