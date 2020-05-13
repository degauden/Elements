/**
 * @file ElementsExamples/src/lib/default/Rectangle.cpp
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

#include "ElementsExamples/Rectangle.h"

namespace Elements {
namespace Examples {

Rectangle::Rectangle(int X0, int Y0, int X1, int Y1) {
    x0 = X0;
    y0 = Y0;
    x1 = X1;
    y1 = Y1;
}

Rectangle::~Rectangle() {
}

int Rectangle::getLength() {
    return (x1 - x0);
}

int Rectangle::getHeight() {
    return (y1 - y0);
}

int Rectangle::getArea() {
    return (x1 - x0) * (y1 - y0);
}

void Rectangle::move(int dx, int dy) {
    x0 += dx;
    y0 += dy;
    x1 += dx;
    y1 += dy;
}

}  // namespace Examples
}  // namespace Elements
