/**
 * @file ElementsTime_test.cpp
 *
 * Created on: Aug 7, 2013
 *     Author: Pavel Binko
 */

#include "ElementsKernel/Time.h"        // for Elements::Time

#include <string>                       // for std::string
#include <boost/test/unit_test.hpp>

using std::string;
using Elements::Time;


//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------


BOOST_AUTO_TEST_SUITE(ElementsTime_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(nanoFormat_test) {


  Time t(2011, 0, 13, 14, 22, 45, 123000, true);
  BOOST_CHECK(string("000123") == t.nanoformat());
  BOOST_CHECK(string("000123000") == t.nanoformat(9));
  BOOST_CHECK(string("000123") == t.nanoformat(2, 7));
  BOOST_CHECK(string("0") == t.nanoformat(1, 3));
  BOOST_CHECK(string("00012") == t.nanoformat(1, 5));
  BOOST_CHECK(string("000") == t.nanoformat(3, 3));

}

BOOST_AUTO_TEST_CASE(format_test) {

  Time t(2011, 0, 13, 14, 22, 45, 1230000, true);

  BOOST_CHECK(string("2011-01-13 14:22:45") == t.format(true, "%Y-%m-%d %H:%M:%S"));
  BOOST_CHECK(string("2011-01-13 14:22:45.001") == t.format(true, "%Y-%m-%d %H:%M:%S.%f"));
  BOOST_CHECK(string("14:22:45 2011/01/13 ") == t.format(true, "%H:%M:%S %Y/%m/%d "));

}



//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
