// $Id: Timing.cpp,v 1.3 2002/11/12 18:34:30 mato Exp $
//====================================================================
//	Timing.cpp
//--------------------------------------------------------------------
//
//	Package    : System (The LHCb System service)
//
//  Description: Implementation of Systems internals
//
//	Author     : M.Frank
//  Created    : 13/1/99
//	Changes    :
//====================================================================

#include "ElementsKernel/Timing.h"
#include "ProcessDescriptor.h"

#include <iostream>
#include <ctime>
#include <climits>
#include <cstdint>       // for std::int64_t
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>


using std::int64_t;

constexpr int64_t UNIX_BASE_TIME = 0;

namespace Elements {
namespace System {



// convert time from internal representation to the appropriate type
// Internal representation for Unix:    1 clock tick (usually 10 milliseconds)
int64_t adjustTime(TimeType typ, int64_t t) {
  if (t != -1) {
    /////////    t *= 10000000;           // in 100 nanosecond intervals
    //  t /= CLK_TCK ;     // needs division by clock tick unit
    /// unfortunately "-ansi" flag turn off the correct definition of CLK_TCK
    /// and forces it to be equal CLOCKS_PER_SEC, it is wrong!
    ///////// t /= 100 ;

    ///  t /= CLOCKS_PER_SEC;     // needs division by clock tick unit
    switch (typ) {
    case Year:
      return adjustTime<Year>(t);
    case Month:
      return adjustTime<Month>(t);
    case Day:
      return adjustTime<Day>(t);
    case Hour:
      return adjustTime<Hour>(t);
    case Min:
      return adjustTime<Min>(t);
    case Sec:
      return adjustTime<Sec>(t);
    case milliSec:
      return adjustTime<milliSec>(t);
    case microSec:
      return adjustTime<microSec>(t);
    case nanoSec:
      return adjustTime<nanoSec>(t);
    case Native:
      return adjustTime<Native>(t);
    }
  }
  return t;
}

/// Retrieve the number of ticks since system startup
int64_t tickCount() {
  int64_t count = 10000;
  struct tms buf;
  count *= 10 * times(&buf);
  return count;
}


/// Retrieve current system time
int64_t currentTime(TimeType typ) {
  int64_t current = 0;
  struct timeval tv;
  struct timezone tz;
  ::gettimeofday(&tv, &tz);
  current = tv.tv_sec;
  current *= 1000000;
  current += tv.tv_usec;
  current *= 10;
  return adjustTime(typ, current);
}

/// Units of time since system startup and begin of epoche
int64_t systemStart(TimeType typ) {
  static int64_t sys_start = 0;
  if (0 == sys_start) {
    int64_t c = currentTime(microSec);
    int64_t t = tickCount();
    sys_start = 10 * c - t;
  }
  return adjustTime(typ, sys_start);
}

/// Units of time since system startup in requested units
int64_t upTime(TimeType typ) {
  static int64_t sys_start = 10 * systemStart(microSec);
  return adjustTime(typ, 10 * currentTime(microSec) - sys_start);
}

/// Units of time between process creation and begin of epoche
int64_t creationTime(TimeType typ, InfoType fetch,
    long pid) {
  int64_t created = 0;
  KERNEL_USER_TIMES info;
  if (fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info)) {
    created = adjustTime(typ, info.CreateTime - UNIX_BASE_TIME);
  }
  return created;
}

/// System Process Limits: Maximum processing time left for this process
int64_t remainingTime(TimeType typ, InfoType fetch,
    long pid) {
  int64_t left = 0;
  QUOTA_LIMITS quota;
  if (fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &quota)) {
    if (left == -1) {
      //left = _I64_MAX;
    } else {
      left = adjustTime(typ, quota.TimeLimit);
    }
  }
  return left;
}

/// Ellapsed time since start of process in milliseconds
int64_t ellapsedTime(TimeType typ, InfoType fetch,
    long pid) {
  KERNEL_USER_TIMES info;
  int64_t ellapsed = currentTime(microSec) * 10;
  getProcess()->query(pid, fetch, &info);
  ellapsed = adjustTime(typ, ellapsed + UNIX_BASE_TIME - info.CreateTime);
  return ellapsed;
}

/// CPU kernel time of process in milliseconds
int64_t kernelTime(TimeType typ, InfoType fetch, long pid) {
  KERNEL_USER_TIMES info;
  int64_t kerneltime = 0;
  if (fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info)) {
    kerneltime = adjustTime(typ, info.KernelTime);
  }
  return kerneltime;
}

/// CPU kernel time of process in milliseconds
int64_t userTime(TimeType typ, InfoType fetch, long pid) {
  int64_t usertime = 0;
  KERNEL_USER_TIMES info;
  if (fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info)) {
    usertime = adjustTime(typ, info.UserTime);
  }
  return usertime;
}

/// CPU kernel time of process in milliseconds
int64_t cpuTime(TimeType typ, InfoType fetch, long pid) {
  int64_t cputime = 0;
  KERNEL_USER_TIMES info;
  if (fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info)) {
    cputime = adjustTime(typ, info.KernelTime + info.UserTime);
  }
  return cputime;
}

ProcessTime getProcessTime(long pid) {
  KERNEL_USER_TIMES info;
  if (getProcess()->query(pid, InfoType::Times, &info)) {
    return ProcessTime(info.KernelTime, info.UserTime,
        currentTime(Native) - info.CreateTime);
  }
  return ProcessTime();  // return 0s in case of problems
}

}  // namespace System
}  // namespace Elements
