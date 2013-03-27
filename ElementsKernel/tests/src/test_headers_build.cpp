/** @file
 * Force the compilation of header files that otherwise would not be read.
 */

// VectorMap.cpp
#include <stdexcept>
#include "GaudiKernel/Map.h"
#include "GaudiKernel/VectorMap.h"
#include "GaudiKernel/HashMap.h"

// PropertyVerifier.cpp
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/PropertyVerifier.h"
#include <string>
#include <vector>

// HistoProperty.cpp
#include "GaudiKernel/HistoProperty.h"

/// Empty executable body, just to please the compiler/linker.
int main() {
  return 0;
}
