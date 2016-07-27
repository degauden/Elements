/**
 * @file ProgramManager.cpp
 *
 * @date Jan 7, 2015
 * @author Pierre Dubath
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

// there is no version of boost with the std::__debug namespace
// TODO question to Hubert: what is this???
#include "ElementsKernel/NoGlibDebug.h"

#include <cstdlib>                         // for the exit function
#include <fstream>
#include <iostream>
#include <typeinfo>                        // for the typid operator
#include <algorithm>                       // for transform

#include "ElementsKernel/ProgramManager.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

#include "ElementsKernel/Exception.h"
#include "ElementsKernel/Logging.h"
#include "ElementsKernel/System.h"

#include "ElementsKernel/PathSearch.h"
#include "ElementsKernel/Path.h"           // for Path::VARIABLE, multiPathAppend, PATH_SEP
#include "ElementsKernel/ModuleInfo.h"     // for getExecutablePath
#include "ElementsKernel/Unused.h"         // for ELEMENTS_UNUSED

using namespace std;

namespace Elements {

const boost::filesystem::path& ProgramManager::getProgramPath() const {
  return m_program_path;
}

const boost::filesystem::path& ProgramManager::getProgramName() const {
  return m_program_name;
}


/**
 * @brief Get default config file
 * @todo check whether priotities are correct if more than one
 * config file is found in pathSearchInEnvVariable
 * */
const fs::path ProgramManager::getDefaultConfigFile(const fs::path & program_name) {

  fs::path default_config_file{};
  // .conf is the standard extension for configuration file
  fs::path conf_name(program_name);
  conf_name.replace_extension("conf");
  // Construct and return the full path
  vector<fs::path> configFile = pathSearchInEnvVariable(conf_name.string(), CONF_ENV_VAR_NAME);
  if (configFile.size() != 0) {
    default_config_file = configFile.at(0);
  }
  return default_config_file;
}

const fs::path ProgramManager::setProgramName(ELEMENTS_UNUSED char* arg0) {

  fs::path full_path = Elements::System::getExecutablePath();

  return full_path.filename();
}

const fs::path ProgramManager::setProgramPath(ELEMENTS_UNUSED char* arg0) {

  fs::path full_path = Elements::System::getExecutablePath();

  return full_path.parent_path();
}

/*
 * Get program options
 */
const po::variables_map ProgramManager::getProgramOptions(
    int argc, char* argv[]) {

  po::variables_map variables_map { };

  // default value for default_log_level option
  string default_log_level = "INFO";

  // Get defaults
  fs::path default_config_file = getDefaultConfigFile(getProgramName());

  // Define the options which can be given only at the command line
  po::options_description cmd_only_generic_options {};
  cmd_only_generic_options.add_options()
      ("version", "Print version string")
      ("help", "Produce help message")
      ("config-file",
          po::value<fs::path>()->default_value(default_config_file),
          "Name of a configuration file");

  // Define the options which can be given both at command line and conf file
  po::options_description cmd_and_file_generic_options {};
  cmd_and_file_generic_options.add_options()
      ("log-level", po::value<string>()->default_value(default_log_level),
         "Log level: FATAL, ERROR, WARN, INFO (default), DEBUG")
      ("log-file",
         po::value<fs::path>(), "Name of a log file");

  // Group all the generic options, for help output. Note that we add the options
  // one by one to avoid having empty lines between the groups
  po::options_description all_generic_options {"Generic options"};
  for (auto o : cmd_only_generic_options.options()) {
    all_generic_options.add(o);
  }
  for (auto o : cmd_and_file_generic_options.options()) {
    all_generic_options.add(o);
  }

  // Get the definition of the specific options and arguments(positional options)
  // from the derived class
  auto specific_options = m_program_ptr->defineSpecificProgramOptions();
  auto program_arguments = m_program_ptr->defineProgramArguments();
  po::options_description all_specific_options {};
  all_specific_options.add(specific_options)
                      .add(program_arguments.first);

  // Put together all the options to parse from the cmd line and the file
  po::options_description all_cmd_and_file_options {};
  all_cmd_and_file_options.add(cmd_and_file_generic_options)
                          .add(all_specific_options);

  // Put together all the options to use for the help message
  po::options_description help_options {};
  help_options.add(all_generic_options).add(all_specific_options);

  // Perform a first parsing of the command line, to handle the cmd only options
  auto cmd_parsed_options = po::command_line_parser(argc, argv)
                                    .options(cmd_only_generic_options)
                                    .allow_unregistered().run();
  po::store(cmd_parsed_options, variables_map);

  // Deal with the "help" option
  if (variables_map.count("help")) {
    cout << help_options << endl;
    exit(0);
  }

  // Deal with the "version" option
  if (variables_map.count("version")) {
    cout << getVersion() << endl;
    exit(0);
  }

  // Get the configuration file. It is guaranteed to exist, because it has default value
  auto config_file = variables_map.at("config-file").as<fs::path>();

  // Parse from the command line the rest of the options. Here we also handle
  // the positional arguments.
  auto leftover_cmd_options = po::collect_unrecognized(cmd_parsed_options.options,
                                                       po::include_positional);
  po::store(po::command_line_parser(leftover_cmd_options)
                      .options(all_cmd_and_file_options)
                      .positional(program_arguments.second)
                      .run(),
            variables_map);

  // Parse from the configuration file if it exists
  if (not config_file.empty() && fs::exists(config_file)) {
    ifstream ifs {config_file.string()};
    if (ifs) {
      po::store(po::parse_config_file(ifs, all_cmd_and_file_options), variables_map);
    }
  }

  // After parsing both the command line and the conf file notify the variables
  // map, so we can get any messages for missing parameters
  po::notify(variables_map);

  // return the variable_map loaded with all options
  return variables_map;
}

