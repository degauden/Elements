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

#ifndef ELEMENTSKERNEL_MODULEINFO_H
#define ELEMENTSKERNEL_MODULEINFO_H

// STL include files
#include <string>
#include <vector>
#include <memory>
#include <dlfcn.h>

// Framework include files
#include "ElementsKernel/Export.h" // ELEMENTS_API

namespace Elements {
namespace System {

/// constant that represent the common prefix of the libraries
static std::string LIB_PREFIX = std::string("lib");
#ifndef __APPLE__
/// constant that represents the standard extension of the libraries
static std::string LIB_EXTENSION = std::string("so");
#else
static std::string LIB_EXTENSION = std::string("dylib");
#endif
/// constant that represents the standard suffix of the libraries:
/// usually "."+LIB_EXTENSION
static std::string LIB_SUFFIX = "." + LIB_EXTENSION;

class ELEMENTS_API ModuleInfo {
public:
  ModuleInfo();
  ModuleInfo(void *);
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
/// Definition of an image handle
using ImageHandle = void*;
/// Definition of the process handle
using ProcessHandle = void*;
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
/// Attach module handle
ELEMENTS_API void setModuleHandle(ImageHandle handle);

}
}
#endif // ELEMENTSKERNEL_MODULEINFO_H
