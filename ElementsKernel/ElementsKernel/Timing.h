#ifndef ELEMENTSKERNEL_TIMING_H
#define ELEMENTSKERNEL_TIMING_H

#include <cstdint>
// Framework include files
#include "ElementsKernel/Export.h" // ELEMENTS_API
#include "ElementsKernel/SystemBase.h"

/** Note: OS specific details for process timing

 Entrypoints:
 - remainingTime returns the time the process could still execute
 - ellapsedTime: returns elapsed time since program start
 - kernelTime:   returns the amount of time the process has spent in kernel mode
 - userTime:     returns the amount of time the process has spent in user mode
 - cpuTime:      returns kernel+user time


 On Windows NT Time is expressed as
 the amount of time that has elapsed since midnight on
 January 1, 1601 at Greenwich, England.

 On Unix time is expressed as
 the amount of time that has elapsed since midnight on
 January 1, 1970 at Greenwich, England.

 <P> History    :

 <PRE>
 +---------+----------------------------------------------+--------+
 |    Date |                 Comment                      | Who    |
 +---------+----------------------------------------------+--------+
 | 11/11/00| Initial version.                             | MF     |
 +---------+----------------------------------------------+--------+
 </PRE>
 @author:  M.Frank
 @version: 1.0
 */
namespace Elements {
namespace System {
/// Time type for conversion
enum TimeType {
  Year, Month, Day, Hour, Min, Sec, milliSec, microSec, nanoSec, Native
};
/// Convert time from OS native time to requested representation (Experts only)
ELEMENTS_API int64_t adjustTime(TimeType typ, int64_t timevalue);

/// Convert the time from OS native time to requested representation (Experts only)
template<TimeType T>
/** Adjust time
 * @param timevalue Time value to be converted.
 */inline int64_t adjustTime(int64_t timevalue);
/** Elapsed time since start of process in milliseconds.
 @param typ     Indicator or the unit the time will be returned.
 @param fetch   Indicator of the information to be fetched.
 @param pid     Process ID of which the information will be returned
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t ellapsedTime(TimeType typ = milliSec, InfoType fetch = InfoType::Times, long pid = -1);
/** CPU kernel mode time of process in milliseconds.
 @param typ     Indicator or the unit the time will be returned.
 @param fetch   Indicator of the information to be fetched.
 If Fetch_None, the information will not be updated.
 @param pid     Process ID of which the information will be returned
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t kernelTime(TimeType typ = milliSec, InfoType fetch = InfoType::Times, long pid = -1);
/** CPU user mode time of process in milliseconds.
 @param typ     Indicator or the unit the time will be returned.
 @param fetch   Indicator of the information to be fetched.
 If Fetch_None, the information will not be updated.
 @param pid     Process ID of which the information will be returned
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t userTime(TimeType typ = milliSec, InfoType fetch = InfoType::Times, long pid = -1);
/** Consumed CPU time of process in milliseconds.
 @param typ     Indicator or the unit the time will be returned.
 @param fetch   Indicator of the information to be fetched.
 If Fetch_None, the information will not be updated.
 @param pid     Process ID of which the information will be returned
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t cpuTime(TimeType typ = milliSec, InfoType fetch = InfoType::Times, long pid = -1);
/** Maximum processing time left for this process.
 @param typ     Indicator or the unit the time will be returned.
 @param fetch   Indicator of the information to be fetched.
 If Fetch_None, the information will not be updated.
 @param pid     Process ID of which the information will be returned
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t remainingTime(TimeType typ = milliSec, InfoType fetch = InfoType::Quota, long pid = -1);
/** Process Creation time.
 @param typ     Indicator or the unit the time will be returned.
 @param fetch   Indicator of the information to be fetched.
 If Fetch_None, the information will not be updated.
 @param pid     Process ID of which the information will be returned
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t creationTime(TimeType typ = milliSec, InfoType fetch = InfoType::Times, long pid = -1);
/** Maximum processing time left for this process.
 @param typ     Indicator or the unit the time will be returned.
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t systemStart(TimeType typ = Sec);
/** Maximum processing time left for this process.
 @param typ     Indicator or the unit the time will be returned.
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t upTime(TimeType typ = Hour);
/** Retrieve absolute system time
 @param typ     Indicator or the unit the time will be returned.
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t currentTime(TimeType typ = milliSec);
/** Retrieve the number of ticks since system startup
 @return        Requested value in the indicated units.
 */
ELEMENTS_API int64_t tickCount();

/** Simple class to hold the time information of a process.
 *
 * Simplify the simultaneous handling of kernel, user and elapsed times of a
 * process.
 *
 * \see {<a href="http://savannah.cern.ch/bugs/?87341">bug #87341</a>}
 */
class ProcessTime {
public:
  typedef int64_t TimeValueType;

