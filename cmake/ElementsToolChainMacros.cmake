CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

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