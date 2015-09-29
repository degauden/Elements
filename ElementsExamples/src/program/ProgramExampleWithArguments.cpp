/**
 * @file SimpleProgramExample.cpp
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#include <utility> // for pair
#include <iostream>

#include "ElementsKernel/ProgramHeaders.h"
#include "ElementsKernel/Unused.h"

namespace po = boost::program_options;

using namespace std;

namespace Elements {
namespace ElementsExamples {

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
  ExitCode mainMethod (ELEMENTS_UNUSED map<std::string, po::variable_value>& args) {

    // Get logger and log the entry into the mainMethod
    Logging logger = Logging::getLogger();

    logger.info("This Works");

    cout << "This Works too!" << endl;

    if (args.count("input-files")) {
      std::vector<std::string> files = args["input-files"].as<std::vector<std::string>>();
      for (std::string file : files) {
        std::cout << "Input file " << file << std::endl;
      }
    }
    return ExitCode::OK;

  }

  pair<po::options_description, po::positional_options_description> defineProgramArguments () {
    po::options_description desc("");
    desc.add_options()("input-files", po::value<std::vector<std::string>>(), "Input files");

    po::positional_options_description pos_desc;
    pos_desc.add("input-files", -1);

    return make_pair(desc, pos_desc);
  }

};

} // namespace ElementsExamples
} // namespace Elements

MAIN_FOR(Elements::ElementsExamples::ProgramExampleWithArguments)
