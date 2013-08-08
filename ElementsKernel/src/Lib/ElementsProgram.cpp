/**
 * @file ElementsProgram.cpp
 *
 * Created on: Aug 7, 2013
 *     Author: Pierre Dubath
 */

#include <string>
#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "ElementsKernel/ElementsException.h"
#include "ElementsKernel/ElementsLogging.h"
#include "ElementsKernel/ElementsProgram.h"

using namespace std;

/*
 * Get default config file
 * TODO write a more elaborate version of this taking into account
 * the system and teh development location of the default config file
 */
const boost::filesystem::path ElementsProgram::getDefaultConfigFile(
    string programName) const {

  // Start with the file name from argv[0]
  boost::filesystem::path pn(programName);
  // .conf as a standard extension for coinfiguration file
  pn.replace_extension("conf");
  // Get the path from an environment variable TODO change this
  boost::filesystem::path programRootPath = getenv("ELEMENTSEXAMPLESROOT");
  // Construct and return the full path
  return programRootPath / "conf" / pn;
}

/*
 * Get the default log file, i.e., ./"programName".log
 */
const boost::filesystem::path ElementsProgram::getDefaultLogFile(
    string programName) const {
  boost::filesystem::path pn(programName);
  pn.replace_extension("log");
  return "." / pn;
}

/*
 * Get program options
 */
const boost::program_options::variables_map ElementsProgram::getProgramOptions(
    int argc, char* argv[]) {

  // Initialization
  int defaultLogLevel = 400;
  boost::filesystem::path configFile;
  string programName = argv[0];

  // Get defaults
  boost::filesystem::path defaultConfigFile = getDefaultConfigFile(programName);
  boost::filesystem::path defaultLogFile = getDefaultLogFile(programName);

  // Define the Generic options
  po::options_description genericOptions("Generic options");
  genericOptions.add_options()
  //
  ("version", "Print version string")
  //
  ("help", "Produce help message")
  //
  ("config-file",
      po::value<boost::filesystem::path>(&configFile)->default_value(
          defaultConfigFile), "Name of a configuration file")
  //
  ("log-level", po::value<int>()->default_value(defaultLogLevel),
      "Log level: NONE=0, FATAL=100, ERROR=200, WARN=300, INFO=400 (default), DEBUG=500")
  //
  ("log-file",
      po::value<boost::filesystem::path>()->default_value(defaultLogFile),
      "Name of a log file");

  // Get the definition of the specific options from the derived class
  po::options_description configFileOptions =
      this->defineSpecificProgramOptions();

  // Put all options together
  po::options_description allOptions;
  allOptions.add(genericOptions).add(configFileOptions);

  // Parse the command line and store the options in the variable map
  po::store(po::command_line_parser(argc, argv).options(allOptions).run(),
      m_variablesMap);
  po::notify(m_variablesMap);

  // Deal with the "help" option
  if (m_variablesMap.count("help")) {
    cout << allOptions << endl;
    exit(0);
  }

  // Deal with the "version" option
  if (m_variablesMap.count("version")) {
    cout << this->getVersion() << endl;
    exit(0);
  }

  // Open the configuration file
  ifstream ifs(configFile.c_str());
  if (!ifs) {
    stringstream errorBuffer;
    errorBuffer << "Cannot open configuration file: " << configFile << "\n";
    throw ElementsException(errorBuffer.str());
  } else {
    /*
     * Parse the configuration file and put option values into the variable map only
     * if they were not already specified on the command line
     */
    po::store(parse_config_file(ifs, configFileOptions), m_variablesMap);
    po::notify(m_variablesMap);
  }
  // return the variable_map load with all options
  return m_variablesMap;
}

