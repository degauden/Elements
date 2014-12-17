//====================================================================
//	Memory.cpp
//--------------------------------------------------------------------
//
//	Package    : System (The LHCb System service)
//
//  Description: Information of memory usage from a given process
//
//	Author     : M.Frank
//  Created    : 13/11/00
//	Changes    :
//
//====================================================================

//#include <cerrno>
//#include <cstring>
//#include <sys/times.h>
//#include <unistd.h>
//#include <libgen.h>
//#include <cstdio>

//#include <climits>

#include "ElementsKernel/Memory.h"      // for MemoryUnit, etc

#include <unistd.h>                     // for getpid

#include "ElementsKernel/SystemBase.h"  // for InfoType, InfoType::NoFetch, etc
#include "ProcessDescriptor.h"          // for getProcess, etc


namespace Elements {
namespace System {


/// Convert requested memory value from kByte to requested value
long adjustMemory( MemoryUnit unit, long value )    {
  if ( value != -1 )    {
    switch ( unit )   {
    case Byte:   break;
    case kByte:     value =       value/1024;    break;
    case MByte:     value =      (value/1024)/1024;    break;
    case GByte:     value =     ((value/1024)/1024)/1024;    break;
    case TByte:     value =    (((value/1024)/1024)/1024)/1024;    break;
    case PByte:     value =   ((((value/1024)/1024)/1024)/1024)/1024;    break;
    case EByte:     value =  (((((value/1024)/1024)/1024)/1024)/1024)/1024;    break;
    default:        value =  -1;    break;
    }
  }
  return value;
}

/// Basic Process Information: Base priority
long basePriority(InfoType fetch, long pid)   {
  PROCESS_BASIC_INFORMATION info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, InfoType::ProcessBasics, &info) )
    return info.BasePriority;
  return 0;
}

/// Basic Process Information: Process ID
long procID()   {
  static long s_pid = ::getpid();
  return s_pid;
}

/// Basic Process Information: Parent's process ID
long parentID(InfoType fetch, long pid)   {
  PROCESS_BASIC_INFORMATION info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, InfoType::ProcessBasics, &info) )
    return info.InheritedFromUniqueProcessId;
  return 0;
}

/// Basic Process Information: Affinity mask
long affinityMask(InfoType fetch, long pid)   {
  PROCESS_BASIC_INFORMATION info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, InfoType::ProcessBasics, &info) )
    return info.AffinityMask;
  return 0;
}

/// Basic Process Information: Exit status (does not really make sense for the running process, but for others!)
long exitStatus(InfoType fetch, long pid)   {
  PROCESS_BASIC_INFORMATION info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, InfoType::ProcessBasics, &info) )
    return info.ExitStatus;
  return -2;
}

/// Basic Process Information: priority boost
long priorityBoost(InfoType fetch, long pid)   {
  long info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return info;
  return -2;
}

/// Basic Process Information: priority boost
long nonPagedMemoryPeak(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.QuotaPeakNonPagedPoolUsage);
  return -2;
}

/// Basic Process Information: priority boost
long nonPagedMemory(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.QuotaNonPagedPoolUsage);
  return -2;
}

/// System Process Limits: Maximum amount of non-paged memory this process is allowed to use
long nonPagedMemoryLimit(MemoryUnit unit, InfoType fetch, long pid)   {
  POOLED_USAGE_AND_LIMITS quota;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &quota) )
    return adjustMemory(unit, quota.NonPagedPoolLimit);
  return 0;
}

/// Basic Process Information: Amount of paged memory currently occupied by the process 'pid'
long pagedMemory(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.QuotaPagedPoolUsage);
  return -2;
}

/// Basic Process Information: Maximum of paged memory occupied by the process 'pid'
long pagedMemoryPeak(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.QuotaPeakPagedPoolUsage);
  return -2;
}

/// Basic Process Information: Amount of paged memory that can be occupied by the process 'pid'
long pagedMemoryLimit(MemoryUnit unit, InfoType fetch, long pid)   {
  POOLED_USAGE_AND_LIMITS quota;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &quota) )
    return adjustMemory(unit, quota.PagedPoolLimit);
  return 0;
}

/// Basic Process Information: priority boost
long numPageFault(InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return info.PageFaultCount;
  return -2;
}

/// Basic Process Information: priority boost
long pagefileUsage(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.PagefileUsage);
  return -2;
}

/// Basic Process Information: priority boost
long pagefileUsagePeak(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.PeakPagefileUsage);
  return -2;
}

/// Basic Process Information: priority boost
long pagefileUsageLimit(MemoryUnit unit, InfoType fetch, long pid)   {
  POOLED_USAGE_AND_LIMITS quota;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &quota) )    {
    if ( long(quota.PagefileLimit) < 0 )
      return -1;//LONG_MAX;
    return adjustMemory(unit, quota.PagefileLimit);
  }
  return -2;
}

/// Basic Process Information: priority boost
long mappedMemory(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.WorkingSetSize);
  return -2;
}

/// Basic Process Information: priority boost
long mappedMemoryPeak(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.PeakWorkingSetSize);
  return -2;
}

/// System Process Limits: Minimum amount of virtual memory this process may use
long minMemoryLimit(MemoryUnit unit, InfoType fetch, long pid)   {
  QUOTA_LIMITS quota;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &quota) )
    return adjustMemory(unit, quota.MinimumWorkingSetSize);
  return 0;
}

/// System Process Limits: Maximum amount of virtual memory this process is allowed to use
long maxMemoryLimit(MemoryUnit unit, InfoType fetch, long pid)   {
  QUOTA_LIMITS quota;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &quota) )
    return adjustMemory(unit, quota.MaximumWorkingSetSize);
  return 0;
}

/// Basic Process Information: priority boost
long virtualMemory(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.VirtualSize);
  return -2;
}

/// Basic Process Information: priority boost
long virtualMemoryPeak(MemoryUnit unit, InfoType fetch, long pid)   {
  VM_COUNTERS info;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &info) )
    return adjustMemory(unit, info.PeakVirtualSize);
  return -2;
}

/// System Process Limits: Maximum amount of the page file this process is allowed to use
long virtualMemoryLimit(MemoryUnit unit, InfoType fetch, long pid)   {
  QUOTA_LIMITS quota;
  if ( fetch != InfoType::NoFetch && getProcess()->query(pid, fetch, &quota) )    {
    if ( long(quota.PagefileLimit) == -1 )
      return -1;//LONG_MAX;
    return adjustMemory(unit, quota.PagefileLimit);
  }
  return 0;
}

} // namespace System
} // namespace Elements
