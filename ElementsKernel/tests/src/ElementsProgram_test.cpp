/**
 * @file ElementsProgram_test.cpp
 *
 * Created on: Aug 13, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */


#include <iostream>
#include <boost/test/unit_test.hpp>
#include "ElementsKernel/ElementsException.h"
//#include "ElementsExamples/ClassExample.h"
//#include "ElementsExamples/ElementsExamples/src/Program/ElementsProgramExample.cpp"
using namespace std;

//-----------------------------------------------------------------------------

// tolerance value to compare floating point numbers
double tolerance = 1e-10;

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
//  BOOST_CHECK(sourceId == classExamplePtr->getSourceId());
//  BOOST_CHECK(ra == classExamplePtr->getRa());
//  BOOST_CHECK(dec == classExamplePtr->getDec());
//  BOOST_CHECK(expectedStaticString == classExamplePtr->getStaticString());
}

//-----------------------------------------------------------------------------

//BOOST_FIXTURE_TEST_CASE( computeSum_test, ClassExampleFixture ) {
//  //
//  double actualSum = classExamplePtr->computeSum(firstNumber, secondNumber);
//  BOOST_CHECK_CLOSE(actualSum, expectedSum, tolerance);
//}
//
////-----------------------------------------------------------------------------
//
//BOOST_FIXTURE_TEST_CASE( divideNumbers_test, ClassExampleFixture ) {
//  //
//  double actualDivisionResult = classExamplePtr->divideNumbers(firstNumber, secondNumber);
//  BOOST_CHECK_CLOSE(actualDivisionResult, expectedDivisionResult, tolerance);
//}
//
////-----------------------------------------------------------------------------
//
//BOOST_FIXTURE_TEST_CASE( divideNumbersByZeroException_test, ClassExampleFixture ) {
//  //
//  bool exception = false;
//  try {
//    classExamplePtr->divideNumbers(firstNumber, 0.0);
//  } catch (const ElementsException & e) {
//    //exception = true;
//    string exception_str = e.what();
//    exception =
//        (exception_str.find(
//            "exception in divideNumbers")
//            != string::npos);
//  }
//  BOOST_CHECK(exception);
//}
//
////-----------------------------------------------------------------------------
//
//BOOST_FIXTURE_TEST_CASE( summingAndDividing_test, ClassExampleFixture ) {
//  //
//  classExamplePtr->summingAndDividing(firstNumber, secondNumber);
//  BOOST_CHECK_CLOSE(classExamplePtr->getResult(), expectedFinalResult, tolerance);
//}
//
////-----------------------------------------------------------------------------
//
//BOOST_FIXTURE_TEST_CASE( summingAndDividingByZeroException_test, ClassExampleFixture ) {
//  //
//  bool exception = false;
//  try {
//    classExamplePtr->summingAndDividing(firstNumber, 0.0);
//  } catch (const ElementsException & e) {
//    //exception = true;
//    string exception_str = e.what();
//    exception =
//        (exception_str.find(
//            "exception in divideNumbers")
//            != string::npos);
//  }
//  BOOST_CHECK(exception);
//}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
