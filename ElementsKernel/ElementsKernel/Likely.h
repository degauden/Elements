/**
 * @file Likely.h
 * @brief defines macros for better branch guessing
 * @date Nov 4, 2013
 * @author Hubert Degaudenzi - The Euclid Consortium
 */

#ifndef ELEMENTSKERNEL_LIKELY_H_
#define ELEMENTSKERNEL_LIKELY_H_

// -------------- LIKELY/UNLIKELY macros (begin)
// Use compiler hinting to improve branch prediction for linux
#ifdef __GNUC__
#  define LIKELY(x)       __builtin_expect((x),1)
#  define UNLIKELY(x)     __builtin_expect((x),0)
#else
#  define LIKELY(x)       x
#  define UNLIKELY(x)     x
#endif
// -------------- LIKELY/UNLIKELY macros (end)



#endif /* ELEMENTSKERNEL_LIKELY_H_ */
