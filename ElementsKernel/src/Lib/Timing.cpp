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
#define ELEMENTSKERNEL_TIMING_CPP

#include "ElementsKernel/Timing.h"
#include "ProcessDescriptor.h"

#include <ctime>
#include <climits>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#endif

#ifdef _WIN32
static const longlong UNIX_BASE_TIME = 116444736000000000;
#else
static const longlong UNIX_BASE_TIME = 0;
#endif

// convert time from internal representation to the appropriate type
// Internal representation for WIN32: 100 nanosecond intervals
//                             Unix:    1 clock tick (usually 10 milliseconds)
longlong Elements::System::adjustTime(TimeType typ, longlong t) {
  if (t != -1) {
#ifndef _WIN32
    /////////    t *= 10000000;           // in 100 nanosecond intervals
    //  t /= CLK_TCK ;     // needs division by clock tick unit
    /// unfortunately "-ansi" flag turn off the correct definition of CLK_TCK
    /// and forces it to be equal CLOCKS_PER_SEC, it is wrong!
    ///////// t /= 100 ;

    ///  t /= CLOCKS_PER_SEC;     // needs division by clock tick unit
#endif
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
longlong Elements::System::tickCount() {
  longlong count = 10000;
#ifdef _WIN32
  count *= ::GetTickCount(); // Number of milliSec since system startup
#else
  struct tms buf;
  count *= 10 * times(&buf);
#endif
  return count;
}

#include <iostream>

/// Retrieve current system time
longlong Elements::System::currentTime(TimeType typ) {
  longlong current = 0;
#ifdef _WIN32
  ::GetSystemTimeAsFileTime((FILETIME*)&current);
  current -= UNIX_BASE_TIME;
#else
  struct timeval tv;
  struct timezone tz;
  ::gettimeofday(&tv, &tz);
  current = tv.tv_sec;
  current *= 1000000;
  current += tv.tv_usec;
  current *= 10;
#endif
  return adjustTime(typ, current);
}

/// Units of time since system startup and begin of epoche
longlong Elements::System::systemStart(TimeType typ) {
  static longlong sys_start = 0;
  if (0 == sys_start) {
    longlong c = currentTime(microSec);
    longlong t = tickCount();
    sys_start = 10 * c - t;
  }
  return adjustTime(typ, sys_start);
}

/// Units of time since system startup in requested units
longlong Elements::System::upTime(TimeType typ) {
  static longlong sys_start = 10 * systemStart(microSec);
  return adjustTime(typ, 10 * currentTime(microSec) - sys_start);
}

/// Units of time between process creation and begin of epoche
longlong Elements::System::creationTime(TimeType typ, InfoType fetch,
    long pid) {
  longlong created = 0;
  KERNEL_USER_TIMES info;
  if (fetch != NoFetch && getProcess()->query(pid, fetch, &info)) {
    created = adjustTime(typ, info.CreateTime - UNIX_BASE_TIME);
  }
  return created;
}

/// System Process Limits: Maximum processing time left for this process
longlong Elements::System::remainingTime(TimeType typ, InfoType fetch,
    long pid) {
  longlong left = 0;
  QUOTA_LIMITS quota;
  if (fetch != NoFetch && getProcess()->query(pid, fetch, &quota)) {
    if (left == -1) {
      //left = _I64_MAX;
    } else {
      left = adjustTime(typ, quota.TimeLimit);
    }
  }
  return left;
}

/// Ellapsed time since start of process in milliseconds
longlong Elements::System::ellapsedTime(TimeType typ, InfoType fetch,
    long pid) {
  KERNEL_USER_TIMES info;
  longlong ellapsed = currentTime(microSec) * 10;
  getProcess()->query(pid, fetch, &info);
  ellapsed = adjustTime(typ, ellapsed + UNIX_BASE_TIME - info.CreateTime);
  return ellapsed;
}

/// CPU kernel time of process in milliseconds
longlong Elements::System::kernelTime(TimeType typ, InfoType fetch, long pid) {
  KERNEL_USER_TIMES info;
  longlong kerneltime = 0;
  if (fetch != NoFetch && getProcess()->query(pid, fetch, &info)) {
    kerneltime = adjustTime(typ, info.KernelTime);
  }
  return kerneltime;
}

/// CPU kernel time of process in milliseconds
longlong Elements::System::userTime(TimeType typ, InfoType fetch, long pid) {
  longlong usertime = 0;
  KERNEL_USER_TIMES info;
  if (fetch != NoFetch && getProcess()->query(pid, fetch, &info)) {
    usertime = adjustTime(typ, info.UserTime);
  }
  return usertime;
}

/// CPU kernel time of process in milliseconds
longlong Elements::System::cpuTime(TimeType typ, InfoType fetch, long pid) {
  longlong cputime = 0;
  KERNEL_USER_TIMES info;
  if (fetch != NoFetch && getProcess()->query(pid, fetch, &info)) {
    cputime = adjustTime(typ, info.KernelTime + info.UserTime);
  }
  return cputime;
}

namespace Elements {
namespace System {
ProcessTime getProcessTime(long pid) {
  KERNEL_USER_TIMES info;
  if (getProcess()->query(pid, Times, &info)) {
    return ProcessTime(info.KernelTime, info.UserTime,
        currentTime(Native) - info.CreateTime);
  }
  return ProcessTime(); // return 0s in case of problems
}
}
}
