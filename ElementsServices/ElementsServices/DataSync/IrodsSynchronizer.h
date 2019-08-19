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

#ifndef ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_IRODSSYNCHRONIZER_H_
#define ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_IRODSSYNCHRONIZER_H_

#include <string>

#include "ElementsKernel/Export.h"

#include "ElementsServices/DataSync/DataSynchronizer.h"

namespace ElementsServices {
namespace DataSync {

/**
 * @brief Check whether the iRODS client is installed.
 * @ingroup ElementsServices
 */
ELEMENTS_API bool irodsIsInstalled();

/**
 * @class IrodsSynchronizer
 * @ingroup ElementsServices
 * @brief A data synchronizer for iRODS hosts.
 */
class ELEMENTS_API IrodsSynchronizer: public DataSynchronizer {

public:

  virtual ~IrodsSynchronizer() = default;

  IrodsSynchronizer(
      const ConnectionConfiguration& connection,
      const DependencyConfiguration& dependency);

  std::string createDownloadCommand(
      path distantFile,
      path localFile) const override;

};

}  // namespace DataSync
}  // namespace ElementsServices

#endif  // ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_IRODSSYNCHRONIZER_H_

/**@}*/
