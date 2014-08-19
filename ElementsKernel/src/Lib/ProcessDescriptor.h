// $Id: ProcessDescriptor.h,v 1.3 2006/12/10 20:29:18 leggett Exp $
#ifndef ELEMENTSKERNEL_PROCESS_H
#define ELEMENTSKERNEL_PROCESS_H

// Framework include files
#include "ElementsKernel/Kernel.h"
#include "ElementsKernel/SystemBase.h"

namespace Elements {
namespace System {
// Forward declarations
class ProcessDescriptor;
/// Retrieve Process structure
ProcessDescriptor* getProcess();

/** Basic Process Information
 NtQueryInformationProcess using ProcessBasicInfo
 */
typedef struct _PEB *PPEB;
struct PROCESS_BASIC_INFORMATION {
  long ExitStatus;
  PPEB PebBaseAddress;
  unsigned long AffinityMask;
  long BasePriority;
  unsigned long UniqueProcessId;
  unsigned long InheritedFromUniqueProcessId;
};

/** Process Quotas
 NtQueryInformationProcess using ProcessQuotaLimits
 NtQueryInformationProcess using ProcessPooledQuotaLimits
 NtSetInformationProcess using ProcessQuotaLimits
 */
struct QUOTA_LIMITS {
  unsigned long PagedPoolLimit;
  unsigned long NonPagedPoolLimit;
  unsigned long MinimumWorkingSetSize;
  unsigned long MaximumWorkingSetSize;
  unsigned long PagefileLimit;
  longlong TimeLimit;
};

/** Process I/O Counters
 NtQueryInformationProcess using ProcessIoCounters
 */
struct IO_COUNTERS {
  unsigned long ReadOperationCount;
  unsigned long WriteOperationCount;
  unsigned long OtherOperationCount;
  //      longlong ReadOperationCount;
  //      longlong WriteOperationCount;
  //      longlong OtherOperationCount;
  longlong ReadTransferCount;
  longlong WriteTransferCount;
  longlong OtherTransferCount;
};

/** Process Virtual Memory Counters
 NtQueryInformationProcess using ProcessVmCounters
 */
struct VM_COUNTERS {
  unsigned long PeakVirtualSize;
  unsigned long VirtualSize;
  unsigned long PageFaultCount;
  unsigned long PeakWorkingSetSize;
  unsigned long WorkingSetSize;
  unsigned long QuotaPeakPagedPoolUsage;
  unsigned long QuotaPagedPoolUsage;
  unsigned long QuotaPeakNonPagedPoolUsage;
  unsigned long QuotaNonPagedPoolUsage;
  unsigned long PagefileUsage;
  unsigned long PeakPagefileUsage;
};

/** Process Pooled Quota Usage and Limits
 NtQueryInformationProcess using ProcessPooledUsageAndLimits
 */
struct POOLED_USAGE_AND_LIMITS {
  unsigned long PeakPagedPoolUsage;
  unsigned long PagedPoolUsage;
  unsigned long PagedPoolLimit;
  unsigned long PeakNonPagedPoolUsage;
  unsigned long NonPagedPoolUsage;
  unsigned long NonPagedPoolLimit;
  unsigned long PeakPagefileUsage;
  unsigned long PagefileUsage;
  unsigned long PagefileLimit;
};

/** Process/Thread System and User Time
 NtQueryInformationProcess using ProcessTimes
 NtQueryInformationThread using ThreadTimes
 */
struct KERNEL_USER_TIMES {
  longlong CreateTime;
  longlong ExitTime;
  longlong KernelTime;
  longlong UserTime;
  //longlong EllapsedTime; // Added by M.Frank
};

/**
 * @class ProcessDescriptor ProcessDescriptor.h
 *
 * Provides access to process information
 *
 * @author M.Frank
 * @author Sebastien Ponce
 */
class ProcessDescriptor {
  class ProcessHandle {
    void* m_handle;
    bool m_needRelease;
  public:
    ProcessHandle(long pid);
    virtual ~ProcessHandle();
    long item() {
      return m_needRelease ? true : false ;
    }
    void* handle() {
      return m_handle;
    }
  };
  long m_PRIORITYBOOST[2]{};
  IO_COUNTERS m_IO_COUNTERS[2]{};
  VM_COUNTERS m_VM_COUNTERS[2]{};
  KERNEL_USER_TIMES m_KERNEL_USER_TIMES[2]{};
  POOLED_USAGE_AND_LIMITS m_POOLED_USAGE_AND_LIMITS[2]{};
  QUOTA_LIMITS m_QUOTA_LIMITS[2]{};
  PROCESS_BASIC_INFORMATION m_PROCESS_BASIC_INFORMATION[2]{};
public:
  ProcessDescriptor();
  virtual ~ProcessDescriptor();
  long query(long pid, InfoType info, PROCESS_BASIC_INFORMATION* buffer);
  long query(long pid, InfoType info, POOLED_USAGE_AND_LIMITS* buffer);
  long query(long pid, InfoType info, KERNEL_USER_TIMES* buffer);
  long query(long pid, InfoType info, QUOTA_LIMITS* buffer);
  long query(long pid, InfoType info, VM_COUNTERS* buffer);
  long query(long pid, InfoType info, IO_COUNTERS* buffer);
  long query(long pid, InfoType info, long* buffer);
};
}
}
#endif //ELEMENTSKERNEL_PROCESS_H