  /// Constructor
  ProcessTime() :
      i_kernel(0), i_user(0), i_elapsed(0) {
  }

  /// Constructor
  ProcessTime(TimeValueType k, TimeValueType u, TimeValueType e) :
      i_kernel(k), i_user(u), i_elapsed(e) {
  }

  /// Retrieve the kernel time in the requested unit.
  template<TimeType T>
  inline TimeValueType kernelTime() const {
    return adjustTime<T>(i_kernel);
  }

  /// Retrieve the user time in the requested unit.
  template<TimeType T>
  inline TimeValueType userTime() const {
    return adjustTime<T>(i_user);
  }

  /// Retrieve the elapsed time in the requested unit.
  template<TimeType T>
  inline TimeValueType elapsedTime() const {
    return adjustTime<T>(i_elapsed);
  }

  /// Retrieve the CPU (user+kernel) time in the requested unit.
  template<TimeType T>
  inline TimeValueType cpuTime() const {
    return adjustTime<T>(i_user + i_kernel);
  }

  /// Return the delta between two \c ProcessTime objects.
  inline ProcessTime operator-(const ProcessTime &rhs) const {
    return ProcessTime(i_kernel - rhs.i_kernel, i_user - rhs.i_user, i_elapsed - rhs.i_elapsed);
  }
private:
  /// Internal storage.
  TimeValueType i_kernel, i_user, i_elapsed;
};
/** Retrieve the process time data for a process.
 *
 * Get the process time data for a process (by default the current) as a
 * \c ProcessTime object.
 */ELEMENTS_API ProcessTime getProcessTime(long pid = -1);

}
}

// implementation of the templated adjustTime
namespace Elements {
namespace System {
template<>
inline int64_t adjustTime<Year>(int64_t t) {
  return (t == -1) ? t : t /= (1LL * 365 * 24 * 60 * 60 * 1000 * 1000 * 10);
}
template<>
inline int64_t adjustTime<Day>(int64_t t) {
  return (t == -1) ? t : t /= (1LL * 24 * 60 * 60 * 1000 * 1000 * 10);
}
template<>
inline int64_t adjustTime<Hour>(int64_t t) {
  return (t == -1) ? t : t /= (1LL * 60 * 60 * 1000 * 1000 * 10);
}
template<>
inline int64_t adjustTime<Min>(int64_t t) {
  return (t == -1) ? t : t /= (60 * 1000 * 1000 * 10);
}
template<>
inline int64_t adjustTime<Sec>(int64_t t) {
  return (t == -1) ? t : t /= (1000 * 1000 * 10);
}
template<>
inline int64_t adjustTime<milliSec>(int64_t t) {
  return (t == -1) ? t : t /= (1000 * 10);
}
template<>
inline int64_t adjustTime<microSec>(int64_t t) {
  return (t == -1) ? t : t /= (10LL);
}
template<>
inline int64_t adjustTime<nanoSec>(int64_t t) {
  return (t == -1) ? t : t *= 100LL;
}
template<>
inline int64_t adjustTime<Month>(int64_t t) {
  return (t == -1) ? t : t /= (1LL * 30 * 24 * 60 * 60 * 1000 * 1000 * 10);
}
template<>
inline int64_t adjustTime<Native>(int64_t t) {
  return t;
}
}
}

#endif    // ELEMENTSKERNEL_TIMING_H
