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

#include <boost/thread/thread.hpp>

namespace Elements {

/// Simple sleep function taken from COOL.
/// @author Marco Clemencic
void normalSleep(int sec) {
  boost::this_thread::sleep_for(boost::chrono::seconds(sec));
}

/** @brief Small variation on the sleep function for nanoseconds sleep.
 * @author Marco Clemencic
 */
void nanoSleep(int64_t nsec) {
  boost::this_thread::sleep_for(boost::chrono::nanoseconds(nsec));
}

}
