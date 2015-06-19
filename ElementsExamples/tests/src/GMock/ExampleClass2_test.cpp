
#include "ElementsKernel/Real.h" // isEqual

#include "ElementsExamples/ExampleClass2.h" // Access the objects you want to test
#include "ExampleClass1Mock.h" // Access the needed mock objects.

using namespace Elements;

/* The Fixture structure allow you to setup some data for your tests. A new instance of the fixture is created when the test begin and member of the fixture can be access directly in the test without any prefix. */
struct ExampleClass2_Fixture {
public:
  ExampleClass2_Fixture() {

  }


  std::vector<double> zs { 0., 1., 2., 3., 4., 5. };
  ExampleClass1Mock class1_mock;


};

// Starts a test suite and name it.
BOOST_AUTO_TEST_SUITE (ExampleClass2_test_suite)

BOOST_FIXTURE_TEST_CASE(getMap_test, ExampleClass2_Fixture) {

  // programming the mock behavior: we expect one call for each value of z
  // order of the calls is unimportant, for later use we output a pair (z,z)
  for (auto& z : zs) {
    EXPECT_CALL(class1_mock, getRedshiftedValue(z)).Times(1).WillOnce(
        testing::Return(std::make_pair(z, z)));


  }

  ExampleClass2 class2 { class1_mock, zs };
  auto map = class2.getRedshiftedMap();

  // Check the map size
  BOOST_CHECK_EQUAL(map.size(), zs.size());

  // Check all the z are present as key
  for (auto& z : zs) {
    BOOST_CHECK(map.find(z) != map.end());
  }

  // Check the values associated to the keys are the right ones
  for (auto& item : map) {
    BOOST_CHECK(Elements::isEqual(item.first, item.second.first));
    BOOST_CHECK(Elements::isEqual(item.first, item.second.second));
  }


}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END ()

