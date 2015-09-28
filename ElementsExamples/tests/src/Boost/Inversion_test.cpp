/*
 * Inversion_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: fdubath
 */


#include <vector>
#include <boost/test/unit_test.hpp> // Gives access to the unit test framework.
#include "ElementsKernel/Real.h" // isEqual
#include "ElementsKernel/Exception.h" // Exception

#include "ElementsExamples/Inversion.h" // Access the objects you want to test

using namespace Elements::ElementsExamples;


// Starts a test suite and name it.
BOOST_AUTO_TEST_SUITE (Inversion_test_suite)

BOOST_AUTO_TEST_CASE(exception_test) {

  Inversion tested_object{};

  BOOST_CHECK_THROW(tested_object.oneOverAbsX(0.),Elements::Exception);
}

BOOST_AUTO_TEST_CASE(nominal_case_test) {
  Inversion tested_object{};
  std::vector<double> input{-10.,-1.,1.,10.,100.};
  std::vector<double> expected{0.1,1.,1.,0.1,0.01};

  for (size_t index=0; index<input.size(); ++index){
    double computed = tested_object.oneOverAbsX(input[index]);
    BOOST_CHECK(Elements::isEqual(computed,expected[index]));
  }

}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END ()
