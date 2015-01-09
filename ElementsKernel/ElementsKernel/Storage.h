/**
 * @file Storage.h
 *
 * @date Dec 22, 2014
 * @author hubert
 */

#ifndef ELEMENTSKERNEL_STORAGE_H_
#define ELEMENTSKERNEL_STORAGE_H_

#include <map>
#include <cstdint>

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
} ;

  ELEMENTS_API extern std::map<StorageType, std::string> StorageShortName ;
  ELEMENTS_API extern std::map<StorageType, std::int64_t> StorageFactor;

  ELEMENTS_API double storageConvert(const std::int64_t& size,
                                           StorageType source_unit,
                                           StorageType target_unit=StorageType::Byte);

  ELEMENTS_API double roundToDigits(const double& value, const std::size_t& max_digits);


}
} // end of namespace Elements




#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_STORAGE_H_