void ProgramManager::logHeader(string program_name) const {

  Logging logger = Logging::getLogger("ElementsProgram");

  logger.info() << "##########################################################";
  logger.info() << "##########################################################";
  logger.info() << "#";
  logger.info() << "#  C++ program:  " <<  program_name << " starts ";
  logger.info() << "#";
  logger.debug() << "# The Program Name: " << m_program_name.string();
  logger.debug() << "# The Program Path: " << m_program_path.string();
}

void ProgramManager::logFooter(string program_name) const {

  Logging logger = Logging::getLogger("ElementsProgram");

  logger.info() << "##########################################################";
  logger.info() << "#";
  logger.info() << "#  C++ program:  " << program_name << " stops ";
  logger.info() << "#";
  logger.info() << "##########################################################";
  logger.info() << "##########################################################";
}


// Log all options with a header
void ProgramManager::logAllOptions() const {


  Logging logger = Logging::getLogger("ElementsProgram");

  logger.info() << "##########################################################";
  logger.info() << "#";
  logger.info() << "# List of all program options";
  logger.info() << "# ---------------------------";
  logger.info() << "#";

  // Build a log message
  stringstream log_message {};

  // Loop over all options included in the variable_map
  for (const auto& v: m_variables_map) {
    // string option
    if (v.second.value().type() == typeid(string)) {
      log_message << v.first << " = " << v.second.as<string>();
      // double option
    } else if (v.second.value().type() == typeid(double)) {
      log_message << v.first << " = " << v.second.as<double>();
      // int64_t option
    } else if (v.second.value().type() == typeid(int64_t)) {
      log_message << v.first << " = " << v.second.as<int64_t>();
      // int option
    } else if (v.second.value().type() == typeid(int)) {
      log_message << v.first << " = " << v.second.as<int>();
      // fs::path option
    } else if (v.second.value().type() == typeid(fs::path)) {
      log_message << v.first << " = "
          << v.second.as<fs::path>();
      // int vector option
    } else if (v.second.value().type() == typeid(vector<int>)) {
      vector<int> intVec = v.second.as<vector<int>>();
      stringstream vecContent {};
      for (const auto& i: intVec) {
        vecContent << " " << i;
      }
      log_message << v.first << " = {" << vecContent.str() << " }";
      // double vector option
    } else if (v.second.value().type() == typeid(vector<double>)) {
      vector<double> intVec = v.second.as<vector<double>>();
      stringstream vecContent {};
      for (const auto& i: intVec) {
        vecContent << " " << i;
      }
      log_message << v.first << " = {" << vecContent.str() << " }";
      // string vector option
    } else if (v.second.value().type() == typeid(vector<string> )) {
      vector<string> intVec = v.second.as<vector<string>>();
      stringstream vecContent {};
      for (const auto& i: intVec) {
        vecContent << " " << i;
      }
      log_message << v.first << " = {" << vecContent.str() << " }";
      // if nothing else
    } else {
      log_message << "Option " << v.first << " of type "
          << v.second.value().type().name() << " not supported in logging !"
          << endl;
    }
    // write the log message
    logger.info(log_message.str());
    log_message.str("");
  }
  logger.info("#");


}

