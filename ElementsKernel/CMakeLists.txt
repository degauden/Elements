elements_subdir(ElementsKernel)

find_package(Boost REQUIRED COMPONENTS filesystem thread program_options system regex)
find_package(Log4CPP)

#---Libraries---------------------------------------------------------------
elements_add_library(ElementsKernel src/Lib/*.cpp
                    LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost Log4CPP
                    INCLUDE_DIRS Boost Log4CPP
                    PUBLIC_HEADERS ElementsKernel)

#---Tests-------------------------------------------------------------------
elements_add_unit_test(Time tests/src/ElementsTime_test.cpp
                       EXECUTABLE ElementsTime_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Timing tests/src/ElementsTiming_test.cpp
                       EXECUTABLE ElementsTiming_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Memory tests/src/Memory_test.cpp
                       EXECUTABLE Memory_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel)
elements_add_unit_test(Real tests/src/Real_test.cpp
                       EXECUTABLE Real_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

elements_add_unit_test(Program tests/src/ElementsProgram_test.cpp
                       EXECUTABLE ElementsProgram_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Version tests/src/Version_test.cpp
                       EXECUTABLE Version_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Logging tests/src/ElementsLogging_test.cpp
                       EXECUTABLE ElementsLogging_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
#-----------------------
# Path_test
elements_add_unit_test(PathSearch tests/src/PathSearch_test.cpp
                       EXECUTABLE PathSearch_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# Temporary_test
elements_add_unit_test(Temporary tests/src/Temporary_test.cpp
                       EXECUTABLE Temporary_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# ThisModule_test
elements_add_unit_test(ThisModule tests/src/ThisModule_test.cpp
                       EXECUTABLE ThisModule_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)



if(ELEMENTS_HIDE_WARNINGS)
  if(UNIX)
    # This hides warnings from a broken header file in CppUnit, but may
    # hide problems in the actual test source
    set_property(SOURCE tests/src/ElementsTime_test.cpp
                 PROPERTY COMPILE_FLAGS -Wno-unused-parameter)
  endif()
endif()

#---Python and Scripts ------------------------------------------------------------

elements_install_python_modules()
elements_install_scripts()

