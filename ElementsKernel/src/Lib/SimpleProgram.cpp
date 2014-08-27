/**
 * @file SimpleProgram.cpp
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */


#include <iostream>

#include "ElementsKernel/Exit.h"
#include "ElementsKernel/SimpleProgram.h"

using namespace std;

namespace Elements {

ExitCode Elements::SimpleProgram::run(int /*argc*/ , char** /*argv*/) noexcept {

  ExitCode exit_code {ExitCode::OK};

  try {
    exit_code = main();
  } catch (const exception & e) {
    cerr << "Exception has been thrown : " << e.what() << endl;
    exit_code = ExitCode::NOT_OK;
  } catch (...) {
    cerr << "An unknown exception has been thrown"<< endl;
    exit_code = ExitCode::NOT_OK;
  }

  return exit_code;
}


}  // namespace Elements
