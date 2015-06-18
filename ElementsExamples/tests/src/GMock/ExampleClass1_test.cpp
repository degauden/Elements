#include <vector>
#include <boost/test/unit_test.hpp> // Gives access to the unit test framework.
#include "ElementsKernel/Real.h" // isEqual
#include "ElementsExamples/ExampleClass1.h" // Access the objects you want to test


using namespace Elements;

/* The Fixture structure allow you to setup some data for your tests. A new instance of the fixture is created when the test begin and member of the fixture can be access directly in the test without any prefix. */
struct ExampleClass1_Fixture {
  double flux=10.0;
  double lambda=7000.0;
  std::vector<double> z_range={0,0.0001,0.001,0.01,0.1,0.2,0.5,1.0,2.0,4.0,6.0,10.0,100.0};
};

// Starts a test suite and name it.
BOOST_AUTO_TEST_SUITE (ExampleClass1_test_suite)

// Add a test named 'null_redshift_test' using the MyTestUnit_Fixture to the test suite.
BOOST_FIXTURE_TEST_CASE(cst_test, ExampleClass1_Fixture) {
  // Main constructor
  auto test_object = ExampleClass1(flux,lambda);

  // Copy constructor / assignment
  auto copied_object(test_object);
  auto copied_object_assigned = test_object;

  // Move constructor / assignment
  auto moved_object(std::move(test_object));
  auto moved_object_assigned = std::move(moved_object);
}

BOOST_FIXTURE_TEST_CASE(z_zero_test, ExampleClass1_Fixture) {
  auto test_object = ExampleClass1(flux,lambda);
  auto result = test_object.getRedshiftedValue(0.0);

  BOOST_CHECK(Elements::isEqual(result.first,flux));
  BOOST_CHECK(Elements::isEqual(result.second,lambda));
}

BOOST_FIXTURE_TEST_CASE(z_range_test, ExampleClass1_Fixture) {
  auto test_object = ExampleClass1(flux,lambda);
  for(auto z:z_range){
      auto redshifted_flux = flux/((1.0+z)*(1.0+z));
      auto redshifted_lambda = lambda*(1.0+z);
      auto result = test_object.getRedshiftedValue(z);

      BOOST_CHECK(Elements::isEqual(result.first,redshifted_flux));
      BOOST_CHECK(Elements::isEqual(result.second,redshifted_lambda));
  }
}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END ()


