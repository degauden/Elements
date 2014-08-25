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
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <typeinfo>

#include "ElementsKernel/System.h"

using namespace std;

static const char* SHLIB_SUFFIX = ".so";
#include <errno.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <libgen.h>
#include <cstdio>
#include <cxxabi.h>
#include <dlfcn.h>
#include <sys/utsname.h>
#include <unistd.h>


// Note: __attribute__ is a GCC keyword available since GCC 3.4
#ifdef __GNUC__
#  if __GNUC__ < 3 || \
      (__GNUC__ == 3 && (__GNUC_MINOR__ < 4 ))
// GCC < 3.4
#    define __attribute__(x)
#  endif
#else
// non-GCC
#  define __attribute__(x)
#endif

static vector<string> s_argvStrings;
static vector<const char*> s_argvChars;

static unsigned long doLoad(const string& name, Elements::System::ImageHandle* handle)  {
# if defined(__linux) || defined(__APPLE__)
   const char* path = name.c_str();
   void *mh = ::dlopen(name.length() == 0 ? 0 : path, RTLD_LAZY | RTLD_GLOBAL);
   *handle = mh;
# endif
  if ( 0 == *handle )   {
    return Elements::System::getLastError();
  }
  return 1;
}

static unsigned long loadWithoutEnvironment(const string& name, Elements::System::ImageHandle* handle)    {

  string dllName = name;
  unsigned long len = static_cast<unsigned long>(strlen(SHLIB_SUFFIX));

  // Add the suffix at the end of the library name only if necessary
  /// @todo cure the logic
  if ((dllName.length() != 0) &&
      ::strncasecmp(static_cast<const char *>(dllName.data()+dllName.length()-len), SHLIB_SUFFIX, len) != 0) {
    dllName += SHLIB_SUFFIX;
  }

  // Load the library
  return doLoad(dllName, handle);
}

