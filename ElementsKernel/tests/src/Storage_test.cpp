/**
 * @file Storage_test.cpp
 *
 * @date Dec 22, 2014
 * @author hubert
 */


#include "ElementsKernel/Storage.h"

#include <boost/test/unit_test.hpp>
//#include <typeinfo>

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


//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END()

