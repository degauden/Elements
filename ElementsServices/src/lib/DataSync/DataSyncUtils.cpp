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

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ElementsKernel/Configuration.h"
#include "ElementsKernel/Environment.h"  // For Environment

#include "ElementsServices/DataSync/DataSyncUtils.h"

namespace Elements {
inline namespace Services {
namespace DataSync {

using std::string;

path confFilePath(path filename) {
  return Configuration::getPath(filename);
}

bool checkCall(const string& command) {
  string    silent_command = command + " > /dev/null";
  const int status         = std::system(silent_command.c_str());
  return status == 0;
}

std::pair<string, string> runCommandAndCaptureOutErr(string command) {
  string                   out;
  string                   err;
  std::array<char, BUFSIZ> buffer;
  std::shared_ptr<FILE>    cmdpipe(popen(command.c_str(), "r"), pclose);
  if (not cmdpipe) {
    throw std::runtime_error(string("Unable to run command: ") + command);
  }
  if (fgets(buffer.data(), BUFSIZ, cmdpipe.get()) != nullptr) {
    out += buffer.data();
  }
  // @TODO get standard error
  return std::make_pair(out, err);
}

bool localDirExists(path local_dir) {
  return boost::filesystem::is_directory(local_dir);
}

void createLocalDirOf(path local_file) {
  if (not local_file.has_parent_path()) {
    return;
  }
  const path dir = local_file.parent_path();
  if (not localDirExists(dir)) {
    boost::filesystem::create_directories(dir);
  }
}

string environmentVariable(string name) {
  return Environment().get(name);  // Already returns "" if not found
}

path localWorkspacePrefix() {
  const string codeen_prefix("WORKSPACE");
  const string prefix_env_variable(codeen_prefix);
  return path(environmentVariable(prefix_env_variable));
}

string lower(string text) {
  string uncased(text);
  std::transform(text.begin(), text.end(), uncased.begin(), ::tolower);
  return uncased;
}

bool containsInThisOrder(string input, std::vector<string> substrings) {
  string::size_type offset(0);
  for (auto substr : substrings) {
    offset = input.find(substr, offset);
    if (offset == string::npos) {
      return false;
    }
  }
  return true;
}

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements
