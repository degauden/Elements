/**
 * @file Factory_test.cpp
 *
 *  Created on: May 22, 2013
 *      Author: Pierre Dubath
 */


#include <boost/test/unit_test.hpp>
#include "ElementsKernel/Real.h"



using namespace std;
using namespace Real;

//-----------------------------------------------------------------------------

struct RealFix {


  ~RealFix() {
  }

};

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (Real_test)

BOOST_FIXTURE_TEST_CASE( FloatCompare_test, RealFix ) {

	float f = 1.0F/2.0F ;

	BOOST_CHECK_EQUAL(f, 0.5F);

	BOOST_CHECK( ! almostEqual2sComplement(f, 0.5100000F));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.5010000F));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.5001000F));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.5000100F));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.5000010F));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.5000003F));

	BOOST_CHECK(   almostEqual2sComplement(f, 0.5000002F));
	BOOST_CHECK(   almostEqual2sComplement(f, 0.5000001F));
	BOOST_CHECK(   almostEqual2sComplement(f, 0.5F));


} // Eof FloatCompare_test

BOOST_FIXTURE_TEST_CASE( DoubleCompare_test, RealFix ) {

	double f = 1.0/2.0 ;

	BOOST_CHECK_EQUAL(f, 0.5);

	BOOST_CHECK( ! almostEqual2sComplement(f, 0.510000000000000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.501000000000000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500100000000000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500010000000000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500001000000000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000100000000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000010000000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000001000000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000100000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000010000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000001000));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000000100));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000000010));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000000005));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000000004));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000000003));
	BOOST_CHECK( ! almostEqual2sComplement(f, 0.500000000000002));

	BOOST_CHECK(   almostEqual2sComplement(f, 0.500000000000001));
	BOOST_CHECK(   almostEqual2sComplement(f, 0.5));


} // Eof DoubleCompare_test

//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END ()
