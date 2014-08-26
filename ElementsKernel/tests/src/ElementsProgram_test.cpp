/**
 * @file ElementsProgram_test.cpp
 *
 * Created on: Aug 13, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */


#include <iostream>
#include <boost/test/unit_test.hpp>
#include "ElementsKernel/Exception.h"
//#include "ElementsExamples/ClassExample.h"
//#include "ElementsExamples/ElementsExamples/src/Program/ElementsProgramExample.cpp"
using namespace std;

//-----------------------------------------------------------------------------


/*
 * Fixture to compare the test result against reference values
 */
struct ElementsProgramFixture {

//  ElementsProgramExample* elementsProgramExample_ptr;




  ElementsProgramFixture() {
    // call the constructor
    //elementsProgramExample_ptr = new ElementsProgramExample();
  }

  ~ElementsProgramFixture() {
    // delete fixture object
    //delete elementsProgramExample_ptr;
  }

  //ClassExampleFixture(const ClassExampleFixture&) = delete;

};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (ElementsProgram_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( constructors_test, ElementsProgramFixture ) {
  //BOOST_CHECK(elementsProgramExample_ptr);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( getters_test, ElementsProgramFixture ) {
  //
  BOOST_CHECK(true);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
