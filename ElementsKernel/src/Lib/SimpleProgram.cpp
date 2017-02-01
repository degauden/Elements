/**
 * @file SimpleProgram.cpp
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */


#include <iostream>

#include <boost/filesystem/path.hpp>

#include "ElementsKernel/Exit.h"
#include "ElementsKernel/Unused.h"
#include "ElementsKernel/SimpleProgram.h"

using boost::filesystem::path;

namespace Elements {

SimpleProgram::~SimpleProgram() {}

ExitCode SimpleProgram::run(int argc, char** argv) noexcept {

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

  path prog_path {argv[0]};

  m_program_name = prog_path.filename();
  m_program_path = prog_path.parent_path();

  defineOptions();

}

const path& SimpleProgram::getProgramPath() const {
  return m_program_path;
}

const path& SimpleProgram::getProgramName() const {
  return m_program_name;
}

}  // namespace Elements
