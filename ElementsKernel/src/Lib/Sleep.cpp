#include "ElementsKernel/Sleep.h"

#include "boost/version.hpp"

#if BOOST_VERSION < 105000
#include "boost/thread/xtime.hpp"
#endif
#include "boost/thread/thread.hpp"

namespace Elements {

/// Simple sleep function taken from COOL.
/// @author Marco Clemencic
void normalSleep(int sec) {
#if BOOST_VERSION >= 105000
  boost::this_thread::sleep_for(boost::chrono::seconds(sec));
#else
  using namespace boost;
  xtime t;
  if (xtime_get(&t, TIME_UTC) == TIME_UTC) {
    t.sec += sec;
    thread::sleep(t);
  }
  ///@todo (MCl) do something if cannot get the time.
#endif
}

/** @brief Small variation on the sleep function for nanoseconds sleep.
 * @author Marco Clemencic
 */
void nanoSleep(int64_t nsec) {
#if BOOST_VERSION >= 105000
  boost::this_thread::sleep_for(boost::chrono::nanoseconds(nsec));
#else
  using namespace boost;
  xtime t;
  if (xtime_get(&t, TIME_UTC) == TIME_UTC) {
    t.sec += nsec / 1000000000;
    t.nsec += nsec % 1000000000;
    thread::sleep(t);
  }
  /// @todo (MCl) do something if cannot get the time.
#endif
}

}
