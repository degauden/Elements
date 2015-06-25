/**
 * @file MathConstants_test.cpp
 *
 * @date Dec 10, 2014
 * @author hubert
 */

#include "ElementsKernel/MathConstants.h" // The interface to test

#include <boost/test/unit_test.hpp>
#include <cmath>  // for sqrt
#include "ElementsKernel/Real.h"   // for the isEqual function


using Elements::isEqual;

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE (MathConstants_test)
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(SqrtOfPi_test) {

  using Elements::Units::sqrt_of_pi;
  using Elements::Units::pi;

  // Get the present module. Here this must be the test executable
  BOOST_CHECK(isEqual(sqrt_of_pi, sqrt(pi)));

}

BOOST_AUTO_TEST_CASE(SqrtOfTwo_test) {

  using Elements::Units::sqrt_of_two;

  // Get the present module. Here this must be the test executable
  BOOST_CHECK(isEqual(sqrt_of_two, sqrt(2.0)));

}

BOOST_AUTO_TEST_CASE(SqrtOfHalfPi_test) {

  using Elements::Units::sqrt_of_halfpi;
  using Elements::Units::pi;

  // Get the present module. Here this must be the test executable
  BOOST_CHECK(isEqual(sqrt_of_halfpi, sqrt(pi/2.0)));

}



//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END ()
//-----------------------------------------------------------------------------
//
// End of the Boost tests
