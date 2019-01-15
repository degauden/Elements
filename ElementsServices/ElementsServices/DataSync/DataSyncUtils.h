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

#ifndef _ELEMENTSSERVICES_DATASYNC_DATASYNCUTILS_H
#define _ELEMENTSSERVICES_DATASYNC_DATASYNCUTILS_H

#include <boost/filesystem.hpp>
#include <string>
#include <utility>
#include <vector>

namespace ElementsServices {
namespace DataSync {

using path = boost::filesystem::path;

path confFilePath (path filename);

bool checkCall (std::string command);

std::pair<std::string, std::string> runCommandAndCaptureOutErr (
    std::string command);

bool localDirExists (path localDir);

void createLocalDirOf (path localFile);

/**
 * @brief Get the value of an environment variable.
 * @return The value if the variable exists; "" otherwise.
 */
std::string environmentVariable (std::string name);

path localWorkspacePrefix ();

std::string lower (std::string text);

template<typename T>
bool valueIsListed (const T& value, const std::vector<T>& list) {
  const auto& begin = list.begin();
  const auto& end = list.end();
  return std::find(begin, end, value) != end;
}

bool containsInThisOrder (
    std::string input,
    std::vector<std::string> substrings);

}
}

#endif
