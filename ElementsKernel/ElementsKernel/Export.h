/**
 * @file Export.h
 * @brief defines the macros to be used for explicit export of the symbols
 * @date Nov 4, 2013
 * @author: Hubert Degaudenzi - The Euclid Consortium
 * @details:
 *  The ELEMENTS_API macro has to be used to expose the part of the public interface
 *  of a library. It has typically to be used in the public header files. For the functions
 *  the syntax looks like:
 *  @code
 *  ELEMENTS_API void func() ;
 *  @endcode
 *  and for the classes:
 *  @code
 *  class ELEMENTS_API MyClasse { ...
 *  @endcode
 *  This macro will only have an effect if the global symbol hiding is active in the framework.
 *  This means that the ELEMENTS_HIDE_SYMBOLS macro has to be defined.
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

#ifndef ELEMENTS_EXPORT_H_
#define ELEMENTS_EXPORT_H_

#include "ElementsKernel/Attribute.h"

// ---------------------------------- Symbol visibility macros (begin)
// Enabled on in Elements v21 mode
#if defined(ELEMENTS_HIDE_SYMBOLS)
/// These macros will allow selection on exported symbols
/// taken from http://gcc.gnu.org/wiki/Visibility
#if defined(__GNUC__) && not defined(__CINT__)
#  define ELEMENTS_HASCLASSVISIBILITY
#endif

#if defined(ELEMENTS_HASCLASSVISIBILITY)
#  define ELEMENTS_IMPORT __attribute__((visibility("default")))
#  define ELEMENTS_EXPORT __attribute__((visibility("default")))
#  define ELEMENTS_LOCAL  __attribute__((visibility("hidden")))
#else
#  define ELEMENTS_IMPORT
#  define ELEMENTS_EXPORT
#  define ELEMENTS_LOCAL
#endif

/// Define ELEMENTS_API for DLL builds
#ifdef ELEMENTS_LINKER_LIBRARY
#define ELEMENTS_API ELEMENTS_EXPORT
#else
#define ELEMENTS_API ELEMENTS_IMPORT
#endif
#else
/// Dummy definitions for the backward compatibility mode.
#define ELEMENTS_API
#define ELEMENTS_IMPORT
#define ELEMENTS_EXPORT
#define ELEMENTS_LOCAL
#endif // defined(ELEMENTS_HIDE_SYMBOLS)

// ---------------------------------- Symbol visibility macros (end)

#endif /* ELEMENTS_EXPORT_H_ */
