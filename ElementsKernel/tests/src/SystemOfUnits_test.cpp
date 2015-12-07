/**
 * @file MathConstants_test.cpp
 *
 * @date Dec 10, 2014
 * @author hubert
 */

#include "ElementsKernel/SystemOfUnits.h"  // The interface to test

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/math/constants/constants.hpp>


#include "ElementsKernel/Real.h"   // for the isEqual function


using Elements::isEqual;

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(SystemOfUnits_test)
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(LuminousFlux_test) {

  using Elements::Units::perMillion;
  using Elements::Units::jansky;
  using Elements::Units::microjansky;

  BOOST_CHECK(isEqual(microjansky, jansky*perMillion));
}

BOOST_AUTO_TEST_CASE(Illuminance_test) {

  using Elements::Units::lux;
  using Elements::Units::lumen;
  using Elements::Units::meter2;

  BOOST_CHECK(isEqual(lux, lumen/meter2));
}

BOOST_AUTO_TEST_CASE(Misc_test) {

  using Elements::Units::perCent;
  BOOST_CHECK(isEqual(perCent, 1.0/100.0));

  using Elements::Units::perThousand;
  BOOST_CHECK(isEqual(perThousand, 1.0/1000.0));

  using Elements::Units::perMillion;
  BOOST_CHECK(isEqual(perMillion, 1.0/1.0e6));
}


//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-----------------------------------------------------------------------------
//
// End of the Boost tests
