/**
 * @file UnitTestExample.cpp
 *
 * @date Sep 22, 2015
 * @author Florian Dubath
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

#include <numeric> // accumulate
#include <algorithm> // sort
#include "ElementsKernel/Exception.h"
#include "ElementsExamples/UnitTestExample.h"

namespace Elements {
namespace Examples {

double UnitTestExample::average (const std::vector<int>& v) {

  double result = 0;
  auto size = v.size();
  // Throw an exception if the number of vector elements is null!
  if (size == 0) {
    throw Elements::Exception() << "Input vector has no element!"; // can be removed to feed a unit test exercise
  }

  // We check if we have enough numbers to compute the median
  //if (size - 5 > 0) { // example mistake to feed a unit test exercise
  if (size > 5) {
    //
    std::vector<int> ordered { v.begin(), v.end() };
    std::sort(ordered.begin(), ordered.end());
    if (size % 2 == 0) {
      result = (ordered[size / 2 - 1] + ordered[size / 2]) / 2.;
    } else {
      result = ordered[size / 2];
    }

  } else {
    // If we have less than 5 numbers we compute the mean
    //auto sum = std::accumulate(v.begin(), v.end(), 0); // example mistake to feed a unit test exercise
    auto sum = std::accumulate(v.begin(), v.end(), 0.); // example mistake to feed a unit test exercise

    result = sum / size;
  }

  return result;
}


} // namespace ElementsExamples
} // namespace Elements

