/*
 * @file Export.h
 * @brief defines the macros to be used for explicit export of the symbols
 * @date Nov 4, 2013
 * @author: Hubert Degaudenzi - The Euclid Consortium
 */

#ifndef ELEMENTS_EXPORT_H_
#define ELEMENTS_EXPORT_H_


// ---------------------------------- Symbol visibility macros (begin)
// Enabled on in Elements v21 mode
#if defined(ELEMENTS_HIDE_SYMBOLS)
// These macros will allow selection on exported symbols
// taken from http://gcc.gnu.org/wiki/Visibility
#if __GNUC__ >= 4 && ! defined(__CINT__)
#  define ELEMENTS_HASCLASSVISIBILITY
#endif

#ifdef _WIN32
/*
 #  define ELEMENTS_IMPORT __declspec(dllimport)
 #  define ELEMENTS_EXPORT __declspec(dllexport)
 #  define ELEMENTS_LOCAL
 */
// The symbol visibility is disabled on Win32 because it is not possible to
// make coexists the gcc and VC ways.
#    define ELEMENTS_IMPORT
#    define ELEMENTS_EXPORT
#    define ELEMENTS_LOCAL
#else
#  if defined(ELEMENTS_HASCLASSVISIBILITY)
#    define ELEMENTS_IMPORT __attribute__((visibility("default")))
#    define ELEMENTS_EXPORT __attribute__((visibility("default")))
#    define ELEMENTS_LOCAL  __attribute__((visibility("hidden")))
#  else
#    define ELEMENTS_IMPORT
#    define ELEMENTS_EXPORT
#    define ELEMENTS_LOCAL
#  endif
#endif

// Define ELEMENTS_API for DLL builds
#ifdef ELEMENTS_LINKER_LIBRARY
#define ELEMENTS_API ELEMENTS_EXPORT
#else
#define ELEMENTS_API ELEMENTS_IMPORT
#endif
#else
// Dummy definitions for the backward compatibility mode.
#define ELEMENTS_API
#define ELEMENTS_IMPORT
#define ELEMENTS_EXPORT
#define ELEMENTS_LOCAL
#endif // defined(ELEMENTS_HIDE_SYMBOLS)

// ---------------------------------- Symbol visibility macros (end)



#endif /* ELEMENTS_EXPORT_H_ */
