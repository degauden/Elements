/**
 * @file ClassExample2_test.cpp
 *
 * Created on: January 9, 2015
 *     Author: Pierre Dubath
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

#include <cstdint>  // for int64_t
#include <string>   // for string

#include "ElementsExamples/ClassExample2.h"
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include "ElementsKernel/Exception.h"

using std::int64_t;
using std::string;

using Elements::Examples::ClassExample2;

// tolerance value to compare floating point numbers
constexpr double EXAMPLE_TOLERANCE = 1e-12;

/*
 * Fixture to compare the test result against reference values
 */
struct ClassExample2Fixture {

  string  static_string{"This is a static field example"};
  int64_t source_id{123456789};
  double  ra{266.40506655};
  double  input_variable{1.273645899};
  double  expected_result{1.273645899};

  ClassExample2 example_class = ClassExample2::factoryMethod(source_id, ra);

  ClassExample2Fixture() {
    // call constructor if needed
  }

  ~ClassExample2Fixture() {
    // delete fixture object if needed
  }
};

BOOST_AUTO_TEST_SUITE(ClassExample2TestSuite)

BOOST_AUTO_TEST_CASE(WithoutFixture) {
  BOOST_CHECK(true);
}

BOOST_FIXTURE_TEST_CASE(fundamentalTypeMethod_test, ClassExample2Fixture) {
  BOOST_CHECK_CLOSE(expected_result, example_class.fundamentalTypeMethod(input_variable), EXAMPLE_TOLERANCE);
}

BOOST_FIXTURE_TEST_CASE(Getter_test, ClassExample2Fixture) {
  BOOST_CHECK_EQUAL(source_id, example_class.getSourceId());
}

BOOST_FIXTURE_TEST_CASE(exception_in_divideNumbers_test, ClassExample2Fixture) {
  //
  BOOST_CHECK_EXCEPTION(example_class.divideNumbers(1.0, 0.0), Elements::Exception,
                        // below is a lambda function used as a predicate to check the exception error message
                        [](const Elements::Exception& e) {
                          string exception_str = e.what();
                          return exception_str.find("exception in ClassExample2::divideNumbers") != string::npos;
                        });
}

BOOST_AUTO_TEST_SUITE_END()
