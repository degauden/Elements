/**
 * @file ElementsKernel/src/Lib/ModuleInfo.cpp
 * @brief OS specific details to access at run-time the module
 * configuration of the process.
 * @date Dec 1, 2014
 * @author hubert
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
 */

#include "ElementsKernel/ModuleInfo.h"

#include <sys/times.h>
#include <sys/param.h>
#include <unistd.h>
#include <libgen.h>
#include <dlfcn.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>      // for _NSGetExecutablePath
#endif

#include <cstring>
#include <cstdlib>
#include <sstream>                            // for stringstream
#include <fstream>                            // for ifstream
#include <iostream>
#include <cerrno>
#include <cstdio>
#include <string>                             // for string
#include <vector>

#ifdef __APPLE__
#include <climits>            // for PATH_MAX
#endif

#include <boost/filesystem/operations.hpp>    // for filesystem::exists, canonical

#include "ElementsKernel/FuncPtrCast.h"
#include "ElementsKernel/Path.h"              // for Path::Item

using std::string;
using std::vector;

namespace {
  vector<string> s_linkedModules;
}

namespace Elements {
namespace System {

ModuleInfo::ModuleInfo() : m_dlinfo{nullptr} {
}

ModuleInfo::ModuleInfo(void *funct) {
  m_dlinfo.reset(new Dl_info);
  ::dladdr(FuncPtrCast<void*>(funct), m_dlinfo.get());
}

const string ModuleInfo::name() const {
  return ::basename(const_cast<char*>(m_dlinfo->dli_fname));
}

const string ModuleInfo::libraryName() const {
  return const_cast<char*>(m_dlinfo->dli_fname);
}

const void* ModuleInfo::addresse() const {
  return m_dlinfo->dli_saddr;
}

bool ModuleInfo::isEmpty() const {
  return (m_dlinfo == nullptr);
}

ModuleInfo::operator const Dl_info&() const {
    return *m_dlinfo;
}

namespace {
  ImageHandle s_module_handle = nullptr;
}
/// Retrieve base name of module
const string& moduleName()   {
  static string module("");
  if (module == "")   {
    if ( processHandle() and moduleHandle() )    {
      string mod = ::basename(const_cast<char *>((reinterpret_cast<Dl_info*>(moduleHandle()))->dli_fname));
      module = mod.substr(static_cast<string::size_type>(0), mod.find('.'));
    }
  }
  return module;
}

/// Retrieve full name of module
const string& moduleNameFull()   {
  static string module("");
  if ( module == "" )   {
    if (processHandle() and moduleHandle())    {
      char name[PATH_MAX] = {"Unknown.module"};
      name[0] = 0;
      const char *path =
          (reinterpret_cast<Dl_info*>(moduleHandle())->dli_fname);
      if (::realpath(path, name)) {
        module = name;
      }
    }
  }
  return module;
}

/// Get type of the module
ModuleType moduleType()   {
  static ModuleType type = ModuleType::UNKNOWN;
  if (type == ModuleType::UNKNOWN)    {
    const string& module = moduleNameFull();
    std::size_t loc = module.rfind('.')+1;
    if (loc == 0) {
      type = ModuleType::EXECUTABLE;
    } else if (module[loc] == 'e' or module[loc] == 'E') {
      type = ModuleType::EXECUTABLE;
    } else if (module[loc] == 's' and module[loc+1] == 'o') {
      type = ModuleType::SHAREDLIB;
    } else {
      type = ModuleType::UNKNOWN;
    }
  }
  return type;
}

/// Retrieve process handle
void* processHandle()   {
  static std::int64_t pid = ::getpid();
  static void* hP = reinterpret_cast<void*>(pid);
  return hP;
}

void setModuleHandle(ImageHandle handle)    {
  s_module_handle = handle;
}

ImageHandle moduleHandle() {
  if (0 == s_module_handle) {
    if (processHandle()) {
      static Dl_info info;
      if (0 != ::dladdr(FuncPtrCast<void*>(moduleHandle), &info)) {
        return &info;
      }
    }
  }
  return s_module_handle;
}

ImageHandle exeHandle()    {
  // This does NOT work!
  static Dl_info infoBuf;
  static Dl_info *info;

  if (0 == info) {
    void* handle = ::dlopen(0, RTLD_LAZY);
    if (0 != handle) {
      void* func = ::dlsym(handle, "main");
      if (0 != func) {
        if (0 != ::dladdr(func, &infoBuf)) {
          info = &infoBuf;
        }
      }
    }
  }
  return info;
}


const string& exeName() {
  static string module("");
  if (module.length() == 0)    {
    module = getExecutablePath().string();
  }
  return module;
}

Path::Item getSelfProc() {

  Path::Item self_proc {"/proc/self"};

  Path::Item exe = self_proc / "exe";

  if (not boost::filesystem::exists(exe)) {
    std::stringstream self_str {};
    self_str << "/proc/" << ::getpid();
    self_proc = Path::Item(self_str.str());
  }

  return self_proc;

}

vector<Path::Item> linkedModulePaths() {

  vector<Path::Item> linked_modules;

  Path::Item self_maps = getSelfProc() / "maps";
  std::ifstream maps_str(self_maps.string());

  string line;
  while (std::getline(maps_str, line)) {
    string address, perms, offset, dev, pathname;
    unsigned inode;
    std::istringstream iss(line);
    if (not(iss >> address >> perms >> offset >> dev >> inode >> pathname)) {
      continue;
    }
    if (perms == "r-xp" and boost::filesystem::exists(pathname)) {
      linked_modules.emplace_back(Path::Item(pathname));
    }
  }

  maps_str.close();

  return linked_modules;

}

const vector<string> linkedModules() {

  if (s_linkedModules.size() == 0) {

    for (auto m : linkedModulePaths()) {
      s_linkedModules.emplace_back(m.string());
    }

  }
  return s_linkedModules;
}

Path::Item getExecutablePath() {

#ifdef __APPLE__
  path self_proc {};
  char pathbuf[PATH_MAX + 1];
  unsigned int  bufsize = sizeof(pathbuf);
  _NSGetExecutablePath(pathbuf, &bufsize);
  path self_exe = path(string(pathbuf));
#else

  Path::Item self_exe = getSelfProc() / "exe";

#endif

  return boost::filesystem::canonical(self_exe);
}

}  // namespace System
}  // namespace Elements
