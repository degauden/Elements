%module SwigExample

%{

#include "ElementsKernel/Export.h"
#include "ElementsExamples/functionExample.h"
#include "ElementsExamples/SwigClassExample.h"
#include "ElementsExamples/ClassExample.h"

%}

namespace Elements {
namespace Examples {

int functionExample(int);


class SwigClassExample {

public:

  SwigClassExample (const double ra, const double dec) :
      m_ra(ra), m_dec(dec) {
  }

  virtual ~SwigClassExample () {
  }


  double getRa () const;

  double getDec () const;

};



} // namespace Examples
} // namespace Elements




