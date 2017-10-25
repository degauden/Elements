/**
 * @file ThisModule.cpp
 *
 * @date Dec 10, 2014
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
 *
 */

#include <cstddef>

#include "ElementsKernel/ThisModule.h"
#include "ElementsKernel/FuncPtrCast.h"  // for FuncPtrCast
#include "ElementsKernel/ModuleInfo.h"   // for ModuleInfo

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
