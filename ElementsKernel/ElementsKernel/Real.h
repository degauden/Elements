/**
 *  @file Real.h
 *  @brief Floating point comparison implementations
 *  @author Hubert Degaudenzi
 *  @date Jun 13, 2013
 *  @details Due to the finite representation of the real numbers in the computing
 *    architecture, the comparison between 2 floating point numbers needs to be
 *    done carefully. In details, even if the representation bit-wise of 2
 *    numbers is different, the real numbers they represent might be the same.
 *
 *    In essence, this is equivalent to compare the 2 numbers \f$x\f$ and \f$y\f$ with a
 *    relative tolerance number \f$\epsilon\f$:
 *    \f[
 *    |x-y| \leq \epsilon |x+y|
 *    \f]
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

// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Authors: wan@google.com (Zhanyong Wan), eefacm@gmail.com (Sean Mcafee)
//
// The Google C++ Testing Framework (Google Test)
#ifndef REAL_H_
#define REAL_H_

//#include <cstddef>
#include <limits>                  // for numeric_limits
#include <type_traits>             // for is_floating_point
#include <cmath>                   // for round

#include "ElementsKernel/Export.h" // ELEMENTS_API


using std::numeric_limits;


namespace Elements {

/// Single precision float default maximum unit in the last place
constexpr std::size_t FLT_DEFAULT_MAX_ULPS { 4 };
/// Double precision float default maximum unit in the last place
constexpr std::size_t DBL_DEFAULT_MAX_ULPS { 10 };

// For testing purposes only. Rather use the isEqual functions for real
// life comparison
/// Single precision float default test tolerance
static const double FLT_DEFAULT_TEST_TOLERANCE = std::pow(10.0, -std::numeric_limits<float>::digits10);
/// Double precision float default test tolerance
static const double DBL_DEFAULT_TEST_TOLERANCE = std::pow(10.0, -std::numeric_limits<double>::digits10);


template<std::size_t size>
class TypeWithSize {
public:
  // This prevents the user from using TypeWithSize<N> with incorrect
  // values of N.
  using UInt = void;
};

// The specialization for size 4.
template<>
class TypeWithSize<4> {
public:
  // unsigned int has size 4 in both gcc and MSVC.
  //
  // As base/basictypes.h doesn't compile on Windows, we cannot use
  // uint32, uint64, and etc here.
  using Int = int;
  using UInt = unsigned int;
};

// The specialization for size 8.
template<>
class TypeWithSize<8> {
public:
  using Int = long long; // NOLINT
  using UInt = unsigned long long; // NOLINT
};

template <typename RawType>
constexpr std::size_t defaultMaxUlps()
{
  return FLT_DEFAULT_MAX_ULPS;
}

template <>
constexpr std::size_t defaultMaxUlps<float>()
{
  return FLT_DEFAULT_MAX_ULPS;
}

template <>
constexpr std::size_t defaultMaxUlps<double>()
{
  return DBL_DEFAULT_MAX_ULPS;
}


// This template class represents an IEEE floating-point number
// (either single-precision or double-precision, depending on the
// template parameters).
//
// The purpose of this class is to do more sophisticated number
// comparison.  (Due to round-off error, etc, it's very unlikely that
// two floating-points will be equal exactly.  Hence a naive
// comparison by the == operation often doesn't work.)
//
// Format of IEEE floating-point:
//
//   The most-significant bit being the leftmost, an IEEE
//   floating-point looks like
//
//     sign_bit exponent_bits fraction_bits
//
//   Here, sign_bit is a single bit that designates the sign of the
//   number.
//
//   For float, there are 8 exponent bits and 23 fraction bits.
//
//   For double, there are 11 exponent bits and 52 fraction bits.
//
//   More details can be found at
//   http://en.wikipedia.org/wiki/IEEE_floating-point_standard.
//
// Template parameter:
//
//   RawType: the raw floating-point type (either float or double)
template<typename RawType>
class FloatingPoint {
public:
  // Defines the unsigned integer type that has the same size as the
  // floating point number.
  using Bits = typename TypeWithSize<sizeof(RawType)>::UInt;

  // Constants.

  // # of bits in a number.
  static const std::size_t s_bitcount = 8 * sizeof(RawType);

  // # of fraction bits in a number.
  static const std::size_t s_fraction_bitcount = std::numeric_limits<RawType>::digits - 1;

  // # of exponent bits in a number.
  static const std::size_t s_exponent_bitcount = s_bitcount - 1 - s_fraction_bitcount;

  // The mask for the sign bit.
  static const Bits s_sign_bitmask = static_cast<Bits>(1) << (s_bitcount - 1);

  // The mask for the fraction bits.
  static const Bits s_fraction_bitmask = ~static_cast<Bits>(0) >> (s_exponent_bitcount + 1);

  // The mask for the exponent bits.
  static const Bits s_exponent_bitmask = ~(s_sign_bitmask | s_fraction_bitmask);

  // How many ULP's (Units in the Last Place) we want to tolerate when
  // comparing two numbers.  The larger the value, the more error we
  // allow.  A 0 value means that two numbers must be exactly the same
  // to be considered equal.
  //
  // The maximum error of a single floating-point operation is 0.5
  // units in the last place.  On Intel CPU's, all floating-point
  // calculations are done with 80-bit precision, while double has 64
  // bits.  Therefore, 4 should be enough for ordinary use.
  //
  // See the following article for more details on ULP:
  // http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm.
  static const std::size_t m_max_ulps = defaultMaxUlps<RawType>();

  // Constructs a FloatingPoint from a raw floating-point number.
  //
  // On an Intel CPU, passing a non-normalized NAN (Not a Number)
  // around may change its bits, although the new value is guaranteed
  // to be also a NAN.  Therefore, don't expect this constructor to
  // preserve the bits in x when x is a NAN.
  explicit FloatingPoint(const RawType& x) {
    m_u.m_value = x;
  }

  // Static methods

  // Reinterprets a bit pattern as a floating-point number.
  //
  // This function is needed to test the AlmostEquals() method.
  static RawType ReinterpretBits(const Bits &bits) {
    FloatingPoint fp(0);
    fp.m_u.m_bits = bits;
    return fp.m_u.m_value;
  }

  // Returns the floating-point number that represent positive infinity.
  static RawType Infinity() {
    return ReinterpretBits(s_exponent_bitmask);
  }

  // Non-static methods

  // Returns the bits that represents this number.
  const Bits &bits() const {
    return m_u.m_bits;
  }

  // Returns the exponent bits of this number.
  Bits exponentBits() const {
    return s_exponent_bitmask & m_u.m_bits;
  }

  // Returns the fraction bits of this number.
  Bits fractionBits() const {
    return s_fraction_bitmask & m_u.m_bits;
  }

  // Returns the sign bit of this number.
  Bits signBit() const {
    return s_sign_bitmask & m_u.m_bits;
  }

  // Returns true iff this is NAN (not a number).
  bool isNan() const {
    // It's a NAN if the exponent bits are all ones and the fraction
    // bits are not entirely zeros.
    return (exponentBits() == s_exponent_bitmask) && (fractionBits() != 0);
  }

  // Returns true iff this number is at most kMaxUlps ULP's away from
  // rhs.  In particular, this function:
  //
  //   - returns false if either number is (or both are) NAN.
  //   - treats really large numbers as almost equal to infinity.
  //   - thinks +0.0 and -0.0 are 0 DLP's apart.
  bool AlmostEquals(const FloatingPoint& rhs) const {
    // The IEEE standard says that any comparison operation involving
    // a NAN must return false.
    if (isNan() || rhs.isNan()) {
      return false;
    }
    return distanceBetweenSignAndMagnitudeNumbers(m_u.m_bits, rhs.m_u.m_bits) <= m_max_ulps;
  }

  // Converts an integer from the sign-and-magnitude representation to
  // the biased representation.  More precisely, let N be 2 to the
  // power of (kBitCount - 1), an integer x is represented by the
  // unsigned number x + N.
  //
  // For instance,
  //
  //   -N + 1 (the most negative number representable using
  //          sign-and-magnitude) is represented by 1;
  //   0      is represented by N; and
  //   N - 1  (the biggest number representable using
  //          sign-and-magnitude) is represented by 2N - 1.
  //
  // Read http://en.wikipedia.org/wiki/Signed_number_representations
  // for more details on signed number representations.
  static Bits signAndMagnitudeToBiased(const Bits &sam) {
    if (s_sign_bitmask & sam) {
      // sam represents a negative number.
      return ~sam + 1;
    } else {
      // sam represents a positive number.
      return s_sign_bitmask | sam;
    }
  }

  // Given two numbers in the sign-and-magnitude representation,
  // returns the distance between them as an unsigned number.
  static Bits distanceBetweenSignAndMagnitudeNumbers(const Bits &sam1, const Bits &sam2) {
    const Bits biased1 = signAndMagnitudeToBiased(sam1);
    const Bits biased2 = signAndMagnitudeToBiased(sam2);
    return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
  }

private:
  // The data type used to store the actual floating-point number.
  union FloatingPointUnion {
    RawType m_value;  // The raw floating-point number.
    Bits m_bits;      // The bits that represent the number.
  };

  FloatingPointUnion m_u;
};


// Usable AlmostEqual function

template <typename FloatType>
bool almostEqual2sComplement(const FloatType& a, const FloatType& b, const std::size_t& max_ulps=0)
{
  return false;
}


template <typename RawType>
bool isNan(const RawType& x)
{
  using Bits = typename TypeWithSize<sizeof(RawType)>::UInt;
  Bits x_bits = *reinterpret_cast<const Bits *>(&x);

  Bits x_exp_bits = FloatingPoint<RawType>::s_exponent_bitmask & x_bits;
  Bits x_frac_bits = FloatingPoint<RawType>::s_fraction_bitmask & x_bits;

  return (x_exp_bits == FloatingPoint<RawType>::s_exponent_bitmask) && (x_frac_bits != 0);

}

template <typename RawType, std::size_t max_ulps=defaultMaxUlps<RawType>()>
bool isEqual(const RawType& left, const RawType& right)
{
  bool is_equal{false};

  if ( not (isNan<RawType>(left) || isNan<RawType>(right)) ) {
    using Bits = typename TypeWithSize<sizeof(RawType)>::UInt;
    Bits l_bits = *reinterpret_cast<const Bits *>(&left);
    Bits r_bits = *reinterpret_cast<const Bits *>(&right);
    is_equal = (FloatingPoint<RawType>::distanceBetweenSignAndMagnitudeNumbers(l_bits, r_bits) <= max_ulps);
  }

  return is_equal;
}

template <std::size_t max_ulps>
inline bool isEqual(const float& left, const float& right)
{
  return (isEqual<float, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isEqual(const double& left, const double& right)
{
  return (isEqual<double, max_ulps>(left, right));
}


template <typename RawType, std::size_t max_ulps=defaultMaxUlps<RawType>()>
inline bool isNotEqual(const RawType& left, const RawType& right)
{
  return (not isEqual<RawType, max_ulps>(left, right) );
}

template <std::size_t max_ulps>
inline bool isNotEqual(const float& left, const float& right)
{
  return (isNotEqual<float, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isNotEqual(const double& left, const double& right)
{
  return (isNotEqual<double, max_ulps>(left, right));
}


template <typename RawType, std::size_t max_ulps=defaultMaxUlps<RawType>()>
bool isLess(const RawType& left, const RawType& right)
{
  bool is_less{false};

  if ( left < right && (not isEqual<RawType, max_ulps>(left, right)) ) {
    is_less = true;
  }

  return is_less;
}

template<std::size_t max_ulps>
inline bool isLess(const float& left, const float& right)
{
  return (isLess<float, max_ulps>(left, right));
}

template<std::size_t max_ulps>
inline bool isLess(const double& left, const double& right)
{
  return (isLess<double, max_ulps>(left, right));
}

template <typename RawType, std::size_t max_ulps=defaultMaxUlps<RawType>()>
bool isGreater(const RawType& left, const RawType& right)
{
  bool is_greater{false};

  if ( left > right && (not isEqual<RawType, max_ulps>(left, right)) ) {
    is_greater = true;
  }

  return is_greater;
}

template<std::size_t max_ulps>
inline bool isGreater(const float& left, const float& right)
{
  return (isGreater<float, max_ulps>(left, right));
}

template<std::size_t max_ulps>
inline bool isGreater(const double& left, const double& right)
{
  return (isGreater<double, max_ulps>(left, right));
}

template <typename RawType, std::size_t max_ulps=defaultMaxUlps<RawType>()>
bool isLessOrEqual(const RawType& left, const RawType& right)
{
  bool is_loe{false};

  if (not isGreater<RawType, max_ulps>(left, right))  {
    is_loe = true;
  }

  return is_loe;
}

template<std::size_t max_ulps>
inline bool isLessOrEqual(const float& left, const float& right)
{
  return (isLessOrEqual<float, max_ulps>(left, right));
}

template<std::size_t max_ulps>
inline bool isLessOrEqual(const double& left, const double& right)
{
  return (isLessOrEqual<double, max_ulps>(left, right));
}

template <typename RawType, std::size_t max_ulps=defaultMaxUlps<RawType>()>
bool isGreaterOrEqual(const RawType& left, const RawType& right)
{
  bool is_goe{false};

  if (not isLess<RawType, max_ulps>(left, right))  {
    is_goe = true;
  }

  return is_goe;
}

template<std::size_t max_ulps>
inline bool isGreaterOrEqual(const float& left, const float& right)
{
  return (isGreaterOrEqual<float, max_ulps>(left, right));
}

template<std::size_t max_ulps>
inline bool isGreaterOrEqual(const double& left, const double& right)
{
  return (isGreaterOrEqual<double, max_ulps>(left, right));
}

/**
 * @brief
 *   This function compare 2 floats with a relative tolerance
 * @details
 *   The comparison is performed by casting the floating point numbers into integers and then compare
 *   their representation with a tolerance for their last bits.
 * @param left
 *   first float number
 * @param right
 *   second float number
 * @param max_ulps
 *   The relative tolerance is expressed as ULPS (units in the last place). They are unit in the last
 *   place of the mantissa. And the recommended default value is 4 for single precision numbers.
 * @return
 *   true if the numbers are equal (or cannot be distinguished) and false otherwise.
 */
