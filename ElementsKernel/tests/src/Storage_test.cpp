/**
 * @file Storage_test.cpp
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
 *
 */


#include "ElementsKernel/Storage.h"

#include <boost/test/unit_test.hpp>

#include "ElementsKernel/MathConstants.h"     // For pi
#include "ElementsKernel/Real.h"              // For isEqual


//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(Storage_test)


BOOST_AUTO_TEST_CASE(StorageFactorValue_test) {

  using Elements::Units::StorageType;
  using Elements::Units::StorageFactor;


  BOOST_CHECK_EQUAL(StorageFactor[StorageType::Byte],                          1);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::KiloByte],                   1024);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::MegaByte],                1048576);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::GigaByte],             1073741824);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::TeraByte],          1099511627776);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::PetaByte],       1125899906842624);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::MetricKiloByte],             1000);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::MetricMegaByte],          1000000);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::MetricGigaByte],       1000000000);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::MetricTeraByte],    1000000000000);
  BOOST_CHECK_EQUAL(StorageFactor[StorageType::MetricPetaByte], 1000000000000000);


}

BOOST_AUTO_TEST_CASE(StorageShortName_test) {

  using Elements::Units::StorageType;
  using Elements::Units::StorageShortName;

  BOOST_CHECK_EQUAL(StorageShortName[StorageType::Byte],             "B");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::KiloByte],        "KiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MegaByte],        "MiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::GigaByte],        "GiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::TeraByte],        "TiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::PetaByte],        "PiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricKiloByte],  "KB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricMegaByte],  "MB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricGigaByte],  "GB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricTeraByte],  "TB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricPetaByte],  "PB");

}

BOOST_AUTO_TEST_CASE(RoundToDigits_test) {

  using Elements::Units::roundToDigits;
  using Elements::Units::pi;
  using Elements::isEqual;

  BOOST_CHECK(isEqual(roundToDigits(pi, 12), 3.14159265359));
  BOOST_CHECK(isEqual(roundToDigits(pi,  0),            3.));
  BOOST_CHECK(isEqual(roundToDigits(pi,  4),        3.1416));

  BOOST_CHECK(isEqual(roundToDigits(-pi, 4),       -3.1416));

}

BOOST_AUTO_TEST_CASE(StorageConvertLower_test) {

  using Elements::Units::StorageType;
  using Elements::isEqual;
  using Elements::Units::storageConvert;

  std::int64_t size_in_byte { 1000 };

  BOOST_CHECK(isEqual(storageConvert(static_cast<double>(size_in_byte),
                                     StorageType::Byte, StorageType::KiloByte),
                      0.977));
  BOOST_CHECK(isEqual(storageConvert<9>(static_cast<double>(size_in_byte),
                                        StorageType::Byte, StorageType::KiloByte),
                      0.9765625));

  BOOST_CHECK(isEqual(storageConvert(static_cast<double>(size_in_byte),
                                     StorageType::Byte, StorageType::MegaByte),
                      0.000954));
  BOOST_CHECK(isEqual(storageConvert<9>(static_cast<double>(size_in_byte),
                                        StorageType::Byte, StorageType::MegaByte),
                      0.000953674));

}


BOOST_AUTO_TEST_CASE(StorageConverterRounding_test) {

  using Elements::Units::StorageType;
  using Elements::isEqual;
  using Elements::Units::storageConvert;

  std::int64_t size_in_byte { 1000 };

  BOOST_CHECK_EQUAL(storageConvert(size_in_byte,
                             StorageType::Byte, StorageType::KiloByte),
                   1);
  BOOST_CHECK_EQUAL(storageConvert<9>(size_in_byte,
                                StorageType::Byte, StorageType::KiloByte),
                    1);

  BOOST_CHECK_EQUAL(storageConvert(size_in_byte,
                             StorageType::Byte, StorageType::MegaByte),
                    0);
  BOOST_CHECK_EQUAL(storageConvert<9>(size_in_byte,
                                StorageType::Byte, StorageType::MegaByte),
                    0);

}


BOOST_AUTO_TEST_CASE(StorageConvertUpper_test) {

  using Elements::Units::StorageType;
  using Elements::isEqual;
  using Elements::Units::storageConvert;

  std::int64_t size { 1 };


  BOOST_CHECK(isEqual(storageConvert(static_cast<double>(size),
                                     StorageType::KiloByte, StorageType::Byte),
                      1024.0));
  BOOST_CHECK(isEqual(storageConvert<9>(static_cast<double>(size), StorageType::KiloByte, StorageType::Byte), 1024.0));

  BOOST_CHECK_EQUAL(storageConvert(size, StorageType::KiloByte, StorageType::Byte), 1024);
  BOOST_CHECK_EQUAL(storageConvert<9>(size, StorageType::KiloByte, StorageType::Byte), 1024);

  BOOST_CHECK(isEqual(storageConvert(static_cast<double>(size), StorageType::MegaByte, StorageType::Byte), 1048576.0));
  BOOST_CHECK(isEqual(storageConvert<9>(static_cast<double>(size), StorageType::MegaByte, StorageType::Byte), 1048576.0));

  BOOST_CHECK_EQUAL(storageConvert(size, StorageType::MegaByte, StorageType::Byte), 1048576);
  BOOST_CHECK_EQUAL(storageConvert<9>(size, StorageType::MegaByte, StorageType::Byte), 1048576);

  BOOST_CHECK_EQUAL(storageConvert(size, StorageType::MegaByte, StorageType::KiloByte), 1024);
  BOOST_CHECK_EQUAL(storageConvert<9>(size, StorageType::MegaByte, StorageType::KiloByte), 1024);

}

BOOST_AUTO_TEST_CASE(StorageConvertMetric_test) {

  using Elements::Units::StorageType;
  using Elements::isEqual;
  using Elements::Units::storageConvert;

  std::int64_t size { 1 };


  BOOST_CHECK(isEqual(storageConvert(static_cast<double>(size), StorageType::MetricKiloByte, StorageType::Byte), 1000.0));
  BOOST_CHECK(isEqual(storageConvert<9>(static_cast<double>(size), StorageType::MetricKiloByte, StorageType::Byte), 1000.0));

  BOOST_CHECK_EQUAL(storageConvert(size, StorageType::MetricKiloByte, StorageType::Byte), 1000);
  BOOST_CHECK_EQUAL(storageConvert<9>(size, StorageType::MetricKiloByte, StorageType::Byte), 1000);

  BOOST_CHECK(isEqual(storageConvert(static_cast<double>(size), StorageType::MetricMegaByte, StorageType::Byte), 1000000.0));
  BOOST_CHECK(isEqual(storageConvert<9>(static_cast<double>(size), StorageType::MetricMegaByte, StorageType::Byte), 1000000.0));

  BOOST_CHECK_EQUAL(storageConvert(size, StorageType::MetricMegaByte, StorageType::Byte), 1000000);
  BOOST_CHECK_EQUAL(storageConvert<9>(size, StorageType::MetricMegaByte, StorageType::Byte), 1000000);

  BOOST_CHECK_EQUAL(storageConvert(size, StorageType::MetricMegaByte, StorageType::MetricKiloByte), 1000);
  BOOST_CHECK_EQUAL(storageConvert<9>(size, StorageType::MetricMegaByte, StorageType::MetricKiloByte), 1000);

}

//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END()

