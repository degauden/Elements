elements_subdir(ElementsExamples 1.0)

elements_depends_on_subdirs(ElementsKernel)


find_package(Boost REQUIRED COMPONENTS filesystem thread system program_options unit_test_framework)
find_package(CppUnit)

#===== Libraries ===============================================================
elements_add_library(ElementsExamples src/Lib/*.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost ElementsKernel
                  INCLUDE_DIRS Boost ElementsKernel
                  PUBLIC_HEADERS ElementsExamples)

#===== Executables =============================================================
elements_add_executable(ElementsProgramExample src/Program/ElementsProgramExample.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost ElementsExamples
                  INCLUDE_DIRS Boost ElementsExamples
                  PUBLIC_HEADERS ElementsExamples)

elements_add_executable(ElementsSimpleProgramExample src/Program/SimpleProgramExample.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} ElementsKernel
                  INCLUDE_DIRS ElementsKernel)

#===== Python programs =========================================================
elements_add_python_program(PythonProgramExample ElementsExamples.PythonProgramExample)

#===== Boost tests =============================================================
# Example with automatically generated TestMain.cpp. This is always the case if the TYPE of
# the test is Boost.
elements_add_unit_test(BoostClassExample tests/src/Boost/ClassExample_test.cpp
                       EXECUTABLE BoostClassExample_test
                       LINK_LIBRARIES ElementsExamples TYPE Boost)

# Example with explict TestMain.cpp. Please mind the "unit_test_framework" in the boost components
# from the find_package command. The Boost components have to be also explicitly added to the
# LINK_LIBRARIES argument.
elements_add_unit_test(BoostClassExampleWithMain tests/src/Boost/ClassExample_test.cpp tests/src/Boost/TestMain.cpp
                       EXECUTABLE BoostClassExampleWithMain_test
                       LINK_LIBRARIES ElementsExamples Boost)


elements_add_unit_test(BoostElementsExampleAllTests tests/src/Boost/*_test.cpp
                       LINK_LIBRARIES ElementsExamples TYPE Boost)

#===== CppUnit tests =============================================================

if(CPPUNIT_FOUND)
# CppUnit Type test
elements_add_unit_test(OtherClassExample tests/src/CppUnit/ClassExample_test.cpp
                       EXECUTABLE OtherClassExample_test
                       LINK_LIBRARIES ElementsExamples TYPE CppUnit)

# Test with the CppUnit library. The TestMain.cpp is not generated
elements_add_unit_test(OtherClassExampleWithMain tests/src/CppUnit/ClassExample_test.cpp tests/src/CppUnit/TestMain.cpp
                       EXECUTABLE OtherClassExampleWithMain_test
                       LINK_LIBRARIES ElementsExamples CppUnit)
endif()


if(USE_ODB)
  find_package(ODB)
  if(ODB_FOUND)
#  if(ODB_PGSQL_LIBRARY)
#    ODB_WRAP_PGSQL_BOOST(ODB_SRCS ODB_HDRS tests/src/ODB/Source.h)
    if(ODB_SQLITE_LIBRARY)
      ODB_WRAP_SQLITE_BOOST(ODB_SRCS ODB_HDRS tests/src/ODB/Source.h)
      elements_add_unit_test(ODB tests/src/ODB/*.cpp ${ODB_SRCS}
                             EXECUTABLE test_ODB
                             LINK_LIBRARIES ODB
                             INCLUDE_DIRS ODB tests/src/ODB)
    endif()
  endif()
endif()

#---Python modules, Scripts, conf and aux files ------------------------------------------------------------

elements_install_conf_files()

elements_install_python_modules()

elements_install_scripts()


elements_add_test(ScriptThatSucceeds COMMAND ScriptThatSucceeds_test)
elements_add_test(ScriptThatFails COMMAND ScriptThatFails_test FAILS)
elements_add_test(ScriptThatGivesOutput COMMAND ScriptThatGivesOutput_test PASSREGEX ".*good")
elements_add_test(ScriptThatGivesError COMMAND ScriptThatGivesError_test FAILREGEX ".*bad" FAILS)
elements_add_test(ScriptThatGivesStdError COMMAND ScriptThatGivesStdError_test FAILREGEX ".*worse" FAILS)

