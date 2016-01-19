/**
 * @file ProgramManager.h
 *
 * Created on: Jan 7, 2015
 *     Author: Pierre Dubath
 */


#ifndef ELEMENTSPROGRAMMANAGER_H_
#define ELEMENTSPROGRAMMANAGER_H_

#include <map>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "ElementsKernel/Export.h" // ELEMENTS_API
#include "ElementsKernel/Exit.h"
#include "ElementsKernel/Program.h"

namespace Elements {

const std::string CONF_ENV_VAR_NAME { "ELEMENTS_CONF_PATH" };

/**
 * @class ProgramManager
 * @brief
 * 		Class for managing all Elements programs
 * @details
 * 		This base class offers solutions for the common needs of
 * 		all Elements programs, such as those dealing with program
 * 		options and logging.
 */
class ELEMENTS_API ProgramManager {

public:

  /**
   * @brief Constructor
   */
  ProgramManager(std::unique_ptr<Elements::Program> program_ptr,
                 std::string parent_project_version="",
                 std::string parent_project_name="") :
      m_program_ptr(std::move(program_ptr)),
      m_parent_project_version(std::move(parent_project_version)),
      m_parent_project_name(std::move(parent_project_name)){
  }

  /**
   * @brief Destructor
   */
  virtual ~ProgramManager() ;

  /**
   * @brief
   *  This is the public entry point, i.e., the only method
   *  called from the main
   *
   * @param argc
   *   Command line argument number
   * @param argv
   *   Command line arguments
   */
  ELEMENTS_API
  ExitCode run(int argc, char* argv[]);

  ELEMENTS_API std::string getVersion() const;

private:

  /**
   * @brief Getter
   *
   * @return
   *   The program path
   */
  const boost::filesystem::path& getProgramPath() const;

  /**
   * @brief Getter
   *
   * @return
   *   The program name
   */
  const boost::filesystem::path& getProgramName() const;

  /**
   * @brief
   *   Get a default configuration file name and path, to be used if not
   *   provided as a command line option
   *
   * @return
   *   A complete name/path to the default configuration file
   */
  const boost::filesystem::path getDefaultConfigFile(
      const boost::filesystem::path & program_name) const;

  /**
   * @brief
   *    Strip the path from argv[0] to set the program name
   * @param argv
   *    The first element of the command line, i.e., argv[0]
   * @return
   *    A BOOST path with the program name
   */
  const boost::filesystem::path setProgramName(char* argv) const;

  /**
   * @brief
   *    Strip the name from argv[0] to set the program path
   * @param argv
   *    The first element of the command line, i.e., argv[0]
   * @return
   *    A BOOST path with the program path
   */
  const boost::filesystem::path setProgramPath(char* argv) const;

  /**
   * @brief
   *  Program setup taking care of command line options and logging
   *  initialization
   */
  void setup(int argc, char* argv[]);

  /**
   * @brief
   *  Get the program options from the command line into the
   *  variables_map
   *
   *  @return
   *    A BOOST variable_map
   */
  const boost::program_options::variables_map getProgramOptions(int argc,
      char* argv[]);

  /**
   * @brief
   *  Log all program options
   *
   */
  void logAllOptions(std::string program_name);

private:

  /**
   * This is the BOOST program options variable_map used to store all
   * program options. It is similar to a std::map but the element can be
   * of different types. See the pseudoMain() in ElementsProgramExample.cpp
   * to see how to retrieve options from this map.
   */
  boost::program_options::variables_map m_variables_map { };

  /**
   * Name of the executable (from argv[0])
   */
  boost::filesystem::path m_program_name;

  /**
   * Path of the executable (from argv[0])
   */
  boost::filesystem::path m_program_path;

  /**
   * Pointer to a program interface, which provides two methods
   *   defineSpecificProgramOption()
   *   mainMethod()
   *
   */
  std::unique_ptr<Elements::Program> m_program_ptr;

  /**
   * Internal version of the program. By convention, it is the same
   * as the version of the parent project
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

};

}

#endif /* ELEMENTSPROGRAMMANAGER_H_ */
