/**
 * @file ElementsProgramExample.h
 * 
 * Created on: Jul 18, 2013
 * 
 * @author Pierre Dubath
 *
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

  virtual void pseudoMain();

  boost::program_options::options_description defineSpecificProgramOptions();

  std::string getVersion();


};


#endif /* ELEMENTSPROGRAMEXAMPLE_H_ */
