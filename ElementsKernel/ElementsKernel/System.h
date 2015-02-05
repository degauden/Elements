#ifndef ELEMENTSKERNEL_SYSTEM_H
#define ELEMENTSKERNEL_SYSTEM_H

// STL include files
#include <vector>
#include <typeinfo>
#include <string>

// Framework include files
#include "ElementsKernel/Export.h" // ELEMENTS_API


#ifdef __linux
# include <pthread.h>
#ifndef __APPLE__
# include <execinfo.h>
#endif
#endif

/** Note: OS specific details as well as Elements details may not occur
 in this definition file, because this header is the used by both, the
 OS specific implementations and the elements specific implementation.
 Since e.g. IID is defined in both, this would lead automatically to
 complete comilation failures.....

 @author M.Frank
 */
namespace Elements {
namespace System {
/// Definition of an image handle
using ImageHandle = void*;
/// Definition of the process handle
using ProcessHandle = void*;
/// Definition of the "generic" DLL entry point function
using EntryPoint = unsigned long (*)(const unsigned long iid, void** ppvObject);
/// Definition of the "generic" DLL entry point function
using Creator = void* (*)();

#ifdef __linux
///A Thread handle
using ThreadHandle = pthread_t;
///thread handle "accessor"
#else
///A Thread handle
using ThreadHandle = void*;
#endif

/// Load dynamic link library
ELEMENTS_API unsigned long loadDynamicLib(const std::string& name,
    ImageHandle* handle);
/// unload dynamic link library
ELEMENTS_API unsigned long unloadDynamicLib(ImageHandle handle);
/// Get a specific function defined in the DLL
ELEMENTS_API unsigned long getProcedureByName(ImageHandle handle,
    const std::string& name, EntryPoint* pFunction);
/// Get a specific function defined in the DLL
ELEMENTS_API unsigned long getProcedureByName(ImageHandle handle,
    const std::string& name, Creator* pFunction);
/// Get last system known error
ELEMENTS_API unsigned long getLastError();
/// Get last system error as string
ELEMENTS_API const std::string getLastErrorString();
/// Retrieve error code as string for a given error
ELEMENTS_API const std::string getErrorString(unsigned long error);
/// Get platform independent information about the class type
ELEMENTS_API const std::string typeinfoName(const std::type_info&);
ELEMENTS_API const std::string typeinfoName(const char*);
/// Host name
ELEMENTS_API const std::string& hostName();
/// OS name
ELEMENTS_API const std::string& osName();
/// OS version
ELEMENTS_API const std::string& osVersion();
/// Machine type
ELEMENTS_API const std::string& machineType();
/// User login name
ELEMENTS_API const std::string& accountName();
/// Number of arguments passed to the commandline
ELEMENTS_API long numCmdLineArgs();
/// Number of arguments passed to the commandline (==numCmdLineArgs()); just to match argv call...
ELEMENTS_API long argc();
/// Command line arguments including executable name as arg[0] as vector of strings
ELEMENTS_API const std::vector<std::string> cmdLineArgs();
///char** command line arguments including executable name as arg[0]; You may not modify them!
ELEMENTS_API char** argv();
///get a particular environment variable (returning "UNKNOWN" if not set)
ELEMENTS_API std::string getEnv(const char* var);
/// get a particular environment variable, storing the value in the passed string if the
/// variable is set. Returns true if the variable is set, false otherwise.
ELEMENTS_API bool getEnv(const char* var, std::string &value);
inline bool getEnv(const std::string &var, std::string &value) {
  return getEnv(var.c_str(), value);
}
///get all environment variables
ELEMENTS_API std::vector<std::string> getEnv();
///Set an environment variables.
///If value is empty, the variable is removed from the environment.
///When overwrite is 0, the variable is not set if already present.
///Returns 0 on success, -1 on failure.
///See man 3 setenv.
ELEMENTS_API int setEnv(const std::string &name, const std::string &value,
    int overwrite = 1);
/// Check if an environment variable is set or not.
ELEMENTS_API bool isEnvSet(const char* var);

///thread handle "accessor"
ELEMENTS_API ThreadHandle threadSelf();

ELEMENTS_API int backTrace(void** addresses __attribute__ ((unused)), const int depth __attribute__ ((unused)));
ELEMENTS_API bool backTrace(std::string& btrace, const int depth,
    const int offset = 0);
ELEMENTS_API bool getStackLevel(void* addresses __attribute__ ((unused)), void*& addr __attribute__ ((unused)), std::string& fnc __attribute__ ((unused)),
    std::string& lib __attribute__ ((unused)));

} // end of namespace System
} // end of namespace Elements

#endif    // SYSTEM_SYSTEM_H
