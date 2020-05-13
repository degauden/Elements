/**
 * @file ElementsExamples/src/lib/default/PiCalculator.cpp
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
 */


#include "ElementsExamples/PiCalculator.h"

namespace Elements {
namespace Examples {

void PiCalculator::calculate(unsigned int terms) {

  double pi = 0.0;
  // Leibniz formula for pi
  double numerator = -1.0;
  double denominator = -1.0;

  for (unsigned int ii = 0; ii < terms; ++ii)  {
    numerator   *= -1;
    denominator += 2.0;
    pi += numerator / denominator;
  }

  pi *= 4.0;

  m_show_result_callback(pi);

}

void PiCalculator::setShowResultCallback(show_result_callback_type f) {

  m_show_result_callback = f;

}


}  // namespace Examples
}  // namespace Elements
