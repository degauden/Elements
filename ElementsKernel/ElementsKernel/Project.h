/**
 * @file ElementsKernel/Project.h
 * @brief Defines tools to describe the current project
 * @date May 14, 2020
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_H_

#include <algorithm>  // for for_each
#include <cstdint>    // for uing_least64_t
#include <iostream>   // for ostream
#include <string>     // for string
#include <vector>     // for vector

#include "ThisProject.h"  // local project constants generated
                          // by the build system

namespace Elements {

struct Project {
  static inline std::string name() {
    return THIS_PROJECT_NAME_STRING;
  }

  static inline std::string versionString() {
    return THIS_PROJECT_VERSION_STRING;
  }
  static inline std::uint_least64_t version() {
    return THIS_PROJECT_VERSION;
  }
  static inline std::uint_least64_t majorVersion() {
    return THIS_PROJECT_MAJOR_VERSION;
  }
  static inline std::uint_least64_t minorVersion() {
    return THIS_PROJECT_MINOR_VERSION;
  }
  static inline std::uint_least64_t patchVersion() {
    return THIS_PROJECT_PATCH_VERSION;
  }
  static inline std::string originalVersion() {
    return THIS_PROJECT_ORIGINAL_VERSION;
  }
  static inline std::string vcsVersion() {
    return THIS_PROJECT_VCS_VERSION;
  }

  static inline std::string installLocation() {
    return THIS_PROJECT_INSTALL_LOCATION_STRING;
  }
  static inline bool useSoVersion() {
    return THIS_PROJECT_USE_SOVERSION;
  }
  static inline std::string installPrefix() {
    return CMAKE_INSTALL_PREFIX_STRING;
  }
  static inline std::vector<std::string> searchDirectories() {
    return THIS_PROJECT_SEARCH_DIRS;
  }
};

inline std::ostream& operator<<(std::ostream& stream, const Project& p) {
  stream << "Name: " << p.name() << std::endl;
  stream << "Version String: " << p.versionString() << std::endl;
  stream << "Version: " << p.version() << std::endl;
  stream << "Major Version: " << p.majorVersion() << std::endl;
  stream << "Minor Version: " << p.minorVersion() << std::endl;
  stream << "Patch Version: " << p.patchVersion() << std::endl;
  stream << "Original Version: " << p.originalVersion() << std::endl;
  stream << "VCS Version: " << p.vcsVersion() << std::endl;
  stream << "Install Location: " << p.installLocation() << std::endl;
  stream << "Use So Version: " << p.useSoVersion() << std::endl;
  stream << "Install Prefix: " << p.installPrefix() << std::endl;
  stream << "Search Directories: ";

  for (const auto& d : p.searchDirectories()) {
    stream << d << " ";
  }

  return stream;
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_H_

/**@}*/
