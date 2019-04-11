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
 * @file tests/src/IrodsSynchronizer_test.cpp
 * @date 04/13/18
 * @author user
 */

#include <boost/test/unit_test.hpp>
#include <exception>

#include "ElementsServices/DataSync/ConnectionConfiguration.h"
#include "ElementsServices/DataSync/DependencyConfiguration.h"
#include "ElementsServices/DataSync/IrodsSynchronizer.h"

#include "fixtures/ConfigFilesFixture.h"

namespace DataSync = ElementsServices::DataSync;
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(IrodsSynchronizer_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(irodsIsNotInstalled_test) {
  if (DataSync::irodsIsInstalled()) {
      return;
  }
  const auto connection = DataSync::ConnectionConfiguration(theIrodsFrConfig());
  const auto dependency = DataSync::DependencyConfiguration("", "", theDependencyConfig());
  BOOST_CHECK_THROW(
      DataSync::IrodsSynchronizer(connection, dependency),
      std::exception);
}

// @TODO test IrodsSynchronizer

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()


