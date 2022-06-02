/**
 * @file ProgramManager.h
 *
 * Created on: Jan 7, 2015
 *     Author: Pierre Dubath
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
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PROGRAMMANAGER_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_PROGRAMMANAGER_H_

#include <map>     // for map
#include <memory>  // for unique_ptr
#include <string>  // for string
#include <vector>  // for vector

#include <boost/program_options/parsers.hpp>
#include <log4cpp/Priority.hh>

#include "ElementsKernel/Environment.h"  // For Environment
#include "ElementsKernel/Exit.h"         // For ExitCode
#include "ElementsKernel/Export.h"       // ELEMENTS_API
#include "ElementsKernel/Path.h"         // for Path::Item
#include "ElementsKernel/Program.h"

namespace Elements {

/**
 * @class ProgramManager
 * @ingroup ElementsKernel
 * @brief
 *    Class for managing all Elements programs
 * @details
 *    This base class offers solutions for the common needs of
 *    all Elements programs, such as those dealing with program
 *    options and logging.
 */
class ELEMENTS_API ProgramManager {

public:
  /**
   * @brief Constructor
   */
  explicit ProgramManager(std::unique_ptr<Program> program_ptr, const std::string& parent_project_version = "",
                          const std::string& parent_project_name        = "",
                          const std::string& parent_project_vcs_version = "",
                          const std::string& parent_module_version = "", const std::string& parent_module_name = "",
                          const std::vector<std::string>& search_dirs       = {},
                          const log4cpp::Priority::Value& elements_loglevel = log4cpp::Priority::DEBUG);

  /**
   * @brief Destructor
   */
  virtual ~ProgramManager();

  /**
   * @brief This is the public entry point,
   *   i.e., the only method called from the main
   *
   * @param argc
   *   Command line argument number
   * @param argv
   *   Command line arguments
   */
  ExitCode run(int argc, char* argv[]);

  /**
   * @brief This function returns the version of the program
   *   computed at compile time. This is the same as the project
   *   version that contains the program
   */
  std::string getVersion() const;

  /**
   * @brief This is the set_terminate handler
   *   that is used in the #MAIN_FOR macro.
   */
  static void onTerminate() noexcept;

private:
  /**
   * @brief Getter
   *
   * @return
   *   The program path
   */
  const Path::Item& getProgramPath() const;

  /**
   * @brief Getter
   *
   * @return
   *   The program name
   */
  const Path::Item& getProgramName() const;

  /**
   * @brief
   *   Get a default configuration file name and path, to be used if not
   *   provided as a command line option
   *
   * @return
   *   A complete name/path to the default configuration file
   */
  static const Path::Item getDefaultConfigFile(const Path::Item& program_name, const std::string& module_name = "");

  /**
   * @brief
   *    Strip the path from argv[0] to set the program name
   * @param arg0
   *    The first element of the command line, i.e., argv[0]
   * @return
   *    A BOOST path with the program name
   */
  static const Path::Item setProgramName(char* arg0);

  /**
   * @brief
   *    Strip the name from argv[0] to set the program path
   * @param arg0
   *    The first element of the command line, i.e., argv[0]
   * @return
   *    A BOOST path with the program path
   */
  static const Path::Item setProgramPath(char* arg0);

  /**
   * @brief
   *  Program setup taking care of command line options and logging
   *  initialization
   */
  void setup(int argc, char* argv[]);

  void tearDown(const ExitCode&);

  /**
   * @brief Get the program options from the command line
   *        into thevariables_map
   *
   *  @return
   *    A BOOST variable_map
   */
  const Program::VariablesMap getProgramOptions(int argc, char* argv[]);

  /**
   * @brief Log Header
   */
  void logHeader(std::string program_name) const;

  /**
   * @brief Log Footer
   */
  void logFooter(std::string program_name) const;

  /**
   * @brief Log all program options
   *
   */
  void logAllOptions() const;

  /**
   * @brief Log the program environment
   */
  void logTheEnvironment() const;

  /**
   * @brief Bootstrap the Environment
   *   from the executable location and the
   *   install path computed at install time.
   */
  void bootstrapEnvironment(char* arg0);

  /**
   * @brief check the explicit command line arguments.
   *   For the moment, it only checks if the configuration
   *   file being passed does exist. It exits with ExitCode::CONFIG
   *   if the file cannot be found.
   */
  template <class charT>
  void checkCommandLineOptions(const boost::program_options::basic_parsed_options<charT>& cmd_line_options);

private:
  /**
   * This is the BOOST program options variable_map used to store all
   * program options. It is similar to a std::map but the element can be
   * of different types. See the pseudoMain() in ElementsExamples/src/program/Program.cpp
   * to see how to retrieve options from this map.
   */
  Program::VariablesMap m_variables_map{};

  /**
   * Name of the executable (from argv[0])
   */
  Path::Item m_program_name;

  /**
   * Path of the executable (from argv[0])
   */
  Path::Item m_program_path;

  /**
   * Pointer to a program interface, which provides two methods
   *   defineSpecificProgramOption()
   *   mainMethod()
   *
   */
  std::unique_ptr<Program> m_program_ptr;

  /**
   * Internal version of the project
   */
  std::string m_parent_project_version;

  /**
   * Name of the parent project. To avoid ambiguities and because a module
   * can be moved from one project to another, the full version of the
   * program will look like
   *
   *     m_parent_project_version [m_parent_project_name]
   */
  std::string m_parent_project_name;

  /**
   * Internal version of the program. By convention, it is the same
   * as the VCS version of the parent project
   */
  std::string m_parent_project_vcs_version;

  /**
   * Version of the parent Elements module
   */
  std::string m_parent_module_version;

  /**
   * Name of the parent project.
   */
  std::string m_parent_module_name;

  /**
   * List of directories needed to update the runtime search
   * environment (PATH, LD_LIBRARY_PATH, ELEMENTS_CONF_PATH,
   * and ELEMENTS_AUX_PATH). This list contains the install
   * locations of all the dependent projects.
   */
  std::vector<std::string> m_search_dirs;

  /**
   * Local environment of the executable
   */
  Environment m_env;

  /**
   * default info level for the Elements internal logging messages
   */
  log4cpp::Priority::Value m_elements_loglevel;
};

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PROGRAMMANAGER_H_

/**@}*/
