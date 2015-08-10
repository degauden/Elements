/**
 * @file Number_test.cpp
 *
 *  Created on: May 22, 2013
 *      Author: Hubert Degaudenzi
 */

#include <boost/test/unit_test.hpp>
#include "ElementsKernel/Number.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(Number_test)

BOOST_AUTO_TEST_CASE(NumberCast_test) {

  using Elements::numberCast;

  BOOST_CHECK_EQUAL(numberCast<int>(3.6), 4);
  BOOST_CHECK_EQUAL(static_cast<int>(3.6), 3);
  BOOST_CHECK_EQUAL(numberCast<int>(3.2), 3);


}

//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END()
