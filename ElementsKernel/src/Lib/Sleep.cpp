/**
 * @file Sleep.cpp
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

#include "ElementsKernel/Sleep.h"

#include <thread>                     // for this_thread
#include <chrono>                     // for chrono
#include <cstdint>                    // for int64_t

using std::this_thread::sleep_for;

namespace Elements {


/** @brief Small variation on the sleep function for seconds sleep.
 * @author Hubert Degaudenzi
 */
void normalSleep(int sec) {
  sleep_for(std::chrono::seconds(sec));
}

/** @brief Small variation on the sleep function for nanoseconds sleep.
 * @author Hubert Degaudenzi
 */
void nanoSleep(std::int64_t nsec) {
  sleep_for(std::chrono::nanoseconds(nsec));
}

}  // namespace Elements
