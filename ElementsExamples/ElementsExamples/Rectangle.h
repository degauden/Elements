#ifndef ELEMENTS_EXAMPLES_RECTANGLE_H__
#define ELEMENTS_EXAMPLES_RECTANGLE_H__

#include "ElementsKernel/Export.h"

namespace Elements {
namespace Examples {

class ELEMENTS_API Rectangle {
public:
  int x0, y0, x1, y1;
  Rectangle(int x0, int y0, int x1, int y1);
  ~Rectangle();
  int getLength();
  int getHeight();
  int getArea();
  void move(int dx, int dy);
};


} // namespace Examples
} // namespace Elements

#endif
