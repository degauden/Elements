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
static const double     pi         = 3.14159265358979323846;
/// @brief \f$2\pi\f$
static const double  twopi         = 2.0*pi;
/// @brief \f$\pi/2\f$
static const double halfpi         = pi/2.0;
/// @brief \f$\pi^2\f$
static const double     pi2        = pi*pi;
/// @brief \f$\sqrt{\pi}\f$
static const double sqrt_of_pi     = 1.77245385090551602730;
/// @brief \f$\sqrt{2}\f$
static const double sqrt_of_two    = 1.41421356237309504880;
/// @brief \f$\sqrt{\frac{\pi}{2}}\f$
static const double sqrt_of_halfpi = 1.25331413731550025121;


}  // namespace Units
}  // namespace Elements




#endif  // ELEMENTSKERNEL_MATHCONSTANTS_H_
