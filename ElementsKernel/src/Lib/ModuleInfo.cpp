#define SYSTEM_MODULEINFO_CPP

#include <cstring>
#include <cstdlib>

#include "ElementsKernel/ModuleInfo.h"
#include "ElementsKernel/System.h"

#ifdef _WIN32
  #define NOMSG
  #define NOGDI
  #  define strcasecmp _stricmp
  #  define strncasecmp _strnicmp
  #include "process.h"
  #include "windows.h"
  #include "Win32PsApi.h"
static PsApiFunctions _psApi;
  #define getpid _getpid
  #undef NOMSG
  #undef NOGDI
  #ifndef PATH_MAX
  #  define PATH_MAX 1024
  #endif
#else  // UNIX...: first the EGCS stuff, then the OS dependent includes
  #include <errno.h>
  #include <string.h>
  #include "sys/times.h"
  #include "sys/param.h"
  #include "unistd.h"
  #include "libgen.h"
  #include <cstdio>
  #include <dlfcn.h>
#endif

using namespace std;

static Elements::System::ImageHandle      ModuleHandle = 0;
static vector<string> s_linkedModules;

/// Retrieve base name of module
const string& Elements::System::moduleName()   {
  static string module("");
  if ( module == "" )   {
    if ( processHandle() && moduleHandle() )    {
#ifdef _WIN32
      char moduleName[256] = {"Unknown.module"};
      moduleName[0] = 0;
      if ( _psApi.isValid() )   {
        _psApi.GetModuleBaseNameA( processHandle(), (HINSTANCE)moduleHandle(), moduleName, sizeof(moduleName) );
      }
      string mod = moduleName;
#elif defined(linux) || defined(__APPLE__)
      string mod = ::basename((char*)((Dl_info*)moduleHandle())->dli_fname);
#elif __hpux
      string mod = ::basename(((HMODULE*)moduleHandle())->dsc.filename);
#endif
      module = mod.substr(0, mod.rfind('.'));
    }
  }
  return module;
}

/// Retrieve full name of module
const string& Elements::System::moduleNameFull()   {
  static string module("");
  if ( module == "" )   {
    if ( processHandle() && moduleHandle() )    {
      char name[PATH_MAX] = {"Unknown.module"};
      name[0] = 0;
#ifdef _WIN32
      if ( _psApi.isValid() )   {
        _psApi.GetModuleFileNameExA( processHandle(), (HINSTANCE)moduleHandle(), name,sizeof(name) );
        module = name;
      }
#else
      const char *path =
#  if defined(linux) || defined(__APPLE__)
          ((Dl_info*)moduleHandle())->dli_fname;
#  elif __hpux
          ((HMODULE*)moduleHandle())->dsc.filename;
#  endif
      if (::realpath(path, name))
        module = name;
#endif
    }
  }
  return module;
}

/// Get type of the module
Elements::System::ModuleType Elements::System::moduleType()   {
  static ModuleType type = UNKNOWN;
  if ( type == UNKNOWN )    {
    const string& module = moduleNameFull();
    int loc = module.rfind('.')+1;
    if ( loc == 0 )
      type = EXECUTABLE;
    else if ( module[loc] == 'e' || module[loc] == 'E' )
      type = EXECUTABLE;
#ifdef _WIN32
    else if ( module[loc] == 'd' || module[loc] == 'D' )
#else
    else if ( module[loc] == 's' && module[loc+1] == 'o' )
#endif
      type = SHAREDLIB;
    else
      type = UNKNOWN;
  }
  return type;
}

/// Retrieve processhandle
void* Elements::System::processHandle()   {
  static long pid = ::getpid();
#ifdef _WIN32
  static HANDLE hP = ::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pid);
#else
  static void* hP = (void*)pid;
#endif
  return hP;
}

void Elements::System::setModuleHandle(Elements::System::ImageHandle handle)    {
  ModuleHandle = handle;
}

