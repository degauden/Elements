/*
 * Likely.h
 *
 *  Created on: Nov 4, 2013
 *      Author: hubert
 */

#ifndef ELELMENTSKERNEL_LIKELY_H_
#define ELELMENTSKERNEL_LIKELY_H_

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



#endif /* ELELMENTSKERNEL_LIKELY_H_ */
