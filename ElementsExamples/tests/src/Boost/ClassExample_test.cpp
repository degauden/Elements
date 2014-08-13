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
#include "ElementsKernel/Real.h" // Provides isEqual

#include "tests/src/Tolerance.h"

using namespace std;

//-----------------------------------------------------------------------------

/*
 * Fixture to compare the test result against reference values
 */
struct ClassExampleFixture {

  ClassExample* m_class_example_ptr { nullptr };

  // Some numbers to feed the constructor
  const int64_t m_source_id { 16253 };
  const double m_ra  { 64.5768 };
  const double m_dec { -34.2857 };

  // expected static string (hard coded in .cpp file!)
  std::string m_expected_static_string = "This is a static field example";

  // Numbrs to test the methods
  const double m_first_number { 2.5647 };
  const double m_second_number { 5.6874 };

  const double m_expected_sum { 8.2521 };
  const double m_expected_division_result { 0.45094419242536132 };
  const double m_expected_final_result { m_expected_division_result };


  ClassExampleFixture() {
    // call the constructor
    m_class_example_ptr = new ClassExample(m_source_id, m_ra, m_dec);
  }

  ClassExampleFixture(const ClassExampleFixture& other)
  : m_class_example_ptr(new ClassExample(*(other.m_class_example_ptr))){
  }

  ~ClassExampleFixture() {
    // delete fixture object
    delete m_class_example_ptr;
  }

  //ClassExampleFixture(const ClassExampleFixture&) = delete;

};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (ClassExample_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( constructors_test, ClassExampleFixture ) {
  BOOST_CHECK(m_class_example_ptr);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( getters_test, ClassExampleFixture ) {
  //
  BOOST_CHECK(m_source_id == m_class_example_ptr->getSourceId());
  BOOST_CHECK_CLOSE(m_ra, m_class_example_ptr->getRa(), TEST_DOUBLE_TOLERANCE);
  BOOST_CHECK_CLOSE(m_dec, m_class_example_ptr->getDec(), TEST_DOUBLE_TOLERANCE);
  BOOST_CHECK(isEqual(m_ra, m_class_example_ptr->getRa()));
  BOOST_CHECK(isEqual(m_dec, m_class_example_ptr->getDec()));
  BOOST_CHECK(m_expected_static_string == m_class_example_ptr->getStaticString());
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( computeSum_test, ClassExampleFixture ) {
  //
  double actualSum = m_class_example_ptr->computeSum(m_first_number, m_second_number);
  BOOST_CHECK_CLOSE(actualSum, m_expected_sum, TEST_DOUBLE_TOLERANCE);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( divideNumbers_test, ClassExampleFixture ) {
  //
  double actualDivisionResult = m_class_example_ptr->divideNumbers(m_first_number, m_second_number);
  BOOST_CHECK_CLOSE(actualDivisionResult, m_expected_division_result, TEST_DOUBLE_TOLERANCE);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( divideNumbersByZeroException_test, ClassExampleFixture ) {
  //
  bool exception = false;
  try {
    m_class_example_ptr->divideNumbers(m_first_number, 0.0);
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
  m_class_example_ptr->summingAndDividing(m_first_number, m_second_number);
  BOOST_CHECK_CLOSE(m_class_example_ptr->getResult(), m_expected_final_result, TEST_DOUBLE_TOLERANCE);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( summingAndDividingByZeroException_test, ClassExampleFixture ) {
  //
  bool exception = false;
  try {
    m_class_example_ptr->summingAndDividing(m_first_number, 0.0);
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
