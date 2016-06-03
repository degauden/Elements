/**
 * @file ProgramExampleWithArguments.cpp
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

#include <utility>                           // for pair, make_pair
#include <iostream>                          // for coud, endl
#include <map>                               // for map
#include <string>                            // for string
#include <vector>                            // for vector

#include "ElementsKernel/ProgramHeaders.h"
#include "ElementsKernel/Unused.h"

namespace po = boost::program_options;

//using namespace std;
using std::map;
using std::string;
using std::vector;

namespace Elements {
namespace Examples {

/**
 * @class ElementsProgramExample
 * @brief
 *    Example of an Elements program
 * @details
 *    This class is an example of a program based on the ElementsProgram class. It can be copied/pasted
 *    conveniently to write a new program.
 */
class ProgramExampleWithArguments: public Program {

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
  ExitCode mainMethod (ELEMENTS_UNUSED map<string, po::variable_value>& args) {

    // Get logger and log the entry into the mainMethod
    Logging logger = Logging::getLogger();

    logger.info("This Works");

    using std::cout;
    using std::endl;

    cout << "This Works too!" << endl;

    if (args.count("input-files")) {
      vector<string> files = args["input-files"].as<vector<string>>();
      for (string file : files) {
        cout << "Input file " << file << endl;
      }
    }
    return ExitCode::OK;

  }

  std::pair<po::options_description, po::positional_options_description> defineProgramArguments () {
    po::options_description desc("");
    desc.add_options()("input-files", po::value<vector<string>>(), "Input files");

    po::positional_options_description pos_desc;
    pos_desc.add("input-files", -1);

    return std::make_pair(desc, pos_desc);
  }

};

} // namespace ElementsExamples
} // namespace Elements

MAIN_FOR(Elements::Examples::ProgramExampleWithArguments)
