#define ELEMENTSKERNEL_PROCSTAT_CPP

#define WINVER 0
#include <cerrno>
#include <string>
#include <unistd.h>
#include <libgen.h>
#include <cstdio>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <fcntl.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/syscall.h>
#ifndef __APPLE__
#include <sys/procfs.h>
#endif
#include <sys/time.h>
#include <sys/resource.h>
#include <cstdio>

//using nameespace std;

/* Format of the Linux proc/stat (man 5 proc, kernel 2.6.35):
 pid %d      The process ID.

 comm %s     The  filename  of the executable, in parentheses.  This is visible
 whether or not the executable is swapped out.

 state %c    One character from the string "RSDZTW" where R is  running,  S  is
 sleeping in an interruptible wait, D is waiting in uninterruptible
 disk sleep, Z is zombie, T is traced or stopped (on a signal), and
 W is paging.

 ppid %d     The PID of the parent.

 pgrp %d     The process group ID of the process.

 session %d  The session ID of the process.

 tty_nr %d   The controlling terminal of the process.  (The minor device number
 is contained in the combination of bits 31 to 20 and 7 to  0;  the
 major device number is in bits 15 t0 8.)

 tpgid %d    The ID of the foreground process group of the controlling terminal
 of the process.

 flags %u (%lu before Linux 2.6.22)
 The kernel flags word of the process.  For bit meanings,  see  the
 PF_*  defines  in  <linux/sched.h>.   Details depend on the kernel
 version.

 minflt %lu  The number of minor faults the process has  made  which  have  not
 required loading a memory page from disk.

 cminflt %lu The  number of minor faults that the process's waited-for children
 have made.

 majflt %lu  The number of  major  faults  the  process  has  made  which  have
 required loading a memory page from disk.

 cmajflt %lu The  number of major faults that the process's waited-for children
 have made.

 utime %lu   Amount of time that this process has been scheduled in user  mode,
 measured  in  clock  ticks  (divide by sysconf(_SC_CLK_TCK).  This
 includes guest time, guest_time (time spent running a virtual CPU,
 see  below),  so that applications that are not aware of the guest
 time field do not lose that time from their calculations.

 stime %lu   Amount of time that this process  has  been  scheduled  in  kernel
 mode, measured in clock ticks (divide by sysconf(_SC_CLK_TCK).

 cutime %ld  Amount  of  time that this process's waited-for children have been
 scheduled in  user  mode,  measured  in  clock  ticks  (divide  by
 sysconf(_SC_CLK_TCK).   (See  also times(2).)  This includes guest
 time, cguest_time (time spent running a virtual CPU, see below).

 cstime %ld  Amount of time that this process's waited-for children  have  been
 scheduled  in  kernel  mode,  measured  in  clock ticks (divide by
 sysconf(_SC_CLK_TCK).

 priority %ld
 (Explanation for Linux 2.6)  For  processes  running  a  real-time
 scheduling  policy (policy below; see sched_setscheduler(2)), this
 is the negated scheduling priority, minus one; that is,  a  number
 in  the  range -2 to -100, corresponding to real-time priorities 1
 to 99.  For processes running  under  a  non-real-time  scheduling
 policy, this is the raw nice value (setpriority(2)) as represented
 in the kernel.  The kernel stores nice values as  numbers  in  the
 range 0 (high) to 39 (low), corresponding to the user-visible nice
 range of -20 to 19.

 Before Linux 2.6, this was a scaled value based on  the  scheduler
 weighting given to this process.

 nice %ld    The  nice value (see setpriority(2)), a value in the range 19 (low
 priority) to -20 (high priority).

 num_threads %ld
 Number of threads in this process (since Linux 2.6).  Before  ker‐
 nel  2.6,  this  field was hard coded to 0 as a placeholder for an
 earlier removed field.

 itrealvalue %ld
 The time in jiffies before the next SIGALRM is sent to the process
 due  to  an interval timer.  Since kernel 2.6.17, this field is no
 longer maintained, and is hard coded as 0.

 starttime %llu (was %lu before Linux 2.6)
 The time in jiffies the process started after system boot.

 vsize %lu   Virtual memory size in bytes.

 rss %ld     Resident Set Size: number of pages the process has in real memory.
 This  is  just  the pages which count towards text, data, or stack
 space.  This does not include pages which have  not  been  demand-
 loaded in, or which are swapped out.

 rsslim %lu  Current  soft  limit  in  bytes on the rss of the process; see the
 description of RLIMIT_RSS in getpriority(2).

 startcode %lu
 The address above which program text can run.

 endcode %lu The address below which program text can run.

 startstack %lu
 The address of the start (i.e., bottom) of the stack.

 kstkesp %lu The current value of ESP (stack pointer), as found in  the  kernel
 stack page for the process.

 kstkeip %lu The current EIP (instruction pointer).

 signal %lu  The  bitmap  of  pending  signals,  displayed as a decimal number.
 Obsolete, because it does not  provide  information  on  real-time
 signals; use /proc/[pid]/status instead.

 blocked %lu The  bitmap  of  blocked  signals,  displayed as a decimal number.
 Obsolete, because it does not  provide  information  on  real-time
 signals; use /proc/[pid]/status instead.

 sigignore %lu
 The  bitmap  of  ignored  signals,  displayed as a decimal number.
 Obsolete, because it does not  provide  information  on  real-time
 signals; use /proc/[pid]/status instead.

 sigcatch %lu
 The  bitmap  of  caught  signals,  displayed  as a decimal number.
 Obsolete, because it does not  provide  information  on  real-time
 signals; use /proc/[pid]/status instead.

 wchan %lu   This  is the "channel" in which the process is waiting.  It is the
 address of a system call, and can be looked up in  a  namelist  if
 you   need   a   textual   name.    (If  you  have  an  up-to-date
 /etc/psdatabase, then try ps -l to see the WCHAN field in action.)

 nswap %lu   Number of pages swapped (not maintained).

 cnswap %lu  Cumulative nswap for child processes (not maintained).

 exit_signal %d (since Linux 2.1.22)
 Signal to be sent to parent when we die.

 processor %d (since Linux 2.2.8)
 CPU number last executed on.

 rt_priority %u (since Linux 2.5.19; was %lu before Linux 2.6.22)
 Real-time scheduling priority, a number in the range 1 to  99  for
 processes  scheduled under a real-time policy, or 0, for non-real-
 time processes (see sched_setscheduler(2)).

 policy %u (since Linux 2.5.19; was %lu before Linux 2.6.22)
 Scheduling policy (see sched_setscheduler(2)).  Decode  using  the
 SCHED_* constants in linux/sched.h.

 delayacct_blkio_ticks %llu (since Linux 2.6.18)
 Aggregated  block  I/O  delays, measured in clock ticks (centisec‐
 onds).

 guest_time %lu (since Linux 2.6.24)
 Guest time of the process (time spent running a virtual CPU for  a
 guest  operating  system),  measured  in  clock  ticks  (divide by
 sysconf(_SC_CLK_TCK).

 cguest_time %ld (since Linux 2.6.24)
 Guest time of the process's  children,  measured  in  clock  ticks
 (divide by sysconf(_SC_CLK_TCK).
 */
