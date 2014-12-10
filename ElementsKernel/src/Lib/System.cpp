// $Id: System.cpp,v 1.45 2008/10/27 21:30:32 marcocle Exp $
//====================================================================
//	System.cpp
//--------------------------------------------------------------------
//
//	Package    : System (The LHCb System service)
//
//  Description: Implementation of Systems internals
//
//	Author     : M.Frank
//  Created    : 13/1/99
//	Changes    :
//====================================================================
#define SYSTEM_SYSTEM_CPP

#include "ElementsKernel/System.h"
#include "ElementsKernel/FuncPtrCast.h"

#include <cstdlib>                      // for free, getenv, malloc, etc
#include <typeinfo>                     // for type_info
#include <sstream>
#include <iomanip>
#include <iostream>

#include <dlfcn.h>                      // for Dl_info, dladdr, dlclose, etc
#include <errno.h>                      // for errno
#include <execinfo.h>                   // for backtrace
#include <string.h>                     // for strlen
#include <unistd.h>                     // for environ
#include <cxxabi.h>
#include <sys/utsname.h>

#include "ElementsKernel/ModuleInfo.h"  // for ImageHandle
#include "ElementsKernel/Unused.h"   // for ELEMENTS_UNUSED

using namespace std;

static const char* SHLIB_SUFFIX = ".so";

static vector<string> s_argvStrings;
static vector<const char*> s_argvChars;

