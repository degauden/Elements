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

#ifndef _ELEMENTSSERVICES_DATASYNC_H
#define _ELEMENTSSERVICES_DATASYNC_H

#include "ElementsServices/DataSync/DataSyncUtils.h"
#include "ElementsServices/DataSync/ConnectionConfiguration.h"
#include "ElementsServices/DataSync/DependencyConfiguration.h"

namespace ElementsServices {
namespace DataSync {


/**
 * @brief A class to download test data from a data repository
 * prior to the unit test run.
 *
 * @details Refer to the corresponding Redmine project for more details.
 *
 * @see https://euclid.roe.ac.uk/projects/testdata/wiki
 */
class DataSync {

public:

  /**
   * @brief Initialize the synchronizer with configuration files.
   *
   * @param connectionFile Path to the connection configuration file
   * relative to the configuration directory.
   *
   * @param dependencyFile Path to the dependency configuration file
   * relative to the configuration directory.
   */
  DataSync (path connectionFile, path dependencyFile);

  /**
   * @brief Download the test data.
   */
  void download ();

  /**
   * @brief Download the test data and provide a fallback host
   * in case the primary host fails.
   *
   * @param connectionFile Path to the connection configuration file
   * of the fallback host relative to the configuration directory.
   */
  void downloadWithFallback (path connectionFile);

  /**
   * @brief Get the absolute path to a local test file
   * which has been downloaded.
   *
   * @details The absolute path is the concatenation of:
   * - a system-defined prefix,
   * - the local workspace,
   * - the path of the file relative to the local workspace.
   *
   * On LODEEN, the prefix is empty.
   * On CODEEN, it is the job workspace which the user do not know.
   * It can be set by the user through the $WORKSPACE environment variable.
   *
   * @warning This function must be used to access any data
   * downloaded by the DataSync tool.
   */
  path absolutePath (path relativePath);

private:

  ConnectionConfiguration m_connectionConfig;
  path m_distantRoot;
  path m_localRoot;
  DependencyConfiguration m_dependencyConfig;

};

}
}

#endif
