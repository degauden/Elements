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

#include "ElementsKernel/ModuleInfo.h"
#include "ElementsKernel/FuncPtrCast.h"


namespace Elements {
namespace System {

static inline const ModuleInfo& getThisModuleInfo() {
  static ModuleInfo this_module;
  if (this_module.isEmpty()) {
    this_module = ModuleInfo(FuncPtrCast<void*>(getThisModuleInfo));
  }

  return this_module;
}


ELEMENTS_API const ModuleInfo& getThisExecutableInfo();



} // namespace System
} // namespace Elements



#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_THISMODULE_H_
