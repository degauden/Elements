#define ALLOW_ALL_TYPES
#include <cstdlib>
#include <vector>

#include "ElementsKernel/Time.h"
#include "ElementsKernel/System.h"

#ifdef _WIN32
// FIXME: (MCl) The generated dictionary produce a few warnings C4345, since I
//              cannot fix them, I just disable them.

// Disable warning C4345: behavior change: an object of POD type constructed with an initializer of the
//                        form () will be default-initialized
#pragma warning ( disable : 4345 )
#endif

#ifdef __ICC
// disable icc warning #858: type qualifier on return type is meaningless
// ... a lot of noise produced by the dictionary
#pragma warning(disable:858)
// disable icc remark #2259: non-pointer conversion from "int" to "const char &" may lose significant bits
//    Strange, things like NTuple::Item<char> produce this warning, as if the operation between chars are done
//    converting them to integers first.
#pragma warning(disable:2259)
// disable icc remark #177: variable "X" was declared but never referenced
#pragma warning(disable:177)
#endif
