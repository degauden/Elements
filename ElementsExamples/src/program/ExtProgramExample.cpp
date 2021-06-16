/**
 * @file ProgramExample.cpp
 * @date January 6th, 2015
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

/**
 * @class ProgramExample
 * @brief
 *    Simple example of an Elements program outside of the Elements namespace
 * @details
 *    All C++ executable must extend the Elements::Program base class
 *
 */
class ExtProgramExample : public Elements::Program {

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
    auto               add = config_options.add_options();

    bool flag = false;

    // Add the specific program options
    add("int-option", value<int>()->default_value(int{111}), "An example int option");
    add("int-option-with-default-and-default-in-conf", value<int>()->default_value(int{222}), "An example int option");
    add("int-option-with-default-no-default-in-conf", value<int>()->default_value(int{444}), "An example int option");
    add("int-option-no-default-not-defined-in-conf", value<int>(), "An example int option");
    add("int-option-with-no-defaults-anywhere", value<int>(), "An example int option");
    add("string-option", value<string>()->default_value(string{}), "An example string option");
    add("boolean-option", value<bool>()->default_value(false), "An example boolean option");
    add("flag,f", bool_switch(&flag), "An option to set to true");
    add("string-option-no-default", value<string>(), "A string option without default value");
    add("long-long-option", value<int64_t>()->default_value(int64_t{}), "An example long long option");
    add("double-option", value<double>()->default_value(double{}), "An example double option");
    add("int-vector-option", value<vector<int>>()->multitoken()->default_value(vector<int>{}, "Empty"),
        "An example vector option");
    add("threshold,t", value<double>()->default_value(double{0.5}), "An example double option");

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
  ExitCode mainMethod(map<string, VariableValue>& args) override {

    using Elements::Examples::ClassExample;

    auto log = Logging::getLogger("ProgramExample");
    log.info("Entering mainMethod()");
    log.info("#");
    /*
     * Check availability of mandatory program arguments (or options)
     *
     * Arguments values may come from
     *  1) the default value provided in above defineSpecificProgramOptions()
     *  2) the configuration file
     *  3) the command line
     *
     *  If an none of the three options provide any values for a mandatory
     *  argument, you should check if your option has any values following the
     *  below example. Note that this may happen for all options without default
     *  values.
     */
    if (args["string-option-no-default"].empty()) {
      log.info() << "No value are available for string-option-no-default";
      /*
       * An exception may be thrown her if the above option is mandatory and there
       * is no way to continue without value
       */
    }
    /*
     * Get and log one of the program arguments (or options)
     *
     * The string-option has a default empty string value, so that it can always be
     * printed event as an empty string
     */
    string string_example{args["string-option"].as<string>()};
    log.info() << "String option value: " << string_example;

    log.info() << "The int-option value is " << args["int-option"].as<int>();
    log.info() << "The threshold value is " << args["threshold"].as<double>();

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
    } catch (const Elements::Exception& e) {
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
    vector<double> object_example{vector<double>{1.0, 2.3, 4.5}};
    example_class_object.passingObjectInGeneral(object_example);

    log.info() << "Function Example: " << Elements::Examples::functionExample(3);

    log.info() << "This executable name: " << Elements::System::getThisExecutableInfo().name();

    Elements::Examples::printProject();

    log.info() << Elements::Project();
    log.info() << "Project Name: " << Elements::Project::name();
    log.info() << "Project Version: " << Elements::Project::versionString();
    log.info() << "Module Name: " << Elements::Module::name();
    log.info() << "Module Version: " << Elements::Module::versionString();

    log.info("#");
    log.info("Exiting mainMethod()");
    return ExitCode::OK;
  }
};

/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(ExtProgramExample)
