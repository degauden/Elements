/*
 * Inversion.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: Florian Dubath
 */

#include <algorithm>
#include "ElementsExamples/UnitTestExample.h"



#include "ElementsKernel/Exception.h"
#include "ElementsKernel/Real.h"

namespace Elements {
namespace ElementsExamples {

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

