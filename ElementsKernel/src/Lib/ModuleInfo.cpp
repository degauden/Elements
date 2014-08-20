#define SYSTEM_MODULEINFO_CPP

#include <cstring>
#include <cstdlib>

#include "ElementsKernel/ModuleInfo.h"
#include "ElementsKernel/System.h"

#include <cerrno>
#include <string.h>
#include <sys/times.h>
#include <sys/param.h>
#include <unistd.h>
#include <libgen.h>
#include <cstdio>
#include <dlfcn.h>

using namespace std;

static Elements::System::ImageHandle      ModuleHandle = 0;
static vector<string> s_linkedModules;

/// Retrieve base name of module
const string& Elements::System::moduleName()   {
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
const string& Elements::System::moduleNameFull()   {
  static string module("");
  if ( module == "" )   {
    if ( processHandle() && moduleHandle() )    {
      char name[PATH_MAX] = {"Unknown.module"};
      name[0] = 0;
      const char *path =
#  if defined(__linux) || defined(__APPLE__)
          ((Dl_info*)moduleHandle())->dli_fname;
#  endif
      if (::realpath(path, name))
        module = name;
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
    else if ( module[loc] == 's' && module[loc+1] == 'o' )
      type = SHAREDLIB;
    else
      type = UNKNOWN;
  }
  return type;
}

/// Retrieve processhandle
void* Elements::System::processHandle()   {
  static long pid = ::getpid();
  static void* hP = (void*)pid;
  return hP;
}

void Elements::System::setModuleHandle(Elements::System::ImageHandle handle)    {
  ModuleHandle = handle;
}

Elements::System::ImageHandle Elements::System::moduleHandle()    {
  if ( 0 == ModuleHandle )    {
    if ( processHandle() )    {
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
    }
  }
  return ModuleHandle;
}

Elements::System::ImageHandle Elements::System::exeHandle()    {
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

const string& Elements::System::exeName()    {
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

const vector<string> Elements::System::linkedModules()    {
  if ( s_linkedModules.size() == 0 )    {
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
  }
  return s_linkedModules;
}
