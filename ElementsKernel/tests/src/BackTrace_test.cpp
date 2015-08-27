/**
 * @file BackTrace_test.cpp
 *
 * @date Aug 27, 2015
 * @author hubert
 */

#include "ElementsKernel/System.h"

#include <string>
#include <boost/test/unit_test.hpp>


// Temporary includes for dev
#include <iostream>

void second(){

}

void first() {
  second();
}


BOOST_AUTO_TEST_SUITE(BackTrace_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Raw_test) {

  const size_t depth = 21;
  void* addresses[depth];
  std::string lib, fnc;

  int a = Elements::System::backTrace(addresses, depth);

  BOOST_CHECK_EQUAL(a,17);

  void* addr = nullptr;
  Elements::System::getStackLevel(addresses[2], addr, fnc, lib);

  BOOST_CHECK_EQUAL(lib, "BackTrace_test");

}




//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

