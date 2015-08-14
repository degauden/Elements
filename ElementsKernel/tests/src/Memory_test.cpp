
#include "ElementsKernel/Memory.h"

#include "ElementsKernel/Unused.h"

#include <cstdlib>    // for free, malloc
#include <iomanip>
#include <iostream>   // for std::cout
#include <memory>     // for unique_ptr

using namespace std;

int main(ELEMENTS_UNUSED int argc, ELEMENTS_UNUSED char** argv) {

  using Elements::System::MemoryUnit;
  using Elements::System::InfoType;

  size_t kB = size_t(1024);
  size_t nBytes = kB * kB * size_t(500);

  unique_ptr<char> p {new char[nBytes]};
  if (p == nullptr) {
    cout << "Failed to allocate " << nBytes << " bytes of memory." << endl;
    return 1;
  }
  cout << endl << "         Elements memory module test" << endl << endl
      << "  Test virtual memory allocation of " << nBytes << " Bytes ["
      << nBytes / kB << " KB, " << nBytes / kB / kB << " MB, "
      << nBytes / kB / kB / kB << " GB]" << endl;
  cout << setw(50) << left << "  Current usage of non paged memory:" << setw(12)
      << right << nonPagedMemory(MemoryUnit::Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << nonPagedMemory(MemoryUnit::kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << nonPagedMemory(MemoryUnit::MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << nonPagedMemory(MemoryUnit::GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak usage of non paged memory:" << setw(12)
      << right << nonPagedMemoryPeak(MemoryUnit::Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << nonPagedMemoryPeak(MemoryUnit::kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << nonPagedMemoryPeak(MemoryUnit::MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << nonPagedMemoryPeak(MemoryUnit::GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of non-paged memory allowed:"
      << setw(12) << right << nonPagedMemoryLimit(MemoryUnit::Byte, InfoType::Quota, -1) << " Bytes "
      << setw(10) << right << nonPagedMemoryLimit(MemoryUnit::kByte, InfoType::Quota, -1) << " kB "
      << setw(8) << right << nonPagedMemoryLimit(MemoryUnit::MByte, InfoType::Quota, -1) << " MB "
      << setw(4) << right << nonPagedMemoryLimit(MemoryUnit::GByte, InfoType::Quota, -1) << " GB "
      << endl;
  cout << setw(50) << left << "  Amount of paged memory currently occupied:"
      << setw(12) << right << pagedMemory(MemoryUnit::Byte, InfoType::Memory, -1) << " Bytes "
      << setw(10) << right << pagedMemory(MemoryUnit::kByte, InfoType::Memory, -1) << " kB "
      << setw(8) << right << pagedMemory(MemoryUnit::MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << pagedMemory(MemoryUnit::GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum of paged memory occupied:" << setw(12)
      << right << pagedMemoryPeak(MemoryUnit::Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << pagedMemoryPeak(MemoryUnit::kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << pagedMemoryPeak(MemoryUnit::MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << pagedMemoryPeak(MemoryUnit::GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of paged memory that can be occupied:"
      << setw(12) << right << pagedMemoryLimit(MemoryUnit::Byte, InfoType::Quota, -1) << " Bytes "
      << setw(10) << right << pagedMemoryLimit(MemoryUnit::kByte, InfoType::Quota, -1) << " kB "
      << setw(8) << right << pagedMemoryLimit(MemoryUnit::MByte, InfoType::Quota, -1) << " MB "
      << setw(4) << right << pagedMemoryLimit(MemoryUnit::GByte, InfoType::Quota, -1) << " GB "
      << endl;
  cout << setw(50) << left << "  Minimum amount of virtual memory:" << setw(12)
      << right << minMemoryLimit(MemoryUnit::Byte, InfoType::Quota, -1) << " Bytes " << setw(10)
      << right << minMemoryLimit(MemoryUnit::kByte, InfoType::Quota, -1) << " kB " << setw(8) << right
      << minMemoryLimit(MemoryUnit::MByte, InfoType::Quota, -1) << " MB " << setw(4) << right
      << minMemoryLimit(MemoryUnit::GByte, InfoType::Quota, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of virtual memory allowed:"
      << setw(12) << right << maxMemoryLimit(MemoryUnit::Byte, InfoType::Quota, -1) << " Bytes "
      << setw(10) << right << maxMemoryLimit(MemoryUnit::kByte, InfoType::Quota, -1) << " kB "
      << setw(8) << right << maxMemoryLimit(MemoryUnit::MByte, InfoType::Quota, -1) << " MB "
      << setw(4) << right << maxMemoryLimit(MemoryUnit::GByte, InfoType::Quota, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of mapped memory:" << setw(12) << right
      << mappedMemory(MemoryUnit::Byte, InfoType::Memory, -1) << " Bytes " << setw(10) << right
      << mappedMemory(MemoryUnit::kByte, InfoType::Memory, -1) << " kB " << setw(8) << right
      << mappedMemory(MemoryUnit::MByte, InfoType::Memory, -1) << " MB " << setw(4) << right
      << mappedMemory(MemoryUnit::GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak of mapped memory usage:" << setw(12)
      << right << mappedMemoryPeak(MemoryUnit::Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << mappedMemoryPeak(MemoryUnit::kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << mappedMemoryPeak(MemoryUnit::MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << mappedMemoryPeak(MemoryUnit::GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Amount of virtual memory used:" << setw(12)
      << right << virtualMemory(MemoryUnit::Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << virtualMemory(MemoryUnit::kByte, InfoType::Memory, -1) << " kB " << setw(8) << right
      << virtualMemory(MemoryUnit::MByte, InfoType::Memory, -1) << " MB " << setw(4) << right
      << virtualMemory(MemoryUnit::GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Peak of virtual memory usage:" << setw(12)
      << right << virtualMemoryPeak(MemoryUnit::Byte, InfoType::Memory, -1) << " Bytes " << setw(10)
      << right << virtualMemoryPeak(MemoryUnit::kByte, InfoType::Memory, -1) << " kB " << setw(8)
      << right << virtualMemoryPeak(MemoryUnit::MByte, InfoType::Memory, -1) << " MB " << setw(4)
      << right << virtualMemoryPeak(MemoryUnit::GByte, InfoType::Memory, -1) << " GB " << endl;
  cout << setw(50) << left << "  Maximum amount of the page file allowed:"
      << setw(12) << right << virtualMemoryLimit(MemoryUnit::Byte, InfoType::Quota, -1) << " Bytes "
      << setw(10) << right << virtualMemoryLimit(MemoryUnit::kByte, InfoType::Quota, -1) << " kB "
      << setw(8) << right << virtualMemoryLimit(MemoryUnit::MByte, InfoType::Quota, -1) << " MB "
      << setw(4) << right << virtualMemoryLimit(MemoryUnit::GByte, InfoType::Quota, -1) << " GB "
      << endl;


}
