/**
 * @file BackTrace_test.cpp
 *
 * @date Aug 27, 2015
 * @author hubert
 */

#include "ElementsKernel/System.h"

#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>


// Temporary includes for dev
#include <iostream>

using namespace std;

void second(){

}

void first() {
  second();
}


BOOST_AUTO_TEST_SUITE(BackTrace_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Raw_test) {

  const size_t depth = 21;


  vector<string> trace = Elements::System::backTrace(depth);

  size_t found = trace[0].find("BackTrace_test");

  BOOST_CHECK_NE(found, string::npos);



}




//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

