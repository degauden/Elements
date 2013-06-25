// $Id: Time.h,v 1.3 2006/01/25 16:02:48 hmd Exp $
#ifndef ELEMENTSKERNEL_TIME_H
#define ELEMENTSKERNEL_TIME_H 1

// Include files
//   for the architecture independent int64 definition (longlong)
#include "ElementsKernel/Kernel.h"
#include "ElementsKernel/StreamBuffer.h"
#include "ElementsKernel/ElementsException.h"

/** @class TimeException Time.h ElementsKernel/Time.h
 *
 *  Exception thrown by Elements::Time.
 *
 *  @see ElementsException
 *  @see Elements::Time
 *
 *  @author Marco Clemencic
 *  @date   2005-12-14
 */
class ELEMENTS_API TimeException: public ElementsException {
public:
  // Standard constructor
  TimeException(const std::string& Message = "unspecified exception",
      const std::string& Tag = "*Elements::Time*", const StatusCode & Code =
          StatusCode::FAILURE) :
      ElementsException(Message, Tag, Code) {
  }
  /// Destructor needed to match the signature of ElementsException::~ElementsException().
  virtual ~TimeException() noexcept {
  }
};

struct tm;
# ifdef WIN32
typedef struct _FILETIME FILETIME;
# endif

namespace Elements {

class Time;
class TimeSpan;

/** @class TimeSpan Time.h ElementsKernel/Time.h
 *
 *  Based on seal::TimeSpan.
 *

 A difference between two #Time values.  In addition to supporting
 normal integer artihmetic and comparisons, the span can also be
 converted to a number useful units.

 @sa #Time.

 *  (Documentation taken from original SEAL class)
 *  @author Marco Clemencic
 *  @date   2005-12-15
 */
class ELEMENTS_API TimeSpan {
  friend class Time;
public:
  typedef longlong ValueType;

  TimeSpan(void);
  TimeSpan(Time t);
  TimeSpan(ValueType nsecs);
  TimeSpan(ValueType secs, int nsecs);
  TimeSpan(int days, int hours, int mins, int secs, int nsecs);

  int days(void) const;
  int hours(void) const;
  int minutes(void) const;
  ValueType seconds(void) const;

  int lastHours(void) const;
  int lastMinutes(void) const;
  int lastSeconds(void) const;
  int lastNSeconds(void) const;

  TimeSpan & operator+=(const TimeSpan &x);
  TimeSpan & operator-=(const TimeSpan &x);
  TimeSpan & operator*=(const TimeSpan &n);
  TimeSpan & operator/=(const TimeSpan &n);
  TimeSpan & operator%=(const TimeSpan &n);

