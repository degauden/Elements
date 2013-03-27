#if !defined(_WIN32)
/* It's called _swab(...) on NT, but swab(...) on egcs.... 
 And in some version it's not even defined
 */
#include <unistd.h>
//  extern "C" void swab( const char*, char*, size_t);
#define _swab(source, target, radix) swab(source, target, radix)
#endif  /* WIN32 */
