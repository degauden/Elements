/**
 * @file Attribute.h
 * @brief define the __attribute__ macro if it doesn't exist for
 * some version of GCC
 * @date Dec 1, 2014
 * @author hubert
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_ATTRIBUTE_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_ATTRIBUTE_H_

/**
* @def __attribute__(x)
* define the __attribute__ macro to nothing if the gcc compiler
* is earlier than GCC 3.4.
*/

// Note: __attribute__ is a GCC keyword available since GCC 3.4
#if __GNUC__ < 4
#  define __attribute__(x)
#endif



#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_ATTRIBUTE_H_
