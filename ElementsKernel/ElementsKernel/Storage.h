/**
 * @file Storage.h
 *
 * @date Dec 22, 2014
 * @author hubert
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_STORAGE_H_
#define ELEMENTSKERNEL_STORAGE_H_

#include <map>      // for std::map
#include <cstdint>  // for std::int64_t

#include "ElementsKernel/Export.h"

namespace Elements {
namespace Units {

enum class StorageType {
  Byte,
  KiloByte,
  MegaByte,
  GigaByte,
  TeraByte,
  PetaByte,
  MetricKiloByte,
  MetricMegaByte,
  MetricGigaByte,
  MetricTeraByte,
  MetricPetaByte
};

ELEMENTS_API extern std::map<StorageType, std::string> StorageShortName;
ELEMENTS_API extern std::map<StorageType, std::int64_t> StorageFactor;

template<typename T>
ELEMENTS_API T roundToDigits(const T& value, const size_t& max_digits);

template<std::size_t max_digits, typename T>
ELEMENTS_API T storageConvert(const T& size, StorageType source_unit, StorageType target_unit);

template<typename T>
ELEMENTS_API T storageConvert(const T& size, StorageType source_unit, StorageType target_unit);

} // end of namespace Units
} // end of namespace Elements

#include "ElementsKernel/_impl/Storage.icpp"

#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_STORAGE_H_

/**@}*/
