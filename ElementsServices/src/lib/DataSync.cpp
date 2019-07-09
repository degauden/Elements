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

#include "ElementsServices/DataSync.h"
#include "ElementsServices/DataSync/DataSynchronizer.h"
#include "ElementsServices/DataSync/DataSynchronizerMaker.h"

namespace ElementsServices {
namespace DataSync {


DataSync::DataSync(path connectionFile, path dependencyFile) :
    m_connectionConfig(connectionFile),
    m_distantRoot(m_connectionConfig.distantRoot),
    m_localRoot(m_connectionConfig.localRoot),
    m_dependencyConfig(m_distantRoot, m_localRoot, dependencyFile) {
}

void DataSync::download() {
  const auto& synchronizer = createSynchronizer(
      m_connectionConfig,
      m_dependencyConfig);
  synchronizer->downloadAllFiles();
}

void DataSync::downloadWithFallback(path connectionFile) {
  try {
    download();
  } catch (std::exception &e) {
    m_connectionConfig = ConnectionConfiguration(connectionFile);
    download();
  }
}

path DataSync::absolutePath(path relativePath) {
  return m_localRoot / relativePath;
}

}  // namespace DataSync
}  // namespace ElementsServices