/// Load dynamic link library
unsigned long Elements::System::loadDynamicLib(const string& name, ImageHandle* handle) {
  unsigned long res;
  // if name is empty, just load it
  if (name.length() == 0) {
    res = loadWithoutEnvironment(name, handle);
  } else {
    // If the name is a logical name (environment variable), the try
    // to load the corresponding library from there.
    string imgName;
    if ( getEnv(name, imgName) )    {
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
    if ( res != 1 ) {
#if defined(__linux) || defined(__APPLE__)
      errno = 0xAFFEDEAD;
#endif
     // cout << "Elements::System::loadDynamicLib>" << getLastErrorString() << endl;
    }
  }
  return res;
}

/// unload dynamic link library
unsigned long Elements::System::unloadDynamicLib(ImageHandle handle)    {
  ::dlclose( handle );
  if ( 0 ) {
    return getLastError();
  }
  return 1;
}

/// Get a specific function defined in the DLL
unsigned long Elements::System::getProcedureByName(ImageHandle handle, const string& name, EntryPoint* pFunction)    {
#if defined(__linux)
#if __GNUC__ < 4
  *pFunction = (EntryPoint)::dlsym(handle, name.c_str());
#else
  *pFunction = FuncPtrCast<EntryPoint>(::dlsym(handle, name.c_str()));
#endif
  if ( 0 == *pFunction )    {
    errno = 0xAFFEDEAD;
   // cout << "Elements::System::getProcedureByName>" << getLastErrorString() << endl;
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
  if ( 0 == *pFunction )    {
    errno = 0xAFFEDEAD;
    cout << "Elements::System::getProcedureByName>" << getLastErrorString() << endl;
    //cout << "Elements::System::getProcedureByName> failure" << endl;
    return 0;
  }
  return 1;
#endif
}

/// Get a specific function defined in the DLL
unsigned long Elements::System::getProcedureByName(ImageHandle handle, const string& name, Creator* pFunction)    {
  return getProcedureByName(handle, name, (EntryPoint*)pFunction);
}

/// Retrieve last error code
unsigned long Elements::System::getLastError()    {
  // convert errno (int) to unsigned long
  return static_cast<unsigned long>(static_cast<unsigned int>(errno));
}

/// Retrieve last error code as string
const string Elements::System::getLastErrorString()    {
  const string errString = getErrorString(getLastError());
  return errString;
}

/// Retrieve error code as string for a given error
const string Elements::System::getErrorString(unsigned long error)    {
  string errString =  "";
  char *cerrString(0);
  // Remember: for linux dl* routines must be handled differently!
  if ( error == 0xAFFEDEAD ) {
    cerrString = (char*)::dlerror();
    if ( 0 == cerrString ) {
      cerrString = ::strerror(error);
    }
    if ( 0 == cerrString ) {
      cerrString = (char *)"Unknown error. No information found in strerror()!";
    }
    else {
      errString = string(cerrString);
    }
    errno = 0;
  }
  else    {
    cerrString = ::strerror(error);
    errString = string(cerrString);
  }
  return errString;
}

const string Elements::System::typeinfoName( const std::type_info& tinfo) {
  return typeinfoName(tinfo.name());
}

const string Elements::System::typeinfoName( const char* class_name) {
  string result;
    if ( ::strlen(class_name) == 1 ) {
      // See http://www.realitydiluted.com/mirrors/reality.sgi.com/dehnert_engr/cxx/abi.pdf
      // for details
      switch(class_name[0]) {
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
    }
    else  {
      int   status;
      char* realname;
      realname = abi::__cxa_demangle(class_name, 0, 0, &status);
      if (realname == 0) return class_name;
      result = realname;
      free(realname);
      /// substitute ', ' with ','
      string::size_type pos = result.find(", ");
      while( string::npos != pos ) {
        result.replace( pos , static_cast<string::size_type>(2) , "," ) ;
        pos = result.find(", ");
      }
    }
  return result;
}

/// Host name
const string& Elements::System::hostName() {
  static string host = "";
  if ( host == "" ) {
    char buffer[512];
    memset(buffer,0,sizeof(buffer));
    ::gethostname(buffer, sizeof(buffer));
    host = buffer;
  }
  return host;
}

/// OS name
const string& Elements::System::osName() {
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
const string& Elements::System::osVersion() {
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
const string& Elements::System::machineType() {
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
const string& Elements::System::accountName() {
  static string account = "";
  if ( account == "" ) {
    const char* acct = ::getlogin();
    if ( 0 == acct ) acct = ::getenv("LOGNAME");
    if ( 0 == acct ) acct = ::getenv("USER");
    account = (acct) ? acct : "Unknown";
  }
  return account;
}

/// Number of arguments passed to the commandline
long Elements::System::numCmdLineArgs()   {
  return cmdLineArgs().size();
}

/// Number of arguments passed to the commandline
long Elements::System::argc()    {
  return cmdLineArgs().size();
}

/// Const char** command line arguments including executable name as arg[0]
const vector<string> Elements::System::cmdLineArgs()    {
  if ( s_argvChars.size() == 0 )    {
    char exe[1024];
    sprintf(exe, "/proc/%d/cmdline", ::getpid());
    FILE *cmdLine = ::fopen(exe,"r");
    char cmd[1024];
    if ( cmdLine )   {
      long len = fread(cmd, sizeof(char), sizeof(cmd), cmdLine);
      if ( len > 0 )   {
        cmd[len] = 0;
        for ( char* token = cmd; token-cmd < len; token += strlen(token)+1 )  {
          s_argvStrings.push_back(token);
          s_argvChars.push_back( s_argvStrings.back().c_str());
        }
        s_argvStrings[0] = exeName();
        s_argvChars[0]   = s_argvStrings[0].c_str();
      }
      ::fclose(cmdLine);
    }
  }
  return s_argvStrings;
}

/// Const char** command line arguments including executable name as arg[0]
char** Elements::System::argv()    {
  ///
  if( s_argvChars.empty() ) { cmdLineArgs(); }  /// added by I.B.
  ///
  // We rely here on the fact that a vector's allocation table is contiguous
  return (char**)&s_argvChars[0];
}


/// get a particular env var, return "UNKNOWN" if not defined
string Elements::System::getEnv(const char* var) {
  char* env;
  if  ( (env = getenv(var)) != 0 ) {
    return env;
  } else {
    return "UNKNOWN";
  }
}

/// get a particular env var, storing the value in the passed string (if set)
bool Elements::System::getEnv(const char* var, string &value) {
  char* env;
  if  ( (env = getenv(var)) != 0 ) {
    value = env;
    return true;
  } else {
    return false;
  }
}

bool Elements::System::isEnvSet(const char* var) {
  return getenv(var) != 0;
}

/// get all defined environment vars
#if defined(__APPLE__)
// Needed for _NSGetEnviron(void)
#include "crt_externs.h"
#endif
vector<string> Elements::System::getEnv() {
#if defined(__APPLE__)
  static char **environ = *_NSGetEnviron();
#endif
  vector<string> vars;
  for (int i=0; environ[i] != 0; ++i) {
    vars.push_back(environ[i]);
  }
  return vars;
}

// -----------------------------------------------------------------------------
// backtrace utilities
// -----------------------------------------------------------------------------
#ifdef __linux
#include <execinfo.h>
#endif

int Elements::System::backTrace(void** addresses __attribute__ ((unused)),
                       const int depth __attribute__ ((unused)))
{

#ifdef __linux

  int count = backtrace( addresses, depth );
  if ( count > 0 ) {
    return count;
  } else {
    return 0;
  }

#else // windows and osx parts not implemented
  return 0;
#endif

}

bool Elements::System::backTrace(string& btrace, const int depth, const int offset)
{
  // Always hide the first two levels of the stack trace (that's us)
  const int totalOffset = offset + 2;
  const int totalDepth = depth + totalOffset;

  string fnc, lib;

  void** addresses = (void**) malloc(totalDepth*sizeof(void *));
  if ( addresses != 0 ){
    int count = Elements::System::backTrace(addresses,totalDepth);
    for (int i = totalOffset; i < count; ++i) {
      void *addr = 0;

      if (Elements::System::getStackLevel(addresses[i],addr,fnc,lib)) {
        ostringstream ost;
        ost << "#" << setw(3) << setiosflags( ios::left ) << i-totalOffset+1;
        ost << hex << addr << dec << " " << fnc << "  [" << lib << "]" << endl;
        btrace += ost.str();
      }
    }
    free(addresses);
  }
  else {
    free(addresses);
    return false;
  }

  return true;
}

bool Elements::System::getStackLevel(void* addresses  __attribute__ ((unused)),
                           void*& addr      __attribute__ ((unused)),
                           string& fnc __attribute__ ((unused)),
                           string& lib __attribute__ ((unused)))
{

#ifdef __linux

  Dl_info info;

  if ( dladdr( addresses, &info ) && info.dli_fname
      && info.dli_fname[0] != '\0' ) {
    const char* symbol = info.dli_sname
    && info.dli_sname[0] != '\0' ? info.dli_sname : 0;

    lib = info.dli_fname;
    addr = info.dli_saddr;
    const char* dmg(0);

    if (symbol != 0) {
      int stat;
      dmg = abi::__cxa_demangle(symbol,0,0,&stat);
      fnc = (stat == 0) ? dmg : symbol;
    } else {
      fnc = "local";
    }
    free((void*)dmg);
    return true ;
  } else {
    return false ;
  }

#else // not implemented for windows and osx
  return false ;
#endif

}

///set an environment variables. @return 0 if successful, -1 if not
int Elements::System::setEnv(const string &name, const string &value, int overwrite)
{
  // UNIX version
  return value.empty() ?
    // remove if set to nothing (and return success)
    ::unsetenv(name.c_str()) , 0 :
    // set the value
    ::setenv(name.c_str(),value.c_str(), overwrite);

}