ELEMENTS_API bool almostEqual2sComplement(const float& left, const float& right, const int& max_ulps=FLT_DEFAULT_MAX_ULPS);
/**
 * @brief
 *   This function compare 2 doubles with a relative tolerance
 * @details
 *   The comparison is performed by casting the floating point numbers into integers and then compare
 *   their representation with a tolerance for their last bits.
 * @param left
 *   first double number
 * @param right
 *   second double number
 * @param max_ulps
 *   The relative tolerance is expressed as ULPS (units in the last place). They are unit in the last
 *   place of the mantissa. And the recommended default value is 10 for double precision numbers.
 * @return
 *   true if the numbers are equal (or cannot be distinguished) and false otherwise.
 */
ELEMENTS_API bool almostEqual2sComplement(const double& left, const double& right, const int& max_ulps=DBL_DEFAULT_MAX_ULPS);

/**
 * @brief
 *   This function compares 2 floating point numbers bitwise. These are the strict
 *   equivalent of the "==". They are only here for the example.
 * @param left
 *   right hand number to compare
 * @param right
 *   left hand number to compare
 * @tparam RawType
 *   raw type: ie float or double
 * @return
 *   true if the 2 numbers are bitwise equal
 */
template<typename RawType>
ELEMENTS_API bool realBitWiseEqual(const RawType& left, const RawType& right) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
  return (left == right);
#pragma GCC diagnostic pop
}

} // Elements namespace


#endif /* REAL_H_ */
