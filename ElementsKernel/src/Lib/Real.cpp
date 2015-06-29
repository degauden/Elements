/*
 * Real.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: hubert
 */


#include <cassert>
#include <cstdlib>
#include <cstdint>                  // for std::int64_t, std::int32_t
#include "ElementsKernel/Real.h"


// using namespace std;

namespace Elements {

// Usable AlmostEqual function
bool almostEqual2sComplement(const float& a, const float& b, const int& max_ulps) {

    using std::abs;
    using std::int32_t;

    // int a_int = *(int*)&a;
    // cppcheck-suppress invalidPointerCast
    int32_t a_int = *reinterpret_cast<const int *>(&a);
    // Make a_int lexicographically ordered as a twos-complement int
    if (a_int < 0)
        a_int = 0x80000000 - a_int;
    // Make b_int lexicographically ordered as a twos-complement int
    //    int b_int = *(int*)&b;
    // cppcheck-suppress invalidPointerCast
    int32_t b_int = *reinterpret_cast<const int32_t *>(&b);
    if (b_int < 0)
        b_int = 0x80000000 - b_int;
    int32_t int_diff = abs(a_int - b_int);
    if (int_diff <= max_ulps)
        return true;
    return false;
}


bool almostEqual2sComplement(const double& a, const double& b, const int& max_ulps) {

    using std::abs;
    using std::int64_t;

    // long long a_int = *(long long*)&a;
    // cppcheck-suppress invalidPointerCast

    int64_t a_int = *reinterpret_cast<const int64_t *>(&a);
    // Make a_int lexicographically ordered as a twos-complement int
    if (a_int < 0)
        a_int = 0x8000000000000000 - a_int;

    // Make b_int lexicographically ordered as a twos-complement int
    //    long long b_int = *(long long*)&b;
    // cppcheck-suppress invalidPointerCast
    int64_t b_int = *reinterpret_cast<const int64_t *>(&b);
    if (b_int < 0)
        b_int = 0x8000000000000000 - b_int;

    int64_t int_diff = abs(a_int - b_int);
    if (int_diff <= max_ulps && -max_ulps <= int_diff)
        return true;
    return false;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"

bool realBitwiseEqual(float x, float y) {
  return (x == y);
}

bool realBitwiseEqual(double x, double y) {
  return (x == y);
}

#pragma GCC diagnostic pop

}  // namespace Elements
