#ifndef ELEMENTS_SYSTEM_SYSTEMBASE_H
#define ELEMENTS_SYSTEM_SYSTEMBASE_H

namespace Elements {
namespace System {
/// Enumeration for fetching information
enum InfoType {
  NoFetch,
  RemainTime,
  Times,
  ProcessBasics,
  PriorityBoost,
  Memory,
  Quota,
  System,
  Modules,
  IO
};
}
}
#endif // ELEMENTS_SYSTEM_SYSTEMBASE_H
