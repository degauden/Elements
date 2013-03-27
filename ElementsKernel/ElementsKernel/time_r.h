#ifndef ELEMENTSKERNEL_TIME_R_H
#define ELEMENTSKERNEL_TIME_R_H

#include <ctime>

/*
 This header file provides the functions localtime_r and time_r (available on Linux)
 to the Win32 platform.

 Marco Clemencic
 */

#ifdef _WIN32

extern "C" {
  inline struct tm * localtime_r(const time_t *sec, struct tm *result) {
    localtime_s(result, sec);
    return result;
  }
  inline struct tm * gmtime_r(const time_t *sec, struct tm *result) {
    gmtime_s(result, sec);
    return result;
  }
}

#endif

#endif    // ELEMENTSKERNEL_TIME_R_H
