/**
 * @file ElementsTiming_test.cpp
 *
 * Created on: Aug 7, 2013
 *     Author: Pavel Binko
 */

#include "ElementsKernel/Timing.h"
#include "ElementsKernel/Sleep.h"

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>                     // for std::int64_t

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

//using namespace std;

using std::int64_t;

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(ElementsTiming_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(adjustTime_test)
{
  int64_t t = 12345678901234560LL;  // units of 100 nanoseconds
  // 1234567890123456000 nanoseconds
  //    1234567890123456 microseconds
  //       1234567890123 milliseconds
  //          1234567890 seconds
  //            20576131 minutes
  //              342935 hours
  //               14288 days
  //                 476 months (of 30 days)
  //                  39 years (of 365 days)
  //   12345678901234560 native
  BOOST_CHECK(
      Elements::System::adjustTime<Elements::System::nanoSec>(t) == static_cast<int64_t>(1234567890123456000LL));
  BOOST_CHECK(
      Elements::System::adjustTime<Elements::System::microSec>(t) == static_cast<int64_t>(1234567890123456LL));
  BOOST_CHECK(
      Elements::System::adjustTime<Elements::System::milliSec>(t) == static_cast<int64_t>(1234567890123LL));
  BOOST_CHECK(Elements::System::adjustTime<Elements::System::Sec>(t) == static_cast<int64_t>(1234567890LL));
  BOOST_CHECK(Elements::System::adjustTime<Elements::System::Min>(t) == static_cast<int64_t>(20576131LL));
  BOOST_CHECK(
      Elements::System::adjustTime<Elements::System::Hour>(t) == static_cast<int64_t>(342935LL));
  BOOST_CHECK(Elements::System::adjustTime<Elements::System::Day>(t) == static_cast<int64_t>(14288LL));
  BOOST_CHECK(
      Elements::System::adjustTime<Elements::System::Month>(t) == static_cast<int64_t>(476LL));
  BOOST_CHECK(
      Elements::System::adjustTime<Elements::System::Year>(t) == static_cast<int64_t>(39LL));
  BOOST_CHECK(
      Elements::System::adjustTime<Elements::System::Native>(t) == static_cast<int64_t>(12345678901234560LL));


}

BOOST_AUTO_TEST_CASE(processTime_test)
{
  Elements::System::ProcessTime t0 = Elements::System::getProcessTime();
  Elements::normalSleep(1);
  Elements::System::ProcessTime t1 = Elements::System::getProcessTime();

  // light check on the conversion
  BOOST_CHECK(t1.kernelTime<Elements::System::Sec>() == t1.kernelTime<Elements::System::milliSec>() / 1000);

  // measure the elapsed time
  BOOST_CHECK((t1 - t0).elapsedTime<Elements::System::Sec>() == static_cast<int64_t>(1LL));

}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
