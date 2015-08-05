/**
 * @file MathConstants.h
 * @date Dec 18, 2013
 * @brief A few math constants
 * @author Hubert Degaudenzi - The Euclid Consortium
 */

#ifndef ELEMENTSKERNEL_MATHCONSTANTS_H_
#define ELEMENTSKERNEL_MATHCONSTANTS_H_

namespace Elements {
namespace Units {

/// @brief \f$\pi\f$
constexpr double     pi         = 3.14159265358979323846;
/// @brief \f$2\pi\f$
constexpr double  twopi         = 2.0*pi;
/// @brief \f$\pi/2\f$
constexpr double halfpi         = pi/2.0;
/// @brief \f$\pi^2\f$
constexpr double     pi2        = pi*pi;
/// @brief \f$\sqrt{\pi}\f$
constexpr double sqrt_of_pi     = 1.77245385090551602730;
/// @brief \f$\sqrt{2}\f$
constexpr double sqrt_of_two    = 1.41421356237309504880;
/// @brief \f$\sqrt{\frac{\pi}{2}}\f$
constexpr double sqrt_of_halfpi = 1.25331413731550025121;

/// @brief The Euler-Mascheroni \f$\gamma\f$ constant
constexpr double gamma          = 0.57721566490153286061;
/// @brief The base of the natural logarithm \f$e\f$
constexpr double e              = 2.71828182845904523536;

}  // namespace Units
}  // namespace Elements




#endif  // ELEMENTSKERNEL_MATHCONSTANTS_H_