namespace Elements {
namespace System {

// --------------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------------
static unsigned long doLoad(const string& name, ImageHandle* handle) {
# if defined(__linux) || defined(__APPLE__)
  const char* path = name.c_str();
  void *mh = ::dlopen(name.length() == 0 ? 0 : path, RTLD_LAZY | RTLD_GLOBAL);
  *handle = mh;
# endif
  if (0 == *handle) {
    return getLastError();
  }
  return 1;
}

static unsigned long loadWithoutEnvironment(const string& name,
    ImageHandle* handle) {

  string dllName = name;
  unsigned long len = static_cast<unsigned long>(strlen(SHLIB_SUFFIX));

  // Add the suffix at the end of the library name only if necessary
  /// @todo cure the logic
  if ((dllName.length() != 0)
      && ::strncasecmp(
          static_cast<const char *>(dllName.data() + dllName.length() - len),
          SHLIB_SUFFIX, len) != 0) {
    dllName += SHLIB_SUFFIX;
  }

  // Load the library
  return doLoad(dllName, handle);
}

// --------------------------------------------------------------------------------------

/// Load dynamic link library
unsigned long loadDynamicLib(const string& name, ImageHandle* handle) {
  unsigned long res;
  // if name is empty, just load it
  if (name.length() == 0) {
    res = loadWithoutEnvironment(name, handle);
  } else {
    // If the name is a logical name (environment variable), the try
    // to load the corresponding library from there.
    string imgName;
    if (getEnv(name, imgName)) {
      res = loadWithoutEnvironment(imgName, handle);
    } else {
      // build the dll name
      string dllName = name;
#if defined(__linux) || defined(__APPLE__)
      dllName = "lib" + dllName;
#endif
      dllName += SHLIB_SUFFIX;
      // try to locate the dll using the standard PATH
      res = loadWithoutEnvironment(dllName, handle);
    }
    if (res != 1) {
#if defined(__linux) || defined(__APPLE__)
      errno = 0xAFFEDEAD;
#endif
    }
  }
  return res;
}

/// unload dynamic link library
unsigned long unloadDynamicLib(ImageHandle handle) {
  ::dlclose(handle);
  if (0) {
    return getLastError();
  }
  return 1;
}

/// Get a specific function defined in the DLL
unsigned long getProcedureByName(ImageHandle handle, const string& name,
    EntryPoint* pFunction) {
#if defined(__linux)
  *pFunction = FuncPtrCast<EntryPoint>(::dlsym(handle, name.c_str()));
  if (0 == *pFunction) {
    errno = 0xAFFEDEAD;
    return 0;
  }
  return 1;
#elif defined(__APPLE__)
  *pFunction = (EntryPoint)::dlsym(handle, name.c_str());
  if(!(*pFunction)) {
    // Try with an underscore :
    string sname = "_" + name;
    *pFunction = (EntryPoint)::dlsym(handle, sname.c_str());
  }
  if ( 0 == *pFunction ) {
    errno = 0xAFFEDEAD;
    std::cout << "Elements::System::getProcedureByName>" << getLastErrorString() << std::endl;
    return 0;
  }
  return 1;
#endif
}

/// Get a specific function defined in the DLL
unsigned long getProcedureByName(ImageHandle handle, const string& name,
    Creator* pFunction) {
  return getProcedureByName(handle, name, (EntryPoint*) pFunction);
}

/// Retrieve last error code
unsigned long getLastError() {
  // convert errno (int) to unsigned long
  return static_cast<unsigned long>(static_cast<unsigned int>(errno));
}

/// Retrieve last error code as string
const string getLastErrorString() {
  const string errString = getErrorString(getLastError());
  return errString;
}

/// Retrieve error code as string for a given error
const string getErrorString(unsigned long error) {
  string errString = "";
  char *cerrString(0);
  // Remember: for linux dl* routines must be handled differently!
  if (error == 0xAFFEDEAD) {
    cerrString = (char*) ::dlerror();
    if (0 == cerrString) {
      cerrString = ::strerror(error);
    }
    if (0 == cerrString) {
      cerrString =
          (char *) "Unknown error. No information found in strerror()!";
    } else {
      errString = string(cerrString);
    }
    errno = 0;
  } else {
    cerrString = ::strerror(error);
    errString = string(cerrString);
  }
  return errString;
}

const string typeinfoName(const std::type_info& tinfo) {
  return typeinfoName(tinfo.name());
}

const string typeinfoName(const char* class_name) {
  string result;
  if (::strlen(class_name) == 1) {
    // See http://www.realitydiluted.com/mirrors/reality.sgi.com/dehnert_engr/cxx/abi.pdf
    // for details
    switch (class_name[0]) {
      case 'v':
        result = "void";
        break;
      case 'w':
        result = "wchar_t";
        break;
      case 'b':
        result = "bool";
        break;
      case 'c':
        result = "char";
        break;
      case 'a':
        result = "signed char";
        break;
      case 'h':
        result = "unsigned char";
        break;
      case 's':
        result = "short";
        break;
      case 't':
        result = "unsigned short";
        break;
      case 'i':
        result = "int";
        break;
      case 'j':
        result = "unsigned int";
        break;
      case 'l':
        result = "long";
        break;
      case 'm':
        result = "unsigned long";
        break;
      case 'x':
        result = "long long";
        break;
      case 'y':
        result = "unsigned long long";
        break;
      case 'n':
        result = "__int128";
        break;
      case 'o':
        result = "unsigned __int128";
        break;
      case 'f':
        result = "float";
        break;
      case 'd':
        result = "double";
        break;
      case 'e':
        result = "long double";
        break;
      case 'g':
        result = "__float128";
        break;
      case 'z':
        result = "ellipsis";
        break;
    }
  } else {
    int status;
    char* realname;
    realname = abi::__cxa_demangle(class_name, 0, 0, &status);
    if (realname == 0)
      return class_name;
    result = realname;
    ::free(realname);
    /// substitute ', ' with ','
    string::size_type pos = result.find(", ");
    while (string::npos != pos) {
      result.replace(pos, static_cast<string::size_type>(2), ",");
      pos = result.find(", ");
    }
  }
  return result;
}

/// Host name
const string& hostName() {
  static string host = "";
  if (host == "") {
    char buffer[512];
    ::memset(buffer, 0, sizeof(buffer));
    ::gethostname(buffer, sizeof(buffer));
    host = buffer;
  }
  return host;
}

/// OS name
const string& osName() {
  static string osname = "";
  struct utsname ut;
  if (uname(&ut) == 0) {
    osname = ut.sysname;
  } else {
    osname = "UNKNOWN";
  }
  return osname;
}

/// OS version
const string& osVersion() {
  static string osver = "";
  struct utsname ut;
  if (uname(&ut) == 0) {
    osver = ut.release;
  } else {
    osver = "UNKNOWN";
  }
  return osver;
}

/// Machine type
const string& machineType() {
  static string mach = "";
  struct utsname ut;
  if (uname(&ut) == 0) {
    mach = ut.machine;
  } else {
    mach = "UNKNOWN";
  }
  return mach;
}

/// User login name
const string& accountName() {
  static string account = "";
  if (account == "") {
    const char* acct = ::getlogin();
    if (0 == acct)
      acct = ::getenv("LOGNAME");
    if (0 == acct)
      acct = ::getenv("USER");
    account = (acct) ? acct : "Unknown";
  }
  return account;
}

/// Number of arguments passed to the commandline
long numCmdLineArgs() {
  return cmdLineArgs().size();
}

/// Number of arguments passed to the commandline
long argc() {
  return cmdLineArgs().size();
}

/// Const char** command line arguments including executable name as arg[0]
const vector<string> cmdLineArgs() {
  if (s_argvChars.size() == 0) {
    char exe[1024];
    sprintf(exe, "/proc/%d/cmdline", ::getpid());
    FILE *cmdLine = ::fopen(exe, "r");
    char cmd[1024];
    if (cmdLine) {
      long len = ::fread(cmd, sizeof(char), sizeof(cmd), cmdLine);
      if (len > 0) {
        cmd[len] = 0;
        for (char* token = cmd; token - cmd < len; token += strlen(token) + 1) {
          s_argvStrings.push_back(token);
          s_argvChars.push_back(s_argvStrings.back().c_str());
        }
        s_argvStrings[0] = exeName();
        s_argvChars[0] = s_argvStrings[0].c_str();
      }
      ::fclose(cmdLine);
    }
  }
  return s_argvStrings;
}

/// Const char** command line arguments including executable name as arg[0]
char** argv() {
  ///
  if (s_argvChars.empty()) {
    cmdLineArgs();
  }  /// added by I.B.
  ///
  // We rely here on the fact that a vector's allocation table is contiguous
  return (char**) &s_argvChars[0];
}

/// get a particular env var, return "UNKNOWN" if not defined
string getEnv(const char* var) {
  char* env;
  if ((env = ::getenv(var)) != 0) {
    return env;
  } else {
    return "UNKNOWN";
  }
}

/// get a particular env var, storing the value in the passed string (if set)
bool getEnv(const char* var, string &value) {
  char* env;
  if ((env = ::getenv(var)) != 0) {
    value = env;
    return true;
  } else {
    return false;
  }
}

bool isEnvSet(const char* var) {
  return ::getenv(var) != 0;
}

/// get all defined environment vars
#if defined(__APPLE__)
// Needed for _NSGetEnviron(void)
#include "crt_externs.h"
#endif
vector<string> getEnv() {
#if defined(__APPLE__)
  static char **environ = *_NSGetEnviron();
#endif
  vector<string> vars;
  for (int i = 0; environ[i] != 0; ++i) {
    vars.push_back(environ[i]);
  }
  return vars;
}

ThreadHandle threadSelf() {
#ifdef __linux
  return pthread_self();
#else
  return (void*)0;
#endif
}

// -----------------------------------------------------------------------------
// backtrace utilities
// -----------------------------------------------------------------------------
#ifdef __linux
#include <execinfo.h>
#endif

int backTrace(void** addresses ELEMENTS_UNUSED,
    const int depth ELEMENTS_UNUSED) {

#ifdef __linux

  int count = ::backtrace(addresses, depth);
  if (count > 0) {
    return count;
  } else {
    return 0;
  }

#else // windows and osx parts not implemented
  return 0;
#endif

}

bool backTrace(string& btrace, const int depth, const int offset) {
  // Always hide the first two levels of the stack trace (that's us)
  const int totalOffset = offset + 2;
  const int totalDepth = depth + totalOffset;
  bool result = false;

  string fnc, lib;

  void** addresses = (void**) malloc(totalDepth * sizeof(void *));
  if (addresses != NULL) {
    int count = backTrace(addresses, totalDepth);
    for (int i = totalOffset; i < count; ++i) {
      void *addr = 0;

      if (getStackLevel(addresses[i], addr, fnc, lib)) {
        ostringstream ost;
        ost << "#" << setw(3) << setiosflags(ios::left) << i - totalOffset + 1;
        ost << hex << addr << dec << " " << fnc << "  [" << lib << "]" << endl;
        btrace += ost.str();
      }
    }
    ::free(addresses);
    addresses = NULL;
    result = true;
  }

  return result;
}

bool getStackLevel(void* addresses ELEMENTS_UNUSED, void*& addr ELEMENTS_UNUSED,
    string& fnc ELEMENTS_UNUSED, string& lib ELEMENTS_UNUSED) {

#ifdef __linux

  Dl_info info;

  if (::dladdr(addresses, &info) && info.dli_fname
      && info.dli_fname[0] != '\0') {
    const char* symbol =
        info.dli_sname && info.dli_sname[0] != '\0' ? info.dli_sname : 0;

    lib = info.dli_fname;
    addr = info.dli_saddr;
    const char* dmg(0);

    if (symbol != 0) {
      int stat;
      dmg = abi::__cxa_demangle(symbol, 0, 0, &stat);
      fnc = (stat == 0) ? dmg : symbol;
    } else {
      fnc = "local";
    }
    ::free((void*) dmg);
    return true;
  } else {
    return false;
  }

#else // not implemented for windows and osx
  return false;
#endif

}

///set an environment variables. @return 0 if successful, -1 if not
int setEnv(const string &name, const string &value, int overwrite) {
  // UNIX version
  return value.empty() ?
  // remove if set to nothing (and return success)
      ::unsetenv(name.c_str()), 0 :
      // set the value
      ::setenv(name.c_str(), value.c_str(), overwrite);

}

} // namespace System
} // namespace Elements
