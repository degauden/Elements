/**
 * @file Deprecated.h
 * @brief define the ELEMENTS_DEPRECATED macros for API evolution
 * @author Hubert Degaudenzi - The Euclid Consortium
 * @date Nov 4, 2013
 */

#ifndef ELEMENTSKERNEL_DEPRECATED_H_
#define ELEMENTSKERNEL_DEPRECATED_H_

#if __GNUC__ >= 4
#  ifndef ELEMENTS_DEPRECATED
/**
 * This macro create a warning message for a deprecated function or class
 */
#    define ELEMENTS_DEPRECATED __attribute__ ((__deprecated__))
#  endif
#  ifndef ELEMENTS_DEPRECATED_MSG
/**
 * This macro create a warning message for a deprecated function or class
 * This version takes a msg as argument. This message will be displayed together
 * with the deprecation warning.
 */
#    define ELEMENTS_DEPRECATED_MSG(msg)  __attribute__ ((__deprecated__(msg)))
#  endif
#endif


#endif /* DEPRECATED_H_ */
