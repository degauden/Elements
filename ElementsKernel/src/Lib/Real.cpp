/*
 * @file Real.cpp
 *
 * @date Jun 13, 2013
 * @author Hubert Degaudenzi
 *
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


#include <cassert>
#include <cstdlib>
#include <cstdint>                  // for std::int64_t, std::int32_t
#include "ElementsKernel/Real.h"


namespace Elements {

extern const double FLT_DEFAULT_TEST_TOLERANCE = std::pow(10.0, -std::numeric_limits<float>::digits10);
extern const double DBL_DEFAULT_TEST_TOLERANCE = std::pow(10.0, -std::numeric_limits<double>::digits10);


// Usable AlmostEqual function
bool almostEqual2sComplement(const float& left, const float& right, const int& max_ulps) {

    using std::abs;
    using std::int32_t;

    // int a_int = *(int*)&a;
    int32_t a_int = *reinterpret_cast<const int32_t *>(&left);
    // Make a_int lexicographically ordered as a twos-complement int
    if (a_int < 0) {
        a_int = 0x80000000 - a_int;
    }
    // Make b_int lexicographically ordered as a twos-complement int
    //    int b_int = *(int*)&b;
    int32_t b_int = *reinterpret_cast<const int32_t *>(&right);
    if (b_int < 0) {
        b_int = 0x80000000 - b_int;
    }
    int32_t int_diff = abs(a_int - b_int);
    if (int_diff <= max_ulps && -max_ulps <= int_diff) {
        return true;
    }
    return false;
}


bool almostEqual2sComplement(const double& left, const double& right, const int& max_ulps) {

    using std::abs;
    using std::int64_t;

    // long long a_int = *(long long*)&a;

    int64_t a_int = *reinterpret_cast<const int64_t *>(&left);
    // Make a_int lexicographically ordered as a twos-complement int
    if (a_int < 0) {
        a_int = 0x8000000000000000LL - a_int;
    }
    // Make b_int lexicographically ordered as a twos-complement int
    //    long long b_int = *(long long*)&b;
    int64_t b_int = *reinterpret_cast<const int64_t *>(&right);
    if (b_int < 0) {
        b_int = 0x8000000000000000LL - b_int;
    }
    int64_t int_diff = abs(a_int - b_int);
    if (int_diff <= max_ulps && -max_ulps <= int_diff) {
        return true;
    }
    return false;
}


template bool realBitWiseEqual<float>(const float& left, const float& right);
template bool realBitWiseEqual<double>(const double& left, const double& right);


}  // namespace Elements
