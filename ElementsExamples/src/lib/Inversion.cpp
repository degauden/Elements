/*
 * Inversion.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: Florian Dubath
 */
#include "ElementsKernel/Exception.h"
#include "ElementsKernel/Real.h"
#include "ElementsExamples/Inversion.h"

namespace Elements {
namespace ElementsExamples {

double Inversion::oneOverAbsX (double x) {
  if (Elements::isEqual(x, 0.)) {
    throw Elements::Exception() << "Division by 0!";
  }

  //return abs(1. / x );
  return std::abs(1. / x );
}

} // namespace ElementsExamples
} // namespace Elements

