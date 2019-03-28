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

#ifndef ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_CONNECTIONCONFIGURATION_H_
#define ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_CONNECTIONCONFIGURATION_H_

#include <string>

#include "ElementsKernel/Export.h"

#include "ElementsServices/DataSync/DataSyncUtils.h"

namespace ElementsServices {
namespace DataSync {

/**
 * @enum DataHost
 * @brief The test data hosting solution.
 */
enum DataHost {
  IRODS, WEBDAV,
};

/**
 * @brief The overwriting policy if the local file already exists.
 */
enum OverwritingPolicy {
  ABORT, OVERWRITE,
};

/**
 * @class UnknownHost
 * @brief Exception raised when a hosting solution is not supported by the tool.
 */
class ELEMENTS_API UnknownHost: public std::runtime_error {
public:
  virtual ~UnknownHost() = default;
  UnknownHost() :
      std::runtime_error("I don't know this hosting solution!") {
  }
  explicit UnknownHost(std::string hostName):
      std::runtime_error("I don't know this hosting solution: " + hostName) {
  }
};

/**
 * @class ConnectionConfiguration
 * @brief The connection configuration mainly holds:
 * * the host type and URL,
 * * the user name and password,
 * * the overwriting policy.
 */
class ELEMENTS_API ConnectionConfiguration {

public:

  virtual ~ConnectionConfiguration() = default;

  /**
   * @brief Create a dependency configuration by reading a configuration file.
   */
  explicit ConnectionConfiguration(path configFile);

  /**
   * @brief Check whether existing local files can be overwritten.
   */
  bool overwritingAllowed() const;

protected:

  void parseConfigurationFile(path filename);

  void parseHost(std::string name);

  void parseOverwritingPolicy(std::string policy);

public:

  DataHost host;
  std::string hostUrl;
  std::string user;
  std::string password;
  OverwritingPolicy overwritingPolicy;
  size_t tries;
  path distantRoot;
  path localRoot;

};

}  // namespace DataSync
}  // namespace ElementsServices

#endif  // ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_CONNECTIONCONFIGURATION_H_
