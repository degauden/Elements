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

#ifdef _WIN32
  #define strcasecmp  _stricmp
  #define strncasecmp _strnicmp
  #define getpid _getpid
  #define NOMSG
  #define NOGDI
  #include "process.h"
  #include "windows.h"
  #undef NOMSG
  #undef NOGDI
  static const char* SHLIB_SUFFIX = ".dll";
#else  // UNIX...: first the EGCS stuff, then the OS dependent includes
  static const char* SHLIB_SUFFIX = ".so";
  #include <errno.h>
  #include <string.h>
  #include "sys/times.h"
  #include "unistd.h"
  #include "libgen.h"
  #include <cstdio>
  #include <cxxabi.h>
#if defined(linux) || defined(__APPLE__)
  #include "dlfcn.h"
  #include <sys/utsname.h>
  #include <unistd.h>
#elif __hpux
  #include "dl.h"
struct HMODULE {
  shl_descriptor dsc;
  long           numSym;
  shl_symbol*    sym;
};
#endif

#endif

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
#ifdef _WIN32
  void* mh = ::LoadLibrary( name.length() == 0 ? Elements::System::exeName().c_str() : name.c_str());
  *handle = mh;
#else
  const char* path = name.c_str();
#if defined(linux) || defined(__APPLE__)
  void *mh = ::dlopen(name.length() == 0 ? 0 : path, RTLD_LAZY | RTLD_GLOBAL);
  *handle = mh;
#elif __hpux
  shl_t mh = ::shl_load(name.length() == 0 ? 0 : path, BIND_IMMEDIATE | BIND_VERBOSE, 0);
  HMODULE* mod = new HMODULE;
  if ( 0 != mh ) {
    if ( 0 != ::shl_gethandle_r(mh, &mod->dsc) ) {
      cout << "Elements::System::loadDynamicLib>" << ::strerror(getLastError()) << endl;
    }
    else {
      typedef void* (*___all)();
      ___all _alloc = (___all)malloc;
      mod->numSym = ::shl_getsymbols(mod->dsc.handle, TYPE_PROCEDURE, EXPORT_SYMBOLS, malloc, &mod->sym);
      *handle = mod;
      // cppcheck-suppress memleak
    }
  }
#endif
#endif
  if ( 0 == *handle )   {
    return Elements::System::getLastError();
  }
  return 1;
}

static unsigned long loadWithoutEnvironment(const string& name, Elements::System::ImageHandle* handle)    {

  string dllName = name;
  long len = strlen(SHLIB_SUFFIX);

  // Add the suffix at the end of the library name only if necessary
  // FIXME: cure the logic
  if ((dllName.length() != 0) &&
      ::strncasecmp(dllName.data()+dllName.length()-len, SHLIB_SUFFIX, len) != 0) {
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
#if defined(linux) || defined(__APPLE__)
      dllName = "lib" + dllName;
#endif
      dllName += SHLIB_SUFFIX;
      // try to locate the dll using the standard PATH
      res = loadWithoutEnvironment(dllName, handle);
    }
    if ( res != 1 ) {
#if defined(linux) || defined(__APPLE__)
      errno = 0xAFFEDEAD;
#endif
     // cout << "Elements::System::loadDynamicLib>" << getLastErrorString() << endl;
    }
  }
  return res;
}

