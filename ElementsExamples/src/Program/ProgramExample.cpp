/**
 * @file ProgramExample.cpp
 * @date Aug 8, 2013
 * @author Pierre Dubath
 */


#include "ElementsKernel/ProgramHeaders.h"
namespace po = boost::program_options;

#include "ElementsExamples/ClassExample.h"


using namespace std;

namespace Elements {


/**
 * @class ProgramExample
 * @brief
 * 		Example of an Elements program
 * @details
 * 		This class is an example of a program based on the Elements::Program class. It can be copied/pasted
 * 		conveniently to write a new program.
 */
class ProgramExample: public Program {

public:

  /**
   * @brief Constructor
   */
  ProgramExample() {
  }

  /**
   * @brief Destructor
   */
  virtual ~ProgramExample() {
  }

  /**
   * @brief
   *    Allows to define the (command line and configuration file) options specific to
   *    this program
   * @details
   *    See the Elements::Program documentation for more details.
   * @return
   *    A BOOST program options_description
   */
  po::options_description defineSpecificProgramOptions() {

    /**
     * Document all program options specific to this program
     */
    po::options_description config_file_options("Configuration options");
    config_file_options.add_options()
    // A example string option, which is required (must be given in conf file or command line)
    ("string-value", po::value<string>()->required(),
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
  ExitCode mainMethod(map<std::string, po::variable_value>& args) {

    // Get logger and log the entry into the mainMethod
    Logging logger = Logging::getLogger("ElementsProgramExample");

    logger.info("#");
    logger.info(
        "#  Logging from the mainMethod() of the ElementsProgramExample ");
    logger.info("#");

    // Retrieve values from the argumen
    // cppcheck-suppress unreadVariable
    string string_value = args["string-value"].as<string>();
    int64_t long_long_value = args["long-long-value"].as<int64_t>();
    double double_value = args["double-value"].as<double>();
    // cppcheck-suppress unreadVariable
    vector<int> int_vector = args["int-vector"].as<vector<int>>();
    // cppcheck-suppress unreadVariable
    vector<string> string_vector = args["string-vector"].as<vector<string>>();

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

    } catch (const Exception & e) {
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
      throw Exception(e.what());
    }

    // Get the result and log it
    double result = classExample.getResult();
    logger.info("#");
    logger.info("#     The result of is %e ", result);
    logger.info("#");

    /*
     * Here we might later introduce a standard mechanism to persist results
     */

    return ExitCode::OK;

  }

};

} // namespace Elements

/*
 * Implementation of a main using a base class macro
 * This must be copy/paste in all programs
 */
MAIN_FOR(Elements::ProgramExample)



