/**
 * @file ElementsProgramExample.cpp
 *
 * Created on: Aug 8, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */

/**
 * This macro includes svn tags that are expanded upon any commit. The program version
 * output on screen with the option --version is extracted from VERSION
 *
 * When creating a new file, naked svn tag should be introduced, they are then
 * expanded with the first commit.
 */
#define VERSION "Version svn: $Id: ElementsProgramExample.cpp 3110 2013-08-13 12:13:13Z pdubath $ --- $Revision: 3110 $"
#define URL "URL svn: $HeadURL$"

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
#include "ElementsKernel/ElementsProgram.h"
#include "ElementsExamples/ClassExample.h"

using namespace std;

/**
 * @class ElementsProgramExample
 * @brief
 * 		Example of an Elements program
 * @details
 * 		This class is an example of a program based on the ElementsProgram class. It can be copied/pasted
 * 		conveniently to write a new program.
 */
class ElementsProgramExample: public ElementsProgram {

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
    po::options_description configFileOptions("Configuration options");
    configFileOptions.add_options()
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

    return configFileOptions;
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
  void mainMethod() {

    // Get logger and log the entry into the mainMethod
    ElementsLogging& logger = ElementsLogging::getLogger();

    logger.info("#");
    logger.info(
        "#  Logging from the mainMethod() of the ElementsProgramExample ");
    logger.info("#");

    // Get the map with all program options
    const po::variables_map variableMap = this->getVariablesMap();

    // Retrieve values from the po::variables_map
    string stringValue = variableMap["string-value"].as<string>();
    int64_t longLongValue = variableMap["long-long-value"].as<int64_t>();
    double doubleValue = variableMap["double-value"].as<double>();
    vector<int> intVector = variableMap["int-vector"].as<vector<int>>();
    vector<string> stringVector =
        variableMap["string-vector"].as<vector<string>>();

    // creating an instance of ClassExample for later use
    int64_t sourceId = longLongValue;
    double ra = 121.123;
    double dec = doubleValue;
    ClassExample classExample { sourceId, ra, dec };

    try {

      // Do something here
      logger.info("#");
      logger.info("#    Calling the summingAndDividing of the ClassExample ");
      logger.info("#");
      // Cast the longLongValue just to get a double to feed the doSomething
      double aDouble = static_cast<double>(longLongValue);
      classExample.summingAndDividing(aDouble, doubleValue);

    } catch (const ElementsException & e) {
      logger.info("#");
      logger.info("  In ElementsProgramExample::mainMethod(),");
      logger.info("      an exception: ");
      logger.info("#");
      logger.info("         %s", e.what());
      logger.info("#");
      logger.info("      is caught.");
      logger.info("      Pretending we do not know what to do, it is thrown again");
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

  }

  /*
   * Implementation of the getVersion() method from the base class
   * DO NOT edit this!
   */
  GET_VERSION()

};

/*
 * Implementation of a main using a base class macro
 * DO NOT edit this!
 */
BUILD_MAIN_FOR(ElementsProgramExample)
