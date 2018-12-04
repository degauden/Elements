/**
 * @file System.cpp
 * @author Hubert Degaudenzi
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

#include "ElementsKernel/System.h"

#include <dlfcn.h>                      // for Dl_info, dladdr, dlclose, etc
#include <execinfo.h>                   // for backtrace
#include <unistd.h>                     // for environ
#include <cxxabi.h>
#include <sys/utsname.h>

#include <cstdlib>                      // for free, getenv, malloc, etc
#include <typeinfo>                     // for type_info
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>                       // for string
#include <vector>                       // for vector

#include <cerrno>                       // for errno
#include <cstring>                      // for strnlen, strerror
#include <climits>                      // for HOST_NAME_MAX

#include "ElementsKernel/FuncPtrCast.h"
#include "ElementsKernel/ModuleInfo.h"  // for ImageHandle
#include "ElementsKernel/Unused.h"      // for ELEMENTS_UNUSED

using std::string;
using std::vector;
using std::size_t;

namespace Elements {
namespace System {

// --------------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------------

namespace {

unsigned long doLoad(const string& name, ImageHandle* handle) {
  void *mh = ::dlopen(name.length() == 0 ? 0 : name.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  *handle = mh;
  if (0 == *handle) {
    return getLastError();
  }
  return 1;
}

unsigned long loadWithoutEnvironment(const string& name,
    ImageHandle* handle) {

  string dll_name = name;
  size_t dll_len = dll_name.size();
  size_t suf_len = SHLIB_SUFFIX.size();

  // Add the suffix at the end of the library name only if necessary
  if (dll_len >= suf_len &&
      dll_name.compare(dll_len - suf_len, suf_len, SHLIB_SUFFIX) != 0) {
    dll_name += SHLIB_SUFFIX;
  }

  // Load the library
  return doLoad(dll_name, handle);
}

}  // anonymous namespace
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
      dllName = "lib" + dllName;
      dllName += SHLIB_SUFFIX;
      // try to locate the dll using the standard PATH
      res = loadWithoutEnvironment(dllName, handle);
    }
    if (res != 1) {
      errno = 0xAFFEDEAD;
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
  if (not *pFunction) {
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
  return getProcedureByName(handle, name, reinterpret_cast<EntryPoint*>(pFunction));
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
    cerrString = reinterpret_cast<char*>(::dlerror());
    if (0 == cerrString) {
      cerrString = std::strerror(error);
    }
    if (0 == cerrString) {
      cerrString =
          const_cast<char *> ("Unknown error. No information found in strerror()!");
    }
    errString = string(cerrString);
    errno = 0;
  } else {
    cerrString = std::strerror(error);
    errString = string(cerrString);
  }
  return errString;
}

const string typeinfoName(const std::type_info& tinfo) {
  return typeinfoName(tinfo.name());
}

const string typeinfoName(const char* class_name) {
  string result;
  if (strnlen(class_name, 1024) == 1) {
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
    if (realname == 0) {
      return class_name;
    }
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
  static string host {};
  if (host.empty()) {
    std::unique_ptr<char> buffer(new char[HOST_NAME_MAX+1]);
    ::gethostname(buffer.get(), HOST_NAME_MAX);
    host = buffer.get();
  }
  return host;
}

/// OS name
const string& osName() {
  static string osname = "";
  struct utsname ut;
  if (::uname(&ut) == 0) {
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

string getEnv(const string& var) {

  string env_str {};

  getEnv(var, env_str);

  return env_str;
}

/// get a particular env var, storing the value in the passed string (if set)
bool getEnv(const string& var, string& value) {
  bool found = false;
  value = "";

  char* env = ::getenv(var.c_str());
  if (env != NULL) {
    found = true;
    value = env;
  }

  return found;
}


bool isEnvSet(const string& var) {
  string result;
  return getEnv(var, result);
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

/// set an environment variables. @return 0 if successful, -1 if not
int setEnv(const string& name, const string& value, bool overwrite) {

  int over = 1;
  if (not overwrite) {
    over = 0;
  }

  return ::setenv(name.c_str(), value.c_str(), over);
}


int unSetEnv(const string& name) {
  return ::unsetenv(name.c_str());
}

// -----------------------------------------------------------------------------
// backtrace utilities
// -----------------------------------------------------------------------------

int backTrace(ELEMENTS_UNUSED std::shared_ptr<void*> addresses,
              ELEMENTS_UNUSED const int depth) {

  int count = ::backtrace(addresses.get(), depth);
  if (count > 0) {
    return count;
  } else {
    return 0;
  }

}

bool backTrace(string& btrace, const int depth, const int offset) {
  // Always hide the first two levels of the stack trace (that's us)
  const int totalOffset = offset + 2;
  const int total_depth = depth + totalOffset;
  bool result = false;


  std::shared_ptr<void*> addresses {new void*[total_depth], std::default_delete<void*[]>()};

  if (addresses != nullptr) {
    int count = backTrace(addresses, total_depth);
    for (int i = totalOffset; i < count; ++i) {
      void *addr = 0;
      string fnc, lib;
      if (getStackLevel(addresses.get()[i], addr, fnc, lib)) {
        std::ostringstream ost;
        ost << "#" << std::setw(3) << std::setiosflags(std::ios::left) << i - totalOffset + 1;
        ost << std::hex << addr << std::dec << " " << fnc << "  [" << lib << "]" << std::endl;
        btrace += ost.str();
      }
    }
    result = true;
  }

  return result;
}


const vector<string> backTrace(const int depth, const int offset) {

  // Always hide the first two levels of the stack trace (that's us)
  const int total_offset = offset + 2;
  const int total_depth = depth + total_offset;
  vector<string> trace {};

  std::shared_ptr<void*> addresses {new void*[total_depth], std::default_delete<void*[]>()};

  if (addresses != nullptr) {

    int count = backTrace(addresses, total_depth);

    for (int i=total_offset; i < count; ++i) {
      void *addr = 0;
      string fnc, lib;
      if (getStackLevel(addresses.get()[i], addr, fnc, lib)) {
        std::ostringstream ost;
        ost << "#" << std::setw(3) << std::setiosflags(std::ios::left) << i - total_offset + 1;
        ost << std::hex << addr << std::dec << " " << fnc << "  [" << lib << "]";
        trace.push_back(ost.str());
      }
    }
  }

  return trace;
}

bool getStackLevel(void* addresses ELEMENTS_UNUSED, void*& addr ELEMENTS_UNUSED,
    string& fnc ELEMENTS_UNUSED, string& lib ELEMENTS_UNUSED) {


  Dl_info info;

  if (::dladdr(addresses, &info) && info.dli_fname
      && info.dli_fname[0] != '\0') {
    const char* symbol =
        info.dli_sname && info.dli_sname[0] != '\0' ? info.dli_sname : 0;

    lib = info.dli_fname;
    addr = info.dli_saddr;

    if (symbol != 0) {
      int stat;
      std::unique_ptr<char, decltype(free)*> dmg(abi::__cxa_demangle(symbol, 0, 0, &stat), free);
      fnc = string((stat == 0) ? dmg.get() : symbol);
    } else {
      fnc = "local";
    }
    return true;
  } else {
    return false;
  }

}

}  // namespace System
}  // namespace Elements
