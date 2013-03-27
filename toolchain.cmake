# Special wrapper to load the declared version of the astrotools toolchain.
set(astrotools_version 1.0)

find_file(use_astrotools_module UseAstroTools.cmake
          HINTS ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# this check is needed because the toolchain seem to be called a second time
# without the proper cache
if(use_astrotools_module)
  include(${use_astrotools_module})
  use_astrotools(${astrotools_version})
endif()
