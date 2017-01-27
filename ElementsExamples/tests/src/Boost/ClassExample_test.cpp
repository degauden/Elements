/**
 * @file ClassExample_test.cpp
 *
 * Created on: January 9, 2015
 *     Author: Pierre Dubath
 */

#include <cstdint>                          // for std::int64_t

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "ElementsKernel/Exception.h"

// class under test
#include "ElementsExamples/ClassExample.h"

using std::string;
using std::int64_t;

using Elements::Examples::ClassExample;

// tolerance value to compare floating point numbers
double tolerance = 1e-12;

/*
 * Fixture to compare the test result against reference values
 */
struct ClassExampleFixture {

  string static_string { "This is a static field example" };
  int64_t source_id { 123456789 };
  double ra { 266.40506655 };
  double input_variable { 1.273645899 };
  double expected_result { 1.273645899 };

  ClassExample example_class = ClassExample::factoryMethod(source_id,
      ra);

  ClassExampleFixture() {
    //call constructor if needed
  }

  ~ClassExampleFixture() {
    // delete fixture object if needed
  }
};

BOOST_AUTO_TEST_SUITE (ClassExampleTestSuite)

BOOST_AUTO_TEST_CASE(WithoutFixture) {
  BOOST_CHECK(true);
}

BOOST_FIXTURE_TEST_CASE(fundamentalTypeMethod_test, ClassExampleFixture) {
  BOOST_CHECK_CLOSE(expected_result,
      example_class.fundamentalTypeMethod(input_variable), tolerance);
}

BOOST_FIXTURE_TEST_CASE(Getter_test, ClassExampleFixture) {
  BOOST_CHECK_EQUAL(source_id, example_class.getSourceId());
}

BOOST_FIXTURE_TEST_CASE(exception_in_divideNumbers_test, ClassExampleFixture ) {
  //
  BOOST_CHECK_EXCEPTION(example_class.divideNumbers(1.0, 0.0), Elements::Exception,
      // below is a lambda function used as a predicate to check the exception error message
      [](const Elements::Exception& e){
            string exception_str = e.what();
            return exception_str.find("exception in ClassExample::divideNumbers") != string::npos;
      }
  );
}

BOOST_AUTO_TEST_SUITE_END ()

