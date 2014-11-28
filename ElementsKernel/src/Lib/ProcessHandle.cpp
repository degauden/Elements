/*
 * ProcessHandle.cpp
 *
 *  Created on: 28 nov. 2014
 *      Author: hubert
 */

#include "ElementsKernel/ModuleInfo.h"

#include "ProcessID.h"
#include "ProcessHandle.h"

ProcessHandle::ProcessHandle(pid_t pid) {
  if (pid > 0) {
    if (pid != MY_PID) {
      // Note: the return type of getpid is pid_t, which is int on 64bit machines too
      m_handle = reinterpret_cast<void*>(static_cast<pid_t>(MY_PID));
      m_needRelease = true;
      return;
    }
  }
  m_handle = Elements::System::processHandle();
  m_needRelease = false;
}

ProcessHandle::~ProcessHandle() {
  if (m_needRelease) {
    m_handle = nullptr;
  }
}

bool ProcessHandle::item() const {
  return m_needRelease ? true : false;
}
void* ProcessHandle::handle() const {
  return m_handle;
}