// Log all options with a header
void ProgramManager::logTheEnvironment() const {

  Logging logger = Logging::getLogger("ElementsProgram");

  logger.debug() << "##########################################################";
  logger.debug() << "#";
  logger.debug() << "# Environment of the Run";
  logger.debug() << "# ---------------------------";
  logger.debug() << "#";

  for(const auto& v: Path::VARIABLE) {
    logger.debug() << v.second << ": " << m_env[v.second];
  }

  logger.debug() << "#";
}

void ProgramManager::bootstrapEnvironment(char* arg0){

  m_program_name = setProgramName(arg0);
  m_program_path = setProgramPath(arg0);

  vector<fs::path> local_search_paths(m_search_dirs.size());

  std::transform(m_search_dirs.cbegin(), m_search_dirs.cend(),
      local_search_paths.begin(),
      [](string s){
      return fs::complete(s);
  });

  // insert local parent dir if it is not already
  // the first one of the list
  const fs::path this_parent_path = fs::canonical(m_program_path.parent_path());
  if (local_search_paths[0] != this_parent_path) {
    auto b = local_search_paths.begin();
    local_search_paths.insert(b, this_parent_path);
  }

  using Path::multiPathAppend;
  using Path::joinPath;
  using Path::Type;

  for(const auto& v: Path::VARIABLE) {
    if (m_env[v.second].exists()) {
      m_env[v.second] += Path::PATH_SEP + joinPath(multiPathAppend(local_search_paths, Path::SUFFIXES[v.first]));
    } else {
      m_env[v.second] = joinPath(multiPathAppend(local_search_paths, Path::SUFFIXES[v.first]));
    }
  }

}

// Get the program options and setup logging
void ProgramManager::setup(int argc, char* argv[]) {

  // store the program name and path in class variable
  // and retrieve the local environment
  bootstrapEnvironment(argv[0]);

  // get all program options into the varaiable_map
  m_variables_map = getProgramOptions(argc, argv);

  // get the program options related to the logging
  string logging_level;
  if (m_variables_map.count("log-level")) {
    logging_level = m_variables_map["log-level"].as<string>();
  } else {
     throw Exception("Required option log-level is not provided!", ExitCode::CONFIG);
  }
  fs::path log_file_name;

  if (m_variables_map.count("log-file")) {
    log_file_name = m_variables_map["log-file"].as<fs::path>();
    Logging::setLogFile(log_file_name);
  }

  // setup the logging
  Logging::setLevel(logging_level);


  logHeader(m_program_name.string());
  // log all program options
  logAllOptions();
  logTheEnvironment();
}

void ProgramManager::tearDown(const ExitCode& c) {

  Logging logger = Logging::getLogger("ElementsProgram");

  logger.debug() << "# Exit Code: " << int(c);

  logFooter(m_program_name.string());
}

// This is the method call from the main which does everything
ExitCode ProgramManager::run(int argc, char* argv[]) {

  setup(argc, argv);

  ExitCode exit_code =  m_program_ptr->mainMethod(m_variables_map);

  tearDown(exit_code);

  return exit_code;

}

string ProgramManager::getVersion() const {

  string version = m_parent_project_name + " " + m_parent_project_version;

  return version;
}

ProgramManager::~ProgramManager() {}

void ProgramManager::onTerminate() noexcept {

  ExitCode exit_code {ExitCode::NOT_OK};

  if ( auto exc = std::current_exception() ) {

    Logging logger = Logging::getLogger("ElementsProgram");

    // we have an exception
    try {
      rethrow_exception( exc ); // throw to recognize the type
    } catch (const Exception & exc) {
      logger.fatal() << "# ";
      logger.fatal() << "# Elements Exception : " << exc.what();
      logger.fatal() << "# ";
      exit_code = exc.exitCode();
    } catch (const exception & exc) {
      /// @todo : set the exit code according to the type of exception
      ///         if a clear match is found.
      logger.fatal() << "# ";
      logger.fatal() << "# Standard Exception : " << exc.what();
      logger.fatal() << "# ";
    } catch (...) {
      logger.fatal() << "# ";
      logger.fatal() << "# An exception of unknown type occured, "
                     << "i.e., an exception not deriving from std::exception ";
      logger.fatal() << "# ";
    }

    logger.fatal() << "This is the back trace:";
    for (auto level: System::backTrace(21, 4)) {
      logger.fatal() << level;
    }
    abort();

  }

  std::_Exit(static_cast<int>(exit_code));

}

} // namespace Elements
