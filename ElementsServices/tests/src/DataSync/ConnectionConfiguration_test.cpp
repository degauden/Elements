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

#include "fixtures/ConnectionConfigurationPublic.h"

using namespace ElementsServices::DataSync;

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (ConnectionConfiguration_test)

//-----------------------------------------------------------------------------

void checkHostParsing (DataHost host, std::vector<std::string> names) {
  ConnectionConfigurationPublic config;
  for (const auto& name : names) {
    config.parseHost(name);
    BOOST_CHECK(config.host == host);
  }
}

BOOST_AUTO_TEST_CASE( host_parsing_test ) {
  const std::vector<std::string> irodsNames { "irods", "iRODS", "IRODS" };
  const std::vector<std::string> webdavNames { "webdav", "WebDAV", "WEBDAV" };
  const std::vector<std::string> dssNames { "dss", "Dss", "DSS" };
  const std::vector<std::string> unknownNames { "blob", "BLOB" };
  checkHostParsing(DataHost::IRODS, irodsNames);
  checkHostParsing(DataHost::WEBDAV, webdavNames);
  checkHostParsing(DataHost::DSS, dssNames);
  BOOST_CHECK_THROW(checkHostParsing(DataHost::IRODS, unknownNames), std::exception);
}

BOOST_AUTO_TEST_CASE( overwriting_policy_test ) {
  ConnectionConfiguration config(theWebdavFrConfig());
  ConnectionConfiguration configNoOverwrite(theNoOverwriteConfig());
  BOOST_CHECK(config.overwritingAllowed());
  BOOST_CHECK(not configNoOverwrite.overwritingAllowed());
}

BOOST_AUTO_TEST_CASE( webdav_fr_config_test ) {
  ConnectionConfiguration config(theWebdavFrConfig());
  BOOST_CHECK(config.host == DataHost::WEBDAV);
  BOOST_CHECK(config.overwritingPolicy == OverwritingPolicy::OVERWRITE);
  BOOST_CHECK_EQUAL(config.distantRoot.string(), "/euclid-fr/ct/mock_test_data");
  BOOST_CHECK_EQUAL(config.localRoot.string(), thePrefixedLocalWorkspace());
  BOOST_CHECK_EQUAL(config.tries, 8);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
