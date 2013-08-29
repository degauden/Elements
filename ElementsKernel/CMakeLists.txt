elements_subdir(ElementsKernel 1.0)

find_package(Boost REQUIRED COMPONENTS filesystem thread program_options system regex)
find_package(Log4CPP)

#---Libraries---------------------------------------------------------------
elements_add_library(ElementsKernel src/Lib/*.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost Log4CPP
                  INCLUDE_DIRS Boost Log4CPP
                  PUBLIC_HEADERS ElementsKernel)

#---Tests-------------------------------------------------------------------
elements_add_unit_test(test_DirSearchPath tests/src/test_DirSearchPath.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(test_PathResolver tests/src/test_PathResolver.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(ElementsTime_test tests/src/ElementsTime_test.cpp tests/src/BoostTestMain.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(ElementsTiming_test tests/src/ElementsTiming_test.cpp tests/src/BoostTestMain.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(test_Memory tests/src/test_Memory.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Real_test tests/src/Real_test.cpp tests/src/BoostTestMain.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

elements_add_unit_test(ElementsProgram_test tests/src/ElementsProgram_test.cpp tests/src/BoostTestMain.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Version_test tests/src/Version_test.cpp tests/src/BoostTestMain.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
#-----------------------
# FileLocator_test
elements_add_unit_test(FileLocator_test tests/src/FileLocator_test.cpp tests/src/BoostTestMain.cpp
                       LINK_LIBRARIES ElementsKernel TYPE Boost)


if(ELEMENTS_BUILD_TESTS)
  set_property(TEST ElementsKernel.test_PathResolver PROPERTY WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if(ELEMENTS_HIDE_WARNINGS)
  if(UNIX)
    # This hides warnings from a broken header file in CppUnit, but may
    # hide problems in the actual test source
    set_property(SOURCE tests/src/test_ElementsTime.cpp
                 PROPERTY COMPILE_FLAGS -Wno-unused-parameter)
  endif()
endif()

#---Python and Scripts ------------------------------------------------------------

elements_install_python_modules()
elements_install_scripts()

