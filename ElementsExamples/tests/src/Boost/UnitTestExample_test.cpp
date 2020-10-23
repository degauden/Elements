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

#include "ElementsExamples/UnitTestExample.h"  // Access the objects you want to test

#include <vector>  // for vector

#include <boost/test/unit_test.hpp>  // Gives access to the unit test framework.

#include "ElementsKernel/Exception.h"  // Exception

using Elements::Examples::UnitTestExample;
using std::vector;

// Starts a test suite and name it.
BOOST_AUTO_TEST_SUITE(UnitTestExample_test_suite)

BOOST_AUTO_TEST_CASE(median_test) {
  UnitTestExample     tested_object{};
  vector<vector<int>> input_vec{{1, 2, 3, 10, 101, 102},
                                {1, 2, 3, 11, 101, 102, 103},
                                {1, 2, 3, 10, 101, 102, 103, 500},
                                {1, 2, 3, 11, 101, 102, 103, 500, 501}};
  vector<double>      expected_result{6.5, 11, 55.5, 101};

  for (size_t test_id = 0; test_id < 4; ++test_id) {
    BOOST_CHECK_EQUAL(expected_result[test_id], tested_object.average(input_vec[test_id]));
  }
}

BOOST_AUTO_TEST_CASE(order_test) {
  UnitTestExample tested_object{};
  vector<int>     input_vec_orderd{1, 2, 3, 11, 101, 102, 103};
  vector<int>     input_vec{103, 101, 2, 1, 3, 11, 102};
  double          expected_result{11};

  BOOST_CHECK_EQUAL(expected_result, tested_object.average(input_vec_orderd));
  BOOST_CHECK_EQUAL(expected_result, tested_object.average(input_vec));
}

BOOST_AUTO_TEST_CASE(mean_test) {
  UnitTestExample     tested_object{};
  vector<vector<int>> input_vec{{1}, {1, 2}, {1, 2, 4}, {1, 2, 4, 8}, {1, 2, 4, 8, 16}};
  vector<double>      expected_result{1.0, 1.5, 2.33333333333333333, 3.75, 6.2};
  double              local_tolerence{1e-10};

  for (size_t test_id = 0; test_id < 5; ++test_id) {
    BOOST_CHECK_CLOSE(expected_result[test_id], tested_object.average(input_vec[test_id]), local_tolerence);
  }
}

BOOST_AUTO_TEST_CASE(exception_test) {
  UnitTestExample tested_object{};
  vector<int>     input_vec{};

  BOOST_CHECK_THROW(tested_object.average(input_vec), Elements::Exception);
}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END()
