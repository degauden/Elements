/**
 * @file Storage.cpp
 *
 * @date Dec 22, 2014
 * @author hubert
 */

#include "ElementsKernel/Storage.h"

#include <map>
#include <cstdint>
#include <cmath>
#include <string>

using namespace std;

namespace Elements {
namespace Units {

map<StorageType, string> StorageShortName      { { StorageType::Byte,             "B"},
                                                 { StorageType::KiloByte,        "KB"},
                                                 { StorageType::MegaByte,        "MB"},
                                                 { StorageType::GigaByte,        "GB"},
                                                 { StorageType::TeraByte,        "TB"},
                                                 { StorageType::PetaByte,        "PB"},
                                                 { StorageType::MetricKiloByte, "KiB"},
                                                 { StorageType::MetricMegaByte, "MiB"},
                                                 { StorageType::MetricGigaByte, "GiB"},
                                                 { StorageType::MetricTeraByte, "TiB"},
                                                 { StorageType::MetricPetaByte, "PiB"} };


map<StorageType, int64_t> StorageFactor        { { StorageType::Byte,                    1},
                                                 { StorageType::KiloByte,       pow(2, 10)},
                                                 { StorageType::MegaByte,       pow(2, 20)},
                                                 { StorageType::GigaByte,       pow(2, 30)},
                                                 { StorageType::TeraByte,       pow(2, 40)},
                                                 { StorageType::PetaByte,       pow(2, 50)},
                                                 { StorageType::MetricKiloByte, pow(10, 3)},
                                                 { StorageType::MetricMegaByte, pow(10, 6)},
                                                 { StorageType::MetricGigaByte, pow(10, 9)},
                                                 { StorageType::MetricTeraByte, pow(10,12)},
                                                 { StorageType::MetricPetaByte, pow(10,15)} };

double roundToDigits(const double& value, const size_t& max_digits) {
  int64_t factor = pow(10, max_digits);
  return round(value * double(factor))/double(factor);
}


double storageConvert(const int64_t& size, StorageType source_unit, StorageType target_unit) {
  double value= double(size);

  if (source_unit != target_unit) {
    int64_t size_in_bytes = size * StorageFactor[StorageType::Byte];
    int64_t target_factor = StorageFactor[target_unit];
    value = roundToDigits(double(size_in_bytes)/double(target_factor), size_t(log10(double(target_factor))));
  }

  return value;

}

} // namespace Units
} // namespace Elements

