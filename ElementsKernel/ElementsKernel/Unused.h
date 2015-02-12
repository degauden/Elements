/**
 * @file Unused.h
 *
 * @date Dec 1, 2014
 * @author hubert
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_UNUSED_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_UNUSED_H_

#include "ElementsKernel/Attribute.h"

/**
 * This macro is there to
 * - allows to pass the argument name, even if it is not used.
 *   it will silence the compiler warning about this
 * - It's a bit better that the ugly gcc form. Until the nice [[]]
 *   will be the mainstream syntax.
 * - we can also hide compiler specific attribute here
 */
#define ELEMENTS_UNUSED __attribute__ ((unused))


#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_UNUSED_H_
