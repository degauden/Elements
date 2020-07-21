/**
 * @file ElementsKernel/ModuleInfo.h
 * @brief OS specific details to access at run-time the module
 * configuration of the process.
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_MODULEINFO_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_MODULEINFO_H_

// STL include files
#include <string>
#include <vector>
#include <memory>
#include <dlfcn.h>

// Framework include files
#include "ElementsKernel/System.h"      // LIB_PREFIX, LIB_EXTENSION
#include "ElementsKernel/Path.h"        // for Path::Item
#include "ElementsKernel/Export.h"      // ELEMENTS_API

namespace Elements {
namespace System {

class ELEMENTS_API ModuleInfo {
public:
  ModuleInfo();
  explicit ModuleInfo(void *);
  const std::string name() const;
  const std::string libraryName() const;
  const void* addresse() const;
  operator const Dl_info&() const;
  bool isEmpty() const;
private:
  std::unique_ptr<Dl_info> m_dlinfo;
};

enum class ModuleType {
  UNKNOWN, SHAREDLIB, EXECUTABLE
};

/// Get the name of the (executable/DLL) file without file-type
ELEMENTS_API const std::string& moduleName();
/// Get the full name of the (executable/DLL) file
ELEMENTS_API const std::string& moduleNameFull();
/// Get type of the module
ELEMENTS_API ModuleType moduleType();
/// Handle to running process
ELEMENTS_API ProcessHandle processHandle();
/// Handle to currently executed module
ELEMENTS_API ImageHandle moduleHandle();
/// Handle to the executable file running
ELEMENTS_API ImageHandle exeHandle();
/// Name of the executable file running
ELEMENTS_API const std::string& exeName();
/// Vector of names of linked modules
ELEMENTS_API const std::vector<std::string> linkedModules();
ELEMENTS_API std::vector<Path::Item> linkedModulePaths();
/// Attach module handle
ELEMENTS_API void setModuleHandle(ImageHandle handle);
/// Get the full executable path
ELEMENTS_API Path::Item getExecutablePath();
/// Get the path to the /proc directory of the process
ELEMENTS_API Path::Item getSelfProc();


}  // namespace System
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_MODULEINFO_H_

/**@}*/
