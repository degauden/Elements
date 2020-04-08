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


#include <omp.h>

#include <chrono>
#include <cstdint>
#include <vector>

#include <boost/test/unit_test.hpp>

using std::uint64_t;

constexpr int target_thread_num = 4;


uint64_t getMilliSecondsSinceEpoch() {

  namespace chrono = std::chrono;

  auto now = chrono::system_clock::now();
  auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);

  auto value = now_ms.time_since_epoch();
  uint64_t duration = value.count();

  return duration;

}

BOOST_AUTO_TEST_SUITE(OpenMP_test)

BOOST_AUTO_TEST_CASE(HasOpenMP_test) {

  omp_set_num_threads(target_thread_num);

  std::vector<uint64_t> times(target_thread_num);

  int is_in_parallel = 0;

  // Initialize all the times
  #pragma omp parallel
  {
     int thread_id = omp_get_thread_num();
     times[thread_id] = getMilliSecondsSinceEpoch();

     is_in_parallel = omp_in_parallel();

     times[thread_id] = getMilliSecondsSinceEpoch();
  }

  BOOST_CHECK_NE(is_in_parallel, 0);
  BOOST_CHECK_EQUAL(omp_in_parallel(), 0);
}

//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END()
