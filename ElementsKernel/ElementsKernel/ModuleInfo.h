#ifndef ELEMENTSKERNEL_MODULEINFO_H
#define ELEMENTSKERNEL_MODULEINFO_H

// Framework include files
#include "ElementsKernel/Export.h" // ELEMENTS_API
// STL include files
#include <string>
#include <vector>

/** ModuleInfo: OS specific details to access at run-time the module
 configuration of the process.

 M.Frank
 */
namespace Elements {
namespace System {

enum class ModuleType {
  UNKNOWN, SHAREDLIB, EXECUTABLE
};
/// Definition of an image handle
typedef void* ImageHandle;
/// Get the name of the (executable/DLL) file without file-type
ELEMENTS_API const std::string& moduleName();
/// Get the full name of the (executable/DLL) file
ELEMENTS_API const std::string& moduleNameFull();
/// Get type of the module
ELEMENTS_API ModuleType moduleType();
/// Handle to running process
ELEMENTS_API void* processHandle();
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
