/**
 * @file ElementsKernel/Number.h
 *
 * @date Aug 10, 2015
 * @author Hubert Degaudenzi
 */

#ifndef ELEMENTSKERNEL_NUMBER_H_
#define ELEMENTSKERNEL_NUMBER_H_

#include <type_traits>             // for is_floating_point, is_integral
#include <cmath>                   // for round

#include "ElementsKernel/Export.h" // ELEMENTS_API

namespace Elements {

/**
 * @brief
 *   this function is a number cast. It behaves exactly as a static_cast except
 *   when casting from a floating point number to an integral one. This one is
 *   doing the correct rounding.
 * @param s
 *   number to cast
 * @return
 *   casted number
 */
template<typename TargetType, typename SourceType>
ELEMENTS_API TargetType numberCast(const SourceType& s) {

  using std::is_floating_point;
  using std::is_integral;
  using std::round;

  TargetType t = static_cast<TargetType>(s);

  if(is_floating_point<SourceType>::value && is_integral<TargetType>::value) {
    t = static_cast<TargetType>(round(s));
  }

  return t;
}



}



#endif // ELEMENTSKERNEL_NUMBER_H_
