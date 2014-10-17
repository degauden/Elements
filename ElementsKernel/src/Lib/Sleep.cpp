#include "ElementsKernel/Sleep.h"

#include <boost/thread/thread.hpp>

namespace Elements {

/// Simple sleep function taken from COOL.
/// @author Marco Clemencic
void normalSleep(int sec) {
  boost::this_thread::sleep_for(boost::chrono::seconds(sec));
}

/** @brief Small variation on the sleep function for nanoseconds sleep.
 * @author Marco Clemencic
 */
void nanoSleep(int64_t nsec) {
  boost::this_thread::sleep_for(boost::chrono::nanoseconds(nsec));
}

}
