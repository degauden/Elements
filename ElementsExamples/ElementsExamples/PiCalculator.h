/**
 * @file ElementsExamples/PiCalculator.h
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

/**
 * @addtogroup ElementsExamples ElementsExamples
 * @{
 */

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PICALCULATOR_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PICALCULATOR_H_

#include "ElementsKernel/Export.h"

namespace Elements {
namespace Examples {

class ELEMENTS_API PiCalculator {
public:
  PiCalculator() = default;
  void calculate(unsigned int terms);

  typedef void (*show_result_callback_type)(double pi);
  void setShowResultCallback(show_result_callback_type f);

private:
  show_result_callback_type m_show_result_callback {};
};


}  // namespace Examples
}  // namespace Elements

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PICALCULATOR_H_

/**@}*/
