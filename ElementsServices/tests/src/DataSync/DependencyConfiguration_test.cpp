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
#include <string>
#include <vector>

#include "ElementsServices/DataSync/DataSyncUtils.h"

#include "fixtures/ConfigFilesFixture.h"
#include "fixtures/DependencyConfigurationPublic.h"

namespace DataSync = ElementsServices::DataSync;

using DataSync::path;
using std::string;

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(DependencyConfiguration_test, WorkspaceFixture)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(parse_line_without_alias_test) {

  const path                    distant_root = "distant";
  const path                    local_root   = "local";
  DependencyConfigurationPublic config(distant_root, local_root, theDependencyConfig());
  const path                    distant_file = "test/some_file.fits";
  const string                  line         = distant_file.string();
  BOOST_CHECK(not config.lineHasAlias(line));
  config.parseLineWithoutAlias(line);
  const path found_distant_file = config.distantPathOf(local_root / distant_file);
  BOOST_CHECK(found_distant_file == distant_root / distant_file);
}

BOOST_AUTO_TEST_CASE(parse_line_with_alias_test) {
  const path                    distant_root = "distant";
  const path                    local_root   = "local";
  DependencyConfigurationPublic config(distant_root, local_root, theDependencyConfig());
  const path                    distant_file = "test/some_distant_file_v1.fits";
  const path                    local_file   = "test/some_local_file.fits";
  const string                  line         = distant_file.string() + config.aliasSeparator() + local_file.string();
  BOOST_CHECK(config.lineHasAlias(line));
  config.parseLineWithAlias(line);
  const path found_distant_file = config.distantPathOf(local_root / local_file);
  BOOST_CHECK(found_distant_file == distant_root / distant_file);
}

BOOST_AUTO_TEST_CASE(conf_dependencies_test) {
  DataSync::DependencyConfiguration config("", "", theDependencyConfig());
  const auto                        found_files = config.localPaths();
  BOOST_CHECK_EQUAL(found_files.size(), theLocalFiles().size());
  for (const auto& expected : theLocalFiles()) {
    BOOST_CHECK(DataSync::valueIsListed(expected, found_files));
  }
  for (const auto& found : found_files) {
    BOOST_CHECK(DataSync::valueIsListed(found, theLocalFiles()));
  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
