/**
 * @file Storage.cpp
 *
 * @date Dec 22, 2014
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
 */

#include "ElementsKernel/Storage.h"

#include <cmath>    // for pow, round, log10
#include <cstddef>  // for size_t
#include <cstdint>  // for int64_t
#include <map>      // for map
#include <string>   // for string

using std::int64_t;
using std::map;
using std::pow;
using std::size_t;

namespace Elements {
inline namespace Kernel {
namespace Units {

map<StorageType, std::string> StorageShortName{{StorageType::Byte, "B"},
                                               {StorageType::KiloByte, "KiB"},
                                               {StorageType::MegaByte, "MiB"},
                                               {StorageType::GigaByte, "GiB"},
                                               {StorageType::TeraByte, "TiB"},
                                               {StorageType::PetaByte, "PiB"},
                                               {StorageType::MetricKiloByte, "KB"},
                                               {StorageType::MetricMegaByte, "MB"},
                                               {StorageType::MetricGigaByte, "GB"},
                                               {StorageType::MetricTeraByte, "TB"},
                                               {StorageType::MetricPetaByte, "PB"}};

map<StorageType, int64_t> StorageFactor{{StorageType::Byte, 1},
                                        {StorageType::KiloByte, pow(2, 10)},
                                        {StorageType::MegaByte, pow(2, 20)},
                                        {StorageType::GigaByte, pow(2, 30)},
                                        {StorageType::TeraByte, pow(2, 40)},
                                        {StorageType::PetaByte, pow(2, 50)},
                                        {StorageType::MetricKiloByte, pow(10, 3)},
                                        {StorageType::MetricMegaByte, pow(10, 6)},
                                        {StorageType::MetricGigaByte, pow(10, 9)},
                                        {StorageType::MetricTeraByte, pow(10, 12)},
                                        {StorageType::MetricPetaByte, pow(10, 15)}};

// explicit instantiation: without the template<>. Otherwise this is a template specialization
template double roundToDigits<double>(const double& value, const size_t& max_digits);
template float  roundToDigits<float>(const float& value, const size_t& max_digits);

template double  storageConvert<double>(const double& size, StorageType source_unit, StorageType target_unit);
template float   storageConvert<float>(const float& size, StorageType source_unit, StorageType target_unit);
template int64_t storageConvert<int64_t>(const int64_t& size, StorageType source_unit, StorageType target_unit);

}  // namespace Units
}  // namespace Kernel
}  // namespace Elements
