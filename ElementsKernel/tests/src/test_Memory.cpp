#include "ElementsKernel/Memory.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace Elements::System;

int main(int /* argc */, char** /* argv */) {
  size_t kB = size_t(1024);
  size_t nBytes = kB * kB * size_t(500);
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
      << right << nonPagedMemory(Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << nonPagedMemory(kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << nonPagedMemory(MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << nonPagedMemory(GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak usage of non paged memory:" << setw(12)
      << right << nonPagedMemoryPeak(Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << nonPagedMemoryPeak(kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << nonPagedMemoryPeak(MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << nonPagedMemoryPeak(GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of non-paged memory allowed:"
      << setw(12) << right << nonPagedMemoryLimit(Byte, InfoType::Quota, -1) << " Bytes "
      << setw(10) << right << nonPagedMemoryLimit(kByte, InfoType::Quota, -1) << " kB "
      << setw(8) << right << nonPagedMemoryLimit(MByte, InfoType::Quota, -1) << " MB "
      << setw(4) << right << nonPagedMemoryLimit(GByte, InfoType::Quota, -1) << " GB "
      << endl;
  cout << setw(50) << left << "  Amount of paged memory currently occupied:"
      << setw(12) << right << pagedMemory(Byte, InfoType::Memory, -1) << " Bytes "
      << setw(10) << right << pagedMemory(kByte, InfoType::Memory, -1) << " kB "
      << setw(8) << right << pagedMemory(MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << pagedMemory(GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum of paged memory occupied:" << setw(12)
      << right << pagedMemoryPeak(Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << pagedMemoryPeak(kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << pagedMemoryPeak(MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << pagedMemoryPeak(GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of paged memory that can be occupied:"
      << setw(12) << right << pagedMemoryLimit(Byte, InfoType::Quota, -1) << " Bytes "
      << setw(10) << right << pagedMemoryLimit(kByte, InfoType::Quota, -1) << " kB "
      << setw(8) << right << pagedMemoryLimit(MByte, InfoType::Quota, -1) << " MB "
      << setw(4) << right << pagedMemoryLimit(GByte, InfoType::Quota, -1) << " GB "
      << endl;
  cout << setw(50) << left << "  Minimum amount of virtual memory:" << setw(12)
      << right << minMemoryLimit(Byte, InfoType::Quota, -1) << " Bytes " << setw(10)
      << right << minMemoryLimit(kByte, InfoType::Quota, -1) << " kB " << setw(8) << right
      << minMemoryLimit(MByte, InfoType::Quota, -1) << " MB " << setw(4) << right
      << minMemoryLimit(GByte, InfoType::Quota, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of virtual memory allowed:"
      << setw(12) << right << maxMemoryLimit(Byte, InfoType::Quota, -1) << " Bytes "
      << setw(10) << right << maxMemoryLimit(kByte, InfoType::Quota, -1) << " kB "
      << setw(8) << right << maxMemoryLimit(MByte, InfoType::Quota, -1) << " MB "
      << setw(4) << right << maxMemoryLimit(GByte, InfoType::Quota, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of mapped memory:" << setw(12) << right
      << mappedMemory(Byte, InfoType::Memory, -1) << " Bytes " << setw(10) << right
      << mappedMemory(kByte, InfoType::Memory, -1) << " kB " << setw(8) << right
      << mappedMemory(MByte, InfoType::Memory, -1) << " MB " << setw(4) << right
      << mappedMemory(GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak of mapped memory usage:" << setw(12)
      << right << mappedMemoryPeak(Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << mappedMemoryPeak(kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << mappedMemoryPeak(MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << mappedMemoryPeak(GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of virtual memory used:" << setw(12)
      << right << virtualMemory(Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << virtualMemory(kByte, InfoType::Memory, -1) << " kB " << setw(8) << right
      << virtualMemory(MByte, InfoType::Memory, -1) << " MB " << setw(4) << right
      << virtualMemory(GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak of virtual memory usage:" << setw(12)
      << right << virtualMemoryPeak(Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << virtualMemoryPeak(kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << virtualMemoryPeak(MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << virtualMemoryPeak(GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of the page file allowed:"
      << setw(12) << right << virtualMemoryLimit(Byte, InfoType::Quota, -1) << " Bytes "
      << setw(10) << right << virtualMemoryLimit(kByte, InfoType::Quota, -1) << " kB "
      << setw(8) << right << virtualMemoryLimit(MByte, InfoType::Quota, -1) << " MB "
      << setw(4) << right << virtualMemoryLimit(GByte, InfoType::Quota, -1) << " GB "
      << endl;
  return 0;
}
