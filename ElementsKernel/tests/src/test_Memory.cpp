#include "ElementsKernel/Memory.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace Elements::System;

int main(int /* argc */, char** /* argv */) {
  size_t kB = size_t(1024);
  size_t nBytes = kB * kB * kB * size_t(5);
  void* p = ::malloc(nBytes);
  if (!p) {
    cout << "Failed to allocate " << nBytes << " bytes of memory." << endl;
    return 1;
  }
  cout << endl << "         Elements memory module test" << endl << endl
      << "  Test virtual memory allocation of " << nBytes << " Bytes ["
      << nBytes / kB << " KB, " << nBytes / kB / kB << " MB, "
      << nBytes / kB / kB / kB << " GB]" << endl;
  cout << setw(50) << left << "  Current usage of non paged memory:" << setw(12)
      << right << nonPagedMemory(Byte, Memory, -1) << " Bytes " << setw(10)
      << right << nonPagedMemory(kByte, Memory, -1) << " kB " << setw(8)
      << right << nonPagedMemory(MByte, Memory, -1) << " MB " << setw(4)
      << right << nonPagedMemory(GByte, Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak usage of non paged memory:" << setw(12)
      << right << nonPagedMemoryPeak(Byte, Memory, -1) << " Bytes " << setw(10)
      << right << nonPagedMemoryPeak(kByte, Memory, -1) << " kB " << setw(8)
      << right << nonPagedMemoryPeak(MByte, Memory, -1) << " MB " << setw(4)
      << right << nonPagedMemoryPeak(GByte, Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of non-paged memory allowed:"
      << setw(12) << right << nonPagedMemoryLimit(Byte, Quota, -1) << " Bytes "
      << setw(10) << right << nonPagedMemoryLimit(kByte, Quota, -1) << " kB "
      << setw(8) << right << nonPagedMemoryLimit(MByte, Quota, -1) << " MB "
      << setw(4) << right << nonPagedMemoryLimit(GByte, Quota, -1) << " GB "
      << endl;
  cout << setw(50) << left << "  Amount of paged memory currently occupied:"
      << setw(12) << right << pagedMemory(Byte, Memory, -1) << " Bytes "
      << setw(10) << right << pagedMemory(kByte, Memory, -1) << " kB "
      << setw(8) << right << pagedMemory(MByte, Memory, -1) << " MB " << setw(4)
      << right << pagedMemory(GByte, Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum of paged memory occupied:" << setw(12)
      << right << pagedMemoryPeak(Byte, Memory, -1) << " Bytes " << setw(10)
      << right << pagedMemoryPeak(kByte, Memory, -1) << " kB " << setw(8)
      << right << pagedMemoryPeak(MByte, Memory, -1) << " MB " << setw(4)
      << right << pagedMemoryPeak(GByte, Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of paged memory that can be occupied:"
      << setw(12) << right << pagedMemoryLimit(Byte, Quota, -1) << " Bytes "
      << setw(10) << right << pagedMemoryLimit(kByte, Quota, -1) << " kB "
      << setw(8) << right << pagedMemoryLimit(MByte, Quota, -1) << " MB "
      << setw(4) << right << pagedMemoryLimit(GByte, Quota, -1) << " GB "
      << endl;
  cout << setw(50) << left << "  Minimum amount of virtual memory:" << setw(12)
      << right << minMemoryLimit(Byte, Quota, -1) << " Bytes " << setw(10)
      << right << minMemoryLimit(kByte, Quota, -1) << " kB " << setw(8) << right
      << minMemoryLimit(MByte, Quota, -1) << " MB " << setw(4) << right
      << minMemoryLimit(GByte, Quota, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of virtual memory allowed:"
      << setw(12) << right << maxMemoryLimit(Byte, Quota, -1) << " Bytes "
      << setw(10) << right << maxMemoryLimit(kByte, Quota, -1) << " kB "
      << setw(8) << right << maxMemoryLimit(MByte, Quota, -1) << " MB "
      << setw(4) << right << maxMemoryLimit(GByte, Quota, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of mapped memory:" << setw(12) << right
      << mappedMemory(Byte, Memory, -1) << " Bytes " << setw(10) << right
      << mappedMemory(kByte, Memory, -1) << " kB " << setw(8) << right
      << mappedMemory(MByte, Memory, -1) << " MB " << setw(4) << right
      << mappedMemory(GByte, Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak of mapped mameory usage:" << setw(12)
      << right << mappedMemoryPeak(Byte, Memory, -1) << " Bytes " << setw(10)
      << right << mappedMemoryPeak(kByte, Memory, -1) << " kB " << setw(8)
      << right << mappedMemoryPeak(MByte, Memory, -1) << " MB " << setw(4)
      << right << mappedMemoryPeak(GByte, Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of virtual memory used:" << setw(12)
      << right << virtualMemory(Byte, Memory, -1) << " Bytes " << setw(10)
      << right << virtualMemory(kByte, Memory, -1) << " kB " << setw(8) << right
      << virtualMemory(MByte, Memory, -1) << " MB " << setw(4) << right
      << virtualMemory(GByte, Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak of virtual memory usage:" << setw(12)
      << right << virtualMemoryPeak(Byte, Memory, -1) << " Bytes " << setw(10)
      << right << virtualMemoryPeak(kByte, Memory, -1) << " kB " << setw(8)
      << right << virtualMemoryPeak(MByte, Memory, -1) << " MB " << setw(4)
      << right << virtualMemoryPeak(GByte, Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of the page file allowed:"
      << setw(12) << right << virtualMemoryLimit(Byte, Quota, -1) << " Bytes "
      << setw(10) << right << virtualMemoryLimit(kByte, Quota, -1) << " kB "
      << setw(8) << right << virtualMemoryLimit(MByte, Quota, -1) << " MB "
      << setw(4) << right << virtualMemoryLimit(GByte, Quota, -1) << " GB "
      << endl;
  return 0;
}
