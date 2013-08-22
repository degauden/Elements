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
namespace fs = boost::filesystem;

#include "ElementsKernel/ElementsException.h"
#include "ElementsKernel/ElementsLogging.h"
#include "ElementsKernel/FileLocator.h"

#include "ElementsKernel/ElementsProgram.h"

using namespace std;

/*
 * Get default config file
 * TODO write a more elaborate version of this taking into account
 * the system and teh development location of the default config file
 */
const fs::path ElementsProgram::getDefaultConfigFile(const
    fs::path & programName) const {
  // .conf as a standard extension for configuration file
  fs::path confName(programName);
  confName.replace_extension("conf");
  // Construct and return the full path
  return searchConfFileInPathVariable(confName.string());
}

/*
 * Get the default log file, i.e., ./"programName".log
 */
const fs::path ElementsProgram::getDefaultLogFile(const
    fs::path & programName) const {
  fs::path logName(programName);
  logName.replace_extension("log");
  return getProgramPath() / logName;
}

const fs::path ElementsProgram::setProgramName(char* argv) const {
  fs::path fullPath(argv);
  return fullPath.filename();
}

const fs::path ElementsProgram::setProgramPath(char* argv) const {
  fs::path fullPath(argv);
  return fullPath.parent_path();
}

/*
 * Get program options
 */
const po::variables_map ElementsProgram::getProgramOptions(
    int argc, char* argv[]) {

  po::variables_map variablesMap { };

  // Initialization
  int defaultLogLevel = 400;
  fs::path configFile;

  // Get defaults
  fs::path defaultConfigFile = getDefaultConfigFile(getProgramName());
  fs::path defaultLogFile = getDefaultLogFile(getProgramName());

  // Define the Generic options
  po::options_description genericOptions("Generic options");
  genericOptions.add_options()
  //
  ("version", "Print version string")
  //
  ("help", "Produce help message")
  //
  ("config-file",
      po::value<fs::path>(&configFile)->default_value(
          defaultConfigFile), "Name of a configuration file")
  //
  ("log-level", po::value<int>()->default_value(defaultLogLevel),
      "Log level: NONE=0, FATAL=100, ERROR=200, WARN=300, INFO=400 (default), DEBUG=500")
  //
  ("log-file",
      po::value<fs::path>()->default_value(defaultLogFile),
      "Name of a log file");

  // Get the definition of the specific options from the derived class
  po::options_description configFileOptions =
      this->defineSpecificProgramOptions();

  // Put all options together
  po::options_description allOptions;
  allOptions.add(genericOptions).add(configFileOptions);

  // Parse the command line and store the options in the variable map
  po::store(po::parse_command_line(argc, argv, allOptions),
      variablesMap);
  po::notify(variablesMap);

  // Deal with the "help" option
  if (variablesMap.count("help")) {
    cout << allOptions << endl;
    exit(0);
  }

  // Deal with the "version" option
  if (variablesMap.count("version")) {
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
    po::store(parse_config_file(ifs, configFileOptions), variablesMap);
    po::notify(variablesMap);
  }

  // return the variable_map load with all options
  return variablesMap;
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
      // fs::path option
    } else if (iter->second.value().type() == typeid(fs::path)) {
      logMessage << iter->first << " = "
          << iter->second.as<fs::path>();
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

  // store the program name and path in class variable
  m_programName = setProgramName(argv[0]);
  m_programPath = setProgramPath(argv[0]);

  // get all program options into the varaiable_map
  m_variablesMap = getProgramOptions(argc, argv);

  // get the program options related to the logging
  ElementsLogging::LoggingLevel loggingLevel;
  if (m_variablesMap.count("log-level")) {
    loggingLevel = (ElementsLogging::LoggingLevel) m_variablesMap["log-level"].as<int>();
  } else {
     throw ElementsException("Required option log-level is not provided!");
  }
  fs::path logFileName;
  if (m_variablesMap.count("log-file")) {
    logFileName = m_variablesMap["log-file"].as<fs::path>();
  } else {
     throw ElementsException("Required option log-file is not provided!");
  }


  // setup the logging
  ElementsLogging::setupLogger(loggingLevel, logFileName);

  // log all program options
  this->logAllOptions(getProgramName().string());
}

// This is the method call from the main which does everything
void ElementsProgram::run(int argc, char* argv[]) {

  setup(argc, argv);

  ElementsLogging& logger = ElementsLogging::getLogger();

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

