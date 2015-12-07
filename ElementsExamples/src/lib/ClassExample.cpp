/**
 * @file ClassExample.cpp
 *
 * Created on: January 9, 2015
 *     Author: Pierre Dubath
 */

#include <cstdint>                           // for std::int64_t
#include <cmath>                             // for std::abs

#include "ElementsExamples/ClassExample.h"
#include "ElementsKernel/Logging.h"
#include "ElementsKernel/Exception.h"

namespace Elements {
namespace ElementsExamples {

using namespace std;

std::string ClassExample::s_static_string = "This is a static field example";

double ClassExample::fundamentalTypeMethod(const double input_variable) const {
  return input_variable;
}

/*
 * This is a silly example just to have a method throwing an exception
 */
double ClassExample::divideNumbers(const double first,
    const double second) const {
  double tolerance = 1e-12;
  if (std::abs(second) < tolerance) {
    throw Elements::Exception() << "Dividing by " << second << " exception in ClassExample::divideNumbers(...)";
  }
  return first / second;
}

void ClassExample::passingUniquePointer(
    unique_ptr<vector<double>> vector_unique_ptr) const {
    vector_unique_ptr->size();
}

void ClassExample::passingObjectInGeneral(
    const vector<double>& input_object) const {
    input_object.size();
}

ClassExample ClassExample::factoryMethod(const int64_t source_id,
    const double ra) {
  return ClassExample { source_id, ra };
}

} // namespace ElementsExamples
} // namespace Elements

