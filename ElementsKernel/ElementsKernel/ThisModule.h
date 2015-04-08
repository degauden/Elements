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

/** @example ElementsExamples/src/Lib/ModuleInfo.cpp
 * This is an example of how to use the getThisModuleInfo function.
 * The call has to be compiled in the module (library)
 */


ELEMENTS_API const ModuleInfo& getThisExecutableInfo();



} // namespace System
} // namespace Elements



#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_THISMODULE_H_
