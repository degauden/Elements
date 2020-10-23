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

#include "ElementsExamples/PiCalculator.h"  // Access the objects you want to test

#include <boost/test/unit_test.hpp>
#include <iostream>

using Elements::Examples::PiCalculator;

// Starts a test suite and name it.
BOOST_AUTO_TEST_SUITE(PiCalculator_test_suite)

void callBackFunction(double pi) {

  std::cout << "Here is the pi value: " << pi << std::endl;
}

BOOST_AUTO_TEST_CASE(Creation_test) {

  auto pc = PiCalculator();

  pc.setShowResultCallback(callBackFunction);

  pc.calculate(10);
}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END()
