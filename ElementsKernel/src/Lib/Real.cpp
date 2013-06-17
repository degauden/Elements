/*
 * Real.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: hubert
 */


#include <cassert>
#include <cmath>
#include "ElementsKernel/Real.h"


using namespace std;

// Usable AlmostEqual function
bool almostEqual2sComplement(float A, float B, int maxUlps=4)
{
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);

    int aInt = *(int*)&A;
    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
        aInt = 0x80000000 - aInt;
    // Make bInt lexicographically ordered as a twos-complement int
    int bInt = *(int*)&B;
    if (bInt < 0)
        bInt = 0x80000000 - bInt;
    int intDiff = abs(aInt - bInt);
    if (intDiff <= maxUlps)
        return true;
    return false;
}


bool almostEqual2sComplement(double A, double B, int maxUlps=10)
{
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);

    long long aInt = *(long long*)&A;
    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
        aInt = 0x8000000000000000 - aInt;

    // Make bInt lexicographically ordered as a twos-complement int
    long long bInt = *(long long*)&B;
    if (bInt < 0)
        bInt = 0x8000000000000000 - bInt;

    long long intDiff = abs(aInt - bInt);
    if (intDiff <= maxUlps && -maxUlps <= intDiff)
    //if (intDiff <= maxUlps)
        return true;
    return false;
}
