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

  ClassExample* class_example_ptr;

  // Some numbers to feed the constructor
  const int64_t source_id = 16253;
  const double ra = 64.5768;
  const double dec = -34.2857;

  // expected static string (hard coded in .cpp file!)
  std::string expected_static_string = "This is a static field example";

  // Numbrs to test the methods
  const double first_number = 2.5647;
  const double second_number = 5.6874;

  const double expected_sum = 8.2521;
  const double expected_division_result = 0.45094419242536132;
  const double expected_final_result = expected_division_result;


  ClassExampleFixture() {
    // call the constructor
    class_example_ptr = new ClassExample(source_id, ra, dec);
  }

  ClassExampleFixture(const ClassExampleFixture& other)
  : class_example_ptr(new ClassExample(*(other.class_example_ptr))){
  }

  ~ClassExampleFixture() {
    // delete fixture object
    delete class_example_ptr;
  }

  //ClassExampleFixture(const ClassExampleFixture&) = delete;

};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (ClassExample_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( constructors_test, ClassExampleFixture ) {
  BOOST_CHECK(class_example_ptr);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( getters_test, ClassExampleFixture ) {
  //
  BOOST_CHECK(source_id == class_example_ptr->getSourceId());
  BOOST_CHECK(ra == class_example_ptr->getRa());
  BOOST_CHECK(dec == class_example_ptr->getDec());
  BOOST_CHECK(expected_static_string == class_example_ptr->getStaticString());
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( computeSum_test, ClassExampleFixture ) {
  //
  double actualSum = class_example_ptr->computeSum(first_number, second_number);
  BOOST_CHECK_CLOSE(actualSum, expected_sum, tolerance);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( divideNumbers_test, ClassExampleFixture ) {
  //
  double actualDivisionResult = class_example_ptr->divideNumbers(first_number, second_number);
  BOOST_CHECK_CLOSE(actualDivisionResult, expected_division_result, tolerance);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( divideNumbersByZeroException_test, ClassExampleFixture ) {
  //
  bool exception = false;
  try {
    class_example_ptr->divideNumbers(first_number, 0.0);
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
  class_example_ptr->summingAndDividing(first_number, second_number);
  BOOST_CHECK_CLOSE(class_example_ptr->getResult(), expected_final_result, tolerance);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( summingAndDividingByZeroException_test, ClassExampleFixture ) {
  //
  bool exception = false;
  try {
    class_example_ptr->summingAndDividing(first_number, 0.0);
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
