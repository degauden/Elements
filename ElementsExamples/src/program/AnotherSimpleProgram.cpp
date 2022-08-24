/**
 * @file AnotherSimpleProgram.cpp
 *
 * @date Dec 23, 2019
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

#include <iostream>

#include "ElementsKernel/Exception.h"  // For Exception
#include "ElementsKernel/Exit.h"       // For ExitCode
#include "ElementsKernel/SimpleProgram.h"

namespace Elements {
namespace Examples {

/**
 * @class AnotherSimpleProgram
 * @brief
 *    Example of an Elements program
 * @details
 *    This class is an example of a program based on the SimpleProgram class. It can be copied/pasted
 *    conveniently to write a new program.
 */
class AnotherSimpleProgram : public SimpleProgram {

public:
  void defineOptions() override {}

  /**
   * @brief
   *    The "main" method.
   * @details
   *    This method is the entry point to the program. In this sense, it is similar to a main
   *    (and it is why it is called main()). The code below provides only example stuff
   *    which should be replaced by real code in any program.
   *
   *    See the SimpleProgram documentation for more details.
   *
   */
  ExitCode main() override {

    std::cout << "Program name: " << getProgramName() << std::endl;
    std::cout << "Program path: " << getProgramPath() << std::endl;

    throw Exception("This is an example exception.");

    return ExitCode::OK;
  }
};

}  // namespace Examples
}  // namespace Elements

MAIN(Elements::Examples::AnotherSimpleProgram)
