/*
 * Copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @addtogroup ElementsServices ElementsServices
 * @{
 */

#ifndef ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCUTILS_H_
#define ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCUTILS_H_

#include <string>
#include <utility>
#include <vector>

#include <boost/filesystem.hpp>

#include "ElementsKernel/Export.h"
#include "ElementsKernel/Path.h"

namespace Elements {
inline namespace Services {
namespace DataSync {

using path = Path::Item;

ELEMENTS_API path confFilePath(path filename);

ELEMENTS_API bool checkCall(std::string command);

ELEMENTS_API std::pair<std::string, std::string> runCommandAndCaptureOutErr(std::string command);

ELEMENTS_API bool localDirExists(path localDir);

ELEMENTS_API void createLocalDirOf(path localFile);

/**
 * @brief Get the value of an environment variable.
 * @ingroup ElementsServices
 * @return The value if the variable exists; "" otherwise.
 */
ELEMENTS_API std::string environmentVariable(std::string name);

ELEMENTS_API path localWorkspacePrefix();

ELEMENTS_API std::string lower(std::string text);

template <typename T>
ELEMENTS_API bool valueIsListed(const T& value, const std::vector<T>& list) {
  const auto& begin = list.begin();
  const auto& end   = list.end();
  return std::find(begin, end, value) != end;
}

ELEMENTS_API bool containsInThisOrder(std::string input, std::vector<std::string> substrings);

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements

namespace ElementsServices = Elements::Services;

#endif  // ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCUTILS_H_

/**@}*/
