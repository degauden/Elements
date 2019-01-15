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

#include "ElementsServices/DataSync/DssSynchronizer.h"

#include <string>
#include "ElementsKernel/Unused.h"

namespace ElementsServices {
namespace DataSync {

DssSynchronizer::DssSynchronizer(
    ConnectionConfiguration connection,
    DependencyConfiguration dependency) :
        DataSynchronizer(connection, dependency) {
}

std::string DssSynchronizer::createDownloadCommand(
    ELEMENTS_UNUSED path distantFile,
    ELEMENTS_UNUSED path localFile) const {
  throw UnknownHost("DSS");
}

}  // namespace DataSync
}  // namespace ElementsServices
