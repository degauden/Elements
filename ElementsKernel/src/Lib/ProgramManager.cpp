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

#include "ElementsKernel/ProgramManager.h"

#include <algorithm>                             // for transform
#include <cstdint>                               // for int64_t
#include <cstdlib>                               // for the exit function
#include <exception>                             // for exception
#include <iostream>                              // for cout
#include <sstream>                               // for stringstream
#include <string>                                // for string
#include <typeinfo>                              // for the typid operator
#include <vector>                                // for vector
#include <fstream>                               // for ifstream

#include <boost/algorithm/string/predicate.hpp>  // for starts_with
#include <boost/filesystem/operations.hpp>       // for filesystem::complete, exists
#include <boost/filesystem/path.hpp>             // for filesystem::path
#include <boost/program_options.hpp>             // for program_options

#include "ElementsKernel/Configuration.h"        // for getConfigurationPath
#include "ElementsKernel/Path.h"                 // for Path::VARIABLE, multiPathAppend, PATH_SEP
#include "ElementsKernel/Exception.h"            // for Exception
#include "ElementsKernel/Exit.h"                 // for ExitCode
#include "ElementsKernel/Logging.h"              // for Logging
#include "ElementsKernel/ModuleInfo.h"           // for getExecutablePath
#include "ElementsKernel/System.h"               // for backTrace
#include "ElementsKernel/Unused.h"               // for ELEMENTS_UNUSED

#include "OptionException.h"                     // local exception for unrecognized options

using std::vector;
using std::string;
using std::move;
using std::endl;
using std::cerr;
using log4cpp::Priority;

using boost::filesystem::path;
using boost::program_options::variables_map;

