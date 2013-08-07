/**
 * @file ElementsProgramExample.h
 * 
 * Created on: Jul 18, 2013
 * 
 * @author dubath
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This class represents ...
 */

#ifndef ELEMENTSPROGRAMEXAMPLE_H_
#define ELEMENTSPROGRAMEXAMPLE_H_

#include "ElementsKernel/ElementsProgram.h"
#include <boost/program_options.hpp>

/**
 *
 */
class ElementsProgramExample : public ElementsProgram {
public:
  ElementsProgramExample();
  virtual ~ElementsProgramExample();

  int pseudoMain();

  boost::program_options::options_description defineSpecificProgramOptions();

  std::string getVersion();


};


#endif /* ELEMENTSPROGRAMEXAMPLE_H_ */
