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

#ifndef _ELEMENTSSERVICES_DATASYNC_WEBDAVSYNCHRONIZER_H
#define _ELEMENTSSERVICES_DATASYNC_WEBDAVSYNCHRONIZER_H

#include "DataSynchronizer.h"

namespace ElementsServices {
namespace DataSync {

/**
 * @class WebdavSynchronizer
 * @brief A data synchronizer for WebDAV hosts.
 *
 */
class WebdavSynchronizer: public DataSynchronizer {

public:

  virtual ~WebdavSynchronizer () = default;

  WebdavSynchronizer (
      ConnectionConfiguration connection,
      DependencyConfiguration dependency);

  virtual std::string createDownloadCommand (
      path distantFile,
      path localFile) const override;

};

}
}

#endif
