/**
 * @file ThisModule_test.cpp
 *
 * @date Dec 10, 2014
 * @author hubert
 */

#include "ElementsKernel/ThisModule.h"

#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace std;

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE (ThisModule_test)
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ThisModuleName_test) {

  // Get the present module. Here this must be the test executable
  BOOST_CHECK_EQUAL(Elements::System::getThisModuleName(), "ThisModule_test");


}



//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END ()
//-----------------------------------------------------------------------------
//
// End of the Boost tests
