/**
 * @file Attribute.h
 * @brief define the \__attribute\__ macro if it doesn't exist for
 * some version of GCC
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

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_ATTRIBUTE_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_ATTRIBUTE_H_

/**
* @def __attribute__(x)
* define the __attribute__ macro to nothing if the gcc compiler
* is earlier than GCC 3.4.
*/

// Note: __attribute__ is a GCC keyword available since GCC 3.4
#ifndef __GNUC__
#  define __attribute__(x)
#endif

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_ATTRIBUTE_H_

/**@}*/
