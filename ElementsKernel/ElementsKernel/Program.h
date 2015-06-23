/**
 * @file ElementsKernel/Program.h
 * @brief define an abstract class for all Elements program
 * @date January 8, 2015
 * @author Pierre Dubath
 */

#ifndef ELEMENTSPROGRAM_H_
#define ELEMENTSPROGRAM_H_

#include <string>
#include <utility>
#include <map>

#include <boost/program_options.hpp>

#include "ElementsKernel/Export.h"      // ELEMENTS_API
#include "ElementsKernel/Exit.h"        // for ExitCode, ExitCode::OK

namespace Elements {

/**
 * @class Program
 * @brief
 * 		Abstract class for all Elements programs
 * @details
 * 		This abstract class defines the three interfaces that should
 * 		be implemented by all Elements programs.
 */
class ELEMENTS_API Program {

public:

  /**
   * @brief Constructor
   */
  Program() = default;

  /**
   * @brief Destructor
   */
  virtual ~Program() {};

  /**
   * @brief
   *   This methods must be used to define specific program options
   * @details
   *  This is the first method that must be implemented by all Elements programs
   *
   * @return
   *   A BOOST options description
   */
  virtual boost::program_options::options_description defineSpecificProgramOptions() ;

  /**
   * @brief
   *   This methods must be used to the program arguments.
   * @details
   *  This is the second method that must be implemented by all Elements programs
   *
   * @return
   *   a pair of  BOOST options description and positional_options_description
   */
  virtual std::pair<boost::program_options::options_description,boost::program_options::positional_options_description> defineProgramArguments() ;

  /**
   * @brief
   *   This is the "main" method of all Elements programs
   * @details
   *  This is the second method that must be implemented by all Elements programs. It
   *  represents the entry point.
   *
   * @param args
   *    A map containing the values given by the user for the program options
   *    defined by the defineSpecificProgramOptions() method
   * @return
   *    The exit code which should be returned when the program exits
   */
  virtual ExitCode mainMethod(std::map<std::string, boost::program_options::variable_value>& args) = 0;

};

/** These are examples of how to create a executable program using
 * the Program class.
 * @example ElementsExamples/src/Program/SimpleProgramExample.cpp
 * @include ElementsExamples/src/Program/ProgramExample.cpp
 */
}

#endif /* ELEMENTSPROGRAM_H_ */
