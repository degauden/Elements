/**
 * @file ElementsKernel/FuncPtrCast.h
 *
 * @brief defines a Small helper function that allows the cast
 *   from void * to function pointer
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

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_

namespace Elements {
namespace System {

/**
 * @brief Cast from void * to function pointer
 * @ingroup ElementsKernel
 * @details
 *   Small helper function that allows the cast from void * to function pointer
 *   and vice versa without the message
 *   @verbatim
 *   warning: ISO C++ forbids casting between pointer-to-function and pointer-to-object
 *   @endverbatim
 *   It is an ugly trick but works.
 *   See:
 *   - http://www.trilithium.com/johan/2004/12/problem-with-dlsym/
 *   - http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#573
 *   - http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#195
 * @param ptr pointer address to convert
 * @tparam SRCPTR source pointer type
 * @tparam DESTPTR destination pointer type
 * @return destination pointer
 */
template<typename DESTPTR, typename SRCPTR>
inline DESTPTR FuncPtrCast(SRCPTR ptr) {
#ifdef __GNUC__
  union {
    SRCPTR src;
    DESTPTR dst;
  } p2p;
  p2p.src = ptr;
  return p2p.dst;
#else
  return reinterpret_cast<DESTPTR>(ptr);
#endif
}

}  // namespace System
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_

/**@}*/
