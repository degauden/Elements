/*
 * Real.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: hubert
 */


#include <cassert>
#include <cstdlib>
#include "ElementsKernel/Real.h"


using namespace std;


// Usable AlmostEqual function
bool almostEqual2sComplement(const float& a, const float& b, const int& max_ulps)
{
//    int a_int = *(int*)&a;
    // cppcheck-suppress invalidPointerCast
    int a_int = *reinterpret_cast<const int *>(&a);
    // Make a_int lexicographically ordered as a twos-complement int
    if (a_int < 0)
        a_int = 0x80000000 - a_int;
    // Make b_int lexicographically ordered as a twos-complement int
    //    int b_int = *(int*)&b;
    // cppcheck-suppress invalidPointerCast
    int b_int = *reinterpret_cast<const int *>(&b);
    if (b_int < 0)
        b_int = 0x80000000 - b_int;
    int int_diff = abs(a_int - b_int);
    if (int_diff <= max_ulps)
        return true;
    return false;
}


bool almostEqual2sComplement(const double& a, const double& b, const int& max_ulps)
{
//    long long a_int = *(long long*)&a;
    // cppcheck-suppress invalidPointerCast
    long long a_int = *reinterpret_cast<const long long *>(&a);
    // Make a_int lexicographically ordered as a twos-complement int
    if (a_int < 0)
        a_int = 0x8000000000000000 - a_int;

    // Make b_int lexicographically ordered as a twos-complement int
//    long long b_int = *(long long*)&b;
    // cppcheck-suppress invalidPointerCast
    long long b_int = *reinterpret_cast<const long long*>(&b);
    if (b_int < 0)
        b_int = 0x8000000000000000 - b_int;

    long long int_diff = abs(a_int - b_int);
    if (int_diff <= max_ulps && -max_ulps <= int_diff)
    //if (int_diff <= maxUlps)
        return true;
    return false;
}

