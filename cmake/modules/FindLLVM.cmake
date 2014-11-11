# -*- cmake -*-

if(NOT LLVM_FOUND)

find_program(LLVM_AR
             NAMES llvm-ar
         	 DOC "LLVM ar")

find_program(LLVM_LINKER
             NAMES llvm-link
         	 DOC "LLVM linker")

find_program(LLVM_NM
             NAMES llvm-nm
         	 DOC "LLVM nm")

find_program(LLVM_OBJDUMP
             NAMES llvm-objdump
         	 DOC "LLVM objdump")

find_program(LLVM_RANLIB
             NAMES llvm-ranlib
         	 DOC "LLVM ranlib")

# handle the QUIETLY and REQUIRED arguments and set LLVM_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LLVM  DEFAULT_MSG
                                  LLVM_AR LLVM_LINKER LLVM_NM LLVM_OBJDUMP LLVM_RANLIB)

mark_as_advanced(LLVM_FOUND LLVM_AR LLVM_LINKER LLVM_NM LLVM_OBJDUMP LLVM_RANLIB)

endif()