  ValueType ns(void) const;

private:
  ValueType m_nsecs; //< The span length.
};

/** @class Time Time.h ElementsKernel/Time.h
 *
 *  Based on seal::Time.
 *
 Calendar time in nanoseconds since 00:00:00 on January 1, 1970,
 Coordinated Universal Time (UTC).

 #Time is represented internally as UTC time, but it can also be
 converted to the local time as necessary.  Most methods take an
 argument flag @c local to indicate which time interpretation is
 desired by the client, and automatically perform the necessary
 adjustments.  The client can also find out about the difference
 between UTC time and local time using the #utcoffset() method,
 and the time zone name with #timezone() method.  Both allow the
 client to discover whether daylight savings is in effect.

 The native representation of #Time is not well suited for human
 handling of time.  #Time provides access in more convenient terms
 such as #year(), #month() and #day(); more are available through
 conversion into a #TimeSpan.  #Time can also be converted to and
 from ISO C standard @c tm structure.  Note however that unlike C's
 @c mktime() which always assumes @c tm in local time, #Time fully
 supports all conversions between local and universal time.  Thus
 it is possible for example to #build() a UTC time directly from a
 @c tm.

 #Time behaves as an integral type.  Differences in time values are
 represented as a #TimeSpan.  Usual integral arithmetic works with
 both types.  Output works in general as any other integral type,
 however since the #ValueType can be a wide type, it may be poorly
 supported by the @c iostream; if so, including the @c LongLong.h
 header will help.  Note that the output value will usually be very
 large as #Time is represented in nanoseconds, not seconds!  When
 constructing #Time values in seconds, such as when reading in, do
 remember to use the two-argument constructor taking seconds and
 nanoseconds instead of the default single-argument one.

 #Time can be formatted into a string using the #format() method,
 which uses the versatile @c strftime() function.  Since the latter
 works on seconds at best (through a struct @c tm), the subsecond
 part cannot be formatted; the #nanoformat() method is provided to
 overcome this limitation.  To combine #format() and #nanoformat()
 output use a suitable string format pattern.

 #Time is linked to the system's concept of calendar time and is
 therefore may not be linear nor monotonic.  System time can jump
 arbitrarily in either direction as real time clock is corrected or
 the system is suspended.  The local time may also jump due to
 daylight savings.  The process' ability to sample system time can
 be limited for reasons such as getting swapped out.  #Time
 provides an alternative time measurement facility not linked to
 calendar and guaranteed to grow monotonically -- though not always
 linearly.  Note that few systems actually provide wall-clock time
 in nanosecond resolution.  Not all system provide an interface to
 get time at that resolution, let alone track it so precisely.

 Because of the time warp issues, scheduling events using #Time is
 not straightforward.  Application code should understand whether
 it is dealing with concrete or abstract calendar calculations, and
 how the events it schedules are linked to wall clock time.

 For calculations on concrete calendar as perceived by people use
 #local() after plain #Time and #TimeSpan integer arithmetic.  The
 method accounts for timezone and daylight savings definitions.  To
 schedule events use #build() to derive times from #local() time to
 get values comparable to the system time returned by #current().
 The applications should know whether events are scheduled in UTC
 or local time---"meeting at 9:00 on Wednesday morning" when the
 device switches timezones may be known to be at 9:00 in the new
 timezone (= locked to local time), or in the timezone where the
 event was created (= locked to UTC).  The #build() and #split()
 methods allow either format to be used, the application just needs
 to know which one to use.  It is also easy to convert between the
 two using #utcoffset().

 For calculations using an abstract calendar, without timezone or
 daylight savings, use #Time in its native UTC representation and
 integer arithmetic with #Time and #TimeSpan.  Do note however that
 "T + 24 hours" may not be the same hour the next day in the local
 calendar time -- timezone changes and daylight savings make a
 difference.  This may require the application to accept as user
 input exception rules to its usual calendar calculations.

 To schedule events, one should choose between three choices: UTC
 time, local time, or delta time.  For the first two cases system
 time should be polled regularly to see if any of the recorded
 events have expired.  It is not a good idea to sleep until the
 next scheduled event, as the system time may jump during the nap;
 instead sleep small increments, recheck the current time after
 each nap and trigger the events that have expired.  A policy must
 be applied when the system time warps; this can happen both
 forwards and backwards with both local and UTC time (daylight
 savings or timezone changes for mobile devices are common local
 time change reasons, but the system time can be updated for any
 reason, e.g. when the real time clock is wrong, or if the system
 is suspended for a long time).  Some events should be executed
 only once in case of time warps backwards.  If the time jumps
 forwards, several events may need to be dealt with in one go.  In
 either case the application should guard against major time
 changes: long system suspends, moving mobile devices and major
 time updates may result in a large number of "missed" events.  One
 possibility is to provide a user-configurable "excessive time
 drift limit" (e.g. N hours): if time changes by more than that,
 missed events are not triggered.

 For the final case of using delta times, sort upcoming events by
 their deltas from the previous event---not by the time they are
 anticipated to occur.  Capture current time before and after the
 sleep and pull events off the queue based on the difference (the
 sleep time may exceed the requested time).  Either guard against
 long time warps like suspends or schedule timer events cautiously.
 Using #Time as schedule base solves such issues simply.  To
 cope with backward system time jumps when using #Time as schedule
 base, assume that sleeps always last at least the requested time;
 if the time delta over the nap is less than the requested, assume
 time warp (this is not foolproof against interrupted system calls
 but works for many event scheduling situations).

 @sa #Time for monotonic time not related to the calendar.
 *  (Documentation taken from original SEAL class)
 *  @author Marco Clemencic
 *  @date   2005-12-15
 */
class ELEMENTS_API Time {
  friend class TimeSpan;
public:
  typedef longlong ValueType;

  /** Symbolic names for months */
  enum Months {
    January = 0,
    February = 1,
    March = 2,
    April = 3,
    May = 4,
    June = 5,
    July = 6,
    August = 7,
    September = 8,
    October = 9,
    November = 10,
    December = 11
  };

  /** Seconds in 24 hours.  */
  static const int SECS_PER_DAY = 86400;

  /** Seconds in one hour hour.  */
  static const int SECS_PER_HOUR = 3600;

  /** Nanoseconds in one second.  */
  static const ValueType SEC_NSECS = 1000000000;

  Time(void);
  Time(TimeSpan ts);
  Time(ValueType nsecs);
  Time(ValueType secs, int nsecs);
  Time(int year, int month, int day, int hour, int min, int sec,
      ValueType nsecs, bool local = true);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor

  /// Returns the minimum time.
  static Time epoch(void);
  /// Returns the maximum time.
  static Time max(void);
  /// Returns the current time.
  static Time current(void);
# ifdef WIN32
  static Time from (const FILETIME *systime);
# endif
  static Time build(bool local, const tm &base, TimeSpan diff = 0);

  tm split(bool local, int *nsecpart = 0) const;
  tm utc(int *nsecpart = 0) const;
  tm local(int *nsecpart = 0) const;

  int year(bool local) const;
  int month(bool local) const;
  int day(bool local) const;
  int hour(bool local) const;
  int minute(bool local) const;
  int second(bool local) const;
  int nsecond(void) const;
  int weekday(bool local) const;
  bool isdst(bool local) const;

  ValueType utcoffset(int *daylight = 0) const;
  const char * timezone(int *daylight = 0) const;

  Time & operator+=(const TimeSpan &x);
  Time & operator-=(const TimeSpan &x);

  ValueType ns(void) const;

  std::string format(bool local, std::string spec = "%c") const;
  std::string nanoformat(size_t minwidth = 1, size_t maxwidth = 9) const;

  static bool isLeap(int year);

  // Conversion helpers
  static unsigned toDosDate(Time time);
  static Time fromDosDate(unsigned dosDate);

private:
  ValueType m_nsecs; //< Time value as nsecs from #epoch().

  inline void TimeAssert(bool cond, const std::string &msg =
      "time assertion failed") const {
    if (!cond)
      throw TimeException(msg);
  }

};

}

#include "ElementsKernel/Time.icpp"

#endif // ELEMENTSKERNEL_TIME_H
