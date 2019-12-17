/*
 * Rectangle_test.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: Hubert Degaudenzi
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

#include "ElementsExamples/Rectangle.h"  // Access the objects you want to test
#include <boost/test/unit_test.hpp>

using Elements::Examples::Rectangle;

// Starts a test suite and name it.
BOOST_AUTO_TEST_SUITE(Rectangle_test_suite)


BOOST_AUTO_TEST_CASE(Dimension_test) {

  auto r = Rectangle(1, 1, 3, 2);

  BOOST_CHECK_EQUAL(r.getLength(), 2);
  BOOST_CHECK_EQUAL(r.getHeight(), 1);

}

BOOST_AUTO_TEST_CASE(Area_test) {

  auto r = Rectangle(1, 1, 3, 2);

  BOOST_CHECK_EQUAL(r.getArea(), 2);

}


// Ends the test suite
BOOST_AUTO_TEST_SUITE_END()
