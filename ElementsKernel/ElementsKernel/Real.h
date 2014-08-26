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

#include <cstddef>
#include <limits>

#include "ElementsKernel/Export.h" // ELEMENTS_API

namespace Elements {

static const size_t FLT_DEFAULT_MAX_ULPS { 4 };
static const size_t DBL_DEFAULT_MAX_ULPS { 10 };


template<size_t size>
class TypeWithSize {
public:
  // This prevents the user from using TypeWithSize<N> with incorrect
  // values of N.
  typedef void UInt;
};

// The specialization for size 4.
template<>
class TypeWithSize<4> {
public:
  // unsigned int has size 4 in both gcc and MSVC.
  //
  // As base/basictypes.h doesn't compile on Windows, we cannot use
  // uint32, uint64, and etc here.
  typedef int Int;
  typedef unsigned int UInt;
};

// The specialization for size 8.
template<>
class TypeWithSize<8> {
public:
  typedef long long Int;  // NOLINT
  typedef unsigned long long UInt;  // NOLINT
};

template <typename RawType>
constexpr size_t defaultMaxUlps()
{
  return FLT_DEFAULT_MAX_ULPS ;
}

template <>
constexpr size_t defaultMaxUlps<float>()
{
  return FLT_DEFAULT_MAX_ULPS ;
}

template <>
constexpr size_t defaultMaxUlps<double>()
{
  return DBL_DEFAULT_MAX_ULPS ;
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
  typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;

  // Constants.

  // # of bits in a number.
  static const size_t s_bitcount = 8 * sizeof(RawType);

  // # of fraction bits in a number.
  static const size_t s_fraction_bitcount = std::numeric_limits<RawType>::digits - 1;

  // # of exponent bits in a number.
  static const size_t s_exponent_bitcount = s_bitcount - 1 - s_fraction_bitcount;

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
  static const size_t m_max_ulps = defaultMaxUlps<RawType>();

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
    if (isNan() || rhs.isNan())
      return false;

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
bool almostEqual2sComplement(const FloatType& a, const FloatType& b, const size_t& max_ulps=0)
{
  return false ;
}


/**
 * @brief
 *   This function compare 2 floats with a relative tolerance
 * @details
 *   The comparison is performed by casting the floating point numbers into integers and then compare
 *   their representation with a tolerance for their last bits.
 * @param a
 *   first float number
 * @param b
 *   second float number
 * @param max_ulps
 *   The relative tolerance is expressed as ULPS (units in the last place). They are unit in the last
 *   place of the mantissa. And the recommended default value is 4 for single precision numbers.
 * @return
 *   true if the numbers are equal (or cannot be distinguished) and false otherwise.
 */
ELEMENTS_API bool almostEqual2sComplement(const float& a, const float& b, const int& max_ulps=FLT_DEFAULT_MAX_ULPS);
/**
 * @brief
 *   This function compare 2 doubles with a relative tolerance
 * @details
 *   The comparison is performed by casting the floating point numbers into integers and then compare
 *   their representation with a tolerance for their last bits.
 * @param a
 *   first double number
 * @param
 *   b second double number
 * @param max_ulps
 *   The relative tolerance is expressed as ULPS (units in the last place). They are unit in the last
 *   place of the mantissa. And the recommended default value is 10 for double precision numbers.
 * @return
 *   true if the numbers are equal (or cannot be distinguished) and false otherwise.
 */
ELEMENTS_API bool almostEqual2sComplement(const double& a, const double& b, const int& max_ulps=DBL_DEFAULT_MAX_ULPS);


template <typename RawType>
bool isNan(const RawType& x)
{
  typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;
  Bits x_bits = *reinterpret_cast<const Bits *>(&x);

  Bits x_exp_bits = FloatingPoint<RawType>::s_exponent_bitmask & x_bits ;
  Bits x_frac_bits = FloatingPoint<RawType>::s_fraction_bitmask & x_bits ;

  return (x_exp_bits == FloatingPoint<RawType>::s_exponent_bitmask) && (x_frac_bits != 0) ;

}

template <typename RawType, size_t max_ulps=defaultMaxUlps<RawType>()>
bool isEqual(const RawType& l, const RawType& r)
{
  bool is_equal{false} ;

  if ( ! (isNan<RawType>(l) || isNan<RawType>(r)) ) {
    typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;
    Bits l_bits = *reinterpret_cast<const Bits *>(&l);
    Bits r_bits = *reinterpret_cast<const Bits *>(&r);
    is_equal = (FloatingPoint<RawType>::distanceBetweenSignAndMagnitudeNumbers(l_bits, r_bits) <= max_ulps);
  }

  return is_equal ;
}

template <size_t max_ulps>
inline bool isEqual(const float& l, const float& r)
{
  return (isEqual<float,max_ulps>(l,r)) ;
}

template <size_t max_ulps>
inline bool isEqual(const double& l, const double& r)
{
  return (isEqual<double,max_ulps>(l,r)) ;
}


template <typename RawType, size_t max_ulps=defaultMaxUlps<RawType>()>
inline bool isNotEqual(const RawType& l, const RawType& r)
{
  return (! isEqual<RawType,max_ulps>(l,r) ) ;
}

template <size_t max_ulps>
inline bool isNotEqual(const float& l, const float& r)
{
  return (isNotEqual<float,max_ulps>(l,r)) ;
}

template <size_t max_ulps>
inline bool isNotEqual(const double& l, const double& r)
{
  return (isNotEqual<double,max_ulps>(l,r)) ;
}





template <typename RawType, size_t max_ulps=defaultMaxUlps<RawType>()>
bool isLess(const RawType& l, const RawType& r)
{
  bool is_less{false} ;

  if ( l < r && (! isEqual<RawType,max_ulps>(l,r)) ) {
    is_less = true ;
  }

  return is_less ;
}

template<size_t max_ulps>
inline bool isLess(const float& l, const float& r)
{
  return (isLess<float,max_ulps>(l,r)) ;
}

template<size_t max_ulps>
inline bool isLess(const double& l, const double& r)
{
  return (isLess<double,max_ulps>(l,r)) ;
}



template <typename RawType, size_t max_ulps=defaultMaxUlps<RawType>()>
bool isGreater(const RawType& l, const RawType& r)
{
  bool is_greater{false} ;

  if ( l > r && (! isEqual<RawType,max_ulps>(l,r)) ) {
    is_greater = true ;
  }

  return is_greater ;
}

template<size_t max_ulps>
inline bool isGreater(const float& l, const float& r)
{
  return (isGreater<float,max_ulps>(l,r)) ;
}

template<size_t max_ulps>
inline bool isGreater(const double& l, const double& r)
{
  return (isGreater<double,max_ulps>(l,r)) ;
}



template <typename RawType, size_t max_ulps=defaultMaxUlps<RawType>()>
bool isLessOrEqual(const RawType& l, const RawType& r)
{
  bool is_loe{false} ;

  if (! isGreater<RawType,max_ulps>(l,r))  {
    is_loe = true ;
  }

  return is_loe ;
}

template<size_t max_ulps>
inline bool isLessOrEqual(const float& l, const float& r)
{
  return (isLessOrEqual<float,max_ulps>(l,r)) ;
}

template<size_t max_ulps>
inline bool isLessOrEqual(const double& l, const double& r)
{
  return (isLessOrEqual<double,max_ulps>(l,r)) ;
}




template <typename RawType, size_t max_ulps=defaultMaxUlps<RawType>()>
bool isGreaterOrEqual(const RawType& l, const RawType& r)
{
  bool is_goe{false} ;

  if (! isLess<RawType,max_ulps>(l,r))  {
    is_goe = true ;
  }

  return is_goe ;
}

template<size_t max_ulps>
inline bool isGreaterOrEqual(const float& l, const float& r)
{
  return (isGreaterOrEqual<float,max_ulps>(l,r)) ;
}

template<size_t max_ulps>
inline bool isGreaterOrEqual(const double& l, const double& r)
{
  return (isGreaterOrEqual<double,max_ulps>(l,r)) ;
}

} // Elements namespace

#endif /* REAL_H_ */
