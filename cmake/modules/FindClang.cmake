# -*- cmake -*-

if(NOT CLANG_FOUND)


find_package(LLVM)

find_program(CLANG_C_COMPILER
             NAMES clang                 
             HINTS ENV CLANG_INSTALL_DIR
             PATH_SUFFIXES bin
             DOC "Clang C Compiler")

find_program(CLANG_CXX_COMPILER
             NAMES clang++
             HINTS ENV CLANG_INSTALL_DIR
             PATH_SUFFIXES bin
             DOC "Clang C++ Compiler")

# handle the QUIETLY and REQUIRED arguments and set CLANG_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Clang  DEFAULT_MSG
                                  CLANG_C_COMPILER CLANG_CXX_COMPILER)
#

  mark_as_advanced(CLANG_FOUND CLANG_C_COMPILER CLANG_CXX_COMPILER)

endif()