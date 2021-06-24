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

#include <string>

#include "ElementsServices/DataSync/IrodsSynchronizer.h"

namespace Elements {
inline namespace Services {
namespace DataSync {

bool irodsIsInstalled() {
  return checkCall("iget --help");
}

IrodsSynchronizer::IrodsSynchronizer(const ConnectionConfiguration& connection,
                                     const DependencyConfiguration& dependency)
    : DataSynchronizer(connection, dependency) {
  if (not irodsIsInstalled()) {
    throw std::runtime_error("You are trying to use iRODS, "
                             "but it does not seem to be installed.");
  }
}

std::string IrodsSynchronizer::createDownloadCommand(path distantFile, path localFile) const {
  std::string cmd = "irsync i:";
  cmd += distantFile.string() + " " + localFile.string();
  return cmd;
}

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements
