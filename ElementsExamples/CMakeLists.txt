elements_subdir(ElementsExamples 1.0)

find_package(Boost REQUIRED COMPONENTS filesystem thread system program_options)

#---Libraries---------------------------------------------------------------
elements_add_library(ElementsExamples src/Lib/*.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost ElementsKernel
                  INCLUDE_DIRS Boost ElementsKernel
                  PUBLIC_HEADERS ElementsExamples)

elements_add_executable(TheProgram src/Program/TheMain.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost ElementsExamples
                  INCLUDE_DIRS Boost ElementsExamples
                  PUBLIC_HEADERS ElementsExamples)

elements_add_executable(ElementsProgramExample src/Program/ElementsProgramExampleMain.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost ElementsExamples
                  INCLUDE_DIRS Boost ElementsExamples)
if(USE_ODB)
  find_package(ODB)
  if(ODB_FOUND)
#  if(ODB_PGSQL_LIBRARY)
#    ODB_WRAP_PGSQL_BOOST(ODB_SRCS ODB_HDRS tests/src/ODB/Source.h)
    if(ODB_SQLITE_LIBRARY)
      ODB_WRAP_SQLITE_BOOST(ODB_SRCS ODB_HDRS tests/src/ODB/Source.h)
      elements_add_unit_test(test_ODB tests/src/ODB/*.cpp ${ODB_SRCS}
                           LINK_LIBRARIES ODB
                           INCLUDE_DIRS ODB tests/src/ODB)
    endif()
  endif()
endif()

#---Python and Scripts ------------------------------------------------------------

#elements_install_python_modules()
#elements_install_scripts()

