/**
 * @file ElementsProgramExample.cpp
 * @date Aug 8, 2013
 * @brief implementation of the ElementsProgramExample.h
 * @author Pierre Dubath
 */

/**
 * This macro includes svn tags that are expanded upon any commit. The program version
 * output on screen with the option --version is extracted from these keywords
 *
 * When creating a new ElementsProgram file, naked svn tags should be introduced
 * (they are then expanded with the first commit), and the svn property svn:keywords
 * must be set with the command:
 *
 * svn propset svn:keywords 'Id Revision HeadURL' filename
 *
 * For more information (and examples of naked svn tags) see the documentation
 * of the getVersionFromKeywords() method in the Version.h file.
 */
#define SVN_ID "SVN $Id$"
#define SVN_URL "SVN $HeadURL$"

// there is no version of boost with the std::__debug namespace
#include "ElementsKernel/NoGlibDebug.h"

// System includes
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
// BOOST includes
#include <boost/program_options.hpp>
namespace po = boost::program_options;
// Service includes
#include "ElementsKernel/ElementsLogging.h"
#include "ElementsKernel/ElementsException.h"
// Other includes
#include "ElementsKernel/Program.h"
#include "ElementsExamples/ClassExample.h"
#include "ElementsKernel/Version.h"


using namespace std;

/**
 * @class ElementsProgramExample
 * @brief
 * 		Example of an Elements program
 * @details
 * 		This class is an example of a program based on the ElementsProgram class. It can be copied/pasted
 * 		conveniently to write a new program.
 */
class ElementsProgramExample: public Elements::Program {

public:

  /**
   * @brief Constructor
   */
  ElementsProgramExample() {
  }

  /**
   * @brief Destructor
   */
  virtual ~ElementsProgramExample() {
  }

  /**
   * @brief
   *    Allows to define the (command line and configuration file) options specific to
   *    this program
   * @details
   *    See the ElementsProgram documentation for more details.
   * @return
   *    A BOOST program options_description
   */
  po::options_description defineSpecificProgramOptions() {

    /**
     * Document all program options specific to this program
     */
    po::options_description config_file_options("Configuration options");
    config_file_options.add_options()
    // A example string option
    ("string-value", po::value<string>()->default_value(string { }),
        "A string option")
    // A example long int option
    ("long-long-value", po::value<int64_t>()->default_value(int64_t { }),
        "A long long option")
    // A double option
    ("double-value", po::value<double>()->default_value(double { }),
        "A double option")
    // A string vector option
    ("string-vector",
        po::value<vector<string>>()->multitoken()->default_value(
            vector<string> { }, "Empty"), "A string vector")
    // A int vector option
    ("int-vector",
        po::value<vector<int>>()->multitoken()->default_value(vector<int> { },
            "Empty"), "An int vector")
    // A double vector option
    ("double-vector",
        po::value<vector<double>>()->multitoken()->default_value(
            vector<double> { }, "Empty"), "A double vector");

    return config_file_options;
  }

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
  Elements::ExitCode mainMethod() {

    // Get logger and log the entry into the mainMethod
    ElementsLogging logger = ElementsLogging::getLogger("ElementsProgramExample");

    logger.info("#");
    logger.info(
        "#  Logging from the mainMethod() of the ElementsProgramExample ");
    logger.info("#");

    // Get the map with all program options
    const po::variables_map variables_map = this->getVariablesMap();

    // Retrieve values from the po::variables_map
    // cppcheck-suppress unreadVariable
    string string_value = variables_map["string-value"].as<string>();
    int64_t long_long_value = variables_map["long-long-value"].as<int64_t>();
    double double_value = variables_map["double-value"].as<double>();
    // cppcheck-suppress unreadVariable
    vector<int> int_vector = variables_map["int-vector"].as<vector<int>>();
    // cppcheck-suppress unreadVariable
    vector<string> string_vector = variables_map["string-vector"].as<vector<string>>();

    // creating an instance of ClassExample for later use
    int64_t source_id = long_long_value;
    double ra = 121.123;
    double dec = double_value;
    ClassExample classExample { source_id, ra, dec };

    try {

      // Do something here
      logger.info("#");
      logger.info("#    Calling the summingAndDividing of the ClassExample ");
      logger.info("#");
      // Cast the longLongValue just to get a double to feed the doSomething
      double a_double = static_cast<double>(long_long_value);
      classExample.summingAndDividing(a_double, double_value);

    } catch (const ElementsException & e) {
      logger.info("#");
      logger.info("  In ElementsProgramExample::mainMethod(),");
      logger.info("      an exception: ");
      logger.info("#");
      logger.info("         %s", e.what());
      logger.info("#");
      logger.info("      is caught.");
      logger.info(
          "      Pretending we do not know what to do, it is thrown again");
      logger.info("      (to show what happens when a program crashes)");
      logger.info("#");
      throw ElementsException(e.what());
    }

    // Get the result and log it
    double result = classExample.getResult();
    logger.info("#");
    logger.info("#     The result of is %e ", result);
    logger.info("#");

    /*
     * Here we might later introduce a standard mechanism to persist results
     */

    return Elements::ExitCode::OK;

  }

  /*
   * This is a standard implementation of getVersion()
   * This must be copy/paste in all programs
   */
  string getVersion() {
    return getVersionFromSvnKeywords(SVN_URL, SVN_ID);
  }
};

/*
 * Implementation of a main using a base class macro
 * This must be copy/paste in all programs
 */
MAIN_FOR(ElementsProgramExample)
