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


set(HAS_ELEMENTS_UTILS ON)

endif()