Elements::System::ImageHandle Elements::System::moduleHandle()    {
  if ( 0 == ModuleHandle )    {
    if ( processHandle() )    {
#ifdef _WIN32
      static HINSTANCE handle = 0;
      DWORD   cbNeeded;
      if ( 0 == handle && _psApi.isValid() )    {
        if ( _psApi.EnumProcessModules( processHandle(), &handle, sizeof(ModuleHandle), &cbNeeded) )   {
        }
      }
      return handle;
#elif defined(linux) || defined(__APPLE__)
      static Dl_info info;
      if ( 0 !=
           ::dladdr(
#if __GNUC__ < 4
               (void*)Elements::System::moduleHandle
#else
               FuncPtrCast<void*>(Elements::System::moduleHandle)
#endif
               , &info) ) {
	return &info;
      }
#elif __hpux
      return 0;  // Don't know how to solve this .....
#endif
    }
  }
  return ModuleHandle;
}

Elements::System::ImageHandle Elements::System::exeHandle()    {
#ifdef _WIN32
  if ( processHandle() )    {
    static HINSTANCE handle = 0;
    DWORD   cbNeeded;
    if ( 0 == handle && _psApi.isValid() )    {
      if ( _psApi.EnumProcessModules( processHandle(), &handle, sizeof(ModuleHandle), &cbNeeded) )   {
      }
    }
    return handle;
  }
  return 0;
#elif defined(linux) || defined(__APPLE__)
  // This does NOT work!
  static Dl_info infoBuf, *info = &infoBuf;
  if ( 0 == info ) {
    void* handle = ::dlopen(0, RTLD_LAZY);
    //printf("Exe handle:%X\n", handle);
    if ( 0 != handle ) {
      void* func = ::dlsym(handle, "main");
      //printf("Exe:Func handle:%X\n", func);
      if ( 0 != func ) {
      	if ( 0 != ::dladdr(func, &infoBuf) ) {
	        //cout << "All OK" << endl;
      	  info = &infoBuf;
      	}
      }
    }
  }
  return info;
#elif __hpux
  // Don't know how to solve this .....
  return 0;
#endif
}

const string& Elements::System::exeName()    {
  static string module("");
  if ( module.length() == 0 )    {
    char name[PATH_MAX] = {"Unknown.module"};
    name[0] = 0;
#ifdef _WIN32
    if ( _psApi.isValid() && processHandle() )   {
      _psApi.GetModuleFileNameExA( processHandle(), (HINSTANCE)exeHandle(), name,sizeof(name) );
      module = name;
    }
#elif defined(linux) || defined(__APPLE__)
    char cmd[512];
    ::sprintf(cmd, "/proc/%d/exe", ::getpid());
    module = "Unknown";
    if (::readlink(cmd, name, sizeof(name)) >= 0)
      module = name;
#elif __hpux
    if (::realpath(((HMODULE*)exeHandle())->dsc.filename, name))
      module = name;
#endif
  }
  return module;
}

const vector<string> Elements::System::linkedModules()    {
  if ( s_linkedModules.size() == 0 )    {
#ifdef _WIN32
    DWORD  cbNeeded;
    HINSTANCE handle[1024];
    if ( _psApi.isValid() )    {
      if ( _psApi.EnumProcessModules(processHandle(),handle,sizeof(handle),&cbNeeded) )   {
        for (size_t i = 0; i < cbNeeded/sizeof(HANDLE); i++ )    {
          char   name[255];  // Maximum file name length on NT 4.0
          if ( 0 < _psApi.GetModuleFileNameExA( processHandle(), handle[i], name, sizeof(name)) )   {
            s_linkedModules.push_back(name);
          }
        }
      }
    }
#elif defined(linux) || defined(__APPLE__)
    char ff[512], cmd[1024], fname[1024], buf1[64], buf2[64], buf3[64], buf4[64];
    ::sprintf(ff, "/proc/%d/maps", ::getpid());
    FILE* maps = ::fopen(ff, "r");
    while( ::fgets(cmd, sizeof(cmd), maps) ) {
      int len;
      sscanf(cmd, "%s %s %s %s %d %s", buf1, buf2, buf3, buf4, &len, fname);
      if ( len > 0 && strncmp(buf2,"r-xp",strlen("r-xp")) == 0 ) {
        s_linkedModules.push_back(fname);
      }
    }
    ::fclose(maps);
#endif
  }
  return s_linkedModules;
}