struct linux_proc {
  int pid;
  char comm[400];
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty;
  int tpgid;
  unsigned long flags;
  unsigned long minflt;
  unsigned long cminflt;
  unsigned long majflt;
  unsigned long cmajflt;
  unsigned long utime;
  unsigned long stime;
  long cutime;
  long cstime;
  long priority;
  long nice;
  long num_threads;
  long itrealvalue;
  unsigned long long starttime;
  unsigned long vsize;
  long rss;
  unsigned long rlim;
  unsigned long startcode;
  unsigned long endcode;
  unsigned long startstack;
  unsigned long kstkesp;
  unsigned long kstkeip;
  unsigned long signal;
  unsigned long blocked;
  unsigned long sigignore;
  unsigned long sigcatch;
  unsigned long wchan;
};

#ifdef __APPLE__
// static long  pg_size = 0;
#else
static long pg_size = ::sysconf(_SC_PAGESIZE); // getpagesize();
#endif
void readProcStat(long pid, linux_proc& pinfo) {

  int cnt, fd;
  char buf[512];

  std::ostringstream ost;

  ost << "/proc/" << pid << "/stat";
  std::string fname = ost.str();
  if ((fd = open(fname.c_str(), O_RDONLY)) < 0) {
    std::cerr << "Failed to open " << ost.str() << std::endl;
    return;
  }

  lseek(fd, 0, SEEK_SET);
  if ((cnt = read(fd, buf, sizeof(buf))) < 0) {
    std::cout << "LINUX Read of Proc file failed:" << std::endl;
    close(fd);
    return;
  }

  // Format
  if (cnt > 0) {
    buf[cnt] = '\0';
    sscanf(buf,
        //1  2  3  4  5  6  7  8  9  10  1   2   3   4   5   6   7   8   9   20  1   2   3   4   5   6   7   8   9   30  1   2   3   4   5
        "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
        &pinfo.pid, pinfo.comm, &pinfo.state, &pinfo.ppid, &pinfo.pgrp,
        &pinfo.session, &pinfo.tty, &pinfo.tpgid, &pinfo.flags, &pinfo.minflt,
        &pinfo.cminflt, &pinfo.majflt, &pinfo.cmajflt, &pinfo.utime,
        &pinfo.stime, &pinfo.cutime, &pinfo.cstime, &pinfo.priority,
        &pinfo.nice, &pinfo.num_threads, &pinfo.itrealvalue, &pinfo.starttime,
        &pinfo.vsize, &pinfo.rss, &pinfo.rlim, &pinfo.startcode, &pinfo.endcode,
        &pinfo.startstack, &pinfo.kstkesp, &pinfo.kstkeip, &pinfo.signal,
        &pinfo.blocked, &pinfo.sigignore, &pinfo.sigcatch, &pinfo.wchan);
  }
  close(fd);
}

