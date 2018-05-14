/**
 * @file MathConstants_test.cpp
 *
 * @date Dec 10, 2014
 * @author Hubert Degaudenzi
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

#include "ElementsKernel/MathConstants.h"        // The interface to test

#include <cmath>  // for sqrt

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/math/constants/constants.hpp>

#include "ElementsKernel/Real.h"                // for the isEqual function


using Elements::isEqual;

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(MathConstants_test)
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

BOOST_AUTO_TEST_CASE(GccConstants_test) {

  using Elements::Units::pi;
  using Elements::Units::e;
  using Elements::Units::halfpi;
  using Elements::Units::sqrt_of_two;
  using Elements::Units::sqrt_of_pi;

  BOOST_CHECK(isEqual(pi, M_PI));
  BOOST_CHECK(isEqual(e, M_E));
  BOOST_CHECK(isEqual(halfpi, M_PI_2));
  BOOST_CHECK(isEqual(sqrt_of_two, M_SQRT2));

  using std::log10;
  using std::log2;
  using std::log;

  BOOST_CHECK(isEqual(log10(e), M_LOG10E));
  BOOST_CHECK(isEqual(log2(e), M_LOG2E));
  BOOST_CHECK(isEqual(log(2.0), M_LN2));
  BOOST_CHECK(isEqual(log(10.0), M_LN10));
  BOOST_CHECK(isEqual(halfpi/2.0, M_PI_4));
  BOOST_CHECK(isEqual(1.0/pi, M_1_PI));
  BOOST_CHECK(isEqual(2.0/pi, M_2_PI));
  BOOST_CHECK(isEqual(2.0/sqrt_of_pi, M_2_SQRTPI));
  BOOST_CHECK(isEqual(1.0/sqrt_of_two, M_SQRT1_2));

}

BOOST_AUTO_TEST_CASE(BoostConstants_test) {

  namespace mc = boost::math::constants;

  using Elements::Units::pi;
  using Elements::Units::e;
  using Elements::Units::halfpi;
  using Elements::Units::sqrt_of_two;
  using Elements::Units::gamma;
  using Elements::Units::pi2;

  BOOST_CHECK(isEqual(pi, mc::pi<double>()));
  BOOST_CHECK(isEqual(e, mc::e<double>()));
  BOOST_CHECK(isEqual(halfpi, mc::half_pi<double>()));
  BOOST_CHECK(isEqual(sqrt_of_two, mc::root_two<double>()));
  BOOST_CHECK(isEqual(gamma, mc::euler<double>()));
  BOOST_CHECK(isEqual(log(2.0), mc::ln_two<double>()));
  BOOST_CHECK(isEqual(pi2, mc::pi_sqr<double>()));



}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-----------------------------------------------------------------------------
//
// End of the Boost tests