namespace Elements {

namespace {
  auto log = Logging::getLogger("ElementsProgram");
}

using System::getExecutablePath;

ProgramManager::ProgramManager(std::unique_ptr<Program> program_ptr,
               const string& parent_project_version,
               const string& parent_project_name,
               const string& parent_project_vcs_version,
               const string& parent_module_version,
               const string& parent_module_name,
               const vector<string>& search_dirs,
               const Priority::Value& elements_loglevel):
               m_program_ptr(move(program_ptr)),
               m_parent_project_version(move(parent_project_version)),
               m_parent_project_name(move(parent_project_name)),
               m_parent_project_vcs_version(move(parent_project_vcs_version)),
               m_parent_module_version(move(parent_module_version)),
               m_parent_module_name(move(parent_module_name)),
               m_search_dirs(move(search_dirs)),
               m_env{},
               m_elements_loglevel(move(elements_loglevel)) {

}

const path& ProgramManager::getProgramPath() const {
  return m_program_path;
}

const path& ProgramManager::getProgramName() const {
  return m_program_name;
}


/**
 * @brief Get default config file
 * @todo check whether priotities are correct if more than one
 * config file is found in pathSearchInEnvVariable
 * */
const path ProgramManager::getDefaultConfigFile(const path & program_name,
                                                const string& module_name) {
  path default_config_file{};

  // .conf is the standard extension for configuration file
  path conf_name(program_name);
  conf_name.replace_extension("conf");

  // Construct and return the full path
  default_config_file = getConfigurationPath(conf_name.string(), false);
  if (default_config_file.empty()) {
    log.warn() << "The " << conf_name << " default configuration file cannot be found in:";
    for (auto loc : getConfigurationLocations()) {
      log.warn() << " " << loc;
    }
    if (not module_name.empty()) {
      conf_name = path {module_name} / conf_name;
      log.warn() << "Trying " << conf_name << ".";
      default_config_file = getConfigurationPath(conf_name.string(), false);
    }
  }

  if (default_config_file.empty()) {
    log.debug() << "Couldn't find " << conf_name << " default configuration file.";
  } else {
    log.debug() << "Found " << conf_name << " default configuration file at " << default_config_file;
  }

  return default_config_file;
}

const path ProgramManager::setProgramName(ELEMENTS_UNUSED char* arg0) {

  path full_path = getExecutablePath();

  return full_path.filename();
}

const path ProgramManager::setProgramPath(ELEMENTS_UNUSED char* arg0) {

  path full_path = getExecutablePath();

  return full_path.parent_path();
}

template<class charT>
void ProgramManager::checkCommandLineOptions(
        const boost::program_options::basic_parsed_options<charT>& cmd_parsed_options) {

  for (const auto& o : cmd_parsed_options.options) {
    if (o.string_key == "config-file") {
      if (o.value.size() != 1) {
        cerr << "Wrong usage of the --config-file option" << endl;
        exit(static_cast<int>(ExitCode::USAGE));
      } else {
        auto conf_file = path { o.value[0] };
        if (not boost::filesystem::exists(conf_file)) {
          cerr << "The " << conf_file
               << " configuration file doesn't exist!" << endl;
          exit(static_cast<int>(ExitCode::CONFIG));
        }
      }
    }
  }
}

/*
 * Get program options
 */
const variables_map ProgramManager::getProgramOptions(
    int argc, char* argv[]) {


  using std::cout;
  using std::exit;
  using boost::program_options::options_description;
  using boost::program_options::value;
  using boost::program_options::store;
  using boost::program_options::command_line_parser;
  using boost::program_options::collect_unrecognized;
  using boost::program_options::include_positional;
  using boost::program_options::notify;
  using boost::program_options::parse_config_file;

  variables_map var_map { };

  // default value for default_log_level option
  string default_log_level = "INFO";

  // Get defaults
  path default_config_file = getDefaultConfigFile(getProgramName(),
                                                  m_parent_module_name);

  // Define the options which can be given only at the command line
  options_description cmd_only_generic_options {};
  cmd_only_generic_options.add_options()
      ("version", "Print version string")
      ("help", "Produce help message")
      ("config-file",
          value<path>()->default_value(default_config_file),
          "Name of a configuration file");

  // Define the options which can be given both at command line and conf file
  options_description cmd_and_file_generic_options {};
  cmd_and_file_generic_options.add_options()
      ("log-level", value<string>()->default_value(default_log_level),
         "Log level: FATAL, ERROR, WARN, INFO (default), DEBUG")
      ("log-file",
         value<path>(), "Name of a log file");

  // Group all the generic options, for help output. Note that we add the
  // options one by one to avoid having empty lines between the groups
  options_description all_generic_options {"Generic options"};
  for (auto o : cmd_only_generic_options.options()) {
    all_generic_options.add(o);
  }
  for (auto o : cmd_and_file_generic_options.options()) {
    all_generic_options.add(o);
  }

  // Get the definition of the specific options and arguments (positional
  // options) from the derived class
  auto specific_options = m_program_ptr->defineSpecificProgramOptions();
  auto program_arguments = m_program_ptr->defineProgramArguments();
  options_description all_specific_options {};
  all_specific_options.add(specific_options)
                      .add(program_arguments.first);

  // Put together all the options to parse from the cmd line and the file
  options_description all_cmd_and_file_options {};
  all_cmd_and_file_options.add(cmd_and_file_generic_options)
                          .add(all_specific_options);

  // Put together all the options to use for the help message
  options_description help_options {};
  help_options.add(all_generic_options).add(all_specific_options);

  // Perform a first parsing of the command line, to handle the cmd only options
  auto cmd_parsed_options = command_line_parser(argc, argv)
                                    .options(cmd_only_generic_options)
                                    .allow_unregistered().run();

  checkCommandLineOptions(cmd_parsed_options);

  store(cmd_parsed_options, var_map);

  // Deal with the "help" option
  if (var_map.count("help") > 0) {
    cout << help_options << endl;
    exit(static_cast<int>(ExitCode::OK));
  }

  // Deal with the "version" option
  if (var_map.count("version") > 0) {
    cout << getVersion() << endl;
    exit(static_cast<int>(ExitCode::OK));
  }

  // Get the configuration file. It is guaranteed to exist, because it has
  // default value
  auto config_file = var_map.at("config-file").as<path>();

  // Parse from the command line the rest of the options. Here we also handle
  // the positional arguments.
  auto leftover_cmd_options = collect_unrecognized(cmd_parsed_options.options,
                                                   include_positional);

  try {

    auto parsed_cmdline_options = command_line_parser(leftover_cmd_options)
                           .options(all_cmd_and_file_options)
                           .positional(program_arguments.second)
                           .run();

    store(parsed_cmdline_options, var_map);

    // Parse from the configuration file if it exists
    if (not config_file.empty() and boost::filesystem::exists(config_file)) {
      std::ifstream ifs {config_file.string()};
      if (ifs) {
        auto parsed_cfgfile_options = parse_config_file(ifs,
                                                        all_cmd_and_file_options);
        store(parsed_cfgfile_options, var_map);
      }
    }

  } catch (const std::exception& e) {
    if (boost::starts_with(e.what(), "unrecognised option") or
        boost::starts_with(e.what(), "too many positional options")) {
      throw OptionException(e.what());
    } else {
      throw;
    }
  }
  // After parsing both the command line and the conf file notify the variables
  // map, so we can get any messages for missing parameters
  notify(var_map);

  // return the var_map loaded with all options
  return var_map;
}

void ProgramManager::logHeader(string program_name) const {
  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "#");
  log.log(m_elements_loglevel, "#  C++ program:  " + program_name + " starts ");
  log.log(m_elements_loglevel, "#");
  log.debug("# The Program Name: " + m_program_name.string());
  log.debug("# The Program Path: " + m_program_path.string());
}

void ProgramManager::logFooter(string program_name) const {
  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "#");
  log.log(m_elements_loglevel, "#  C++ program:  " + program_name + " stops ");
  log.log(m_elements_loglevel, "#");
  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "##########################################################");
}


