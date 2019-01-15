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

#include <boost/test/unit_test.hpp>

#include "ElementsServices/DataSync/DataSyncUtils.h"
#include "ElementsServices/DataSync/WebdavSynchronizer.h"

#include "fixtures/ConfigFilesFixture.h"

using namespace ElementsServices::DataSync;

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (WebdavSynchronizer_test)

//-----------------------------------------------------------------------------

WebdavSynchronizer createTestSynchronizer () {
  ConnectionConfiguration connection(theWebdavFrConfig());
  auto distantRoot = connection.distantRoot;
  auto localRoot = connection.localRoot;
  DependencyConfiguration dependencies(
      distantRoot, localRoot, theDependencyConfig());
  return WebdavSynchronizer(connection, dependencies);
}

BOOST_AUTO_TEST_CASE( webdavGetCmd_test ) {
  std::string distantFile = "src/distant_file.fits";
  std::string localFile = "dst/local_file.fits";
  auto synchronizer = createTestSynchronizer();
  std::string cmd = synchronizer.createDownloadCommand(distantFile, localFile);
  std::vector<std::string> chunks = {
      "wget ", "-O",
      localFile, distantFile,
      "8" };
  BOOST_CHECK(containsInThisOrder(cmd, chunks));
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
