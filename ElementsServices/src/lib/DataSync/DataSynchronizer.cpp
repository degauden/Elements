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
#include "ElementsKernel/Unused.h"

#include "ElementsServices/DataSync/DataSynchronizer.h"
#include "ElementsServices/DataSync/DataSyncUtils.h"

namespace ElementsServices {
namespace DataSync {

DataSynchronizer::DataSynchronizer(
    const ConnectionConfiguration& connection,
    const DependencyConfiguration& dependency) :
        m_connection(connection), m_fileMap(dependency.fileMap()) {
}

void DataSynchronizer::downloadAllFiles() const {
  for (const auto& item : m_fileMap) {
    const auto& localFile = item.first;
    const auto& distantFile = item.second;
    if (fileShouldBeWritten(localFile)) {
      downloadOneFile(distantFile, localFile);
    }
  }
}

bool DataSynchronizer::fileShouldBeWritten(path localFile) const {
  if (not fileAlreadyExists(localFile)) {
    return true;
  }
  return m_connection.overwritingAllowed();
}

bool DataSynchronizer::fileAlreadyExists(path localFile) const {
  return boost::filesystem::is_regular_file(localFile);
}

void DataSynchronizer::downloadOneFile(
    path distantFile,
    path localFile) const {
  std::string command = createDownloadCommand(distantFile, localFile);
  createLocalDirOf(localFile);
  const auto outErr = runCommandAndCaptureOutErr(command);
  if (not hasBeenDownloaded(distantFile, localFile)) {
    throw DownloadFailed(distantFile, localFile);
  }
}

bool DataSynchronizer::hasBeenDownloaded(
    ELEMENTS_UNUSED path distantFile,
    path localFile) const {
  if (not boost::filesystem::is_regular_file(localFile)) {
    return false;
  }
  return boost::filesystem::file_size(localFile) > 0;
}

}  // namespace DataSync
}  // namespace ElementsServices
