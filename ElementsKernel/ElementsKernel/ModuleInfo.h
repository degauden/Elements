#ifndef ELEMENTSKERNEL_MODULEINFO_H
#define ELEMENTSKERNEL_MODULEINFO_H

// STL include files
#include <string>
#include <vector>
#include <memory>
#include <dlfcn.h>

// Framework include files
#include "ElementsKernel/Export.h" // ELEMENTS_API
/** ModuleInfo: OS specific details to access at run-time the module
 configuration of the process.

 M.Frank
 */
namespace Elements {
namespace System {

static std::string LIB_PREFIX = std::string("lib");
static std::string LIB_EXTENSION = std::string("so");
static std::string LIB_SUFFIX = "." + LIB_EXTENSION;

class ELEMENTS_API ModuleInfo {
public:
  ModuleInfo();
  ModuleInfo(void *);
  const std::string name() const;
  operator const Dl_info&() const ;
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
