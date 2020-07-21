/**
 * @file ClassExample2.cpp
 *
 * @date January 9, 2015
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

#include "ElementsExamples/ClassExample2.h"

#include <cstdint>                           // for std::int64_t
#include <cmath>                             // for std::abs
#include <vector>                            // for vector
#include <memory>                            // for unique_ptr
#include <string>                            // for string

#include "ElementsKernel/Exception.h"

using std::vector;

namespace Elements {
namespace Examples {

std::string ClassExample2::s_static_string = "This is a static field example";

double ClassExample2::fundamentalTypeMethod(const double input_variable) const {
  return input_variable;
}

/*
 * This is a silly example just to have a method throwing an exception
 */
double ClassExample2::divideNumbers(const double first,
    const double second) const {
  double tolerance = 1e-12;
  if (std::abs(second) < tolerance) {
    throw Elements::Exception() << "Dividing by " << second << " exception in ClassExample2::divideNumbers(...)";
  }
  return first / second;
}

void ClassExample2::passingUniquePointer(
    std::unique_ptr<vector<double>> vector_unique_ptr) const {
    vector_unique_ptr->size();
}

void ClassExample2::passingObjectInGeneral(
    const vector<double>& input_object) const {
    input_object.size();
}

ClassExample2 ClassExample2::factoryMethod(const std::int64_t source_id,
    const double ra) {
  return ClassExample2 { source_id, ra };
}

}  // namespace Examples
}  // namespace Elements
