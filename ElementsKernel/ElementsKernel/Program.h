/**
 * @file ElementsKernel/Program.h
 * @brief define an abstract class for all Elements program
 * @date January 8, 2015
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

/**
 * \addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PROGRAM_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_PROGRAM_H_

#include <string>                       // for string
#include <utility>                      // for pair
#include <map>                          // for map
#include <memory>                       // for unique_ptr

#include <boost/program_options.hpp>

#include "ElementsKernel/Export.h"      // ELEMENTS_API
#include "ElementsKernel/Exit.h"        // for ExitCode

namespace Elements {

/**
 * @class Program
 * @ingroup ElementsKernel
 * @brief
 *   Abstract class for all Elements programs
 * @details
 *   This abstract class defines the three interfaces that should
 *   be implemented by all Elements programs.
 */
class ELEMENTS_API Program {

public:

  // backwards compatible type aliases
  using options_description = boost::program_options::options_description;
  using positional_options_description = boost::program_options::positional_options_description;
  using variable_value = boost::program_options::variable_value;
  using variables_map = boost::program_options::variables_map;

  // camel case type aliases
  using OptionsDescription = options_description;
  using PositionalOptionsDescription = positional_options_description;
  using VariableValue = variable_value;
  using VariablesMap = variables_map;

  /**
   * @brief Constructor
   */
  Program() = default;

  /**
   * @brief Destructor
   */
  virtual ~Program();

  /**
   * @brief
   *   This methods must be used to define specific program options
   * @details
   *  This is the first method that must be implemented by all Elements programs
   *
   * @return
   *   A BOOST options description
   */
  virtual OptionsDescription defineSpecificProgramOptions();

  /**
   * @brief
   *   This methods must be used to the program arguments.
   * @details
   *  This is the second method that must be implemented by all Elements programs
   *
   * @return
   *   a pair of  BOOST options description and positional_options_description
   */
  virtual std::pair<OptionsDescription, PositionalOptionsDescription> defineProgramArguments();

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
  virtual ExitCode mainMethod(std::map<std::string, VariableValue>& args) = 0;

};

/** These are examples of how to create a executable program using
 * the Program class.
 * @example ElementsExamples/src/program/SimpleProgramExample.cpp
 * @include ElementsExamples/src/program/ProgramExample.cpp
 */
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PROGRAM_H_

/**@}*/
