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
#include <vector>

#include "ElementsServices/DataSync/DataSyncUtils.h"

#include "fixtures/DependencyConfigurationPublic.h"
#include "fixtures/ConfigFilesFixture.h"

using namespace ElementsServices::DataSync;

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (DependencyConfiguration_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( parse_line_without_alias_test ) {
  const path distantRoot = "distant";
  const path localRoot = "local";
  DependencyConfigurationPublic config(distantRoot, localRoot, theDependencyConfig());
  const path distantFile = "test/some_file.fits";
  const std::string line = distantFile.string();
  BOOST_CHECK(not config.lineHasAlias(line));
  config.parseLineWithoutAlias(line);
  const path foundDistantFile = config.distantPathOf(localRoot / distantFile);
  BOOST_CHECK(foundDistantFile == distantRoot / distantFile);
}

BOOST_AUTO_TEST_CASE( parse_line_with_alias_test ) {
  const path distantRoot = "distant";
  const path localRoot = "local";
  DependencyConfigurationPublic config(distantRoot, localRoot, theDependencyConfig());
  const path distantFile = "test/some_distant_file_v1.fits";
  const path localFile = "test/some_local_file.fits";
  const std::string line =
      distantFile.string() + config.aliasSeparator() + localFile.string();
  BOOST_CHECK(config.lineHasAlias(line));
  config.parseLineWithAlias(line);
  const path foundDistantFile = config.distantPathOf(localRoot / localFile);
  BOOST_CHECK(foundDistantFile == distantRoot / distantFile);
}

BOOST_AUTO_TEST_CASE( conf_dependencies_test ) {
  DependencyConfiguration config("", "", theDependencyConfig());
  const auto foundFiles = config.localPaths();
  BOOST_CHECK_EQUAL(foundFiles.size(), theLocalFiles().size());
  for (const auto& expected : theLocalFiles())
    BOOST_CHECK(valueIsListed(expected, foundFiles));
  for (const auto& found : foundFiles)
    BOOST_CHECK(valueIsListed(found, theLocalFiles()));
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
