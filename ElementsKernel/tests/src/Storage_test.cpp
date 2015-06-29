/**
 * @file Storage_test.cpp
 *
 * @date Dec 22, 2014
 * @author hubert
 */


#include "ElementsKernel/Storage.h"

#include <boost/test/unit_test.hpp>
#include <typeinfo>


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
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::KiloByte],        "KB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MegaByte],        "MB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::GigaByte],        "GB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::TeraByte],        "TB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::PetaByte],        "PB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricKiloByte], "KiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricMegaByte], "MiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricGigaByte], "GiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricTeraByte], "TiB");
  BOOST_CHECK_EQUAL(StorageShortName[StorageType::MetricPetaByte], "PiB");

}

//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END()

