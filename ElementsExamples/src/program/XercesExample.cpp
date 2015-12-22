/**
 * @file ProgramExample.cpp
 * @date January 6th, 2015
 * @author Pierre Dubath
 */

#include <cstdio>

#include <boost/program_options.hpp>        // for program options from configuration file of command line arguments
#include <boost/format.hpp>                 // for format

#include <xercesc/util/XercesVersion.hpp>

#include "ElementsKernel/ProgramHeaders.h"  // for including all Program/related headers
#include "ElementsKernel/Unused.h"          // for ELEMENTS_UNUSED


namespace Elements {
namespace Examples {

namespace po = boost::program_options;

using namespace std;

class XercesExample: public Elements::Program {

public:


  Elements::ExitCode mainMethod(ELEMENTS_UNUSED map<string, po::variable_value>& args) override {

    Elements::Logging logger = Elements::Logging::getLogger("XercesExample");
    logger.info() << "XercesC version:" << gXercesFullVersionStr;
    logger.info() << "done with test program! ";

    return Elements::ExitCode::OK;

  }

};

} // namespace ElementsExamples
} // namespace Elements


/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::XercesExample)

