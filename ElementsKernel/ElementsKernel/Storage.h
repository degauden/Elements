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

#ifndef ELEMENTSKERNEL_STORAGE_H_
#define ELEMENTSKERNEL_STORAGE_H_

#include <map>
#include <cstdint>  // for std::int64_t
#include <cmath>    // for std::pow, std::round

#include "ElementsKernel/Export.h"
#include "ElementsKernel/Number.h"   // for numberCast

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

  ELEMENTS_API extern std::map<StorageType, std::string> StorageShortName ;
  ELEMENTS_API extern std::map<StorageType, std::int64_t> StorageFactor;

  template<typename T>
  ELEMENTS_API T roundToDigits(const T& value, const size_t& max_digits) {
    std::int64_t factor = std::pow(10, max_digits);
    return std::round(value * static_cast<T>(factor))/static_cast<T>(factor);
  }


  template<std::size_t max_digits, typename T>
  ELEMENTS_API T storageConvert(const T& size, StorageType source_unit, StorageType target_unit) {

    using std::log10;

    T converted_value = size ;

    if (source_unit != target_unit) {
      T size_in_bytes = size * StorageFactor[source_unit];
      int64_t target_factor = StorageFactor[target_unit];
      double value = roundToDigits(static_cast<double>(size_in_bytes)/static_cast<double>(target_factor),
                            max_digits);
      converted_value = Elements::numberCast<T>(value);
    }

    return converted_value;

  }

  template<typename T>
  ELEMENTS_API T storageConvert(const T& size, StorageType source_unit, StorageType target_unit) {

    using std::log10;

    T converted_value = size ;

    if (source_unit != target_unit) {
      T size_in_bytes = size * StorageFactor[source_unit];
      int64_t target_factor = StorageFactor[target_unit];
      double value = roundToDigits(static_cast<double>(size_in_bytes)/static_cast<double>(target_factor),
                            static_cast<size_t>(log10(static_cast<double>(target_factor))));
      converted_value = Elements::numberCast<T>(value);
    }

    return converted_value;

  }



}
} // end of namespace Elements




#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_STORAGE_H_
