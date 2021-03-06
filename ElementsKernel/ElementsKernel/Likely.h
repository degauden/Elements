/**
 * @file ElementsKernel/Likely.h
 * @brief defines macros for better branch guessing
 * @date Nov 4, 2013
 * @author Hubert Degaudenzi
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_LIKELY_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_LIKELY_H_

/** @def LIKELY(x)
 * Macro for compiler branch optimisation. To be used when is \a x
 * condition is more likely to happen that the other one
 */
/** @def UNLIKELY(x)
 * Macro for compiler branch optimisation. To be used when is \a x
 * condition is less likely to happen that the other one
 */
#ifdef __GNUC__
#define LIKELY(x) __builtin_expect(not not(x), 1)
#define UNLIKELY(x) __builtin_expect(not not(x), 0)
#else
#define LIKELY(x) x
#define UNLIKELY(x) x
#endif

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_LIKELY_H_

/**@}*/
