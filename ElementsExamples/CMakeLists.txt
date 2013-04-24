elements_subdir(ElementsExamples 1.0)

find_package(Boost REQUIRED COMPONENTS filesystem thread system)

#---Libraries---------------------------------------------------------------
elements_add_library(ElementsExamples src/Lib/*.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost
                  INCLUDE_DIRS Boost
                  PUBLIC_HEADERS ElementsExamples)

elements_add_executable(TheProgram src/main/TheMain.cpp
                  LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost ElementsExamples
                  INCLUDE_DIRS Boost ElementsExamples
                  PUBLIC_HEADERS ElementsExamples)



#---Python and Scripts ------------------------------------------------------------

#elements_install_python_modules()
#elements_install_scripts()

