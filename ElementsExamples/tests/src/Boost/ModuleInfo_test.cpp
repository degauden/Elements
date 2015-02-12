/**
 * @file ClassExample_test.cpp
 *
 * Created on: Aug 12, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>

#include "ElementsExamples/ModuleInfo.h"

using namespace Elements;
using namespace std;

//-----------------------------------------------------------------------------

/*
 * Fixture to compare the test result against reference values
 */
struct ModuleInfoFixture {

  ModuleInfoFixture() {
    // call the constructor
  }


  ~ModuleInfoFixture() {
    // delete fixture object
  }


};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (ModuleInfo_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(ModuleName_test, ModuleInfoFixture ) {
  BOOST_CHECK_EQUAL(getModuleInfo().name(), string("libElementsExamples.so"));
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
