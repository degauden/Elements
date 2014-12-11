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


const ModuleInfo& getThisExecutableInfo() {
  static ModuleInfo this_module;
  if (this_module.isEmpty()) {
    void* handle = ::dlopen(0, RTLD_LAZY);
    if ( NULL != handle) {
      void* func = ::dlsym(handle, "main");
      if ( NULL != func) {
        this_module = ModuleInfo(FuncPtrCast<void*>(func));
      }
    }
  }

  return this_module;
}


} // namespace System
} // namespace Elements
