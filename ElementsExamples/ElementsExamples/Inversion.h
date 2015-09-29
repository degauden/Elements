/*
 * Inversion.h
 *
 *  Created on: Sep 22, 2015
 *      Author: Florian Dubath
 */

#ifndef ELEMENTSEXAMPLES_INVERSION_H_
#define ELEMENTSEXAMPLES_INVERSION_H_

#include "ElementsKernel/Export.h"

namespace Elements {
namespace ElementsExamples {

/**
 * @class Inversion
 * @brief This class has been created to demonstrate unit testing.
 */
class ELEMENTS_API Inversion {

public:

  /**
   * @brief Computes the inverse of the abs of a number.
   *
   * @param x
   * A double to be inverted.
   *
   * @return
   * A double with value 1/|x|.
   *
   * @throw Elements::Exception
   * If the parameter x is equals to zero.
   */
  double oneOverAbsX (double x);

};

} // namespace ElementsExamples
} // namespace Elements


#endif /* ELEMENTSEXAMPLES_INVERSION_H_ */