//static long s_myPid  = ::getpid();
// In order to properly support e.g. fork() calls, we cannot keep a copy of the pid!
#define s_myPid (::getpid())
static inline long processID(long pid) {
  long thePid = (pid > 0) ? pid : s_myPid;
  return thePid;
}

// Framework include files
#include "ProcessDescriptor.h"
#include "ElementsKernel/ModuleInfo.h"
#include "ElementsKernel/System.h"


static const long TICK_TO_100NSEC = 100000;

namespace Elements {
namespace System {

ProcessDescriptor* getProcess() {
  static ProcessDescriptor p;
  return &p;
}

ProcessDescriptor::ProcessHandle::ProcessHandle(long pid) {
  if (pid > 0) {
    if (pid != s_myPid) {
      // Note: the return type of getpid is pid_t, which is int on 64bit machines too
      m_handle = reinterpret_cast<void*>(static_cast<long>(s_myPid));
          m_needRelease = true;
          return;
        }
      }
      m_handle = processHandle();
      m_needRelease = false;
    }

ProcessDescriptor::ProcessHandle::~ProcessHandle() {
  if (m_needRelease) {
    m_handle = nullptr;
  }
}

ProcessDescriptor::ProcessDescriptor() {
}

ProcessDescriptor::~ProcessDescriptor() {
}

long ProcessDescriptor::query(long pid, InfoType fetch,
    IO_COUNTERS* info) {
  long status = 1;
  ProcessHandle h(pid);
  IO_COUNTERS* vb = &m_IO_COUNTERS[h.item()];
  if (fetch == InfoType::IO) {
#if defined(__linux)
    linux_proc prc;
    readProcStat(processID(pid), prc);
    rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    vb->ReadOperationCount = usage.ru_inblock;
    vb->WriteOperationCount = usage.ru_oublock;
    vb->OtherOperationCount = 0;
    vb->ReadTransferCount = usage.ru_inblock;
    vb->WriteTransferCount = usage.ru_oublock;
    vb->OtherTransferCount = 0;
#endif
  }
  if (info)
    *info = *vb;
  return status;
}

long ProcessDescriptor::query(long pid, InfoType fetch,
    POOLED_USAGE_AND_LIMITS* info) {
  long status = 1;
  ProcessHandle h(pid);
  POOLED_USAGE_AND_LIMITS* vb = &m_POOLED_USAGE_AND_LIMITS[h.item()];
  if (fetch == InfoType::Quota) {
#if defined(__linux)
    //rusage usage;
    //getrusage(RUSAGE_SELF, &usage);
    rlimit lim;

    getrlimit(RLIMIT_DATA, &lim);
    if (lim.rlim_max == RLIM_INFINITY )
      lim.rlim_max = 0xFFFFFFFF;
    vb->PeakPagedPoolUsage = lim.rlim_cur;
    vb->PagedPoolUsage = lim.rlim_cur;
    vb->PagedPoolLimit = lim.rlim_max;

    getrlimit(RLIMIT_STACK, &lim);
    if (lim.rlim_max == RLIM_INFINITY )
      lim.rlim_max = 0xFFFFFFFF;
    vb->PeakNonPagedPoolUsage = lim.rlim_cur;
    vb->NonPagedPoolUsage = lim.rlim_cur;
    vb->NonPagedPoolLimit = lim.rlim_max;

    linux_proc prc;
    readProcStat(processID(pid), prc);
    vb->PeakPagefileUsage = prc.rss * pg_size;
    vb->PagefileUsage = prc.rss * pg_size;
    vb->PagefileLimit = 0xFFFFFFFF;
#elif defined(__APPLE__)
#else                                     // All Other
#endif                                    // End ALL OS
  }
  if (info)
    *info = *vb;
  return status;
}

long ProcessDescriptor::query(long pid, InfoType fetch,
    long* info) {
  long status = 1, *vb;
  ProcessHandle h(pid);
  vb = &m_PRIORITYBOOST[h.item()];
  *vb = 0;
  switch (fetch) {
  case InfoType::PriorityBoost:
    // Not applicable
    status = 0;
    *vb = 0;
    status = (status == 0) ? 1 : 0;
    break;
  default:
    status = -1;
    vb = &status;
    break;
  }
  if (info)
    *info = *vb;
  return status;
}

long ProcessDescriptor::query(long pid, InfoType fetch,
    VM_COUNTERS* info) {
  long status = 1;
  ProcessHandle h(pid);
  VM_COUNTERS* vb = &m_VM_COUNTERS[h.item()];
  if (fetch == InfoType::Memory) {
#if defined(__linux)
    const ssize_t bufsize = 1024;
    char buf[bufsize];
    sprintf(buf, "/proc/%ld/statm", processID(pid));
    long size, resident, share, trs, lrs, drs, dt;
    int fd = open(buf, O_RDONLY);
    ssize_t nread = read(fd, buf, bufsize);
    close(fd);
    if (nread < bufsize && nread >= 0)
      buf[nread] = '\0';
    sscanf(buf, "%ld %ld %ld %ld %ld %ld %ld", &size, &resident, &share,
                                               &trs, &drs, &lrs, &dt);
    linux_proc prc;
    readProcStat(processID(pid), prc);
    vb->PeakVirtualSize = prc.vsize;
    vb->VirtualSize = prc.vsize;
    vb->PeakWorkingSetSize = resident * pg_size;
    vb->WorkingSetSize = resident * pg_size;
    vb->QuotaPeakPagedPoolUsage = share * pg_size;
    vb->QuotaPagedPoolUsage = share * pg_size;
    vb->QuotaNonPagedPoolUsage = (trs + drs) * pg_size; // drs = data/stack size
    vb->QuotaPeakNonPagedPoolUsage = (trs + drs) * pg_size;  // trs = VmExe size
    vb->PageFaultCount = prc.majflt + prc.minflt;
    vb->PagefileUsage = prc.vsize - resident * pg_size;
    vb->PeakPagefileUsage = prc.vsize - resident * pg_size;
#elif defined(__APPLE__)
#else                                     // All Other
#endif                                    // End ALL OS
  }
  if (info)
    *info = *vb;
  return status;
}

long ProcessDescriptor::query(long pid, InfoType fetch,
    QUOTA_LIMITS* info) {
  long status = 1;
  ProcessHandle h(pid);
  QUOTA_LIMITS* vb = &m_QUOTA_LIMITS[h.item()];
  if (fetch == InfoType::Quota) {
#if defined(__linux)
    // On linux all this stuff typically is not set
    // (ie. rlim_max=RLIM_INFINITY...)
    rlimit lim;
    getrlimit(RLIMIT_DATA, &lim);
    if (lim.rlim_max == RLIM_INFINITY )
      lim.rlim_max = 0xFFFFFFFF;
    vb->PagedPoolLimit = lim.rlim_max;

    getrlimit(RLIMIT_STACK, &lim);
    if (lim.rlim_max == RLIM_INFINITY )
      lim.rlim_max = 0xFFFFFFFF;
    vb->NonPagedPoolLimit = lim.rlim_max;
    vb->MinimumWorkingSetSize = 0;

    getrlimit(RLIMIT_RSS, &lim);
    if (lim.rlim_max == RLIM_INFINITY )
      lim.rlim_max = 0xFFFFFFFF;
    vb->MaximumWorkingSetSize = lim.rlim_max;

    getrlimit(RLIMIT_AS, &lim);
    if (lim.rlim_max == RLIM_INFINITY )
      lim.rlim_max = 0xFFFFFFFF;
    vb->PagefileLimit = lim.rlim_max;

    getrlimit(RLIMIT_CPU, &lim);
    if (lim.rlim_max == RLIM_INFINITY )
      lim.rlim_max = 0xFFFFFFFF;
    vb->TimeLimit = lim.rlim_max;
#elif defined(__APPLE__)
#else                                     // All Other
#endif                                    // End ALL OS
  }
  if (info)
    *info = *vb;
  return status;
}

long ProcessDescriptor::query(long pid, InfoType fetch,
    PROCESS_BASIC_INFORMATION* info) {
  long status = 1;
  ProcessHandle h(pid);
  PROCESS_BASIC_INFORMATION* vb = &m_PROCESS_BASIC_INFORMATION[h.item()];
  if (fetch == InfoType::ProcessBasics) {
#if defined(__linux)
    linux_proc prc;
    readProcStat(processID(pid), prc);
    vb->ExitStatus = 0;
    vb->PebBaseAddress = (PPEB) prc.startcode;
    vb->BasePriority = 2 * 15 - prc.priority;
    // cout << "Base Priority=" << vb->BasePriority << "|"
    // << prc.priority << endl;
    vb->AffinityMask = prc.flags;
    // cout << "Flags        =" << vb->AffinityMask << "|"
    // << prc.flags << endl;
    vb->UniqueProcessId = processID(pid);
    vb->InheritedFromUniqueProcessId = prc.ppid;
#else                                     // All Other
#endif                                    // End ALL OS
  }
  if (info)
    *info = *vb;
  return status;
}

long ProcessDescriptor::query(long pid, InfoType fetch,
    KERNEL_USER_TIMES* info) {
  long status = 1;
  ProcessHandle h(pid);
  KERNEL_USER_TIMES* tb = &m_KERNEL_USER_TIMES[h.item()];
  ;
  if (fetch == InfoType::Times) {
#if defined(__linux)                      // Linux
    // prc.startup is in ticks since system start :
    // need to offset for absolute time
    tms tmsb;
    static long long prc_start = 0;
    //    static longlong offset = 100*longlong(time(0)) - longlong(times(0));
    static long long offset = 100 * (long long)(time(0)) - (long long)(times(&tmsb));
    if (processID(pid) == s_myPid && prc_start == 0) {
      linux_proc prc;
      readProcStat(processID(pid), prc);
      prc_start = prc.starttime + offset;
    }

    if (processID(pid) == s_myPid) {
      struct rusage r;
      getrusage(RUSAGE_SELF, &r);
      tb->UserTime = (static_cast<long long>(r.ru_utime.tv_sec) * 1000000
          + r.ru_utime.tv_usec) * 10;
      tb->KernelTime = (static_cast<long long>(r.ru_stime.tv_sec) * 1000000
          + r.ru_stime.tv_usec) * 10;
      tb->CreateTime = prc_start;
    } else {
      linux_proc prc;
      readProcStat(processID(pid), prc);

      tms t;
      times(&t);
      tb->UserTime = t.tms_utime * TICK_TO_100NSEC;
      tb->KernelTime = t.tms_stime * TICK_TO_100NSEC;
      tb->CreateTime = (prc.starttime + offset);
    }
    tb->CreateTime *= TICK_TO_100NSEC;
    tb->ExitTime = 0;

    status = 1;

#elif defined(__APPLE__)
    /// @todo (MCl) Make an alternative function get timing on OSX
    /// times() seems to cause a segmentation fault
#else  // no /proc file system: assume sys_start for the first call
    tms tmsb;
    static clock_t sys_start = times(0);
    static longlong offset = 100*longlong(time(0)) - sys_start;
    clock_t now = times(&tmsb);
    tb->CreateTime = offset + now;
    tb->UserTime = tmsb.tms_utime;
    tb->KernelTime = tmsb.tms_stime;
    tb->CreateTime *= TICK_TO_100NSEC;
    tb->UserTime *= TICK_TO_100NSEC;
    tb->KernelTime *= TICK_TO_100NSEC;
    tb->ExitTime = 0;
    status = 1;
#endif
  }
  if (info)
    *info = *tb;
  return status;
}

} // namespace System
} // namespace Elements

