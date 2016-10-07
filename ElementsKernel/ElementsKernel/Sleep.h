/**
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
 */


#ifndef ELEMENTSKERNEL_SLEEP_H_
#define ELEMENTSKERNEL_SLEEP_H_

#include <cstdint>
#include "ElementsKernel/Export.h" // ELEMENTS_API

namespace Elements {

/// Simple sleep function.
/// @author Marco Clemencic
ELEMENTS_API void normalSleep(int sec);

/// Small variation on the sleep function for nanoseconds sleep.
/// @author Marco Clemencic
ELEMENTS_API void nanoSleep(int64_t nsec);

}

#endif /*ELEMENTSKERNEL_SLEEP_H_*/
