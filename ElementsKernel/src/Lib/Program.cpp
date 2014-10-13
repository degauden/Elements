/**
 * @file ElementsProgram.cpp
 *
 * Created on: Aug 7, 2013
 *     Author: Pierre Dubath
 */

// there is no version of boost with the std::__debug namespace
#include "ElementsKernel/NoGlibDebug.h"

#include <string>
#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
namespace fs = boost::filesystem;

#include "ElementsKernel/Exception.h"
#include "ElementsKernel/Logging.h"
#include "ElementsKernel/Exit.h"
#include "ElementsKernel/PathSearch.h"

#include "ElementsKernel/Program.h"

using namespace std;

namespace Elements {
/**
 * @brief Get default config file
 * @todo write a more elaborate version of this taking into account
 * the system and the development location of the default config file
 */
const fs::path ProgramWithConfFile::getDefaultConfigFile(const
    fs::path & program_name) const {

  // .conf as a standard extension for configuration file
  fs::path conf_name(program_name);
  conf_name.replace_extension("conf");
  // Construct and return the full path
  vector<fs::path> configFile = pathSearchInEnvVariable(conf_name.string(), CONF_ENV_VAR_NAME);
  if (configFile.size() == 0) {
    stringstream error_buffer;
        error_buffer << "No config file " << conf_name.string() << " in " << CONF_ENV_VAR_NAME << "\n";
        throw Exception(error_buffer.str(), ExitCode::CONFIG);
  }
  return configFile.at(0);
}

/*
 * Get the default log file, i.e., ./"programName".log
 */
const fs::path ProgramWithConfFile::getDefaultLogFile(const
    fs::path & program_name) const {
  fs::path log_name(program_name);
  log_name.replace_extension("log");
  return getProgramPath() / log_name;
}

const fs::path ProgramWithConfFile::setProgramName(char* argv) const {
  fs::path fullPath(argv);
  return fullPath.filename();
}

const fs::path ProgramWithConfFile::setProgramPath(char* argv) const {
  fs::path fullPath(argv);
  return fullPath.parent_path();
}

/*
 * Get program options
 */
const po::variables_map ProgramWithConfFile::getProgramOptions(
    int argc, char* argv[]) {

  po::variables_map variables_map { };

  // Initialization
  int default_log_level = 400;
  fs::path config_file;

  // Get defaults
  fs::path default_config_file = getDefaultConfigFile(getProgramName());
  fs::path default_log_file = getDefaultLogFile(getProgramName());

  // Define the Generic options
  po::options_description generic_options("Generic options");
  generic_options.add_options()
  //
  ("version", "Print version string")
  //
  ("help", "Produce help message")
  //
  ("config-file",
      po::value<fs::path>(&config_file)->default_value(
          default_config_file), "Name of a configuration file")
  //
  ("log-level", po::value<int>()->default_value(default_log_level),
      "Log level: NONE=0, FATAL=100, ERROR=200, WARN=300, INFO=400 (default), DEBUG=500")
  //
  ("log-file",
      po::value<fs::path>()->default_value(default_log_file),
      "Name of a log file");

  // Get the definition of the specific options from the derived class
  po::options_description config_file_options =
      this->defineSpecificProgramOptions();

  // Put all options together
  po::options_description all_options;
  all_options.add(generic_options).add(config_file_options);

  // Parse the command line and store the options in the variable map
  po::store(po::parse_command_line(argc, argv, all_options),
      variables_map);
  po::notify(variables_map);

  // Deal with the "help" option
  if (variables_map.count("help")) {
    cout << all_options << endl;
    exit(0);
  }

  // Deal with the "version" option
  if (variables_map.count("version")) {
    cout << this->getVersion() << endl;
    exit(0);
  }

  // Open the configuration file
  ifstream ifs(config_file.c_str());
  if (!ifs) {
    stringstream error_buffer;
    error_buffer << "Cannot open configuration file: " << config_file << "\n";
    throw Exception(error_buffer.str(), ExitCode::CONFIG);
  } else {
    /*
     * Parse the configuration file and put option values into the variable map only
     * if they were not already specified on the command line
     */
    po::store(parse_config_file(ifs, config_file_options), variables_map);
    po::notify(variables_map);
  }

  // return the variable_map load with all options
  return variables_map;
}

// Log all options with a header
void ProgramWithConfFile::logAllOptions(string program_name) {

  Logging logger = Logging::getLogger("ElementsProgram");

  logger.info("##########################################################");
  logger.info("##########################################################");
  logger.info("#");
  logger.info("#    %s start ", program_name.c_str());
  logger.info("#");
  logger.info("##########################################################");
  logger.info("#");
  logger.info("# List of all program options");
  logger.info("# ---------------------------");
  logger.info("#");

  // Build a log message
  stringstream log_message { };

  // Loop over all options included in the variable_map
  for (po::variables_map::iterator iter = m_variables_map.begin();
      iter != m_variables_map.end(); ++iter) {
    // string option
    if (iter->second.value().type() == typeid(string)) {
      log_message << iter->first << " = " << iter->second.as<string>();
      // double option
    } else if (iter->second.value().type() == typeid(double)) {
      log_message << iter->first << " = " << iter->second.as<double>();
      // int64_t option
    } else if (iter->second.value().type() == typeid(int64_t)) {
      log_message << iter->first << " = " << iter->second.as<int64_t>();
      // int option
    } else if (iter->second.value().type() == typeid(int)) {
      log_message << iter->first << " = " << iter->second.as<int>();
      // fs::path option
    } else if (iter->second.value().type() == typeid(fs::path)) {
      log_message << iter->first << " = "
          << iter->second.as<fs::path>();
      // int vector option
    } else if (iter->second.value().type() == typeid(vector<int> )) {
      vector<int> intVec = iter->second.as<vector<int>>();
      stringstream vecContent { };
      for (vector<int>::iterator it = intVec.begin(); it != intVec.end();
          ++it) {
        vecContent << " " << *it;
      }
      log_message << iter->first << " = {" << vecContent.str() << " }";
      // double vector option
    } else if (iter->second.value().type() == typeid(vector<double> )) {
      vector<double> intVec = iter->second.as<vector<double>>();
      stringstream vecContent { };
      for (vector<double>::iterator it = intVec.begin(); it != intVec.end();
          ++it) {
        vecContent << " " << *it;
      }
      log_message << iter->first << " = {" << vecContent.str() << " }";
      // string vector option
    } else if (iter->second.value().type() == typeid(vector<string> )) {
      vector<string> intVec = iter->second.as<vector<string>>();
      stringstream vecContent { };
      for (vector<string>::iterator it = intVec.begin(); it != intVec.end();
          ++it) {
        vecContent << " " << *it;
      }
      log_message << iter->first << " = {" << vecContent.str() << " }";
      // if nothing else
    } else {
      log_message << "Option " << iter->first << " of type "
          << iter->second.value().type().name() << " not supported in logging !"
          << "\n";
    }
    // write the log message
    logger.info(log_message.str());
    log_message.str("");
  }
  logger.info("#");

}

// Get the program options and setup logging
void ProgramWithConfFile::setup(int argc, char* argv[]) {

  // store the program name and path in class variable
  m_program_name = setProgramName(argv[0]);
  m_program_path = setProgramPath(argv[0]);

  // get all program options into the varaiable_map
  m_variables_map = getProgramOptions(argc, argv);

  // get the program options related to the logging
  Logging::Level logging_level;
  if (m_variables_map.count("log-level")) {
    logging_level = (Logging::Level) m_variables_map["log-level"].as<int>();
  } else {
     throw Exception("Required option log-level is not provided!", ExitCode::CONFIG);
  }
  fs::path log_file_name;
  if (m_variables_map.count("log-file")) {
    log_file_name = m_variables_map["log-file"].as<fs::path>();
  } else {
     throw Exception("Required option log-file is not provided!", ExitCode::CONFIG);
  }


  // setup the logging
  Logging::setLevel(logging_level);
  Logging::setLogFile(log_file_name);

  // log all program options
  this->logAllOptions(getProgramName().string());
}

// This is the method call from the main which does everything
ExitCode ProgramWithConfFile::run(int argc, char* argv[]) {

  ExitCode exit_code {ExitCode::NOT_OK};

  setup(argc, argv);

  Logging logger = Logging::getLogger("ElementsProgram");

  try {
    exit_code = mainMethod();
  } catch (const Exception & ee) {
    logger.fatal() << "# " ;
    logger.fatal() << "# Elements Exception : " << ee.what();
    logger.fatal() << "# ";
    exit_code = ee.exitCode();
  } catch (const exception & e) {
    logger.fatal() << "# ";
    logger.fatal() << "# Standard Exception : " << e.what() ;
    logger.fatal() << "# ";
  } catch (...) {
    logger.fatal() << "# ";
    logger.fatal() << "# An exception of unknown type occured, "
                   << "i.e., an exception not deriving from std::exception ";
    logger.fatal() << "# ";
  }

  return exit_code ;

}

} // namespace Elements
