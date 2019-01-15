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

#include <array>
#include <cstdlib>

#include "ElementsKernel/Configuration.h"
#include "ElementsKernel/System.h"

#include "ElementsServices/DataSync/DataSyncUtils.h"

namespace ElementsServices {
namespace DataSync {

path confFilePath (path filename) {
  return Elements::getConfigurationPath(filename);
}

bool checkCall (std::string command) {
  const int status = system(command.c_str());
  return status == 0;
}

std::pair<std::string, std::string> runCommandAndCaptureOutErr (
    std::string command) {
  std::string out, err;
  std::array<char, BUFSIZ> buffer;
  std::shared_ptr<FILE> cmdpipe(popen(command.c_str(), "r"), pclose);
  if (not cmdpipe)
    throw std::runtime_error(std::string("Unable to run command: ") + command);
  if (fgets(buffer.data(), BUFSIZ, cmdpipe.get()) != NULL)
    out += buffer.data();
  //TODO get standard error
  return std::make_pair(out, err);
}

bool localDirExists (path localDir) {
  return boost::filesystem::is_directory(localDir);
}

void createLocalDirOf (path localFile) {
  if (not localFile.has_parent_path())
    return;
  const path dir = localFile.parent_path();
  if (not localDirExists(dir))
    boost::filesystem::create_directories(dir);
}

std::string environmentVariable (std::string name) {
  return Elements::System::getEnv(name); // Already returns "" if not found
}

path localWorkspacePrefix () {
  const std::string codeenPrefix("WORKSPACE");
  const std::string prefixEnvVariable(codeenPrefix);
  return path(environmentVariable(prefixEnvVariable));
}

std::string lower (std::string text) {
  std::string uncased(text);
  std::transform(text.begin(), text.end(), uncased.begin(), ::tolower);
  return uncased;
}

bool containsInThisOrder (
    std::string input,
    std::vector<std::string> substrings) {
  size_t offset(0);
  for (auto substr : substrings) {
    offset = input.find(substr, offset);
    if (offset == std::string::npos)
      return false;
  }
  return true;
}

}
}