/// unload dynamic link library
unsigned long Elements::System::unloadDynamicLib(ImageHandle handle)    {
#ifdef _WIN32
  if ( !::FreeLibrary((HINSTANCE)handle) ) {
#elif defined(linux) || defined(__APPLE__)
  ::dlclose( handle );
  if ( 0 ) {
#elif __hpux
  // On HP we have to run finalization ourselves.....
  Creator pFinalize = 0;
  if ( getProcedureByName(handle, "_fini", &pFinalize) ) {
    pFinalize();
  }
  HMODULE* mod = (HMODULE*)handle;
  if ( 0 == ::shl_unload( mod->dsc.handle ) ) {
    delete mod;
  }
  else {
#else
  if (false){
#endif
    return getLastError();
  }
  return 1;
}

/// Get a specific function defined in the DLL
unsigned long Elements::System::getProcedureByName(ImageHandle handle, const string& name, EntryPoint* pFunction)    {
#ifdef _WIN32
  *pFunction = (EntryPoint)::GetProcAddress((HINSTANCE)handle, name.data());
  if ( 0 == *pFunction )    {
    return Elements::System::getLastError();
  }
  return 1;
#elif defined(linux)
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
#elif __hpux
  HMODULE* mod = (HMODULE*)handle;
  if ( 0 != mod ) {
    long ll1 = name.length();
    for ( int i = 0; i < mod->numSym; i++ ) {
      long ll2 = strlen(mod->sym[i].name);
      if ( 0 != ::strncmp(mod->sym[i].name, name.c_str(), (ll1>ll2) ? ll1 : ll2)==0 ) {
	      *pFunction = (EntryPoint) mod->sym[i].value;
	      return 1;
      }
    }
  }
  return 0;
#endif
}

/// Get a specific function defined in the DLL
unsigned long Elements::System::getProcedureByName(ImageHandle handle, const string& name, Creator* pFunction)    {
  return getProcedureByName(handle, name, (EntryPoint*)pFunction);
}

/// Retrieve last error code
unsigned long Elements::System::getLastError()    {
#ifdef _WIN32
  return ::GetLastError();
#else
  // convert errno (int) to unsigned long
  return static_cast<unsigned long>(static_cast<unsigned int>(errno));
#endif
}

/// Retrieve last error code as string
const string Elements::System::getLastErrorString()    {
  const string errString = getErrorString(getLastError());
  return errString;
}

/// Retrieve error code as string for a given error
const string Elements::System::getErrorString(unsigned long error)    {
  string errString =  "";
#ifdef _WIN32
  LPVOID lpMessageBuffer;
  ::FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |  FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    error,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //The user default language
    (LPTSTR) &lpMessageBuffer,
    0,
    NULL );
  errString = (const char*)lpMessageBuffer;
  // Free the buffer allocated by the system
  ::LocalFree( lpMessageBuffer );
#else
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
#endif
  return errString;
}

const string Elements::System::typeinfoName( const std::type_info& tinfo) {
  return typeinfoName(tinfo.name());
}

const string Elements::System::typeinfoName( const char* class_name) {
  string result;
#ifdef _WIN32
  long off = 0;
  if ( ::strncmp(class_name, "class ", 6) == 0 )   {
    // The returned name is prefixed with "class "
    off = 6;
  }
  if ( ::strncmp(class_name, "struct ", 7) == 0 )   {
    // The returned name is prefixed with "struct "
    off = 7;
  }
  if ( off > 0 )    {
    string tmp = class_name + off;
    long loc = 0;
    while( (loc = tmp.find("class ")) > 0 )  {
      tmp.erase(loc, 6);
    }
    loc = 0;
    while( (loc = tmp.find("struct ")) > 0 )  {
      tmp.erase(loc, 7);
    }
    result = tmp;
  }
  else  {
    result = class_name;
  }
  // Change any " *" to "*"
  while ( (off=result.find(" *")) != string::npos ) {
    result.replace(off, 2, "*");
  }
  // Change any " &" to "&"
  while ( (off=result.find(" &")) != string::npos ) {
    result.replace(off, 2, "&");
  }

#elif defined(__linux) || defined(__APPLE__)
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
        result.replace( pos , 2 , "," ) ;
        pos = result.find(", ");
      }
    }
#endif
  return result;
}

/// Host name
const string& Elements::System::hostName() {
  static string host = "";
  if ( host == "" ) {
    char buffer[512];
    memset(buffer,0,sizeof(buffer));
#ifdef _WIN32
    unsigned long len = sizeof(buffer);
    ::GetComputerName(buffer, &len);
#else
    ::gethostname(buffer, sizeof(buffer));
#endif
    host = buffer;
  }
  return host;
}

/// OS name
const string& Elements::System::osName() {
  static string osname = "";
#ifdef _WIN32
  osname = "Windows";
#else
  struct utsname ut;
  if (uname(&ut) == 0) {
    osname = ut.sysname;
  } else {
    osname = "UNKNOWN";
  }
#endif
  return osname;
}


