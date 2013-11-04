/*
 * Deprecated.h
 *
 *  Created on: Nov 4, 2013
 *      Author: hubert
 */

#ifndef ELEMENTSKERNEL_DEPRECATED_H_
#define ELEMENTSKERNEL_DEPRECATED_H_


#ifndef ELEMENTS_DEPRECATED
#  define ELEMENTS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef ELEMENTS_DEPRECATED_MSG
#  define ELEMENTS_DEPRECATED_MSG(msg)  __attribute__ ((__deprecated__(msg)))
#endif



#endif /* DEPRECATED_H_ */
