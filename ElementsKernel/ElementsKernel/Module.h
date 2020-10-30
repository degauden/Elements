/**
 * @file ElementsKernel/Module.h
 * @brief Defines tools to describe the current Elmeents module
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

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_H_

#include <algorithm>  // for for_each
#include <cstdint>    // for uing_least64_t
#include <iostream>   // for ostream
#include <string>     // for string
#include <vector>     // for vector

#include "ThisElementsModule.h"  // local module constants generated
                                 // by the build system

namespace Elements {

struct Module {
  static inline std::string name() {
    return THIS_MODULE_NAME_STRING;
  }
  static inline std::string versionString() {
    return THIS_MODULE_VERSION_STRING;
  }
  static inline std::uint_least64_t version() {
    return THIS_MODULE_VERSION;
  }
  static inline std::uint_least64_t majorVersion() {
    return THIS_MODULE_MAJOR_VERSION;
  }
  static inline std::uint_least64_t minorVersion() {
    return THIS_MODULE_MINOR_VERSION;
  }
  static inline std::uint_least64_t patchVersion() {
    return THIS_MODULE_PATCH_VERSION;
  }
};

inline std::ostream& operator<<(std::ostream& stream, const Module& m) {

  stream << "Name: " << m.name() << std::endl;
  stream << "Version String: " << m.versionString() << std::endl;
  stream << "Version: " << m.version() << std::endl;
  stream << "Major Version: " << m.majorVersion() << std::endl;
  stream << "Minor Version: " << m.minorVersion() << std::endl;
  stream << "Patch Version: " << m.patchVersion() << std::endl;

  return stream;
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_H_

/**@}*/
