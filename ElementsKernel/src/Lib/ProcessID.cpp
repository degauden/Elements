/*
 * ProcessID.cpp
 *
 *  Created on: 28 nov. 2014
 *      Author: hubert
 */


#include <unistd.h>

#include "ProcessID.h"

const pid_t MY_PID = ::getpid();

pid_t processID(pid_t pid) {
  pid_t thePid = (pid > 0) ? pid : MY_PID;
  return thePid;
}
