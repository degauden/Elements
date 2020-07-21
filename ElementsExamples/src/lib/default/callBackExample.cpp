/**
 * @file callBackExample.cpp
 *
 * @date Apr 1, 2020
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

#include <vector>                              // for vector
#include <functional>                          // for function

#include "ElementsKernel/Logging.h"            // for Logging

#include "ElementsExamples/callBackExample.h"

using std::vector;

namespace Elements {
namespace Examples {

double testCallBack(vector<double> x, const std::function<double(vector<double>)> &fun) {

  auto log = Logging::getLogger("ElementsExamples");

  log.info() << "Calling the Python function from C++";

  double f = fun(x);

  log.info() << "The value of the function is " << f;

  log.info() << "Returning the result";

    return (f);
}

}  // namespace Examples
}  // namespace Elements

