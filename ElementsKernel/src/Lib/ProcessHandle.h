/*
 * ProcessHandle.h
 *
 *  Created on: 28 nov. 2014
 *      Author: hubert
 */

#ifndef ELEMENTSKERNEL_SRC_LIB_PROCESSHANDLE_H_
#define ELEMENTSKERNEL_SRC_LIB_PROCESSHANDLE_H_

#include <sys/types.h>

class ProcessHandle {
public:
  ProcessHandle(pid_t pid);
  virtual ~ProcessHandle();
  bool item() const;
  void* handle() const;
private:
  void* m_handle;
  bool m_needRelease;

};


#endif /* ELEMENTSKERNEL_SRC_LIB_PROCESSHANDLE_H_ */
