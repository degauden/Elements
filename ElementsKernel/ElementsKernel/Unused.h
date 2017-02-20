/**
 * @file ElementsKernel/Unused.h
 *
 * @date Dec 1, 2014
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

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_UNUSED_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_UNUSED_H_

#include "ElementsKernel/Attribute.h"

/** @def ELEMENTS_UNUSED
 * This macro is there to
 * - allows to pass the argument name, even if it is not used.
 *   it will silence the compiler warning about this
 * - It's a bit better that the ugly gcc form. Until the nice [[]]
 *   will be the mainstream syntax.
 * - we can also hide compiler specific attribute here
 */
#define ELEMENTS_UNUSED __attribute__ ((unused))

#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_UNUSED_H_

/**@}*/
