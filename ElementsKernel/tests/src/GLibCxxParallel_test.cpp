/**
 * @file OpenMP_test.cpp
 *
 * @date March 17th 2020
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include <numeric>
#include <vector>

#include <boost/test/unit_test.hpp>

#include "ElementsKernel/Real.h"

BOOST_AUTO_TEST_SUITE(GLibCxxParallel_test)

BOOST_AUTO_TEST_CASE(GLibCxxParallelWorks) {

  const std::size_t val_size_t = 1000;

  std::vector<double> vals(val_size_t, 1.0);

  std::iota(vals.begin(), vals.end(), 0.0);

  auto   squared  = std::accumulate(vals.begin(), vals.end(), 0.0, [](const double& tot, const double& vA) {
    return tot + vA * vA;
     });
  double squared2 = 0.0;
  for (const auto& v : vals) {
    squared2 += v * v;
  }

  BOOST_CHECK(Elements::isEqual(squared, squared2));
}

//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END()