// Log all options with a header
void ProgramManager::logAllOptions() const {

  using std::stringstream;
  using std::int64_t;

  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "#");
  log.log(m_elements_loglevel, "# List of all program options");
  log.log(m_elements_loglevel, "# ---------------------------");
  log.log(m_elements_loglevel, "#");

  // Build a log message
  stringstream log_message {};

  // Loop over all options included in the variable_map
  for (const auto& v : m_variables_map) {
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
      // bool option
    } else if (v.second.value().type() == typeid(bool)) {
      log_message << v.first << " = " << v.second.as<bool>();
      // path option
    } else if (v.second.value().type() == typeid(path)) {
      log_message << v.first << " = "
          << v.second.as<path>();
      // int vector option
    } else if (v.second.value().type() == typeid(vector<int>)) {
      vector<int> intVec = v.second.as<vector<int>>();
      stringstream vecContent {};
      for (const auto& i : intVec) {
        vecContent << " " << i;
      }
      log_message << v.first << " = {" << vecContent.str() << " }";
      // double vector option
    } else if (v.second.value().type() == typeid(vector<double>)) {
      vector<double> intVec = v.second.as<vector<double>>();
      stringstream vecContent {};
      for (const auto& i : intVec) {
        vecContent << " " << i;
      }
      log_message << v.first << " = {" << vecContent.str() << " }";
      // string vector option
    } else if (v.second.value().type() == typeid(vector<string> )) {
      vector<string> intVec = v.second.as<vector<string>>();
      stringstream vecContent {};
      for (const auto& i : intVec) {
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
    log.log(m_elements_loglevel, log_message.str());
    log_message.str("");
  }
  log.log(m_elements_loglevel, "#");

}

// Log all options with a header
void ProgramManager::logTheEnvironment() const {

  log.debug() << "##########################################################";
  log.debug() << "#";
  log.debug() << "# Environment of the Run";
  log.debug() << "# ---------------------------";
  log.debug() << "#";

  for (const auto& v : Path::VARIABLE) {
    log.debug() << v.second << ": " << m_env[v.second];
  }

  log.debug() << "#";
}

void ProgramManager::bootstrapEnvironment(char* arg0) {

  m_program_name = setProgramName(arg0);
  m_program_path = setProgramPath(arg0);

  vector<path> local_search_paths(m_search_dirs.size());

  std::transform(m_search_dirs.cbegin(), m_search_dirs.cend(),
      local_search_paths.begin(),
      [](const string& s){
      return boost::filesystem::complete(s);
  });

  // insert local parent dir if it is not already
  // the first one of the list
  const path this_parent_path = boost::filesystem::canonical(m_program_path.parent_path());
  if (local_search_paths[0] != this_parent_path) {
    auto b = local_search_paths.begin();
    local_search_paths.insert(b, this_parent_path);
  }

  using Path::multiPathAppend;
  using Path::joinPath;

  for (const auto& v : Path::VARIABLE) {
    if (m_env[v.second].exists()) {
      m_env[v.second] += Path::PATH_SEP + joinPath(multiPathAppend(local_search_paths, Path::SUFFIXES.at(v.first)));
    } else {
      m_env[v.second] = joinPath(multiPathAppend(local_search_paths, Path::SUFFIXES.at(v.first)));
    }
  }

}

// Get the program options and setup logging
void ProgramManager::setup(int argc, char* argv[]) {

  // store the program name and path in class variable
  // and retrieve the local environment
  bootstrapEnvironment(argv[0]);

  // get all program options into the varaiable_map
  try {
    m_variables_map = getProgramOptions(argc, argv);
  } catch (const OptionException& e) {
    auto exit_code = e.exitCode();
    log.fatal() << "# Elements Exception : " << e.what();
    std::_Exit(static_cast<int>(exit_code));
  }

  // get the program options related to the logging
  string logging_level;
  if (m_variables_map.count("log-level")) {
    logging_level = m_variables_map["log-level"].as<string>();
  } else {
     throw Exception("Required option log-level is not provided!",
                     ExitCode::CONFIG);
  }
  path log_file_name;

  if (m_variables_map.count("log-file")) {
    log_file_name = m_variables_map["log-file"].as<path>();
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

  log.debug() << "# Exit Code: " << int(c);

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

  string version = m_parent_project_name + " " + m_parent_project_vcs_version;

  return version;
}

ProgramManager::~ProgramManager() {}

void ProgramManager::onTerminate() noexcept {

  ExitCode exit_code {ExitCode::NOT_OK};

  if ( auto exc = std::current_exception() ) {

    log.fatal() << "Crash detected";
    log.fatal() << "This is the back trace:";
    for (auto level : System::backTrace(21, 4)) {
      log.fatal() << level;
    }

    // we have an exception
    try {
      std::rethrow_exception(exc);  // throw to recognise the type
    } catch (const Exception & exc1) {
      log.fatal() << "# ";
      log.fatal() << "# Elements Exception : " << exc1.what();
      log.fatal() << "# ";
      exit_code = exc1.exitCode();
    } catch (const std::exception & exc2) {
      /// @todo : set the exit code according to the type of exception
      ///         if a clear match is found.
      log.fatal() << "# ";
      log.fatal() << "# Standard Exception : " << exc2.what();
      log.fatal() << "# ";
    } catch (...) {
      log.fatal() << "# ";
      log.fatal() << "# An exception of unknown type occurred, "
                  << "i.e., an exception not deriving from std::exception ";
      log.fatal() << "# ";
    }

    abort();

  }

  std::_Exit(static_cast<int>(exit_code));

}

}  // namespace Elements