// Log all options with a header
void ElementsProgram::logAllOptions(string programName) {

  ElementsLogging& logger = ElementsLogging::getLogger();

  logger.info("##########################################################");
  logger.info("##########################################################");
  logger.info("#");
  logger.info("#    %s start ", programName.c_str());
  logger.info("#");
  logger.info("##########################################################");
  logger.info("#");
  logger.info("# List of all program options");
  logger.info("# ---------------------------");
  logger.info("#");

  // Build a log message
  stringstream logMessage { };

  // Loop over all options included in the variable_map
  for (po::variables_map::iterator iter = m_variablesMap.begin();
      iter != m_variablesMap.end(); ++iter) {
    // string option
    if (iter->second.value().type() == typeid(string)) {
      logMessage << iter->first << " = " << iter->second.as<string>();
      // double option
    } else if (iter->second.value().type() == typeid(double)) {
      logMessage << iter->first << " = " << iter->second.as<double>();
      // int64_t option
    } else if (iter->second.value().type() == typeid(int64_t)) {
      logMessage << iter->first << " = " << iter->second.as<int64_t>();
      // int option
    } else if (iter->second.value().type() == typeid(int)) {
      logMessage << iter->first << " = " << iter->second.as<int>();
      // boost::filesystem::path option
    } else if (iter->second.value().type() == typeid(boost::filesystem::path)) {
      logMessage << iter->first << " = "
          << iter->second.as<boost::filesystem::path>();
      // int vector option
    } else if (iter->second.value().type() == typeid(vector<int> )) {
      vector<int> intVec = iter->second.as<vector<int>>();
      stringstream vecContent { };
      for (vector<int>::iterator it = intVec.begin(); it != intVec.end();
          ++it) {
        vecContent << " " << *it;
      }
      logMessage << iter->first << " = {" << vecContent.str() << " }";
      // double vector option
    } else if (iter->second.value().type() == typeid(vector<double> )) {
      vector<double> intVec = iter->second.as<vector<double>>();
      stringstream vecContent { };
      for (vector<double>::iterator it = intVec.begin(); it != intVec.end();
          ++it) {
        vecContent << " " << *it;
      }
      logMessage << iter->first << " = {" << vecContent.str() << " }";
      // string vector option
    } else if (iter->second.value().type() == typeid(vector<string> )) {
      vector<string> intVec = iter->second.as<vector<string>>();
      stringstream vecContent { };
      for (vector<string>::iterator it = intVec.begin(); it != intVec.end();
          ++it) {
        vecContent << " " << *it;
      }
      logMessage << iter->first << " = {" << vecContent.str() << " }";
      // if nothing else
    } else {
      logMessage << "Option " << iter->first << " of type "
          << iter->second.value().type().name() << " not supported in logging !"
          << "\n";
    }
    // write the log message
    logger.info(logMessage.str());
    logMessage.str("");
  }
  logger.info("#");

}

// Get the program options and setup logging
void ElementsProgram::setup(int argc, char* argv[]) noexcept {

  // get all program options into the varaiable_map
  m_variablesMap = getProgramOptions(argc, argv);

  // get the program options related to the logging
  ElementsLogging::LoggingLevel loggingLevel =
      (ElementsLogging::LoggingLevel) m_variablesMap["log-level"].as<int>();
  boost::filesystem::path logFileName = m_variablesMap["log-file"].as<
      boost::filesystem::path>();

  // setup the logging
  ElementsLogging::setupLogger(loggingLevel, logFileName);

  // log all program options
  this->logAllOptions(argv[0]);
}

// This is the method call from teh main which does everything
void ElementsProgram::run(int argc, char* argv[]) {
  ElementsLogging& logger = ElementsLogging::getLogger();

  setup(argc, argv);

  try {
    mainMethod();
  } catch (const exception & e) {
    logger.fatal("# ");
    logger.fatal("# Exception : %s ", e.what());
    logger.fatal("# ");
  } catch (...) {
    logger.fatal("# ");
    logger.fatal(
        "# An exception of unknown type occured, i.e., an exception not deriving from std::exception ");
    logger.fatal("# ");
  }
}

