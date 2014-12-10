/**
 * @file ThisModule.h
 *
 * @date Dec 1, 2014
 * @author hubert
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_THISMODULE_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_THISMODULE_H_

#include <memory>
#include <dlfcn.h>

#include "ElementsKernel/FuncPtrCast.h"


namespace Elements {
namespace System {


static std::unique_ptr<Dl_info> this_module;

static inline const Dl_info& getThisModuleInfo() {

  if ( this_module == nullptr) {
    this_module.reset(new Dl_info);
    ::dladdr(FuncPtrCast<void*>(getThisModuleInfo), this_module.get());
  }

  return *this_module;
}


static inline std::string getThisModuleName() {
  return std::string(getThisModuleInfo().dli_fname);
}

} // namespace System
} // namespace Elements



#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_THISMODULE_H_