/// OS version
const string& Elements::System::osVersion() {
  static string osver = "";
#ifdef _WIN32
  OSVERSIONINFO ut;
  ut.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx(&ut);
  ostringstream ver;
  ver << ut.dwMajorVersion << '.' << ut.dwMinorVersion;
  osver = ver.str();
#else
  struct utsname ut;
  if (uname(&ut) == 0) {
    osver = ut.release;
  } else {
    osver = "UNKNOWN";
  }
#endif
  return osver;
}

/// Machine type
const string& Elements::System::machineType() {
  static string mach = "";
#ifdef _WIN32
  SYSTEM_INFO ut;
  ::GetSystemInfo(&ut);
  ostringstream arch;
  arch << ut.wProcessorArchitecture;
  mach = arch.str();
#else
  struct utsname ut;
  if (uname(&ut) == 0) {
    mach = ut.machine;
  } else {
    mach = "UNKNOWN";
  }
#endif
  return mach;
}

/// User login name
const string& Elements::System::accountName() {
  static string account = "";
  if ( account == "" ) {
#ifdef _WIN32
    char buffer[512];
    unsigned long buflen = sizeof(buffer);
    ::GetUserName(buffer, &buflen);
    account = buffer;
#else
    const char* acct = ::getlogin();
    if ( 0 == acct ) acct = ::getenv("LOGNAME");
    if ( 0 == acct ) acct = ::getenv("USER");
    account = (acct) ? acct : "Unknown";
#endif
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
#ifdef _WIN32
    /// @todo: rewrite the tokenizer to avoid strncpy, etc
    // Disable warning C4996 triggered by C standard library calls
#pragma warning(push)
#pragma warning(disable:4996)
    // For compatibility with UNIX we CANNOT use strtok!
    // If we would use strtok, options like -g="My world" at
    // the command line level would result on NT in TWO options
    // instead in one as in UNIX.
    char *next, *tmp1, *tmp2;
    for(LPTSTR cmd = ::GetCommandLine(); *cmd; cmd=next)   {
      memset(exe,0,sizeof(exe));
      while ( *cmd == ' ' ) cmd++;
      next=::strchr(cmd,' ');
      if ( !next ) next = cmd + strlen(cmd);
      if ( (tmp1=::strchr(cmd,'\"')) > 0 && tmp1 < next )  {
        tmp2 = ::strchr(++tmp1,'\"');
        if ( tmp2 > 0 )   {
          next = ++tmp2;
          if ( cmd < tmp1 ) strncpy(exe, cmd, tmp1-cmd-1);
          strncpy(&exe[strlen(exe)], tmp1, tmp2-tmp1-1);
        }
        else    {
          cout << "Mismatched \" in command line arguments" << endl;
          s_argvChars.erase(s_argvChars.begin(), s_argvChars.end());
          s_argvStrings.erase(s_argvStrings.begin(), s_argvStrings.end());
          return s_argvStrings;
        }
      }
      else    {
        strncpy(exe, cmd, next-cmd);
      }
      s_argvStrings.push_back(exe);
      s_argvChars.push_back( s_argvStrings.back().c_str());
    }
#pragma warning(pop)
#elif defined(linux) || defined(__APPLE__)
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
#endif
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

#ifdef WIN32
// disable warning
//   C4996: 'getenv': This function or variable may be unsafe.
#pragma warning(disable:4996)
#endif

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
#if defined(_WIN32)
#  define environ _environ
#elif defined(__APPLE__)
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
#ifndef WIN32
  // UNIX version
  return value.empty() ?
    // remove if set to nothing (and return success)
    ::unsetenv(name.c_str()) , 0 :
    // set the value
    ::setenv(name.c_str(),value.c_str(), overwrite);
#else
  // Windows version
  if ( value.empty() ) {
    // equivalent to unsetenv
    return ::_putenv((name+"=").c_str());
  }
  else {
    if ( !getenv(name.c_str()) || overwrite ) {
      // set if not yet present or overwrite is set (force)
      return ::_putenv((name+"="+value).c_str());
    }
  }
  return 0; // if we get here, we are trying to set a variable already set, but
            // not to overwrite.
            // It is considered a success on Linux (man P setenv)
#endif

}
