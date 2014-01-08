/**
 * @file Integer.h
 * @date Nov 4, 2013
 * @author Hubert Degaudenzi - The Euclid Consortium
 * @brief Large integer definition depends of the platform
 * @todo This has to be reviewed completely
 */

#ifndef ELEMENTS_INTEGER_H_
#define ELEMENTS_INTEGER_H_

#ifdef _WIN32
#ifndef NO_LONGLONG_TYPEDEF
typedef __int64 longlong;
typedef unsigned __int64 ulonglong;
#endif

#ifndef LONGLONG_MAX
#define LONGLONG_MAX 0x7FFFFFFFFFFFFFFFLL
#endif
#ifndef LONGLONG_MIN
#define LONGLONG_MIN 0x8000000000000000LL
#endif

#ifndef ULONGLONG_MAX
#define ULONGLONG_MAX 0xFFFFFFFFFFFFFFFFLL
#endif
#ifndef ULONGLONG_MIN
#define ULONGLONG_MIN 0x0000000000000000LL
#endif
#elif defined(__linux) || defined(__APPLE__)
#ifndef NO_LONGLONG_TYPEDEF
typedef long long int longlong;
typedef unsigned long long int ulonglong;
#endif

#ifndef LONGLONG_MAX
#define LONGLONG_MAX 0x7FFFFFFFFFFFFFFFLL
#endif
#ifndef LONGLONG_MIN
#define LONGLONG_MIN 0x8000000000000000LL
#endif

#ifndef ULONGLONG_MAX
#define ULONGLONG_MAX 0xfFFFFFFFFFFFFFFFLL
#endif
#ifndef ULONGLONG_MIN
#define ULONGLONG_MIN 0x0000000000000000LL
#endif
#else
// This will not really work !!
struct __longlong {public: long __data[2];};
typedef __longlong longlong;
typedef __longlong ulonglong;
static const __longlong LONGLONG_MAX = {0x7FFFFFFF, 0xFFFFFFFF};
static const __longlong LONGLONG_MIN = {0x80000000, 0x00000000};
static const __ulonglong ULONGLONG_MAX = {0xFFFFFFFF, 0xFFFFFFFF};
static const __ulonglong ULONGLONG_MIN = {0x00000000, 0x00000000};
#endif    // linux



#endif /* INTEGER_H_ */
