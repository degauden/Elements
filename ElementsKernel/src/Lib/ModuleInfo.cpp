#define SYSTEM_MODULEINFO_CPP

#include <cstring>
#include <cstdlib>

#include "ElementsKernel/ModuleInfo.h"
#include "ElementsKernel/FuncPtrCast.h"

#include <cerrno>
#include <string.h>
#include <sys/times.h>
#include <sys/param.h>
#include <unistd.h>
#include <libgen.h>
#include <cstdio>
#include <dlfcn.h>

using namespace std;

static vector<string> s_linkedModules;

namespace Elements {
namespace System {



static ImageHandle      ModuleHandle = 0;

/// Retrieve base name of module
const string& moduleName()   {
  static string module("");
  if ( module == "" )   {
    if ( processHandle() && moduleHandle() )    {
      string mod = ::basename((char*)((Dl_info*)moduleHandle())->dli_fname);
      module = mod.substr(static_cast<string::size_type>(0), mod.rfind('.'));
    }
  }
  return module;
}

/// Retrieve full name of module
const string& moduleNameFull()   {
  static string module("");
  if ( module == "" )   {
    if ( processHandle() && moduleHandle() )    {
      char name[PATH_MAX] = {"Unknown.module"};
      name[0] = 0;
      const char *path =
          ((Dl_info*)moduleHandle())->dli_fname;
      if (::realpath(path, name))
        module = name;
    }
  }
  return module;
}

/// Get type of the module
ModuleType moduleType()   {
  static ModuleType type = ModuleType::UNKNOWN;
  if ( type == ModuleType::UNKNOWN )    {
    const string& module = moduleNameFull();
    int loc = module.rfind('.')+1;
    if ( loc == 0 )
      type = ModuleType::EXECUTABLE;
    else if ( module[loc] == 'e' || module[loc] == 'E' )
      type = ModuleType::EXECUTABLE;
    else if ( module[loc] == 's' && module[loc+1] == 'o' )
      type = ModuleType::SHAREDLIB;
    else
      type = ModuleType::UNKNOWN;
  }
  return type;
}

/// Retrieve processhandle
void* processHandle()   {
  static long pid = ::getpid();
  static void* hP = (void*)pid;
  return hP;
}

void setModuleHandle(ImageHandle handle)    {
  ModuleHandle = handle;
}

ImageHandle moduleHandle()    {
  if ( 0 == ModuleHandle )    {
    if ( processHandle() )    {
      static Dl_info info;
      if ( 0 !=
           ::dladdr(
#if __GNUC__ < 4
               (void*)moduleHandle
#else
               FuncPtrCast<void*>(moduleHandle)
#endif
               , &info) ) {
	return &info;
      }
    }
  }
  return ModuleHandle;
}

ImageHandle exeHandle()    {
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
}

const string& exeName()    {
  static string module("");
  if ( module.length() == 0 )    {
    char name[PATH_MAX] = {"Unknown.module"};
    name[0] = 0;
    char cmd[512];
    ::sprintf(cmd, "/proc/%d/exe", ::getpid());
    module = "Unknown";
    if (::readlink(cmd, name, sizeof(name)) >= 0)
      module = name;
  }
  return module;
}

const vector<string> linkedModules()    {
  if ( s_linkedModules.size() == 0 )    {
    char ff[512], cmd[1024], fname[1024], buf1[64], buf2[64], buf3[64], buf4[64];
    ::sprintf(ff, "/proc/%d/maps", ::getpid());
    FILE* maps = ::fopen(ff, "r");
    while( ::fgets(cmd, sizeof(cmd), maps) ) {
      int len;
      ::sscanf(cmd, "%s %s %s %s %d %s", buf1, buf2, buf3, buf4, &len, fname);
      if ( len > 0 && strncmp(buf2,"r-xp",strlen("r-xp")) == 0 ) {
        s_linkedModules.push_back(fname);
      }
    }
    ::fclose(maps);
  }
  return s_linkedModules;
}

} // namespace System
} // namespace Elements
