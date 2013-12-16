/*
 * NoGlibDebug.h
 *
 *  Created on: Dec 16, 2013
 *      Author: hubert
 */

#ifndef NOGLIBDEBUG_H_
#define NOGLIBDEBUG_H_


// this head has to be used with some boost library that do no supoprt the scheme
// by default.
// there is no version of boost with the std::__debug namespace
#ifdef _GLIBCXX_DEBUG
# undef _GLIBCXX_DEBUG
#endif



#endif /* NOGLIBDEBUG_H_ */
