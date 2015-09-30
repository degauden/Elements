/*
 * Inversion_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: fdubath
 */

//#include <cstddef>                    // for std::size_t
//#include <vector>                     // for std::vector
#include <boost/test/unit_test.hpp>   // Gives access to the unit test framework.

#include "ElementsExamples/UnitTestExample.h" // Access the objects you want to test
#include "ElementsKernel/Exception.h" // Exception


using namespace Elements::ElementsExamples;


// Starts a test suite and name it.
BOOST_AUTO_TEST_SUITE (UnitTestExample_test_suite)

BOOST_AUTO_TEST_CASE(exception_test) {
  UnitTestExample tested_object{};
  std::vector<int> input_vec {};

  BOOST_CHECK_THROW(tested_object.average(input_vec),Elements::Exception);
}

BOOST_AUTO_TEST_CASE(median_test) {
  UnitTestExample tested_object{};
  std::vector<int> input_vec {1, 2, 3, 11, 101, 102, 103};
  double expected_result {11};

  BOOST_CHECK_EQUAL(expected_result, tested_object.average(input_vec));
}

BOOST_AUTO_TEST_CASE(mean_test) {
  UnitTestExample tested_object{};
  std::vector<int> input_vec {1, 2, 14};
  double expected_result {5.666666666666};
  double tolerence {1e-10};

  BOOST_CHECK_CLOSE(expected_result, tested_object.average(input_vec), tolerence);
}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END ()
