/**
 * @file ElementsKernel/Number.h
 *
 * @date Aug 10, 2015
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

  if (is_floating_point<SourceType>::value && is_integral<TargetType>::value) {
    t = static_cast<TargetType>(round(s));
  }

  return t;
}

}  // Elements Namespace

#endif // ELEMENTSKERNEL_NUMBER_H_
