/*
 * UnitTestExample_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: fdubath
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

#include <boost/test/unit_test.hpp>            // Gives access to the unit test framework.
#include <string>
#include <iostream>

#include "ElementsKernel/Environment.h"        // for Environment

// Starts a test suite and name it.
BOOST_AUTO_TEST_SUITE(TestEnvironment_test_suite)


BOOST_AUTO_TEST_CASE(MyEnv_test) {

  Elements::Environment current;

  std::cout << "MYTESTENV: " <<current["MYTESTENV"] << std::endl;

  BOOST_CHECK_EQUAL(std::string(current["MYTESTENV"]), "FooBar");
  BOOST_CHECK(std::string(current["MYTESTENV"]) == "FooBar");


}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END()
