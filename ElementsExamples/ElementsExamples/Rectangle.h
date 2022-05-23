/**
 * @file ElementsExamples/Rectangle.h
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

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_RECTANGLE_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_RECTANGLE_H_

#include <utility>  // for move

#include "ElementsKernel/Export.h"  // for ELEMENTS_API

namespace Elements {
namespace Examples {

class ELEMENTS_API Rectangle {
public:
  int x0;
  int y0;
  int x1;
  int y1;
  Rectangle(int xl, int yl, int xu, int yu);
  ~Rectangle();
  int  getLength();
  int  getHeight();
  int  getArea();
  void move(int dx, int dy);
};

}  // namespace Examples
}  // namespace Elements

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_RECTANGLE_H_

/**@}*/
