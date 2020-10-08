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

#ifndef ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATION_H_
#define ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATION_H_

#include <map>
#include <string>
#include <vector>

#include "ElementsServices/DataSync/DataSyncUtils.h"

namespace ElementsServices {
namespace DataSync {

/**
 * @class DependencyConfiguration
 * @ingroup ElementsServices
 * @brief The dependency configurations holds,
 * for each test file to be retrieved:
 * * the distant source path,
 * * the local destination path.
 */
class ELEMENTS_API DependencyConfiguration {

public:
  virtual ~DependencyConfiguration() = default;

  DependencyConfiguration(path distantRoot, path localRoot, path configFile);

  std::map<path, path> fileMap() const;

  path distantPathOf(path localFile) const;

  size_t dependencyCount() const;

  std::vector<path> distantPaths() const;

  std::vector<path> localPaths() const;

protected:
  void parseConfigurationFile(path filename);

  void parseConfigurationLine(std::string line);

  char aliasSeparator() const;

  bool lineHasAlias(std::string line) const;

  void parseLineWithAlias(std::string line);

  void parseLineWithoutAlias(std::string line);

private:
  char                 m_aliasSeparator;
  path                 m_distantRoot;
  path                 m_localRoot;
  std::map<path, path> m_fileMap;
};

}  // namespace DataSync
}  // namespace ElementsServices

#endif  // ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATION_H_

/**@}*/
