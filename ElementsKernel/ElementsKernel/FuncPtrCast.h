/**
 * @file FuncPtrCast.h
 *
 * @date Dec 1, 2014
 * @author hubert
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_

namespace Elements {
namespace System {

#if __GNUC__ >= 4
/** Small helper function that allows the cast from void * to function pointer
 * and vice versa without the message
 * @verbatim
 * warning: ISO C++ forbids casting between pointer-to-function and pointer-to-object
 * @endverbatim
 * It is an ugly trick but works.
 * See:
 * @secreflist
 * @refitem http://www.trilithium.com/johan/2004/12/problem-with-dlsym/
 * @refitem http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#573
 * @refitem http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#195
 * @endsecreflist
 * @param ptr pointer address to convert
 * @tparam SRCPTR source pointer type
 * @tparam DESTPTR destination pointer type
 * @return
 */
template<typename DESTPTR, typename SRCPTR>
inline DESTPTR FuncPtrCast(SRCPTR ptr) {
  union {
    SRCPTR src;
    DESTPTR dst;
  } p2p;
  p2p.src = ptr;
  return p2p.dst;
}
#endif


} // namespace System
} // namespace Elements



#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_
