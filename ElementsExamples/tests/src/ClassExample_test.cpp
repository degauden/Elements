/**
 * @file ClassExample_test.cpp
 *
 * Created on: Aug 12, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "ElementsKernel/ElementsException.h"
#include "ElementsExamples/ClassExample.h"
using namespace std;

//-----------------------------------------------------------------------------

// tolerance value to compare floating point numbers
double tolerance = 1e-10;

/*
 * Fixture to compare the test result against reference values
 */
struct ClassExampleFixture {

  ClassExample* classExamplePtr;

  // Some numbers to feed the constructor
  const int64_t sourceId = 16253;
  const double ra = 64.5768;
  const double dec = -34.2857;

  // expected static string (hard coded in .cpp file!)
  std::string expectedStaticString = "This is a static field example";

  // Numbrs to test the methods
  const double firstNumber = 2.5647;
  const double secondNumber = 5.6874;

  const double expectedSum = 8.2521;
  const double expectedDivisionResult = 0.45094419242536132;
  const double expectedFinalResult = expectedDivisionResult;


  ClassExampleFixture() {
    // call the constructor
    classExamplePtr = new ClassExample(sourceId, ra, dec);
  }

  ~ClassExampleFixture() {
    // delete fixture object
    delete classExamplePtr;
  }

  //ClassExampleFixture(const ClassExampleFixture&) = delete;

};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (ClassExample_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( constructors_test, ClassExampleFixture ) {
  BOOST_CHECK(classExamplePtr);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( getters_test, ClassExampleFixture ) {
  //
  BOOST_CHECK(sourceId == classExamplePtr->getSourceId());
  BOOST_CHECK(ra == classExamplePtr->getRa());
  BOOST_CHECK(dec == classExamplePtr->getDec());
  BOOST_CHECK(expectedStaticString == classExamplePtr->getStaticString());
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( computeSum_test, ClassExampleFixture ) {
  //
  double actualSum = classExamplePtr->computeSum(firstNumber, secondNumber);
  BOOST_CHECK_CLOSE(actualSum, expectedSum, tolerance);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( divideNumbers_test, ClassExampleFixture ) {
  //
  double actualDivisionResult = classExamplePtr->divideNumbers(firstNumber, secondNumber);
  BOOST_CHECK_CLOSE(actualDivisionResult, expectedDivisionResult, tolerance);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( divideNumbersByZeroException_test, ClassExampleFixture ) {
  //
  bool exception = false;
  try {
    classExamplePtr->divideNumbers(firstNumber, 0.0);
  } catch (const ElementsException & e) {
    //exception = true;
    string exception_str = e.what();
    exception =
        (exception_str.find(
            "exception in divideNumbers")
            != string::npos);
  }
  BOOST_CHECK(exception);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( summingAndDividing_test, ClassExampleFixture ) {
  //
  classExamplePtr->summingAndDividing(firstNumber, secondNumber);
  BOOST_CHECK_CLOSE(classExamplePtr->getResult(), expectedFinalResult, tolerance);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( summingAndDividingByZeroException_test, ClassExampleFixture ) {
  //
  bool exception = false;
  try {
    classExamplePtr->summingAndDividing(firstNumber, 0.0);
  } catch (const ElementsException & e) {
    //exception = true;
    string exception_str = e.what();
    exception =
        (exception_str.find(
            "exception in divideNumbers")
            != string::npos);
  }
  BOOST_CHECK(exception);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
