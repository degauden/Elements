/**
 * @file ThisModule_test.cpp
 *
 * @date Dec 10, 2014
 * @author hubert
 */

#include "ElementsKernel/ThisModule.h"

#include <boost/test/unit_test.hpp>
#include <libgen.h>


using Elements::System::getThisModuleInfo;
using Elements::System::getThisExecutableInfo;

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(ThisModule_test)
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ThisModuleName_test) {

  // Get the present module. Here this must be the test executable
  BOOST_CHECK_EQUAL(getThisModuleInfo().name(), "ThisModule_test");

}

BOOST_AUTO_TEST_CASE(ThisExeName_test) {

  // Get the present module. Here this must be the test executable

  BOOST_CHECK_EQUAL(getThisExecutableInfo().name(), "ThisModule_test");

}

BOOST_AUTO_TEST_CASE(ThisModuleConversion_test) {

  Dl_info info = getThisModuleInfo();

  BOOST_CHECK_EQUAL(::basename(const_cast<char*>(info.dli_fname)), "ThisModule_test");

}


//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-----------------------------------------------------------------------------
//
// End of the Boost tests
