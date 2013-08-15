/**
 * @file ElementsProgram.h
 *
 * Created on: Aug 7, 2013
 *     Author: Pierre Dubath
 */

#ifndef ELEMENTSPROGRAM_H_
#define ELEMENTSPROGRAM_H_

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "ElementsKernel/ElementsLogging.h"

/**
 * Macro which must be used to create a main in classes
 * that derived from ElementsProgram, i.e., these derived classes
 * must end with the following line:
 *
 * 		BUILD_MAIN_FOR(ElementsProgramExample)
 *
 * 	ElementsProgramExample.cpp shows how to use this macro
 */
#define MAIN_FOR(ElementsProgramName) 		    \
  int main(int argc, char* argv[]) 					        \
  {                               				        	\
    ElementsProgramName elementProgramInstance {} ;	\
    elementProgramInstance.run(argc, argv) ;	      \
    return 0;                                       \
  }

/**
 * @class ElementsProgram
 * @brief
 * 		Base class for all Elements programs
 * @details
 * 		This base class offers solutions for the common needs of
 * 		all Elements programs, such as those dealing with program
 * 		options and logging.
 */
class ElementsProgram {

public:

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
  void run(int argc, char* argv[]);

protected:

  /**
   * @brief Constructor
   */
  ElementsProgram() = default;

  /**
   * @brief Destructor
   */
  virtual ~ElementsProgram() = default;

  /**
   * @brief
   *   This methods must be used to define specific program options
   * @details
   *  This is the first method that must be implemented by all Elements programs
   *  It is called from getProgramOptions(int argc,char* argv[])
   *
   * @return
   *   A BOOST options description
   */
  virtual boost::program_options::options_description defineSpecificProgramOptions() = 0;

  /**
   * @brief
   *   This is the "main" method of all Elements programs
   * @details
   *  This is the second method that must be implemented by all Elements programs. It
   *  represents the entry point, called from run(int argc,char* argv[]).
   */
  virtual void mainMethod() = 0;

  /**
   * @brief
   *   Get the program version from svn tags
   * @details
    *
   * @return
   *   A string with the version information
   */
  virtual std::string getVersion() = 0;

  /**
   * @brief Getter
   *
   * @return
   *   The program path
   */
  const boost::filesystem::path& getProgramPath() const {
    return m_programPath;
  }

   /**
    * @brief Getter
    *
    * @return
    *   The program name
    */
  const boost::filesystem::path& getProgramName() const {
    return m_programName;
  }

  /**
   * @brief Getter
   *
   * @return
   *   The BOOST variables_map which includes all program options.
   */
  const boost::program_options::variables_map& getVariablesMap() const {
    return m_variablesMap;
  }

private:

  /**
   * This is the BOOST program options variable_map used to store all
   * program options. It is similar to a std::map but the element can be
   * of different types. See the pseudoMain() in ElementsProgramExample.cpp
   * to see how to retrieve options from this map.
   */
  boost::program_options::variables_map m_variablesMap { };

  /**
   * Name of the executable (from argv[0])
   */
   boost::filesystem::path m_programName;

  /**
   * Path of the executable (from argv[0])
   */
  boost::filesystem::path m_programPath;

  /**
   * @brief
   *   Get a default configuration file name and path, to be used if not
   *   provided as a command line option
   *
   * @return
   *   A complete name/path to the default configuration file
   */
   const boost::filesystem::path getDefaultConfigFile(
       const boost::filesystem::path & programName) const;

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
      const boost::filesystem::path & programName) const;

  /**
   * @brief
   *    Strip the path from argv[0] to set the program name

   * @param
   *    The first element of the command line, i.e., argv[0]
   * @return
   *    A BOOST path with the program name
   */
  const boost::filesystem::path setProgramName(char* argv) const;

  /**
   * @brief
   *    Strip the name from argv[0] to set the program path

   * @param
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
  void setup(int argc, char* argv[]) noexcept;

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
  void logAllOptions(std::string programName);

};

#endif /* ELEMENTSPROGRAM_H_ */
