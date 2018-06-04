/**
 * @file Deprecated.h
 * @brief define the ELEMENTS_DEPRECATED macros for API evolution
 * @author Hubert Degaudenzi - The Euclid Consortium
 * @date Nov 4, 2013
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

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_DEPRECATED_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_DEPRECATED_H_

#include "ElementsKernel/Attribute.h"

#ifndef ELEMENTS_DEPRECATED
/** @def ELEMENTS_DEPRECATED
 * This macro create a warning message for a deprecated function or class
 */
#  define ELEMENTS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef ELEMENTS_DEPRECATED_MSG
/** @def ELEMENTS_DEPRECATED_MSG(msg)
 * This macro create a warning message for a deprecated function or class
 * This version takes a \a msg as argument. This message will be displayed together
 * with the deprecation warning.
 */
#  define ELEMENTS_DEPRECATED_MSG(msg)  __attribute__ ((__deprecated__(msg)))
#endif

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_DEPRECATED_H_

/**@}*/
