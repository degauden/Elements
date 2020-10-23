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

#ifndef ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCHRONIZER_H_
#define ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCHRONIZER_H_

#include <map>
#include <stdexcept>
#include <string>

#include "ElementsKernel/Export.h"

#include "ElementsServices/DataSync/ConnectionConfiguration.h"
#include "ElementsServices/DataSync/DataSyncUtils.h"
#include "ElementsServices/DataSync/DependencyConfiguration.h"

namespace ElementsServices {
namespace DataSync {

/**
 * @class DownloadFailed
 * @ingroup ElementsServices
 * @brief An exception raised when downloading fails.
 */
class ELEMENTS_API DownloadFailed : public std::runtime_error {
public:
  virtual ~DownloadFailed() = default;
  DownloadFailed(path distantFile, path localFile)
      : std::runtime_error("Unable to download file: '" + distantFile.string() + "' as: '" + localFile.string() +
                           "'.") {}
};

/**
 * @class DataSynchronizer
 * @ingroup ElementsServices
 * @brief Base class to synchronize test data.
 * @details
 * Child classes should provide their synchronization command line.
 */
class ELEMENTS_API DataSynchronizer {

public:
  virtual ~DataSynchronizer() = default;

  DataSynchronizer(const ConnectionConfiguration& connection, const DependencyConfiguration& dependency);

  void downloadAllFiles() const;

protected:
  bool fileShouldBeWritten(path localFile) const;

  bool fileAlreadyExists(path localFile) const;

  void downloadOneFile(path distantFile, path localFile) const;

  bool hasBeenDownloaded(path distantFile, path localFile) const;

  virtual std::string createDownloadCommand(path distantFile, path localFile) const = 0;

protected:
  ConnectionConfiguration m_connection;
  std::map<path, path>    m_fileMap;
};

}  // namespace DataSync
}  // namespace ElementsServices

#endif  // ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCHRONIZER_H_

/**@}*/
