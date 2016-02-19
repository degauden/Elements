/*
 * Copyright (C) 2012-2020 Euclid Science Ground Segment
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

#ifndef SWIG_CLASSEXAMPLE_H_
#define SWIG_CLASSEXAMPLE_H_


#include "ElementsKernel/Export.h" // for ELEMENTS_API

namespace Elements {
namespace Examples {

class ELEMENTS_API SwigClassExample {

public:

  SwigClassExample (const double ra=0.0, const double dec=0.0) :
      m_ra(ra), m_dec(dec) {
  }

  virtual ~SwigClassExample () {
  }


  double getRa () const;

  double getDec () const;

private:


  double m_ra ;
  double m_dec ;

};

} // namespace Examples
} // namespace Elements

#endif /* SWIG_CLASSEXAMPLE_H_ */
