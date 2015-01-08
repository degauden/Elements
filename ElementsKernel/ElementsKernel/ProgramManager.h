/**
 * @file ElementsKernel/Program.h
 * @brief define the class for the base Elements program
 * @date Aug 7, 2013
 * @author Pierre Dubath - The Euclid Consortium
 */

#ifndef ELEMENTSPROGRAMMANAGER_H_
#define ELEMENTSPROGRAMMANAGER_H_

#include <map>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "ElementsKernel/Export.h" // ELEMENTS_API
#include "ElementsKernel/Main.h"

#include "ElementsKernel/Program.h"

namespace Elements {

const std::string CONF_ENV_VAR_NAME {"ELEMENTS_CONF_PATH"};

// Forward declaration
enum class ExitCode;

/**
 * @class ProgramWithConf
 * @brief
 * 		Base class for all Elements programs
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
  ProgramManager(std::unique_ptr<Elements::Program> program_ptr) : m_program_ptr(std::move(program_ptr)) {
  }

  /**
     * @brief Destructor
     */
    virtual ~ProgramManager() {
    }

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
  ELEMENTS_API ExitCode run(int argc, char* argv[]);

private:

  /**
   * @brief Getter
   *
   * @return
   *   The program path
   */
  const boost::filesystem::path& getProgramPath() const ;

   /**
    * @brief Getter
    *
    * @return
    *   The program name
    */
  const boost::filesystem::path& getProgramName() const ;

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
   *   Get a default log file name and path, to be used if not
   *   provided as a command line option. By default a log file
   *
   *     ./"programName".log
   *
   *    will be returned
   *
   * @return
   *   A complete name/path to the default log file
   */
  const boost::filesystem::path getDefaultLogFile(
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

  // TODO
  std::unique_ptr<Elements::Program> m_program_ptr;

};

//typedef ProgramManager Program;

}

#endif /* ELEMENTSPROGRAMMANAGER_H_ */
