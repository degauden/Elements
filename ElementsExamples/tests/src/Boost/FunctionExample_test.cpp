/**
 * @file functionExample_test.cpp
 *
 * Created on: Feb 22nd, 2017
 *     Author: Pierre Dubath
 */

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

// function under test
#include "ElementsExamples/functionExample.h"


using Elements::Examples::functionExample;

BOOST_AUTO_TEST_SUITE (FunctionExampleTestSuite)

BOOST_AUTO_TEST_CASE(Value) {
  BOOST_CHECK(functionExample(3)==9);
}


BOOST_AUTO_TEST_SUITE_END ()

