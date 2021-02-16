/**
 * @file OtherProgramExample.cpp
 * @date February 16th, 2021
 * @author Hubert Degaudenzi
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

#include <cstdint>  // for int64_t
#include <map>      // for map
#include <memory>   // for unique_ptr
#include <string>   // for string
#include <utility>  // for move
#include <vector>   // for vector

#include "ElementsExamples/ClassExample.h"
#include <boost/current_function.hpp>  // for BOOST_CURRENT_FUNCTION
#include <boost/program_options.hpp>   // for program options from configuration file of command line arguments

#include "ElementsKernel/Module.h"          // for Module
#include "ElementsKernel/ProgramHeaders.h"  // for including all Program/related headers
#include "ElementsKernel/Project.h"         // for Project
#include "ElementsKernel/ThisModule.h"      // for getThisExecutableInfo

#include "ElementsExamples/functionExample.h"
#include "ElementsExamples/printProject.h"

using std::map;
using std::string;
using std::vector;

using boost::program_options::bool_switch;
using boost::program_options::value;

using std::int64_t;

namespace Elements {
namespace Examples {

/**
 * @brief
 *    test function to demonstrate the logger
 * @details
 *    test function to demonstrate the logger
 */

void myLocalLogTestFunc() {

  auto logger = Logging::getLogger();
  logger.info("Test of Message");

  auto logger2 = Logging::getLogger(__func__);
  logger2.info("Test2 of Message");

  auto logger3 = Logging::getLogger(BOOST_CURRENT_FUNCTION);
  logger3.info("Test3 of Message");
}

/**
 * @class ProgramExample
 * @brief
 *    Simple example of an Elements program
 * @details
 *    All C++ executable must extend the Elements::Program base class
 *
 */
class OtherProgramExample : public Program {

private:
  bool        m_flag{false};
  int         m_int_option;
  string      m_string_option;
  bool        m_boolean_option;
  int64_t     m_long_long_option;
  double      m_double_option;
  vector<int> m_int_vector_option;

public:
  /**
   * @brief
   *    Allows to define the (command line and configuration file) options specific to
   *    this program
   * @details
   *    See the ElementsProgram documentation for more details.
   * @return
   *    A BOOST program options_description
   */
  OptionsDescription defineSpecificProgramOptions() override {

    OptionsDescription config_options{"Example program options"};

    // Add the specific program options
    config_options.add_options()("int-option", value<int>(&m_int_option)->default_value(int{111}),
                                 "An example int option")(
        "string-option", value<string>(&m_string_option)->default_value(string{}), "An example string option")(
        "boolean-option", value<bool>(&m_boolean_option)->default_value(false),
        "An example boolean option")("flag,f", bool_switch(&m_flag), "An option to set to true")(
        "long-long-option", value<int64_t>(&m_long_long_option)->default_value(int64_t{3}),
        "An example long long option")("double-option", value<double>(&m_double_option)->default_value(double{}),
                                       "An example double option")(
        "int-vector-option",
        value<vector<int>>(&m_int_vector_option)->multitoken()->default_value(vector<int>{}, "Empty"),
        "An example vector option");

    return config_options;
  }

  /**
   * @brief
   *    The "main" method.
   * @details
   *    This method is the entry point to the program. In this sense, it is similar to a main
   *    (and it is why it is called mainMethod()). The code below contains the calls to the
   *    different classes created for the first developer's workshop
   *
   *    See the ElementsProgram documentation for more details.
   *
   */
  ExitCode mainMethod(ELEMENTS_UNUSED map<string, VariableValue>& args) override {

    auto log = Logging::getLogger("ProgramExample");
    log.info("Entering mainMethod()");
    log.info("#");
    /*
     * Get and log one of the program arguments (or options)
     *
     * The string-option has a default empty string value, so that it can always be
     * printed event as an empty string
     */
    log.info() << "String option value: " << m_string_option;

    log.info() << "The int-option value is " << m_int_option;
    log.info() << "The flag value is " << m_flag;
    log.info() << "The string-option value is " << m_string_option;
    log.info() << "The long-long-option value is " << m_long_long_option;

    // Some initialization
    double  input_variable = 3.4756;
    int64_t source_id      = 12345;
    double  ra             = 45.637;

    // Factory method example
    ClassExample example_class_object = ClassExample::factoryMethod(source_id, ra);

    /*
     * All fundamental type variables can be copied forth and back without significant
     * cost in (almost) all cases
     */
    double method_result = example_class_object.fundamentalTypeMethod(input_variable);
    log.info() << "Some result: " << method_result;

    double first = 1.0;
    double division_result{};
    try {
      log.info("#");
      log.info("#   Calling a method throwing an exception ");
      log.info("#");
      double second   = 0.0;
      division_result = example_class_object.divideNumbers(first, second);
      //
    } catch (const Exception& e) {
      log.info("#");
      log.info() << e.what();
      log.info("#");
      log.info("#   In this silly example we continue with a fake fix ");
      log.info("#");
      division_result = example_class_object.divideNumbers(first, 0.000001);
    }
    log.info() << "Second result is: " << division_result;

    /*
     * Illustration on how best to use smart pointer (regular pointer should not
     * be used anymore). The move() indicate that the ownership of the pointer is given to the
     * method called. The vector_unique_ptr cannot be used in this method anymore after the
     * call.
     */
    std::unique_ptr<vector<double>> vector_unique_ptr{new vector<double>{1.0, 2.3, 4.5}};
    example_class_object.passingUniquePointer(std::move(vector_unique_ptr));

    /*
     * Illustration on how best to pass any object. The passingObjectInGeneral() is taking
     * a reference to this object.
     */
    //    vector<double> object_example{vector<double>{1.0, 2.3, 4.5}};
    vector<double> object_example{1.0, 2.3, 4.5};
    example_class_object.passingObjectInGeneral(object_example);

    log.info() << "Function Example: " << functionExample(3);

    log.info() << "This executable name: " << Elements::System::getThisExecutableInfo().name();

    myLocalLogTestFunc();

    printProject();

    log.info() << Project();
    log.info() << "Project Name: " << Project::name();
    log.info() << "Project Version: " << Project::versionString();
    log.info() << "Module Name: " << Module::name();
    log.info() << "Module Version: " << Module::versionString();

    log.info("#");
    log.info("Exiting mainMethod()");
    return ExitCode::OK;
  }
};

}  // namespace Examples
}  // namespace Elements

/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::OtherProgramExample)
