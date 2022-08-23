/**
 * @file ProgramWithArguments.cpp
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

#include <iostream>  // for cout, endl
#include <map>       // for map
#include <string>    // for string
#include <utility>   // for pair, make_pair
#include <vector>    // for vector

#include <boost/program_options.hpp>  // for program options from configuration file of command line arguments

#include "ElementsKernel/ProgramHeaders.h"
#include "ElementsKernel/Unused.h"

using std::map;
using std::string;
using std::vector;

using boost::program_options::value;

namespace Elements {
namespace Examples {

/**
 * @class ProgramWithArguments
 * @brief
 *    Example of an Elements program
 * @details
 *    This class is an example of a program based on the ElementsProgram class. It can be copied/pasted
 *    conveniently to write a new program.
 */
class ProgramWithArguments : public Program {

public:
  /**
   * @brief
   *    The "main" method.
   * @details
   *    This method is the entry point to the program. In this sense, it is similar to a main
   *    (and it is why it is called mainMethod()). The code below provides only example stuff
   *    which should be replaced by real code in any program.
   *
   *    See the ElementsProgram documentation for more details.
   *
   */
  ExitCode mainMethod(ELEMENTS_UNUSED map<string, VariableValue>& args) override {

    // Get logger and log the entry into the mainMethod
    auto log = Logging::getLogger();

    log.info("This Works");

    using std::cout;
    using std::endl;

    cout << "This Works too!" << endl;

    if (args.count("input-files") > 0) {
      vector<string> files = args["input-files"].as<vector<string>>();
      for (string file : files) {
        cout << "Input file " << file << endl;
      }
    }
    return ExitCode::OK;
  }

  std::pair<OptionsDescription, PositionalOptionsDescription> defineProgramArguments() override {
    OptionsDescription desc{};
    auto               add = desc.add_options();

    add("input-files", value<vector<string>>(), "Input files");

    PositionalOptionsDescription pos_desc;
    pos_desc.add("input-files", -1);

    return std::make_pair(desc, pos_desc);
  }
};

}  // namespace Examples
}  // namespace Elements

MAIN_FOR(Elements::Examples::ProgramWithArguments)
