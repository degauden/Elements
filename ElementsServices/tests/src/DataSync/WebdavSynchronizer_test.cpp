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
#include <vector>

#include <boost/test/unit_test.hpp>

#include "ElementsServices/DataSync/DataSyncUtils.h"
#include "ElementsServices/DataSync/WebdavSynchronizer.h"

#include "fixtures/ConfigFilesFixture.h"

using std::string;

namespace DataSync = ElementsServices::DataSync;

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(WebdavSynchronizer_test, WorkspaceFixture)

//-----------------------------------------------------------------------------

DataSync::WebdavSynchronizer createTestSynchronizer() {
  DataSync::ConnectionConfiguration connection(theWebdavFrConfig());
  auto distant_root = connection.distantRoot;
  auto local_root = connection.localRoot;
  DataSync::DependencyConfiguration dependencies(
      distant_root, local_root, theDependencyConfig());
  return DataSync::WebdavSynchronizer(connection, dependencies);
}

BOOST_AUTO_TEST_CASE(webdavGetCmd_test) {
  string distant_file = "src/distant_file.fits";
  string local_file = "dst/local_file.fits";
  auto synchronizer = createTestSynchronizer();
  string cmd = synchronizer.createDownloadCommand(distant_file, local_file);
  std::vector<string> chunks = {
      "wget ", "-O",
      local_file, distant_file,
      "8" };
  BOOST_CHECK(DataSync::containsInThisOrder(cmd, chunks));
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
