/**
 * @file ElementsKernel/ThisModule.h
 * @brief header to get the module info statically
 * @date Dec 1, 2014
 * @author hubert
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_THISMODULE_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_THISMODULE_H_

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

/** @example ElementsExamples/src/lib/ModuleInfo.cpp
 * This is an example of how to use the getThisModuleInfo function.
 * The call has to be compiled in the module (library)
 */

ELEMENTS_API const ModuleInfo& getThisExecutableInfo();

} // namespace System
} // namespace Elements

#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_THISMODULE_H_
