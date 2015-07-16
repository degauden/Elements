/**
 * @file SimpleProgram.cpp
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */


#include <iostream>

#include <boost/filesystem/path.hpp>

#include "ElementsKernel/Exit.h"
#include "ElementsKernel/Unused.h"
#include "ElementsKernel/SimpleProgram.h"

namespace fs = boost::filesystem;

namespace Elements {

SimpleProgram::~SimpleProgram() {}

ExitCode SimpleProgram::run(int argc , char** argv) noexcept {

  ExitCode exit_code {ExitCode::OK};

  setup(argc, argv);

  using std::cerr;
  using std::endl;

  try {
    exit_code = main();
  } catch (const std::exception & e) {
    cerr << "Exception has been thrown : " << e.what() << endl;
    exit_code = ExitCode::NOT_OK;
  } catch (...) {
    cerr << "An unknown exception has been thrown"<< endl;
    exit_code = ExitCode::NOT_OK;
  }

  return exit_code;
}


void SimpleProgram::setup(ELEMENTS_UNUSED int argc, char** argv) {

  fs::path prog_path {argv[0]};

  m_program_name = prog_path.filename();
  m_program_path = prog_path.parent_path();

  defineOptions();

}

const fs::path& SimpleProgram::getProgramPath() const {
  return m_program_path;
}

const fs::path& SimpleProgram::getProgramName() const {
  return m_program_name;
}

}  // namespace Elements
