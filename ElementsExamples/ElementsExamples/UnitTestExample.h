/*
 * UnitTestExample.h
 *
 *  Created on: Sep 22, 2015
 *      Author: Florian Dubath
 */

#ifndef ELEMENTSEXAMPLES_UNITTESTEXAMPLE_H_
#define ELEMENTSEXAMPLES_UNITTESTEXAMPLE_H_

#include <vector>
#include "ElementsKernel/Export.h"

namespace Elements {
namespace Examples {

/**
 * @class UnitTestExample
 * @brief This class has been created to demonstrate unit testing.
 */
class ELEMENTS_API UnitTestExample {

public:

  /**
   * @brief
   *  Returns a particular version of the "average" of the vector values

   * @details
   *  This method computes the "average" as the median of the given values if there are more
   *  than 5 values. Otherwise, it returns the mean value instead of the median.
   *
   * If an even number of numbers is given, the returned value is the mean of the
   * two middle values.
   *
   * @param v
   *    The values to compute the "average" for
   * @return
   *    The "average" of the given values
   * @throws Elements::Exception
   *    If the given vector is empty
   */
  double average(const std::vector<int>& v);

};

} // namespace ElementsExamples
} // namespace Elements

#endif /* ELEMENTSEXAMPLES_UNITTESTEXAMPLE_H_ */
