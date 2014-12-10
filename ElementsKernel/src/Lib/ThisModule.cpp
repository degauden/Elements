/**
 * @file ThisModule.cpp
 *
 * @date Dec 10, 2014
 * @author hubert
 */

#include <cstddef>

#include "ElementsKernel/ThisModule.h"

namespace Elements {
namespace System {


const Dl_info& getThisExecutableInfo() {

  static std::unique_ptr<Dl_info> this_exe_info;
  if (this_exe_info == nullptr) {
    void* handle = ::dlopen(0, RTLD_LAZY);
    if ( NULL != handle) {
      void* func = ::dlsym(handle, "main");
      if ( NULL != func) {
        this_exe_info.reset(new Dl_info);
        ::dladdr(func, this_exe_info.get());
      }
    }
  }

  return *this_exe_info;

}

std::string getThisExecutableName() {
  return std::string(getThisExecutableInfo().dli_fname);
}


} // namespace System
} // namespace Elements
