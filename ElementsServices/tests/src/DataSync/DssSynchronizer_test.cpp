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
 * @file tests/src/DssSynchronizer_test.cpp
 * @date 04/13/18
 * @author user
 */

#include <boost/test/unit_test.hpp>

#include "ElementsServices/DataSync/DssSynchronizer.h"

#include "fixtures/ConfigFilesFixture.h"

using namespace ElementsServices::DataSync;

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (DssSynchronizer_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( notImplemented_test ) {
  const path dependency = theDependencyConfig();
  const ConnectionConfiguration connectionConfig(theIrodsFrConfig());
  const path distantRoot = connectionConfig.distantRoot;
  const path localRoot = connectionConfig.localRoot;
  const DependencyConfiguration dependencyConfig(distantRoot, localRoot, dependency);
  DssSynchronizer mock(connectionConfig, dependencyConfig);
  BOOST_CHECK_THROW(mock.downloadAllFiles(), UnknownHost);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()


